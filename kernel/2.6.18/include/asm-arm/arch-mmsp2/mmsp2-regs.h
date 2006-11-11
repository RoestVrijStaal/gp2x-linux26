#ifndef MMSP2REGS_H_
#define MMSP2REGS_H_

/* Interrupt */
#define SRCPEND		__REG(0xc0000800)
#define INTMOD		__REG(0xc0000804)
#define INTMASK		__REG(0xc0000808)
#define IPRIORITY	__REG(0xc000080c)
#define INTPEND		__REG(0xc0000810)
#define INTOFFSET	__REG(0xc0000814)

/* Timer / Watch-dog */
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

/* UART */
#define UINTSTAT		__REGW(0xc0001280)

#endif /*MMSP2REGS_H_*/
