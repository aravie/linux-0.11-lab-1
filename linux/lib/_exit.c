/*
 *  linux/lib/_exit.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__
#include <unistd.h>

#ifdef _WIN32
volatile void _exit(int exit_code)
{
	__asm mov eax, __NR_exit
	__asm mov ebx, exit_code
	__asm int 0x80
}
#else
void _exit(int exit_code)
{
	__asm__ __volatile__("int $0x80"::"a"(__NR_exit), "b"(exit_code));
}
#endif /* _WIN32 */
