/* os/sharedunixlibrary.c - implementation of the SharedUnixLibrary module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "monty/monty.h"
#include "types.h"
#include "osmodule.h"
#include "sharedunixlibrary.h"
#include "arm.h"
#include "mem.h"
#include "rom/rom.h"
#include "swi.h"

static sharedunixlibrary_process *sul_process_list;

/* ---- sharedunixlibrary_swi_register_extra ------------------------ */

static os_error *sul_malloc(WORD n)
{
  WORD size = ARM_R1;
  void *res = NULL;
  os_error *err;

  err = xosmodule_alloc(size, &res);
  arm_set_reg(0, err ? 0 : MEM_TOARM(res));

  return NULL;
}

static os_error *sul_free(WORD n)
{
  WORD ptr = ARM_R1;

  xosmodule_free(MEM_TOHOST(ptr));

  return NULL;
}

static os_error *sul_exit(WORD n)
{
  WORD pid = ARM_R0;
  WORD status = ARM_R1;
  sharedunixlibrary_process *p = (void *)MEM_TOHOST(pid << 2);
  sharedunixlibrary_process *i, *prev;

  /* No support for dynamic linking */
  assert((p->status & (1<<23)) == 0);
  /* Or armeabi for the moment (XXX: gonna need this at some point) */
  assert((p->status & (1<<24)) == 0);

  /* delink */
  for (i = sul_process_list, prev = NULL;
       i != NULL;
       prev = i, i = (void *)MEM_TOHOST(i->reserved1)) {
    if (i == p)
      break;
  }
  if (prev == NULL && sul_process_list == p) {
    sul_process_list = (void *)MEM_TOHOST(p->reserved1);
  } else if (prev != NULL && i == p) {
    prev->reserved1 = p->reserved1;
  }

  if (p->environ != 0)
    xosmodule_free(MEM_TOHOST((uintptr_t)(p->environ)));
  xosmodule_free(p);

  return xos_exit(ERR_RC_EXC(), status);
}

static os_error *sul_fork(WORD n)
{
  // XXX: fork not supported: return -1
  arm_set_reg(0, 0xffffffff);
  return NULL;
}

static os_error *sul_exec(WORD n)
{
  // XXX: exec: not supported
  abort();
  return NULL;
}

static os_error *sul_wimpslot(WORD n)
{
  // XXX: not supported: return NULL
  arm_set_reg(0, 0);
  return NULL;
}

void sharedunixlibrary_swi_register_extra(void)
{
    swi_register(0x55cbf, "SharedUnixLibrary_ProcMalloc", sul_malloc);
    swi_register(0x55cbe, "SharedUnixLibrary_ProcFree", sul_free);
    swi_register(0x55cbd, "SharedUnixLibrary_ProcExit", sul_exit);
    swi_register(0x55cbc, "SharedUnixLibrary_ProcFork", sul_fork);
    swi_register(0x55cbb, "SharedUnixLibrary_ProcExec", sul_exec);
    swi_register(0x55cba, "SharedUnixLibrary_ProcWimpslot", sul_wimpslot);
}

/* ---- xsharedunixlibrary_register_up_call ------------------------- */

os_error *xsharedunixlibrary_register_up_call(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_de_register_up_call ---------------------- */

os_error *xsharedunixlibrary_de_register_up_call(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_set_value -------------------------------- */

os_error *xsharedunixlibrary_set_value(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_count ------------------------------------ */

os_error *xsharedunixlibrary_count(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xsharedunixlibrary_initialise ------------------------------- */

/* Array of ARM instructions that implements SUL process callbacks */
static uint32_t sulcode[] = {
    /* Upcall handler */
    0xe3300c01,  // TEQ r0, #256
    0x03a00000,  // MOVEQ r0, #0
    0xe1a0f00e,  // MOV pc, lr
    /* Malloc */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cbf,  // SWI SUL_ProcMalloc
    0xe49df004,  // LDR pc,[sp],#4
    /* Free */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cbe,  // SWI SUL_ProcFree
    0xe49df004,  // LDR pc,[sp],#4
    /* Exit */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cbd,  // SWI SUL_ProcExit
    0xe49df004,  // LDR pc,[sp],#4
    /* Fork */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cdc,  // SWI SUL_ProcFork
    0xe49df004,  // LDR pc,[sp],#4
    /* Exec */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cdb,  // SWI SUL_ProcExec
    0xe49df004,  // LDR pc,[sp],#4
    /* Wimpslot */
    0xe52de004,  // STR lr,[sp,#-4]!
    0xef055cda,  // SWI SUL_ProcWimpslot
    0xe49df004,  // LDR pc,[sp],#4
};
static size_t sulfuncsize = sizeof(sulcode) / 7;

os_error *xsharedunixlibrary_initialise(int version_required,
    sharedunixlibrary_process **proc,
    void **upcall_handler_addr,
    byte **upcall_handler_r12)
{
    os_error *err;
    sharedunixlibrary_process *p;

    if (version_required < 105)
        return ERR_SHARED_UNIX_LIBRARY_TOO_OLD();

    err = xosmodule_alloc(sizeof(*p) + sizeof(sulcode), (void **)&p);
    if (err != NULL)
        return err;

    memset(p, 0, sizeof(*p));
    memcpy(((char *)p) + sizeof(*p), sulcode, sizeof(sulcode));

    p->reserved2 = -1;
    p->sul_malloc = MEM_TOARM(((char *)p) + sizeof(*p) + (1 * sulfuncsize));
    p->sul_free = MEM_TOARM(((char *)p) + sizeof(*p) + (2 * sulfuncsize));
    p->sul_exit = MEM_TOARM(((char *)p) + sizeof(*p) + (3 * sulfuncsize));
    p->pid = MEM_TOARM(p) >> 2;  // 30 bits, as per the real thing
    p->ppid = 1;
    p->uid = 1;
    p->euid = 1;
    p->gid = 1;
    p->egid = 1;
    p->pgrp = 2;
    p->umask = 022;
    p->maxfd = 256;
    p->fdsize = 12;
    p->fdhandlesize = 12;
    p->sul_fork = MEM_TOARM(((char *)p) + sizeof(*p) + (4 * sulfuncsize));
    p->sul_exec = MEM_TOARM(((char *)p) + sizeof(*p) + (5 * sulfuncsize));
    p->sul_wimpslot = MEM_TOARM(((char *)p) + sizeof(*p) + (6 * sulfuncsize));
    p->reserved1 = MEM_TOARM((void *)sul_process_list);
    sul_process_list = p;

    *proc = p;
    *upcall_handler_r12 = (byte *) p;
    *upcall_handler_addr = (void *)((char *)p) + sizeof(*p);

    return NULL;
}

