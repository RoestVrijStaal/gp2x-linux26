#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "mmsp2-regs.h"

/* macro to get at IO space when running virtually  3 | c = f */
#define IO_ADDRESS(x) ((x) | 0x30000000)

#ifndef __ASSEMBLY__
	#define __REG(x)	(*((volatile u32 *)IO_ADDRESS(x)))
	#define __REGW(x)	(*((volatile u16 *)IO_ADDRESS(x)))
#endif

/* Normal IO space */
#define NIO_START	0xc0000000
#define NIO_SIZE	0x00010000
#define NIO_BASE	0xf0000000
/* Fast IO space */
#define FIO_START	0xe0000000
#define FIO_SIZE 	0x00010000
#define FIO_BASE	0xf1000000

#endif /*HARDWARE_H_*/
