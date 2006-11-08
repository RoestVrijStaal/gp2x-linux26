#ifndef IRQS_H_
#define IRQS_H_

/* main irqs */
#define MAIN_IRQ_BASE	0
#define IRQ_DISP		0  /* Display controller */#define IRQ_IMGH                        1               /* Image capture controller (HSYNC) */
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
#define IRQ_TIMER0			(0 + MMSP2_IRQS)
#define IRQ_TIMER1			(1 + MMSP2_IRQS)
#define IRQ_TIMER2			(2 + MMSP2_IRQS)
#define IRQ_TIMER3			(3 + MMSP2_IRQS)

/* TODO: DMA GPIO UART */

#endif /*IRQS_H_*/
