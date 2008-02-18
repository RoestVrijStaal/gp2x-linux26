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
 * GPIOH	7		0xc000104e	-		0xc000102e	7:0	(W)
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

#define GPIOA	0x0
#define GPIOB	0x2	
#define GPIOC	0x4
#define GPIOD	0x6
#define GPIOE	0x8
#define GPIOF	0xa
#define GPIOG	0xc
#define GPIOH	0xe
#define GPIOI	0x10
#define GPIOJ	0x12
#define GPIOK	0x14
#define GPIOL	0x16
#define GPIOM	0x18
#define GPION	0x1a
#define GPIOO	0x1c

//#define GPIO_GROUP_OFFSET(x)	(x << 2)

/* Note there are other addresses for reading the alternate registers */
#define GPIOxALTFNHI(x)			__REGW(0xc0001020 + 0x20 + (x))
#define GPIOxALTFNLOW(x) 		__REGW(0xc0001020 + (x))
#define GPIOxOUT(x) 			__REGW(0xc0001060 + (x))  


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


/**
 * Display Controller Registers
 * 
 */

#define DPC_CLKCNTL 		__REGW(0xc0002848) 	/* Clock Control */
#define DPC_HS_WIDTH 		__REGW(0xc000281a) 	/* Horizontal Sync Width */
#define DPC_HS_STR 			__REGW(0xc000281c) 	/* Horizontal Sync Start */
#define DPC_HS_END 			__REGW(0xc000291e) 	/* Horizontal Sync End */

/**
 * Video Post Processor Registers
 * It has a multilayer controller that handles the following sources:
 * YUV Overlay Layer [drivers/v4l/mmsp2.c]
 * OSD
 * SPU (sub picture unit)
 * RGB Layer [drivers/fb/mmsp2fb.c] with 5 independent regions
 * 
 */

/* RGB Layer (8/16/24 bpp) */
#define MLC_STL_CNTL 		__REGW(0xc00028da)	/* Still Image Control */
#define MLC_STL_MIXMUX		__REGW(0xc00028dc) 	/* Mix/Mux Control */
#define MLC_STL_ALPHAL 		__REGW(0xc00028de) 	/* */ 
#define MLC_STL_ALPHAH 		__REGW(0xc00028e0) 	/* */ 
#define MLC_STLn_STX(n) 	__REGW(0xc000) 		/* Horizontal Start */ 
#define MLC_STLn_ENDX(n) 	__REGW(0xc000) 		/* Horizontal End */ 
#define MLC_STL_CKEY_GR 	__REGW(0xc0002902) 	/* */ 
#define MLC_STL_CKEY_B 		__REGW(0xc0002904) 	/* */ 
#define MLC_STL_HSC 		__REGW(0xc0002906) 	/* Horizontal Scale Factor */ 
#define MLC_STL_ __REGW(0xc000) /* TODO Scale Factor */ 
#define MLC_STL_HW 			__REGW(0xc000290c) 	/* Horizontal Width*/ 
#define MLC_STL_OADRL 		__REGW(0xc000290e) 	/* Source Odd Address Low */ 
#define MLC_STL_OADRH 		__REGW(0xc0002910) 	/* Source Odd Address High */ 
#define MLC_STL_EADRL 		__REGW(0xc0002912) 	/* Source Even Address Low */ 
#define MLC_STL_EADRH 		__REGW(0xc0002914) 	/* Source Even Address High */ 
#define MLC_STL_PALLTA 		__REGW(0xc0002958) 	/* Palette Table Index */ 
#define MLC_STL_PALLTD 		__REGW(0xc000295a) 	/* Palette Table Data */ 

/* Cursor Layer (2bpp) */

#endif /*MMSP2REGS_H_*/
