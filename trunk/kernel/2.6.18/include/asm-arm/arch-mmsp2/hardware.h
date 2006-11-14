#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "mmsp2-regs.h"

/* when the shadow pin is set the offset is 0x8000000 */
#define MMSP2_SHADOW_OFFSET 0x80000000

#ifdef CONFIG_MMSP2_SHADOW_ENABLE
        #define MMSP2_IO(x) ((x) + MMSP2_SHADOW_OFFSET)
#else
        #define MMSP2_IO(x) ((x))
#endif

/* mapping:
 * we can do this because the length of each area isnt bigger than 0x10000
 * basically we map everything starting at 0xf0000000.
 * 
 * 0x1c000000 or 0x9c000000 => 0xf1c0000 or 0xf9c00000
 */

#define io_p2v(x) (((((x) & 0xff000000) >> 4) | 0xf0000000) | ((x) & 0x000ffff))
#define io_v2p(x) ((((x) & 0x0ff00000) << 4) | ((x) & 0x000fffff)) 
/* macro to get at IO space when running virtually  3 | c = f */
/*#define IO_ADDRESS(x) ((x) | 0x30000000)*/

#ifndef __ASSEMBLY__
	#define __REG(x)	(*((volatile u32 *)io_p2v(x)))
	#define __REGW(x)	(*((volatile u16 *)io_p2v(x)))
	#define __REGB(x)	(*((volatile u8 *)io_p2v(x)))
#endif


/* NAND flash IO space */
#define NF_IO_START     MMSP2_IO(0x1c000000)
#define NF_IO_SIZE      0x00010000
#define NF_IO_BASE      io_p2v(NF_IO_START)
/* PCMCIA0 IO space */
#define CF0_IO_START    MMSP2_IO(0x20000000)
#define CF0_IO_SIZE     0x00010000
#define CF0_IO_BASE     io_p2v(CF0_IO_START)
/* PCMCIA0 ATTR space */
#define CF0_ATTR_START  MMSP2_IO(0x24000000)
#define CF0_ATTR_SIZE   0x00010000
#define CF0_ATTR_BASE   io_p2v(CF0_ATTR_START)
/* PCMCIA0 MEM space */
#define CF0_MEM_START   MMSP2_IO(0x28000000)
#define CF0_MEM_SIZE    0x00010000
#define CF0_MEM_BASE    io_p2v(CF0_MEM_START)
/* 0x2c000000 reserved */
/* PCMCIA1 IO space */
#define CF1_IO_START    MMSP2_IO(0x30000000)
#define CF1_IO_SIZE     0x00010000
#define CF1_IO_BASE     io_p2v(CF1_IO_START)
/* PCMCIA1 ATTR space */
#define CF1_ATTR_START  MMSP2_IO(0x34000000)
#define CF1_ATTR_SIZE   0x00010000
#define CF1_ATTR_BASE   io_p2v(CF1_ATTR_START)
/* PCMCIA1 MEM space */
#define CF1_MEM_START   MMSP2_IO(0x38000000)
#define CF1_MEM_SIZE    0x00010000
#define CF1_MEM_BASE    io_p2v(CF1_MEM_START)
/* IDE IO space */
#define IDE_IO_START    MMSP2_IO(0x3c000000)
#define IDE_IO_SIZE     0x00010000
#define IDE_IO_BASE     io_p2v(IDE_IO_START)
/* the NIO and the FIO aren't modified by the SHADOW pin */
/* Normal IO space */
#define NIO_START	0xc0000000
#define NIO_SIZE	0x00010000
#define NIO_BASE	io_p2v(NIO_START)
/* Fast IO space */
#define FIO_START	0xe0000000
#define FIO_SIZE 	0x00010000
#define FIO_BASE	io_p2v(FIO_START)

#endif /*HARDWARE_H_*/
