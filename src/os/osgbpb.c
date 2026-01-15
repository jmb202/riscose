/* os/osgbpb.c - implementation of the OSGBPB module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>

#include "monty/monty.h"
#include "types.h"
#include "rom/rom.h"
#include "osgbpb.h"

/* ---- osgbpb_swi_register_extra ----------------------------------- */

void osgbpb_swi_register_extra(void)
{
    return;
}

/* ---- xosgbpb_write_atw ------------------------------------------- */

os_error *xosgbpb_write_atw(os_fw file,
    byte *data,
    int32_t size,
    int32_t ptr,
    int32_t *unwritten)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_writew ---------------------------------------------- */

os_error *xosgbpb_writew(os_fw file,
    byte *data,
    int32_t size,
    int32_t *unwritten)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_read_atw -------------------------------------------- */

os_error *xosgbpb_read_atw(os_fw file,
    byte *buffer,
    int32_t size,
    int32_t ptr,
    int32_t *unread)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_readw ----------------------------------------------- */

os_error *xosgbpb_readw(os_fw file,
    byte *buffer,
    int32_t size,
    int32_t *unread)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_read_disc_name -------------------------------------- */

os_error *xosgbpb_read_disc_name(osgbpb_disc_name *disc_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_read_csd_name --------------------------------------- */

os_error *xosgbpb_read_csd_name(osgbpb_dir_name *csd_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_read_lib_name --------------------------------------- */

os_error *xosgbpb_read_lib_name(osgbpb_dir_name *lib_name)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_csd_entries ----------------------------------------- */

os_error *xosgbpb_csd_entries(osgbpb_name_list *name_list,
    int32_t count,
    int32_t context,
    int32_t *unread,
    int32_t *context_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_dir_entries ----------------------------------------- */

os_error *xosgbpb_dir_entries(char *dir_name,
    osgbpb_string_list *name_list,
    int32_t count,
    int32_t context,
    int32_t size,
    char *entries,
    int32_t *read_count,
    int32_t *context_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_dir_entries_info ------------------------------------ */

os_error *xosgbpb_dir_entries_info(char *dir_name,
    osgbpb_info_list *info_list,
    int32_t count,
    int32_t context,
    int32_t size,
    char *entries,
    int32_t *read_count,
    int32_t *context_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_dir_entries_system_info ----------------------------- */

os_error *xosgbpb_dir_entries_system_info(char *dir_name,
    osgbpb_system_info_list *info_list,
    int32_t count,
    int32_t context,
    int32_t size,
    char *entries,
    int32_t *read_count,
    int32_t *context_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosgbpb_dir_entries_info_stamped ---------------------------- */

os_error *xosgbpb_dir_entries_info_stamped(char *dir_name,
    osgbpb_info_stamped_list *info_list,
    int32_t count,
    int32_t context,
    int32_t size,
    char *entries,
    int32_t *read_count,
    int32_t *context_out)
{
    return ERR_NO_SUCH_SWI();
}

