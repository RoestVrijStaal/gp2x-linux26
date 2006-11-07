#ifndef IO_H_
#define IO_H_

#define IO_SPACE_LIMIT	0xffffffff

/* this is needed for inb/outb support */
#define __io(a)			((void __iomem *)(a))
#define __mem_pci(a)	((unsigned long)(a))
#define __mem_isa(a)	((unsigned long)(a))


#endif /*IO_H_*/
