/* os/iconvmod.c - implementation of the Iconv module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <assert.h>
#include <errno.h>
#include <iconv.h>
#include <stdio.h>
#include <string.h>

#include "monty/monty.h"
#include "types.h"
#include "rom/rom.h"
#include "arm.h"
#include "mem.h"
#include "iconvmod.h"

static iconv_t cdtab[256];

static os_error *oserror_from_host(int hosterr)
{
    switch (hosterr) {
        case ENOMEM:
            return ERR_ICONV_NO_MEM();
        case EINVAL:
            return ERR_ICONV_INVAL();
        case E2BIG:
            return ERR_ICONV2_BIG();
        case EILSEQ:
            return ERR_ICONV_ILSEQ();
    }

    return ERR_ICONV_INVAL();
}

/* ---- iconvmod_swi_register_extra --------------------------------- */

void iconvmod_swi_register_extra(void)
{
    return;
}

/* ---- xiconv_open ------------------------------------------------- */

os_error *xiconv_open(char *tocode,
    char *fromcode,
    iconv_cd *cd)
{
    iconv_t it;
    size_t i;

    it = iconv_open(tocode, fromcode);
    if (it == (iconv_t) -1)
        return oserror_from_host(errno);

    for (i = 0; i < (sizeof(cdtab) / sizeof(iconv_t)); i++) {
        if (cdtab[i] == NULL)
            break;
    }
    assert(i < (sizeof(cdtab) / sizeof(iconv_t)));
    cdtab[i] = it;

    *cd = i;

    return NULL;
}

/* ---- xiconv_iconv ------------------------------------------------ */

os_error *xiconv_iconv(iconv_cd cd,
    char **inbuf,
    int32_t *inlen,
    char **outbuf,
    int32_t *outlen,
    int32_t *nrcount)
{
    /* This API is a bit ugly: the SWI parameters are pointers
     * to the values.  Therefore we need to compute the _host_
     * addresses for the buffers before using them. */
    char *hin = (char *) MEM_TOHOST(*((WORD *) inbuf));
    char *hout = (char *) MEM_TOHOST(*((WORD *) outbuf));
    size_t hinl = (size_t) *inlen, houtl = (size_t) *outlen;
    size_t ret;

    if (cd > (sizeof(cdtab) / sizeof(iconv_t)))
        return oserror_from_host(EINVAL);

    ret = iconv(cdtab[cd], &hin, &hinl, &hout, &houtl);

    /* Update the parameters, successful or not, translating
     * pointers back into addresses in the ARM memory map. */
    *((WORD *) inbuf) = MEM_TOARM((void *) hin);
    *inlen = (int32_t) hinl;
    *((WORD *) outbuf) = MEM_TOARM((void *) hout);
    *outlen = (int32_t) houtl;

    if (ret == (size_t) -1)
        return oserror_from_host(errno);

    *nrcount = (int32_t) ret;
    return NULL;
}

/* ---- xiconv_close ------------------------------------------------ */

os_error *xiconv_close(iconv_cd cd)
{
    if (cd > (sizeof(cdtab) / sizeof(iconv_t)))
        return oserror_from_host(EINVAL);

    if (iconv_close(cdtab[cd]) == -1)
        return oserror_from_host(errno);

    cdtab[cd] = NULL;

    return NULL;
}

/* ---- xiconv_convert ---------------------------------------------- */

os_error *xiconv_convert(iconv_cd cd,
    char *inbuf,
    int32_t inlen,
    char *outbuf,
    int32_t outlen,
    int32_t *nrcount,
    char **out_inbuf,
    int32_t *out_inlen,
    char **out_outbuf,
    int32_t *out_outlen)
{
    os_error *err;
    /* We can implement this in terms of the xiconv_iconv SWI, but
     * we must first convert the host buffer addresses into ARM space. */
    WORD ain = MEM_TOARM((void *) inbuf);
    WORD aout = MEM_TOARM((void *) outbuf);

    err = xiconv_iconv(cd, (char **) &ain, &inlen,
        (char **) &aout, &outlen, nrcount);

    /* Populate the output parameters, successful or not,
     * converting the buffer pointers back into host addresses */
    *out_inbuf = (char *) MEM_TOHOST(ain);
    *out_inlen = inlen;
    *out_outbuf = (char *) MEM_TOHOST(aout);
    *out_outlen = outlen;

    if (err != NULL) {
        /* The osapi glue will only set the output registers
         * if successful.  However, we need them set here, too.
	 * Do so, ensuring the buffer pointers are translated
	 * into addresses in the ARM memory map.
	 * */
        ARM_SET_R1(MEM_TOARM((void *) *out_inbuf));
        ARM_SET_R2(*out_inlen);
        ARM_SET_R3(MEM_TOARM((void *) *out_outbuf));
        ARM_SET_R4(*out_outlen);
        return oserror_from_host(errno);
    }

    return err;
}

/* ---- xiconv_create_menu ------------------------------------------ */

os_error *xiconv_create_menu(bits flags,
    byte *buffer,
    int32_t buflen,
    char *selenc,
    byte *indbuf,
    int32_t indbuflen,
    int32_t *out_buflen,
    int32_t *out_indbuflen)
{
    return ERR_NO_SUCH_SWI();
}

/* ---- xiconv_decode_menu ------------------------------------------ */

os_error *xiconv_decode_menu(bits flags,
    byte *menudef,
    byte *selections,
    byte *buf,
    int32_t buflen,
    int32_t *out_buflen)
{
    return ERR_NO_SUCH_SWI();
}

