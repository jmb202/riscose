/* os/osargs.c - implementation of the OSArgs module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "monty/monty.h"
#include "types.h"
#include "osargs.h"
#include "rom/rom.h"

/* ---- osargs_swi_register_extra ----------------------------------- */

void osargs_swi_register_extra(void)
{
    return;
}

/* ---- xosargs_read_ptrw ------------------------------------------- */

os_error *xosargs_read_ptrw(os_fw file,
    int *ptr)
{
    off_t pos;

    pos = lseek(file, 0, SEEK_CUR);
    if (pos == (off_t) -1)
        return ERR_OUT_OF_RANGE();

    *ptr = (int) pos;
    return NULL;
}

/* ---- xosargs_read_temporary_fs ----------------------------------- */

os_error *xosargs_read_temporary_fs(fileswitch_fs_no *temp_fs_no)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_ptrw -------------------------------------------- */

os_error *xosargs_set_ptrw(os_fw file,
    int ptr)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_extw ------------------------------------------- */

os_error *xosargs_read_extw(os_fw file,
    int *ext)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_extw -------------------------------------------- */

os_error *xosargs_set_extw(os_fw file,
    int ext)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_allocationw ------------------------------------ */

os_error *xosargs_read_allocationw(os_fw file,
    int *allocation)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_eof_statusw ------------------------------------ */

os_error *xosargs_read_eof_statusw(os_fw file,
    osbool *eof_status)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_set_allocationw ------------------------------------- */

os_error *xosargs_set_allocationw(os_fw file,
    int allocation,
    int *allocation_out)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_pathw ------------------------------------------ */

os_error *xosargs_read_pathw(os_fw file,
    char *buffer,
    int size,
    int *spare)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_image_stampedw -------------------------------------- */

os_error *xosargs_image_stampedw(os_fw file,
    int image_stamp)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_io_ctl ---------------------------------------------- */

os_error *xosargs_io_ctl(os_f file,
    osargs_io_ctl_parameters *parameters)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xosargs_read_infow ------------------------------------------ */

os_error *xosargs_read_infow(os_fw file,
    osargs_stream_status *stream_status,
    fileswitch_fs_info *fs_info)
{
    int flags;

    flags = fcntl(file, F_GETFL, NULL);
    if (flags == -1 && errno == EBADF) {
        /* Not an fd: say so */
        *stream_status = osargs_STREAM_UNALLOCATED;
        *fs_info = 0;
        return NULL;
    } else if (flags == -1)
        return ERR_BAD_PARAMETERS();

    *stream_status = osargs_STREAM_SUPPORTS_GBPB | osargs_STREAM_UNBUFFERED;
    if ((flags & O_ACCMODE) == O_RDONLY)
      *stream_status |= osargs_STREAM_READABLE;
    else if ((flags & O_ACCMODE) == O_WRONLY)
      *stream_status |= osargs_STREAM_WRITABLE;
    else if ((flags & O_ACCMODE) == O_RDWR)
      *stream_status |= (osargs_STREAM_READABLE | osargs_STREAM_WRITABLE);

    /* XXX: STREAM_WRITTEN/EOF_ERROR_ON_NEXT_READ? */

    /* XXX: what to report here? */
    *fs_info = 0;

    return NULL;
}

/* ---- xosargs_ensurew --------------------------------------------- */

os_error *xosargs_ensurew(os_fw file)
{
    return ERR_NO_SUCH_SWI();
}

