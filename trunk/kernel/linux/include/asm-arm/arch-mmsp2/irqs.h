#ifndef IRQS_H_
#define IRQS_H_

/* main irqs */
#define MAIN_IRQ_BASE	0
#define IRQ_DISP		0  /* Display controller */
#define IRQ_IMGH		1  /* Image capture controller (HSYNC) */
#define IRQ_IMGV		2  /* Image capture controller (VSYNC) */ 
/* IRQ 3 reserved */ 
/* IRQ 4 reserved */
#define IRQ_TIMER		5  /* Timer -> Timer0,1,2,3*/
#define IRQ_MSTICK		6  /* Memory stick */
#define IRQ_SSP			7  /* SSP */
#define IRQ_PPM			8  /* PPM */
#define IRQ_DMA			9  /* DMA -> DMA ch0~15*/
#define IRQ_UART		10 /* UART -> UART 0,1,2,3 */
#define IRQ_GRP2D		11 /* 2D graphic controller */
#define IRQ_SCALER		12 /* Image scaler */
#define IRQ_USBH		13 /* USB Host */
#define IRQ_SD			14 /* MMC/SD */
#define IRQ_USBD		15 /* USB device */
#define IRQ_RTC			16 /* RTC */
#define IRQ_ADC			17 /* ADC */
#define IRQ_IIC			18 /* IIC */
#define IRQ_AC97		19 /* AC97 */
#define IRQ_IRDA		20 /* IrDA */
/* IRQ 21 reserved */ 
/* IRQ 22 reserved */
#define IRQ_GPIO		23 /* GPIO -> GPIOA[15:0]~GPIOO[15:0]*/
#define IRQ_CDROM		24 /* CDROM */
#define IRQ_OWM			25 /* One wire master */
#define IRQ_DUALCPU		26 /* Dual CPU */
#define IRQ_MCUC		27 /* Non DRAM Memory controller */
#define IRQ_VLD			28 /* VLD */
#define IRQ_VIDEO		29 /* Video processor */
#define IRQ_MPEGIF		30 /* MPEG Interface */
#define IRQ_IIS			31 /* IIS controller */

#define MMSP2_IRQS		(32)

/* TIMER sub irqs */
#define IRQ_TIMER_START		MMSP2_IRQS
#define IRQ_TIMER_END		(IRQ_TIMER_START + 4)
#define IRQ_TIMER_0			(0 + IRQ_TIMER_START)
#define IRQ_TIMER_1			(1 + IRQ_TIMER_START)
#define IRQ_TIMER_2			(2 + IRQ_TIMER_START)
#define IRQ_TIMER_3			(3 + IRQ_TIMER_START)

/* UART sub irqs */
#define IRQ_UART_START		IRQ_TIMER_END
#define IRQ_UART_END		(IRQ_UART_START + 16)

#define IRQ_UART_TXD0 		(0 + IRQ_UART_START)
#define IRQ_UART_RXD0 		(1 + IRQ_UART_START)
#define IRQ_UART_ERROR0 	(2 + IRQ_UART_START)
#define IRQ_UART_MODEM0 	(3 + IRQ_UART_START)
#define IRQ_UART_TXD1 		(4 + IRQ_UART_START)
#define IRQ_UART_RXD1 		(5 + IRQ_UART_START)
#define IRQ_UART_ERROR1 	(6 + IRQ_UART_START)
#define IRQ_UART_MODEM1 	(7 + IRQ_UART_START)
#define IRQ_UART_TXD2   	(8 + IRQ_UART_START)
#define IRQ_UART_RXD2   	(9 + IRQ_UART_START)
#define IRQ_UART_ERROR2 	(10 + IRQ_UART_START)
#define IRQ_UART_MODEM2 	(11 + IRQ_UART_START)
#define IRQ_UART_TXD3   	(12 + IRQ_UART_START)
#define IRQ_UART_RXD3   	(13 + IRQ_UART_START)
#define IRQ_UART_ERROR3 	(14 + IRQ_UART_START)
#define IRQ_UART_MODEM3 	(15 + IRQ_UART_START)

/* TODO: DMA GPIO */
#define NR_IRQS	(1 + IRQ_UART_END)

#endif /*IRQS_H_*/
