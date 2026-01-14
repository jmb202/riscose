/* os/osword.c
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
**
**   Emulation of the OSWord SWIs.
**
**   Template written by defmod (riscose version 1.00)
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <monty/monty.h>
#include "types.h"
#include "osword.h"
#include <rom/rom.h>

void osword_swi_register_extra(void)
{

}

/* ------------------------------------------------------------------------
 * Function:      osword_read_system_clock()
 *
 * Description:   Reads the system clock
 *
 * Input:         clock - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x1.
 */

os_error *xosword_read_system_clock (osword_timer_block *clock)
{
  uint64_t time_ros;
  struct timeval tv;

  if (gettimeofday(&tv, NULL) != 0) abort();

  time_ros = tv.tv_sec * 100UL + 613608L*3600L*100L + tv.tv_usec / 1000UL;

  for (size_t i = 0; i < sizeof(clock->b); i++) {
    clock->b[i] = time_ros & 0xff;
    time_ros >>= 8;
  }

  return 0;
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_system_clock()
 *
 * Description:   Writes the system clock
 *
 * Input:         clock - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x2.
 */

os_error *xosword_write_system_clock (osword_timer_block *clock)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_interval_timer()
 *
 * Description:   Reads the interval timer
 *
 * Input:         timer - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x3.
 */

os_error *xosword_read_interval_timer (osword_timer_block *timer)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_interval_timer()
 *
 * Description:   Writes the interval timer
 *
 * Input:         timer - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x4.
 */

os_error *xosword_write_interval_timer (osword_timer_block *timer)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_char_definition()
 *
 * Description:   Reads a character definition - Client must partly
 *                initialise the parameter block
 *
 * Input:         definition - Partly filled in on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xA.
 */

os_error *xosword_read_char_definition (osword_char_definition_block *definition)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_palette()
 *
 * Description:   Reads the palette - Client must partly initialise the
 *                parameter block - prefer ColourTrans_ReadPalette
 *
 * Input:         entry - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xB.
 */

os_error *xosword_read_palette (osword_palette_block *entry)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_palette()
 *
 * Description:   Writes the palette - Client must initialise the parameter
 *                block - prefer ColourTrans_WritePalette
 *
 * Input:         entry - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xC.
 */

os_error *xosword_write_palette (osword_palette_block *entry)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_read_cursor_position()
 *
 * Description:   Reads the current and previous graphics cursor positions
 *                - prefer OS_ReadVduVariables
 *
 * Input:         position - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xD.
 */

os_error *xosword_read_cursor_position (osword_cursor_position_block *position)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_local_string()
 *
 * Description:   Reads the real-time clock as a string, converting to
 *                local time - Client must partly initialise the parameter
 *                block
 *
 * Input:         string - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_local_string (oswordreadclock_local_string_block *string)
{
  // XXX: need a better answer for this kind of thing
  switch (string->op) {
  case oswordreadclock_OP_LOCAL_STRING:
    break;
  case oswordreadclock_OP_LOCAL_BCD:
    return xoswordreadclock_local_bcd((oswordreadclock_local_bcd_block *)string);
  case oswordreadclock_OP_CONVERT_BCD_TO_STRING:
    return xoswordreadclock_convert_bcd_to_string((oswordreadclock_convert_bcd_to_string_block *)string);
  case oswordreadclock_OP_UTC:
    return xoswordreadclock_utc((oswordreadclock_utc_block *)string);
  default:
    return ERR_NO_SUCH_SWI();
  }
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_local_bcd()
 *
 * Description:   Reads the real-time clock in BCD format, converting to
 *                local time - Client must partly initialise the parameter
 *                block
 *
 * Input:         bcd - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_local_bcd (oswordreadclock_local_bcd_block *bcd)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_convert_bcd_to_string()
 *
 * Description:   Converts a BCD clock value into string format - Client
 *                must partly initialise the parameter block
 *
 * Input:         convert_bcd_to_String - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_convert_bcd_to_string (oswordreadclock_convert_bcd_to_string_block *convert_bcd_to_String)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordreadclock_utc()
 *
 * Description:   Reads the real-time clock in 5-byte format - Client must
 *                partly initialise the parameter block
 *
 * Input:         utc - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xE.
 */

os_error *xoswordreadclock_utc (oswordreadclock_utc_block *utc)
{
  osword_timer_block t;
  os_error *err;

  err = xosword_read_system_clock (&t);
  if (err == NULL)
    memcpy(utc->utc, t.b, sizeof(utc->utc));

  return err;
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_time_local_string()
 *
 * Description:   Writes the real-time clock with a time string, converting
 *                from local time - Client must partly initialise the
 *                parameter block
 *
 * Input:         time - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_time_local_string (oswordwriteclock_block *time)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_date_local_string()
 *
 * Description:   Writes the real-time clock with a date string, converting
 *                from local time - Client must partly initialise the
 *                parameter block
 *
 * Input:         date - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_date_local_string (oswordwriteclock_block *date)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordwriteclock_date_and_time_local_string()
 *
 * Description:   Writes the real-time clock with a time and date string,
 *                converting from local time - Client must partly
 *                initialise the parameter block
 *
 * Input:         date_and_time - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0xF.
 */

os_error *xoswordwriteclock_date_and_time_local_string (oswordwriteclock_block *date_and_time)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_define()
 *
 * Description:   Defines the pointer size, shape and active point - Client
 *                must partly initialise the parameter block - prefer
 *                OSSpriteOp_SetPointerShape
 *
 * Input:         define - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x2.
 */

os_error *xoswordpointer_define (oswordpointer_define_block *define)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_bbox()
 *
 * Description:   Defines the mouse coordinate bounding box - Client must
 *                partly initialise the parameter block
 *
 * Input:         bbox - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x3.
 */

os_error *xoswordpointer_set_bbox (oswordpointer_bbox_block *bbox)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_step()
 *
 * Description:   Defines the mouse multipliers. - Client must partly
 *                initialise the parameter block
 *
 * Input:         step - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15.
 */

os_error *xoswordpointer_step (oswordpointer_step_block *step)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_position()
 *
 * Description:   Sets the mouse position. - Client must partly initialise
 *                the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_set_position (oswordpointer_position_block *pos)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_read_position()
 *
 * Description:   Reads the unbuffered mouse position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_read_position (oswordpointer_position_block *pos)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_set_unlinked_position()
 *
 * Description:   Sets the pointer position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_set_unlinked_position (oswordpointer_position_block *pos)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      oswordpointer_read_unlinked_position()
 *
 * Description:   Reads the pointer position. - Client must partly
 *                initialise the parameter block
 *
 * Input:         pos - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x15, R1 += 0x1.
 */

os_error *xoswordpointer_read_unlinked_position (oswordpointer_position_block *pos)
{
  return ERR_NO_SUCH_SWI();
}

/* ------------------------------------------------------------------------
 * Function:      osword_write_screen_base()
 *
 * Description:   Writes the screen base address. - Client must partly
 *                initialise the parameter block
 *
 * Input:         screen_base - value of R1 on entry
 *
 * Other notes:   Emulation of SWI 0x7 with R0 = 0x16, R1 += 0x3.
 */

os_error *xosword_write_screen_base (osword_screen_base_block *screen_base)
{
  return ERR_NO_SUCH_SWI();
}

