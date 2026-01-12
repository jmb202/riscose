/* os/cryptrandom.c - implementation of the CryptRandom module. */

/* See http://riscose.sf.net/ for terms of distribution, and to
 * pick up a later version of the software.
 *
 * Created by defmod, riscose version 1.01. */

#include <stdio.h>
#include <sys/random.h>

#include "monty/monty.h"
#include "types.h"
#include "rom/rom.h"
#include "cryptrandom.h"

/* ---- cryptrandom_swi_register_extra ------------------------------ */

void cryptrandom_swi_register_extra(void)
{
    return;
}

/* ---- xcryptrandom_byte ------------------------------------------- */

os_error *xcryptrandom_byte(byte *value)
{
    while (getrandom(value, 1, 0) != 1);
    return NULL;
}

/* ---- xcryptrandom_stir ------------------------------------------- */

os_error *xcryptrandom_stir(void)
{
    return NULL;
}

/* ---- xcryptrandom_add_noise -------------------------------------- */

os_error *xcryptrandom_add_noise(byte *buf,
    int32_t size)
{
    (void) buf;
    (void) size;
    return NULL;
}

/* ---- xcryptrandom_block ------------------------------------------ */

os_error *xcryptrandom_block(byte *buf,
    int32_t len)
{
    while (getrandom(buf, len, 0) != len);
    return NULL;
}

/* ---- xcryptrandom_word ------------------------------------------- */

os_error *xcryptrandom_word(bytes *value)
{
    while (getrandom(value, sizeof(*value), 0) != sizeof(*value));
    return NULL;
}

