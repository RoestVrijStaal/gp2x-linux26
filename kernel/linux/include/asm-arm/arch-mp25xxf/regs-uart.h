#ifndef MP25XXF_REGS_UART_H_
#define MP25XXF_REGS_UART_H_

/*============================================================================*
 *                                   UART                                     * 
 *============================================================================*/
#define MP25XXF_UART_STARTx(x)	((0x20 * x) + 0x1200 + MP25XXF_NIO_START)
#define MP25XXF_UART_ENDx(x)		((0x20 * x) + 0x121f + MP25XXF_NIO_START)
#define MP25XXF_UART_SIZE		(0x20)

#define INTSTATREG 							__REGW(0xc0001280) /* Interrupt Status */
/* FIXME, make this relative? fix irq.c */
/* UART control (1202, 1222, 1242, 1262) */ 
#define UCONx(x)							__REGW(0xc0001202 + (0x20 * x)) 
/* Modem control (1206, 1226, 1246, 1266) */ 
#define UMODx(x)							__REGW(0xc0001206 + (0x20 * x))
/* the below registers are relative to the uart mapping base */
#define MCONx(x)							__REGW(x + 0x06) /* Modem Control */
#define MCON_AFC							(1 << 4)
#define MCON_MODEM_INT						(1 << 3)
#define MCON_DTR_ACTIVE						(1 << 1)
#define MCON_RTS_ACTIVE						(1 << 0)
#define TRSTATUSx(x)						__REGW(x + 0x08) /* Tx/Rx Status  */
#define TRSTATUS_TRANSMITTER_EMPTY 			(1 << 2)
#define TRSTATUS_TRANSMIT_BUFFER_EMPTY 		(1 << 1)
#define TRSTATUS_RECEIVE_BUFFER_DATA_READY 	(1 << 0)
#define ESTATUSx(x)							__REGW(x + 0x0a) /* Error Status  */
#define FSTATUSx(x)							__REGW(x + 0x0c) /* Fifo Status  */
#define FSTATUS_RX_FIFO_ERROR 				(1 << 10)
#define FSTATUS_TX_FIFO_FULL 				(1 << 9)
#define FSTATUS_RX_FIFO_FULL 				(1 << 8)
#define MSTATUSx(x)							__REGW(x + 0x0e) /* Modem Status  */
#define MSTATUS_DELTA_DCD					(1 << 7)
#define MSTATUS_DELTA_RI					(1 << 6)
#define MSTATUS_DELTA_DSR					(1 << 5)
#define MSTATUS_DELTA_CTS					(1 << 4)
#define MSTATUS_DCD							(1 << 3)
#define MSTATUS_RI							(1 << 2)
#define MSTATUS_DSR							(1 << 1)
#define MSTATUS_CTS							(1 << 0)
#define THBx(x)								__REGW(x + 0x10) /* Transfer register */


#endif
