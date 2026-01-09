/* os/osmodule.c - implementation of the OSModule module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "mem.h"
#include "types.h"
#include "rom/rom.h"
#include "osmodule.h"

/* ---- osmodule_swi_register_extra --------------------------------- */

void osmodule_swi_register_extra(void)
{
    return;
}

/* ---- xosmodule_run ----------------------------------------------- */

os_error *xosmodule_run(char *tail)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_load ---------------------------------------------- */

os_error *xosmodule_load(char *tail)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_enter --------------------------------------------- */

os_error *xosmodule_enter(char *module_name,
    char *args)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_reinit -------------------------------------------- */

os_error *xosmodule_reinit(char *tail)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_kill ---------------------------------------------- */

os_error *xosmodule_kill(char *module_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_info ---------------------------------------------- */

os_error *xosmodule_info(int *max,
    int *free)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_alloc --------------------------------------------- */

os_error *xosmodule_alloc(int size,
    void **blk)
{
    void *p = mem_rma_alloc(size);
    if (p == NULL)
        return ERR_MODULE_MH_NO_ROOM();

    *blk = p;

    return NULL;
}

/* ---- xosmodule_free ---------------------------------------------- */

os_error *xosmodule_free(void *blk)
{
    mem_free(blk);
    return NULL;
}

/* ---- xosmodule_tidy ---------------------------------------------- */

os_error *xosmodule_tidy(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_clear --------------------------------------------- */

os_error *xosmodule_clear(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_add_area ------------------------------------------ */

os_error *xosmodule_add_area(byte *module)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_copy_area ----------------------------------------- */

os_error *xosmodule_copy_area(byte *module,
    int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_enumerate ----------------------------------------- */

os_error *xosmodule_enumerate(int module_no,
    int instance_no,
    int *module_no_out,
    int *instance_no_out,
    byte **module,
    void **workspace,
    char **postfix)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_realloc ------------------------------------------- */

os_error *xosmodule_realloc(void *blk,
    int size_increase,
    void **blk_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_instantiate --------------------------------------- */

os_error *xosmodule_instantiate(char *instance_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_rename -------------------------------------------- */

os_error *xosmodule_rename(char *old_name,
    char *new_postfix)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_prefer -------------------------------------------- */

os_error *xosmodule_prefer(char *instance_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_add_podule_area ----------------------------------- */

os_error *xosmodule_add_podule_area(char *environment,
    int chunk,
    osmodule_section_no rom_section)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_lookup -------------------------------------------- */

os_error *xosmodule_lookup(char *module_name,
    int *module_no,
    int *instance_no,
    byte **module,
    void **workspace,
    char **postfix)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_enumerate_rom ------------------------------------- */

os_error *xosmodule_enumerate_rom(int module_no,
    osmodule_section_no section,
    int *module_no_out,
    int *section_out,
    char **module_name,
    osmodule_status *status,
    int *chunk_no)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosmodule_enumerate_rom_with_info --------------------------- */

os_error *xosmodule_enumerate_rom_with_info(int module_no,
    osmodule_section_no section,
    int *module_no_out,
    int *section_out,
    char **module_name,
    osmodule_status *status,
    int *chunk_no,
    int *bcd_version)
{
    return ERR_NO_SUCH_SWI();
}

