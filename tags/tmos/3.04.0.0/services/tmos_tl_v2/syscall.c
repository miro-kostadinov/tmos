/*
 * syscall.c
 *
 *  Created on: Mar 18, 2013
 *      Author: miro
 */

//#include <tmos.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/***************************************************************************/

int _read_r (struct _reent *r, int file, char * ptr, int len)
{
  r = r;
  file = file;
  ptr = ptr;
  len = len;

  errno = EINVAL;
  return -1;
}

/***************************************************************************/

int _lseek_r (struct _reent *r, int file, int ptr, int dir)
{
  r = r;
  file = file;
  ptr = ptr;
  dir = dir;

  return 0;
}

/***************************************************************************/

int _write_r (struct _reent *r, int file, char * ptr, int len)
{
  r = r;
  file = file;
  ptr = ptr;

  usr_trace_str(ptr);
  return len;
}

/***************************************************************************/

int _close_r (struct _reent *r, int file)
{
  return 0;
}

/***************************************************************************/

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");

caddr_t _sbrk_r (struct _reent *r, int incr)
{
	void *p;

	(void) r;
	p = tsk_malloc( incr);
	if (p == NULL)
	{
		__errno_r(r)  = ENOMEM;
		return (caddr_t)-1;
	}
	return (caddr_t) p;
}

/***************************************************************************/

int _fstat_r (struct _reent *r, int file, struct stat * st)
{
  r = r;
  file = file;

  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  return 0;
}

/***************************************************************************/

int _isatty_r(struct _reent *r, int fd)
{
  r = r;
  fd = fd;

  return 1;
}

