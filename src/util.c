/* util.c
**
** (c) Matthew Bloch 2000
**
** See http://riscose.sourceforge.net/ for terms of distribution, and to
** pick up a later version of the software.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include <monty/mem.h>

#include "util.h"

/* FIXME:  poor name.  strncpy doesn't malloc, this does.  strncpy
 * doesn't always terminate the new string, this does. */

char *xstrncpy(char *src, int n)
{
    char *dst;

    dst = emalloc(n + 1);
    strncpy(dst, src, n);
    dst[n] = '\0';

    return dst;
}

int file_objecttype(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return OBJECT_NOTFOUND;
  if ((file_info.st_mode & S_IFDIR) != 0)
    return OBJECT_DIR;
  return OBJECT_FILE;
}

size_t file_size(char *name)
{
  struct stat file_info;
  if (stat(name, &file_info) == -1)
    return -1;
  return file_info.st_size;
}

int file_loadat(char *name, void *addr)
{
  FILE *fh = fopen(name, "r");
  if (fh == NULL)
    return -1;
  size_t s = file_size(name);
  size_t r = fread(addr, 1, s, fh);
  assert(s == r);
  fclose(fh);
  return 0;
}

char *host_path_from_ro_path(const char *rostr) {
  const char *end = rostr;
  size_t len = 0;
  char *out, *p;

  while (*end > 31) {
    switch (*end) {
      case '$': /* Root of disc */
        break;
      case '&': /* URD */
        break;
      case '@': /* CSD */
        len++;
        break;
      case '^': /* .. */
        len += 2;
        break;
      case '%': /* CSL */
        break;
      case '\\': /* PSD */
        break;
      default:
        len++;
        break;
    }
    end++;
  }

  out = malloc(len + 1);
  if (out == NULL)
    return NULL;

  /* Unixify */
  fprintf(stderr, "hpfrp: '%.*s'\n", (int)(end-rostr), rostr);
  for (end = rostr, p = out; *end > 31; end++) {
    switch (*end) {
      case '$': /* Root of disc */
        break;
      case '&': /* URD */
        break;
      case '@': /* CSD */
        *p++ = '.';
        break;
      case '^': /* .. */
        *p++ = '.';
        *p++ = '.';
        break;
      case '%': /* CSL */
        break;
      case '\\': /* PSD */
        break;
      case '/':
        *p++ = '.';
        break;
      case '.':
        *p++ = '/';
        break;
      default:
        *p++ = *end;
        break;
    }
  }
  *p = '\0';
  fprintf(stderr, "-> hpfrp: '%s'\n", out);

  return out;
}
