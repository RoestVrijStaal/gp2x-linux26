#ifndef MP25XXF_REGS_GPIO_H_
#define MP25XXF_REGS_GPIO_H_

/*============================================================================*
 *                                   GPIO                                     * 
 *============================================================================*/
/* GPIO */
#define GPIO_START	0xc0001020
#define GPIO_END	0xc00011bc
#define GPIO_BASE	io_p2v(GPIO_START)


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


#endif
