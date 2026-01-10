/* os/ddeutils.c - implementation of the DDEUtils module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>
#include <string.h>

#include "monty/monty.h"
#include "types.h"
#include "ddeutils.h"
#include "map.h"
#include "mem.h"
#include "rom/rom.h"

/* ---- ddeutils_swi_register_extra --------------------------------- */

void ddeutils_swi_register_extra(void)
{
    return;
}

/* ---- xddeutils_prefix -------------------------------------------- */

os_error *xddeutils_prefix(char *dir_name)
{
    if (dir_name && strlen(dir_name) != 0) {
        error("swi XDDEUtils_Prefix unimplemented: %s\n", dir_name);
    }

    return NULL;
}

/* ---- xddeutils_set_cl_size --------------------------------------- */

os_error *xddeutils_set_cl_size(int size)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_set_cl -------------------------------------------- */

os_error *xddeutils_set_cl(char *tail)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_get_cl_size --------------------------------------- */

os_error *xddeutils_get_cl_size(int *size)
{
    mem_private *priv = mem_get_private();
    char *space = strchr((const char *)priv->cli, ' ');
    if (space == NULL)
      *size = 0;
    else
      *size = strlen(space + 1);

    return NULL;
}

/* ---- xddeutils_get_cl -------------------------------------------- */

os_error *xddeutils_get_cl(char *tail)
{
    mem_private *priv = mem_get_private();
    char *space = strchr((const char *)priv->cli, ' ');
    if (space == NULL)
      *tail = '\0';
    else
      strcpy(tail, space + 1);

    return NULL;
}

/* ---- xddeutils_throwback_register -------------------------------- */

os_error *xddeutils_throwback_register(wimp_t task)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_throwback_un_register ----------------------------- */

os_error *xddeutils_throwback_un_register(wimp_t task)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_throwback_start ----------------------------------- */

os_error *xddeutils_throwback_start(void)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_throwback_send ------------------------------------ */

os_error *xddeutils_throwback_send(int reason,
    char *file_name,
    int line_no,
    int severity,
    char *message)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xddeutils_throwback_end ------------------------------------- */

os_error *xddeutils_throwback_end(void)
{
    return ERR_NO_SUCH_SWI();
}

