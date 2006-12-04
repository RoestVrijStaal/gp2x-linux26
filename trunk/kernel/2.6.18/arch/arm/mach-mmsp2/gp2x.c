#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

#include <asm/setup.h>
#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "generic.h"

/* ==== platform API ==== */
static struct mmsp2_platform_lcd gp2x_lcd_device = 
{
	.max_width 	= 320,
	.max_height = 240,
	.init 		= NULL,	
};

static struct resource mmsp2_lcd_resources[] = 
{
	[0] = {
		.start	= IRQ_DISP,
		.end	= IRQ_DISP,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device mmsp2_lcd_device = {
	.name			= "mmsp2_lcd",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(mmsp2_lcd_resources),
	.resource		= mmsp2_lcd_resources,
};

static void __init
gp2x_init(void)
{
	printascii("gp2x_init\n");
}

static void __init
gp2x_map_io(void)
{
	printascii("map_io\n");
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
