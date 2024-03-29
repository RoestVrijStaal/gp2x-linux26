#ifndef MMSP2REGS_H_
#define MMSP2REGS_H_

/* TODO remove this file */

/* Interrupt */
#define SRCPEND		__REG(0xc0000800)
#define INTMOD		__REG(0xc0000804)
#define INTMASK		__REG(0xc0000808)
#define IPRIORITY	__REG(0xc000080c)
#define INTPEND		__REG(0xc0000810)
#define INTOFFSET	__REG(0xc0000814)

/* BANK C (Static) Memory Control Register */
#define MEMNANDCTRL     __REGW(0xc0003a3a)      /* NAND FLASH Control */
#define MEMNANDTIME     __REGW(0xc0003a3c)      /* NAND FLASH Timing */
#define MEMNANDECC0     __REGW(0xc0003a3e)      /* NAND FLASH ECC0 */
#define MEMNANDECC1     __REGW(0xc0003a40)      /* NAND FLASH ECC1 */
#define MEMNANDECC2     __REGW(0xc0003a42)      /* NAND FLASH ECC2 */
#define MEMNANDCNT      __REGW(0xc0003a44)      /* NAND FLASH Data Counter */

#define NFDATA          __REGW(NF_IO_START + 0x00)   /* NAND FLASH Data */
#define NFCMD           __REGB(NF_IO_START + 0x10)   /* NAND FLASH Command */
#define NFADDR          __REGB(NF_IO_START + 0x18)   /* NAND FLASH Address */
#define NAND_READY_DETECT       (1 << 15)
#define NAND_IRQEN                      (1 << 6)
#define NAND_RNB                        (1 << 7)


/* ==== Timer / Watch-dog  ==== */
#define TCOUNT			__REG(0xc0000a00)
#define TMATCH0			__REG(0xc0000a04)
#define TMATCH1			__REG(0xc0000a08)
#define TMATCH2			__REG(0xc0000a0c)
#define TMATCH3			__REG(0xc0000a10)
#define TCONTROL		__REGW(0xc0000a14)
#define TSTATUS			__REGW(0xc0000a16)
#define TINTEN			__REGW(0xc0000a18)

#define TCNT0			(1 << 0)
#define TCNT1			(1 << 1)
#define TCNT2			(1 << 2)
#define TCNT3			(1 << 3)

#define TIMER_EN		(1 << 0)
#define WDT_EN			(1 << 1)




/**
 * Clocks and Power Registers 
 */
#define FPLLVSETREG __REGW(CPW_START + 0x12)
#define UPLLVSETREG __REGW(CPW_START + 0x16)
#define APLLVSETREG __REGW(CPW_START + 0x1a)
#define xMDIV_MASK	0xff00
#define xPDIV_MASK	0x00fc
#define xSDIV_MASK	0x0003
#define SYSCSETREG	__REGW(CPW_START + 0x1c)
#define SYSCSETREG_DCLKDIV_MASK 0x01f000

#define GET_MDIV(x)	((x & xMDIV_MASK) >> 8)
#define GET_PDIV(x)	((x & xPDIV_MASK) >> 2)
#define GET_SDIV(x)	(x & xSDIV_MASK)
#define GET_DDIV	((SYSCSETREG & SYSCSETREG_DCLKDIV_MASK) >> 6)






#endif /*MMSP2REGS_H_*/
