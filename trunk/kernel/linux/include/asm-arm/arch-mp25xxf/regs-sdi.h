#ifndef MP25XXF_REGS_SDI_H_
#define MP25XXF_REGS_SDI_H_

/*============================================================================*
 *                                 SD/MMC                                     * 
 *============================================================================*/
/* MMC/SD, start: C000 1500 end: C000 153C */
#define MMC_START	0xc0001500
#define MMC_END		0xc000153c
#define MMC_BASE	io_p2v(MMC_START)

/* SDI Control Register */
#define SDICON					__REGW(MMC_START + 0x00)
#define SDICON_BYTE				(1 << 4) 	/* Byte Order Type */
#define SDICON_BYTE_LE				(0 << 4)	/* D[7:0],D[15:8],D[23:16],D[31:24] */
#define SDICON_BYTE_BE				(1 << 4)	/* D[31:24],D[23:16],D[15:8],D[7:0] */
#define SDICON_RCVIOINT				(1 << 3)	/* Receive SDIO Interrupt from card */
#define SDICON_RWE				(1 << 2)	/* Read Wait Enable */
#define SDICON_FRESET				(1 << 1)	/* FIFO Reset */
#define SDICON_ENCLK				(1 << 0)	/* Clock Out Enable */

/* SDI Prescaler Register */
#define SDIPRE					__REGW(MMC_START + 0x02)

/* SDI Command Argument Register */
#define SDICMDARG				__REG(MMC_START + 0x04)	

/* SDI Command Control Register */
#define SDICMDCON				__REGW(MMC_START + 0x08)
#define SDICMDCON_ABTCMD			(1 << 12) 	/* Command type: Abort(CMD12,CMD52) */
#define SDICMDCON_CMDDAT			(1 << 11)	/* Command type: with Data */
#define SDICMDCON_HOSTRCV			(1 << 10)	/* Response is 136-bit long */
#define SDICMDCON_HOSTWAITS			(1 << 9)	/* Wait for Response */
#define SDICMDCON_CMDOPST			(1 << 8)	/* 0: cmd ready, 1: cmd start */
#define SDICMDCON_CMDINDEX_MSK			(0xff)		/* with start 2bit */

/* SDI Command Status Register */
#define SDICMDSTA 				__REGW(MMC_START + 0x0A)
#define SDICMDSTA_RSPCRC			(1 << 12)
#define SDICMDSTA_CMDSENT			(1 << 11)
#define SDICMDSTA_CMDTOUT			(1 << 10)
#define SDICMDSTA_RSPFIN			(1 << 9)
#define SDICMDSTA_CMDON				(1 << 8)
#define SDICMDSTA_RSPINDEX			(0xff)

/* SDI Response Register 0 */
#define SDIRSP0					__REGW(MMC_START + 0x0c)

/* SDI Response Register 1 */
#define SDIRSP1					__REGW(MMC_START + 0x0e)

/* SDI Response Register 2 */
#define SDIRSP2					__REGW(MMC_START + 0x10)

/* SDI Response Register 3 */
#define SDIRSP3					__REGW(MMC_START + 0x12)

/* SDI Response Register 4 */
#define SDIRSP4					__REGW(MMC_START + 0x14)

/* SDI Response Register 5 */
#define SDIRSP5					__REGW(MMC_START + 0x16)

/* SDI Response Register 6 */
#define SDIRSP6					__REGW(MMC_START + 0x18)

/* SDI Response Register 7 */
#define SDIRSP7					__REGW(MMC_START + 0x1a)

/* SDI Block Size Register */
#define SDIBSIZE				__REGW(MMC_START + 0x1e)

/* SDI Data Control Register */
#define SDIDATCON				__REG(MMC_START + 0x20)
#define SDIDATCON_PRDTYPE 			(1 << 21)	
#define SDIDATCON_TARSP 			(1 << 20)	/* Transmit after response */
#define SDIDATCON_RACMD 			(1 << 19)	/* Receive after command */
#define SDIDATCON_BLKMODE 			(1 << 17)	/* Rx or Tx mode */
#define SDIDATCON_WIDE 				(1 << 16)	/* SD 4 wide SDIDAT[0:3], MMC 1 wide SDIDAT[0:1] */
#define SDIDATCON_DATMODE_RS			(2 << 12)	/* Data Mode Receive Start */
#define SDIDATCON_DATMODE_TS			(3 << 12)	/* Data Mode Transmit Start */
#define SDIDATCON_STOPMODE			(1 << 14)	/* Determines whether data transfer was stopped by force */
#define SDIDATCON_DMAMODE			(1 << 15)	/* ERR: Use DMA Mode, this isnt documented */

