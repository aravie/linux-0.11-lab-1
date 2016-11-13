/*
 *  linux/lib/open.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__
#include <unistd.h>
#include <stdarg.h>

#ifdef _WIN32
int open(const char *filename, int flag, ...)
{
	register int res;
	va_list arg;
	int d;

	va_start(arg, flag);
	d = va_arg(arg, int);
	__asm mov eax, __NR_open
	__asm mov ebx, filename
	__asm mov ecx, flag
	__asm mov edx, d
	__asm int 0x80
	__asm mov res, eax
	if (res >= 0)
		return res;
	errno = -res;
	return -1;
}
#else /* _WIN32 */
int open(const char *filename, int flag, ...)
{
	register int res;
	va_list arg;

	va_start(arg, flag);
__asm__("int $0x80":"=a"(res)
:		"0"(__NR_open), "b"(filename), "c"(flag),
		"d"(va_arg(arg, int)));
	if (res >= 0)
		return res;
	errno = -res;
	return -1;
}
#endif /* _WIN32 */