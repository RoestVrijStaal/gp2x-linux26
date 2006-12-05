#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

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
	return 0;
}

static struct mmsp2_platform_lcd gp2x_platform_lcd = 
{
	.max_width 	= 320,
	.max_height = 240,
	.init 		= gp2x_lcd_init,	
};

static struct resource gp2x_lcd_resources[] = 
{
	[0] = {
		.start	= IRQ_DISP,
		.end	= IRQ_DISP,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device gp2x_lcd_device = {
	.name			= "mmsp2_lcd",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(gp2x_lcd_resources),
	.resource		= gp2x_lcd_resources,
	.dev 			= 
	{
		.platform_data = &gp2x_platform_lcd,
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
	mi->bank[0].size =  (32 * 1024 * 1024); 
	mi->bank[0].node =  0;
	mi->nr_banks = 1;
}


MACHINE_START(GP2X, "GameparkHoldings GP2X")
	.phys_io		= NIO_START,
	.io_pg_offst	= ((NIO_BASE) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.map_io			= gp2x_map_io,
	.init_irq		= mmsp2_init_irq,
	.timer			= &mmsp2_timer,
	.init_machine	= gp2x_init,
	.fixup			= gp2x_fixup,
MACHINE_END
