#ifndef MMSP2REGS_H_
#define MMSP2REGS_H_

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

/* GPIO
 * 189 pin for GPIO divided in 15 groups:
 * group	#pins	high		pins	low			pins
 * GPIOA	16		0xc0001040	15:8	0xc0001020	7:0
 * GPIOB	16		0xc0001042	15:8	0xc0001022	7:0
 * GPIOC	16		0xc0001044	15:8	0xc0001024	7:0
 * GPIOD	14		0xc0001046	11:8	0xc0001026	7:0
 * GPIOE	16
 * GPIOF	10
 * GPIOG	16
 * GPIOH	7		0xc000104e	-		0xc000104e	7:0	(W)
 * 					0xc000104e	6:3		0xc000102e	3:0 (R)
 * GPIOI	16
 * GPIOJ	16
 * GPIOK	8		0xc0001054	-		0xc0001034	7:0 (W)
 * 					0xc0001054	7:4		0xc0001034	3:0 (R)
 * GPIOL	15		0xc0001056	14:7	0xc0001036	7:0
 * GPIOM	9
 * GPION	8
 * GPIOO	6
 *
 * length in bits:
 * group = 4
 * pin = 4
 * mode = 2
 * 
 * 00 = gpio input
 * 01 = gpio output
 * 10 = gpio alt function 1
 * 11 = gpio alt function 2
 * 
 */

#define GPIO_FN_INPUT	0
#define GPIO_FN_OUTPUT	1	
#define GPIO_FN_ALT1	2	
#define GPIO_FN_ALT2	3	

#define GPIOA	1
#define GPIOB	2	
#define GPIOC	3
#define GPIOD	4
#define GPIOE	5
#define GPIOF	6
#define GPIOG	7
#define GPIOH	8
#define GPIOI	9
#define GPIOJ	10
#define GPIOK	11
#define GPIOL	12
#define GPIOM	13
#define GPION	14
#define GPIOO	15
#define GPIO_GROUP_OFFSET(x)	(x << 2)

#define GPIOxALTFNHI(x)			__REGW(GPIO_START + 0x20 GPIO_GROUP_OFFSET(x))
#define GPIOxALTFNLOW(x) 		__REGW(GPIO_START + GPIO_GROUP_OFFSET(x))


/* MMC/SD */
#define SDICON		__REGW(MMC_START + 0x00)	/* SDI Control Register */
#define SDIPRE		__REGW(MMC_START + 0x02)	/* SDI Prescaler Register */
#define SDICARG		__REG(MMC_START + 0x04)	/* SDI Command Argument Register */
#define SDICCON		__REGW(MMC_SART + 0x08)	/* SDI Command Control Register */
#define SDICSTA		__REGW(MMC_SART + 0x0A)	/* SDI Command Status Register */
#define SDIRSP0		__REG(MMC_START + 0x0C)	/* SDI Response Register 0 */
#define SDIRSP1		__REG(MMC_START + 0x10)	/* SDI Response Register 1 */
#define SDIRSP2		__REG(MMC_START +0x14)	/* SDI Response Register 2 */
#define SDIRSP3		__REG(MMC_START + 0x18)	/* SDI Response Register 3 */
#define SDIBSIZE	__REGW(MMC_START + 0x1E)	/* SDI Block Size Register */
#define SDIDCON		__REG(MMC_START + 0x20)	/* SDI Data Control Register */
#define SDIDCNT		__REG(MMC_START + 0x24)	/* SDI Data Remain Counter Register */
#define SDIDSTA		__REGW(MMC_START + 0x28)	/* SDI Data Status Register */
#define SDIFSTA		__REGW(MMC_START + 0x2A)	/* SDI FIFO Status Register */
#define SDIDAT		__REG(MMC_START + 0x2C)	/* SDI Data Register */
#define SDIIMSK1	__REG(MMC_START + 0x30)	/* SDI Interrupt Mask Register 1 */
#define SDIIMSK0	__REG(MMC_START + 0x32)	/* SDI Interrupt Mask Register 0 */
#define SDIDTIMERL	__REGW(MMC_START + 0x36)	/* SDI Data Timer Register */
#define SDIDTIMERH	__REGW(MMC_START + 0x38)	/* SDI Data Timer Register */
#define SDISFTL		__REGW(MMC_START + 0x3A)	/* SDI Shift Regisrt Low */
#define SDISFTH		__REGW(MMC_START + 0x3C)	/* SDI Shift Regisrt High */

/* Clocks and Power */
#define FPLLVSETREG __REGW(CPW_START + 0x12)
#define UPLLVSETREG __REGW(CPW_START + 0x16)
#define APLLVSETREG __REGW(CPW_START + 0x1a)

#define GET_MDIV(x)	((x & MDIV_MASK) >> 8)
#define GET_PDIV(x)	((x & PDIV_MASK) >> 2)
#define GET_SDIV(x)	(x & SDIV_MASK)
	
#define MDIV_MASK	0xFF00
#define PDIV_MASK	0x00FC
#define SDIV_MASK	0x0003


#endif /*MMSP2REGS_H_*/
