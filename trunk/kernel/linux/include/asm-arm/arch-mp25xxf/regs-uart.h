#ifndef MP25XXF_REGS_UART_H_
#define MP25XXF_REGS_UART_H_

/*============================================================================*
 *                                   UART                                     * 
 *============================================================================*/
/* Device Memory Start */
#define MP25XXF_UART0_START 			0xc0001200
#define MP25XXF_UART0_END 			0xc000121f
#define MP25XXF_UART1_START 			0xc0001220
#define MP25XXF_UART1_END 			0xc000123f
#define MP25XXF_UART2_START 			0xc0001240
#define MP25XXF_UART2_END 			0xc000125f
#define MP25XXF_UART3_START 			0xc0001260
#define MP25XXF_UART3_END 			0xc000127f
#define MP25XXF_UART_SIZE			(0x20)

/* Line Control Register */
#define LCONx(x)				__REGW(x)

/* Control Register */
#define UCONx(x) 				__REGW(x + 0x2)
#define UCONn(n) 				__REGW(0xc0001202 + (0x20 * n))
#define UCON_TX_INT 				(1 << 9)
#define UCON_RX_INT 				(1 << 8)
#define UCON_RX_TIMEOUT 			(1 << 7)
#define UCON_RX_ERRSTATUS 			(1 << 6)
#define UCON_LOOPBACK_MODE			(1 << 5)
#define UCON_SEND_BREAK 			(1 << 4)
#define UCON_TRANS_MODE				(3 << 2)
#define UCON_RECEIVE_MODE			(3 << 0)
#define UCON_RECEIVE_MODE_DMA 			(1)
#define UCON_RECEIVE_MODE_IRQ 			(2)

/* Fifo Control Register */
#define FCONx(x) 				__REGW(x + 0x4)

/* Modem Control Register */
#define MCONx(x) 				__REGW(x + 0x6)
#define MCONn(n) 				__REGW(0xc0001206 + (0x20 * n))
#define MCON_AFC 				(1 << 4)
#define MCON_MODEM_INT				(1 << 3)
#define MCON_DTR_ACTIVE				(1 << 1)
#define MCON_RTS_ACTIVE				(1 << 0)

/* Tx/Rx Status Register */
#define TRSTATUSx(x) 				__REGW(x + 0x8)
#define TRSTATUS_TRANSMITTER_EMPTY 		(1 << 2)
#define TRSTATUS_TRANSMIT_BUFFER_EMPTY 		(1 << 1)
#define TRSTATUS_RECEIVE_BUFFER_DATA_READY 	(1 << 0)

/* Error Status */
#define ESTATUSx(x) 				__REGW(x + 0xa)
#define ESTATUS_BREAK_DETECT			(1 << 3)
#define ESTATUS_FRAME_ERROR			(1 << 2)
#define ESTATUS_PARITY_ERROR			(1 << 1)
#define ESTATUS_OVERRUN_ERROR			(1 << 0)
#define ESTATUS_ANY 				(ESTATUS_OVERRUN_ERROR | \
						ESTATUS_FRAME_ERROR | \
						ESTATUS_BREAK_DETECT)


/* Fifo Status */
#define FSTATUSx(x) 				__REGW(x + 0xc)
#define FSTATUS_RX_FIFO_ERROR 			(1 << 10)
#define FSTATUS_TX_FIFO_FULL 			(1 << 9)
#define FSTATUS_RX_FIFO_FULL 			(1 << 8)
#define FSTATUS_TX_FIFO_COUNT 			(0xf << 4)
#define FSTATUS_RX_FIFO_COUNT 			(0xf << 0)


/* Modem Status */
#define MSTATUSx(x) 				__REGW(x + 0xe)
#define MSTATUS_DELTA_DCD			(1 << 7)
#define MSTATUS_DELTA_RI			(1 << 6)
#define MSTATUS_DELTA_DSR			(1 << 5)
#define MSTATUS_DELTA_CTS			(1 << 4)
#define MSTATUS_DCD				(1 << 3)
#define MSTATUS_RI				(1 << 2)
#define MSTATUS_DSR				(1 << 1)
#define MSTATUS_CTS				(1 << 0)

/* Transmit Buffer Register */
#define THBx(x) 				__REGW(x + 0x10)

/* Receive Buffer Register */
#define RHBx(x) 				__REGW(x + 0x12)

 /* Interrupt Status */
#define INTSTATREG 				__REGW(0xc0001280)

#endif
