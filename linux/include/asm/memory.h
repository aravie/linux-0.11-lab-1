/*
 *  NOTE!!! memcpy(dest,src,n) assumes ds=es=normal data segment. This
 *  goes for all kernel functions (ds=es=kernel space, fs=local data,
 *  gs=null), as well as for all well-behaving user programs (ds=es=
 *  user data space). This is NOT a bug, as any user program that changes
 *  es deserves to die if it isn't careful.
 */
#ifdef _WIN32
static __inline void* memcpy(void *dest, void *src, int n)
{
	void *_res = dest;

	__asm mov	edi, _res
	__asm mov	esi, src
	__asm mov	ecx, n
	__asm cld
	__asm rep	movsb

	return _res;
}
#else
#define memcpy(dest,src,n) ({ \
void * _res = dest; \
__asm__ ("cld;rep;movsb" \
	::"D" ((long)(_res)),"S" ((long)(src)),"c" ((long) (n)) \
	); \
_res; \
})
#endif /* _WIN32 */