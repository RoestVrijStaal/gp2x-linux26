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

#define GPIOxALTFNHI(x)			__REGW(GPIO_START + 0x20 + GPIO_GROUP_OFFSET(x))
#define GPIOxALTFNLOW(x) 		__REGW(GPIO_START + GPIO_GROUP_OFFSET(x))


/* MMC/SD */
#define SDICON					__REGW(MMC_START + 0x00)	/* SDI Control Register */
#define SDICON_BYTE     		(1 << 4)        /* Byte Order Type */
#define SDICON_BYTE_LE			(0 << 4)        /* D[7:0],D[15:8],D[23:16],D[31:24] */
#define SDICON_BYTE_BE			(1 << 4)        /* D[31:24],D[23:16],D[15:8],D[7:0] */
#define SDICON_RCVIOINT			(1 << 3)        /* Receive SDIO Interrupt from card */
#define SDICON_RWE				(1 << 2)        /* Read Wait Enable */
#define SDICON_FRESET			(1 << 1)        /* FIFO Reset */
#define SDICON_ENCLK			(1 << 0)        /* Clock Out Enable */
#define SDIPRE					__REGW(MMC_START + 0x02)	/* SDI Prescaler Register */
#define SDICMDARG				__REG(MMC_START + 0x04)	/* SDI Command Argument Register */
#define SDICMDCON				__REGW(MMC_START + 0x08)	/* SDI Command Control Register */
#define SDICMDCON_ABTCMD		(1 << 12)       /* Command type: Abort(CMD12,CMD52) */
#define SDICMDCON_CMDDAT		(1 << 11)       /* Command type: with Data */
#define SDICMDCON_HOSTRCV		(1 << 10)       /* Response is 136-bit long */
#define SDICMDCON_HOSTWAITS		(1 << 9)        /* Wait for Response */
#define SDICMDCON_CMDOPST		(1 << 8)        /* 0: cmd ready, 1: cmd start */
#define SDICMDCON_CMDINDEX_MSK	(0xff)          /* with start 2bit */
#define SDICMDSTA 				__REGW(MMC_START + 0x0A)	/* SDI Command Status Register */
#define SDICMDSTA_RSPCRC		(1 << 12)
#define SDICMDSTA_CMDSENT		(1 << 11)
#define SDICMDSTA_CMDTOUT		(1 << 10)
#define SDICMDSTA_RSPFIN		(1 << 9)
#define SDICMDSTA_CMDON			(1 << 8)
#define SDICMDSTA_RSPINDEX		(0xff)
#define SDIRSP0					__REGW(MMC_START + 0x0c)	/* SDI Response Register 0 */
#define SDIRSP1					__REGW(MMC_START + 0x0e)	/* SDI Response Register 1 */
#define SDIRSP2					__REGW(MMC_START + 0x10)	/* SDI Response Register 2 */
#define SDIRSP3					__REGW(MMC_START + 0x12)	/* SDI Response Register 3 */
#define SDIRSP4					__REGW(MMC_START + 0x14)	/* SDI Response Register 4 */
#define SDIRSP5					__REGW(MMC_START + 0x16)	/* SDI Response Register 5 */
#define SDIRSP6					__REGW(MMC_START + 0x18)	/* SDI Response Register 6 */
#define SDIRSP7					__REGW(MMC_START + 0x1a)	/* SDI Response Register 7 */
#define SDIBSIZE				__REGW(MMC_START + 0x1e)	/* SDI Block Size Register */
#define SDIDATCON				__REG(MMC_START + 0x20)	/* SDI Data Control Register */
#define SDIDATCON_PRDTYPE 		(1 << 21)			/*  */
#define SDIDATCON_TARSP 		(1 << 20)			/* Transmit after response */
#define SDIDATCON_RACMD 		(1 << 19)			/* Receive after command */
#define SDIDATCON_BLKMODE 		(1 << 17)			/* Rx or Tx mode */
#define SDIDATCON_WIDE 			(1 << 16)			/* SD 4 wide SDIDAT[0:3], MMC 1 wide SDIDAT[0:1] */
#define SDIDATCON_DATMODE_RS	(2 << 12)			/* Data Mode Receive Start */
#define SDIDATCON_DATMODE_TS	(3 << 12)			/* Data Mode Transmit Start */
#define SDIDATCON_STOPMODE		(1 << 14)			/* Determines whether data transfer was stopped by force */
#define SDIDATCON_DMAMODE		(1 << 15)			/* ERR: Use DMA Mode, this isnt documented */
#define SDIDATCNT				__REG(MMC_START + 0x24)		/* Data Remain Counter Register */
#define SDIDATCNT_BLKNUMCNT		(0x00fff800)		/* Remaining block number */
#define SDIDATCNT_BLKCNT		(0x000007ff)		/* Remaining data of 1 block */
#define SDIDATSTA 				__REGW(MMC_START + 0x28)	/* Data Status Register */
#define SDIDATSTA_DATTOUT		(1 << 5)					/* data time out */
#define SDIFSTA					__REGW(MMC_START + 0x2A)	/* FIFO Status Register */
#define SDIFSTA_RFDET			(1 << 12)
#define SDIFSTA_RFLAST			(1 << 9)
#define SDIFSTA_RFFULL			(1 << 8)
#define SDIFSTA_RFHALF			(1 << 7) 
#define SDIDAT 					__REGB(MMC_START + 0x2C)	/* SDI Data Register */
#define SDIINTENB1 				__REGW(MMC_START + 0x30)	/* SDI Interrupt Mask Register 1 */
#define SDIINTENB1_TOUTMSK		(1 << 15)	/* Response timeout occur interrupt mask */
#define SDIINTENB1_CMDRCVMSK	(1 << 14) 	/* Command response receive interrupt mask */
#define SDIINTENB1_RWRMSK		(1 << 13) 	/* Read wait request occur interrupt mask */
#define SDIINTENB1_SDIOIRQMSK	(1 << 12) 	/* SDIO interrupt from the card occur interrupt mask */
#define SDIINTENB1_FFFAILMSK	(1 << 11) 	/* FIFO fail error occur interrupt mask */
#define SDIINTENB1_SCRCERRMSK	(1 << 10) 	/* CRC status error occur interrupt mask */
#define SDIINTENB1_DCRCERRMSK	(1 << 9) 	/* Data receive CRC error occur interrupt mask */
#define SDIINTENB1_DTIMOUTMSK	(1 << 8) 	/* Data receive timeout occur interrupt mask */
#define SDIINTENB1_DCNT0MSK		(1 << 7) 	/* Data counter zero interrupt mask */
#define SDIINTENB1_BUSCHKMSK	(1 << 6) 	/* Busy check complete interrupt mask */
#define SDIINTENB1_SB_ERRMSK	(1 << 5) 	/* Start bit error detect interrupt mask */
#define SDIINTENB1_TXHFMSK 		(1 << 4) 	/* Tx FIFO half full interrupt mask */
#define SDIINTENB1_TXEMTMSK 	(1 << 3) 	/* Tx FIFO empty interrupt mask */
#define SDIINTENB1_RXLDMSK 		(1 << 2) 	/* Rx FIFO has last data interrupt mask */
#define SDIINTENB1_RXFULMSK 	(1 << 1) 	/* Rx FIFO full interrupt mask */
#define SDIINTENB1_RXHFMSK 		(1 << 0) 	/* Rx FIFO half full interrupt mask */
#define SDIINTENB0 				__REGW(MMC_START + 0x32)	/* SDI Interrupt Mask Register 0 */
#define SDIINTENB0_CMDSNTMSK 	(1 << 0)	/* Command Sent */
#define SDIINTENB0_RESCRCMSK 	(1 << 1)	/* Response CRC check */
#define SDIDTIMERL	__REGW(MMC_START + 0x36)	/* SDI Data Timer Register */
#define SDIDTIMERH	__REGW(MMC_START + 0x38)	/* SDI Data Timer Register */
#define SDISFTL		__REGW(MMC_START + 0x3A)	/* SDI Shift Regisrt Low */
#define SDISFTH		__REGW(MMC_START + 0x3C)	/* SDI Shift Regisrt High */

/* Clocks and Power */
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
