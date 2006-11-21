#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <asm/setup.h>
#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "generic.h"

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
	 * the rest is for other devices (fb,...)
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
