#ifdef _WIN32
#include <linux/head.h>

static __inline void move_to_user_mode()
{
	__asm mov	eax, esp
	__asm push	0x17
	__asm push	eax
	__asm pushfd
	__asm push	0x0F
	__asm push	LN1
	__asm iretd
LN1 :
	__asm mov	eax, 0x17
	__asm mov	ds, ax
	__asm mov	es, ax
	__asm mov	fs, ax
	__asm mov	gs, ax
}

#define sti()	__asm sti
#define cli()	__asm cli
#define nop()	__asm nop

#define iret()	__asm iretd

static __inline void _set_gate(desc_table gate_addr, int type, int dpl, void *addr)
{
	gate_addr->a = ((unsigned long)addr & 0x0000FFFF) + \
		0x00080000;
	gate_addr->b = ((unsigned long)addr & 0xFFFF0000) + \
		(0x8000 + (dpl << 13) + (type << 8));
}

#define set_intr_gate(n,addr) \
	_set_gate(&idt[n],14,0,addr)

#define set_trap_gate(n,addr) \
	_set_gate(&idt[n],15,0,addr)

#define set_system_gate(n,addr) \
	_set_gate(&idt[n],15,3,addr)

static __inline void _set_tssldt_desc(char *n, void *addr, char type)
{
	*(short*)n = 0x68;
	*(short*)(n + 2) = (unsigned long)addr & 0xFFFF;
	*(n + 4) = ((unsigned long)addr >> 16) & 0xFF;
	*(n + 5) = type;
	*(n + 6) = 0;
	*(n + 7) = (char)(((unsigned long)addr >> 28));
}

#define set_tss_desc(n,addr) _set_tssldt_desc(((char*)(n)), addr, 0x89)
#define set_ldt_desc(n,addr) _set_tssldt_desc(((char*)(n)), addr, 0x82)

#else /* _WIN32 */

#define move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t" \
	"pushl $0x17\n\t" \
	"pushl %%eax\n\t" \
	"pushfl\n\t" \
	"pushl $0x0f\n\t" \
	"pushl $1f\n\t" \
	"iret\n" \
	"1:\tmovl $0x17,%%eax\n\t" \
	"movw %%ax,%%ds\n\t" \
	"movw %%ax,%%es\n\t" \
	"movw %%ax,%%fs\n\t" \
	"movw %%ax,%%gs" \
	:::"ax")

#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define nop() __asm__ ("nop"::)

#define iret() __asm__ ("iret"::)

#define _set_gate(gate_addr,type,dpl,addr) \
__asm__ ("movw %%dx,%%ax\n\t" \
	"movw %0,%%dx\n\t" \
	"movl %%eax,%1\n\t" \
	"movl %%edx,%2" \
	: \
	: "i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
	"o" (*((char *) (gate_addr))), \
	"o" (*(4+(char *) (gate_addr))), \
	"d" ((char *) (addr)),"a" (0x00080000))

#define set_intr_gate(n,addr) \
	_set_gate(&idt[n],14,0,addr)

#define set_trap_gate(n,addr) \
	_set_gate(&idt[n],15,0,addr)

#define set_system_gate(n,addr) \
	_set_gate(&idt[n],15,3,addr)

#define _set_seg_desc(gate_addr,type,dpl,base,limit) {\
	*(gate_addr) = ((base) & 0xff000000) | \
		(((base) & 0x00ff0000)>>16) | \
		((limit) & 0xf0000) | \
		((dpl)<<13) | \
		(0x00408000) | \
		((type)<<8); \
	*((gate_addr)+1) = (((base) & 0x0000ffff)<<16) | \
		((limit) & 0x0ffff); }

#define _set_tssldt_desc(n,addr,type) \
__asm__ ("movw $104,%1\n\t" \
	"movw %%ax,%2\n\t" \
	"rorl $16,%%eax\n\t" \
	"movb %%al,%3\n\t" \
	"movb $" type ",%4\n\t" \
	"movb $0x00,%5\n\t" \
	"movb %%ah,%6\n\t" \
	"rorl $16,%%eax" \
	::"a" (addr), "m" (*(n)), "m" (*(n+2)), "m" (*(n+4)), \
	 "m" (*(n+5)), "m" (*(n+6)), "m" (*(n+7)) \
	)

#define set_tss_desc(n,addr) _set_tssldt_desc(((char *) (n)),((int)(addr)),"0x89")
#define set_ldt_desc(n,addr) _set_tssldt_desc(((char *) (n)),((int)(addr)),"0x82")
#endif /* _WIN32 */