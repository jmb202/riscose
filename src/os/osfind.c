/* os/osfind.c - implementation of the OSFind module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "osfind.h"
#include "rom/rom.h"

/* ---- osfind_swi_register_extra ----------------------------------- */

void osfind_swi_register_extra(void)
{
    return;
}

/* ---- xosfind_openup_with_no_pathw -------------------------------- */

os_error *xosfind_openup_with_no_pathw(osfind_flags flags,
    char *file_name,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openup_with_path_varw ------------------------------- */

os_error *xosfind_openup_with_path_varw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    error("swi XOSFind_OpeninW '%s' unimplemented.\n", file_name);
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openup_with_pathw ----------------------------------- */

os_error *xosfind_openup_with_pathw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openupw --------------------------------------------- */

os_error *xosfind_openupw(osfind_flags flags,
    char *file_name,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openout_with_no_pathw ------------------------------- */

os_error *xosfind_openout_with_no_pathw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openout_with_path_varw ------------------------------ */

os_error *xosfind_openout_with_path_varw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openout_with_pathw ---------------------------------- */

os_error *xosfind_openout_with_pathw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openoutw -------------------------------------------- */

os_error *xosfind_openoutw(osfind_flags flags,
    char *file_name,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openin_with_no_pathw -------------------------------- */

os_error *xosfind_openin_with_no_pathw(osfind_flags flags,
    char *file_name,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openin_with_path_varw ------------------------------- */

os_error *xosfind_openin_with_path_varw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openin_with_pathw ----------------------------------- */

os_error *xosfind_openin_with_pathw(osfind_flags flags,
    char *file_name,
    char *path,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_openinw --------------------------------------------- */

os_error *xosfind_openinw(osfind_flags flags,
    char *file_name,
    os_fw *file)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosfind_closew ---------------------------------------------- */

os_error *xosfind_closew(os_fw file)
{
    return ERR_NO_SUCH_SWI();
}

