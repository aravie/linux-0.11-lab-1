/*
 *  linux/lib/string.c
 *
 *  (C) 1991  Linus Torvalds
 */

#ifdef _WIN32
#ifndef _MSC_VER
#error I want msc!
#endif
#else /* _WIN32 */
#ifndef __GNUC__
#error I want gcc!
#endif
#endif /* _WIN32 */

#define extern
#define inline
#define __LIBRARY__
#include <string.h>
