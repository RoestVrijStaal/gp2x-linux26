#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#include <asm/setup.h>
#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <asm/arch/lcd.h>

#include "generic.h"

/* ==== platform API ==== */
#ifdef CONFIG_FB_MMSP2

/**
 * gp2x_lcd_init Initialize the GP2X LCD
 * 
 * Setups the corresponding GPIO pins to initialize the LCD
 */
static int gp2x_lcd_init(void)
{
	printk("init lcd\n");
	/* setup the gpio */
	
	mmsp2_gpio_mode(GPIOH, 1, GPIO_FN_OUTPUT); //GPIOMD_OUT, GPIOPU_NOSET); // LCD_VGH_ONOFF esto pone la pantalla en AZUL!
	mmsp2_gpio_mode(GPIOH, 2, GPIO_FN_OUTPUT); //, GPIOPU_NOSET); // LCD_BACK_ONOFF
	mmsp2_gpio_mode(GPIOB, 3, GPIO_FN_OUTPUT); // GPIOMD_OUT, GPIOPU_NOSET); // LCD_RST
	mmsp2_gpio_write(GPIOH,1,1);              //LCD_VGH_ON
	mmsp2_gpio_write(GPIOH,2,1);              //BACK_ON
	mmsp2_gpio_write(GPIOB,3,1);              //LCD_RESET HIGH
	
	mmsp2_gpio_mode(GPIOB, 7, GPIO_FN_ALT1); // GPIOPU_NOSET); // CLKH
	mmsp2_gpio_mode(GPIOB, 6, GPIO_FN_ALT1); //, GPIOPU_NOSET); // DE
	mmsp2_gpio_mode(GPIOB, 5, GPIO_FN_ALT1); //, GPIOPU_NOSET); // HSYNC
	mmsp2_gpio_mode(GPIOB, 4, GPIO_FN_ALT1); //, GPIOPU_NOSET); // VSYNC
	
	return 0;
}

static struct mmsp2_platform_lcd gp2x_platform_lcd = 
{
	.width 		= 320,
	.height 	= 240,
	.bpp 		= 24,
	.hsync_len 	= 30,
	.vsync_len 	= 4,
	.init 		= gp2x_lcd_init,
};

static struct resource gp2x_lcd_resources[] = 
{
	#if 0
	/* physical addres of the fb memory */
	[0] = {
		.start	= FB_MEM_START, 
		.end	= FB_MEM_START + FB_MEM_LENGTH - 1,
		.flags	= IORESOURCE_MEM,
	},
	#endif
	/* irq for the vertical sync interrupt */
	[0] = {
		.start	= IRQ_DISP,
		.end	= IRQ_DISP,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 gp2x_lcd_dma_mask = 0xffffffffUL;

static struct platform_device gp2x_lcd_device = {
	.name			= "mmsp2_fb",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(gp2x_lcd_resources),
	.resource		= gp2x_lcd_resources,
	.dev 			= 
	{
		.platform_data = &gp2x_platform_lcd,
		.dma_mask	= &gp2x_lcd_dma_mask,
		.coherent_dma_mask = 0xffffffff,
	},
};
#endif

static struct platform_device *gp2x_devices[] __initdata = {
#ifdef CONFIG_FB_MMSP2
	&gp2x_lcd_device,
#endif
};


static void __init
gp2x_init(void)
{
	/* common mapping */
	mmsp2_init();
	/* gp2x mapping */
	platform_add_devices(gp2x_devices, ARRAY_SIZE(gp2x_devices));
}

static void __init
gp2x_map_io(void)
{
	mmsp2_map_io();
}

static void __init
gp2x_fixup(struct machine_desc *desc, struct tag *tags, char **cmdline, struct meminfo *mi)
{
	/* MCU_A: 64 MB DRAM memory but we only allocate 32MB 
	 * the rest is for other devices (fb,v4l,...)
	 */
	mi->bank[0].start = DRAM_START;
	mi->bank[0].size =  DRAM_LENGTH;
	mi->bank[0].node =  0;
	mi->nr_banks = 1;
}


MACHINE_START(GP2X, "GameparkHoldings GP2X")
	.phys_io		= NIO_START,
	.io_pg_offst	= ((NIO_BASE) >> 18) & 0xfffc,
	.boot_params	= 0x00100100,
	.map_io			= gp2x_map_io,
	.init_irq		= mmsp2_init_irq,
	.timer			= &mmsp2_timer,
	.init_machine	= gp2x_init,
	.fixup			= gp2x_fixup,
MACHINE_END