/* Data Remain Counter Register */
#define SDIDATCNT				__REG(MMC_START + 0x24)
#define SDIDATCNT_BLKNUMCNT			(0x00fff800)
#define SDIDATCNT_BLKCNT			(0x000007ff)

/* Data Status Register */
#define SDIDATSTA 				__REGW(MMC_START + 0x28)	
#define SDIDATSTA_RWREQ				(1 << 10)
#define SDIDATSTA_SDIOINT			(1 << 9)
#define SDIDATSTA_FFFAIL			(1 << 8)
#define SDIDATSTA_CRCSTA			(1 << 7)
#define SDIDATSTA_DATCRC			(1 << 6)
#define SDIDATSTA_DATTOUT			(1 << 5)
#define SDIDATSTA_DATFIN 			(1 << 4)
#define SDIDATSTA_BUSYFIN			(1 << 3)
#define SDIDATSTA_SBITERR			(1 << 2)
#define SDIDATSTA_TXDATON			(1 << 1)
#define SDIDATSTA_RXDATON			(1 << 0)

#define SDIFSTA					__REGW(MMC_START + 0x2A)	
#define SDIFSTA_RFDET				(1 << 12)
#define SDIFSTA_RFLAST				(1 << 9)
#define SDIFSTA_RFFULL				(1 << 8)
#define SDIFSTA_RFHALF				(1 << 7)

/* SDI Data Register */
#define SDIDAT 					__REGB(MMC_START + 0x2C)	

/* SDI Interrupt Mask Register 1 */
#define SDIINTENB1 				__REGW(MMC_START + 0x30)	
#define SDIINTENB1_TOUTMSK			(1 << 15)	/* Response timeout occur interrupt mask */
#define SDIINTENB1_CMDRCVMSK			(1 << 14) 	/* Command response receive interrupt mask */
#define SDIINTENB1_RWRMSK			(1 << 13) 	/* Read wait request occur interrupt mask */
#define SDIINTENB1_SDIOIRQMSK			(1 << 12) 	/* SDIO interrupt from the card occur interrupt mask */
#define SDIINTENB1_FFFAILMSK			(1 << 11) 	/* FIFO fail error occur interrupt mask */
#define SDIINTENB1_SCRCERRMSK			(1 << 10) 	/* CRC status error occur interrupt mask */
#define SDIINTENB1_DCRCERRMSK			(1 << 9) 	/* Data receive CRC error occur interrupt mask */
#define SDIINTENB1_DTIMOUTMSK			(1 << 8) 	/* Data receive timeout occur interrupt mask */
#define SDIINTENB1_DCNT0MSK			(1 << 7) 	/* Data counter zero interrupt mask */
#define SDIINTENB1_BUSCHKMSK			(1 << 6) 	/* Busy check complete interrupt mask */
#define SDIINTENB1_SB_ERRMSK			(1 << 5) 	/* Start bit error detect interrupt mask */
#define SDIINTENB1_TXHFMSK 			(1 << 4) 	/* Tx FIFO half full interrupt mask */
#define SDIINTENB1_TXEMTMSK 			(1 << 3) 	/* Tx FIFO empty interrupt mask */
#define SDIINTENB1_RXLDMSK 			(1 << 2) 	/* Rx FIFO has last data interrupt mask */
#define SDIINTENB1_RXFULMSK 			(1 << 1) 	/* Rx FIFO full interrupt mask */
#define SDIINTENB1_RXHFMSK 			(1 << 0) 	/* Rx FIFO half full interrupt mask */

/* SDI Interrupt Mask Register 0 */
#define SDIINTENB0 				__REGW(MMC_START + 0x32)
#define SDIINTENB0_CMDSNTMSK 			(1 << 0)	/* Command Sent */
#define SDIINTENB0_RESCRCMSK 			(1 << 1)	/* Response CRC check */

/* SDI Data Timer Register */
#define SDIDTIMERL				__REGW(MMC_START + 0x36)
/* SDI Data Timer Register */
#define SDIDTIMERH				__REGW(MMC_START + 0x38)

/* SDI Shift Register Low */
#define SDISFTL					__REGW(MMC_START + 0x3a)

/* SDI Shift Register High */
#define SDISFTH					__REGW(MMC_START + 0x3c)

#endif
