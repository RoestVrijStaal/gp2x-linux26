#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

#include <asm/hardware.h>

#include <asm/mach/map.h>

/*
 * Note that 0xfffe0000-0xffffffff is reserved for the vector table and
 * cache flush area.
 */
static struct map_desc mmsp2_io_desc[] __initdata = {
	{	/* Normal IO */
		.virtual	= NIO_BASE,
		.pfn		= __phys_to_pfn(NIO_START),
		.length		= NIO_SIZE,
		.type		= MT_DEVICE
	}
#if 0
	,
	{	/* Fast IO */
		.virtual	= FIO_BASE,
		.pfn		= __phys_to_pfn(FIO_START),
		.length		= FIO_SIZE,
		.type		= MT_DEVICE
	},
	{	/* IDE IO */
		.virtual	= IDE_IO_BASE,
		.pfn		= __phys_to_pfn(IDE_IO_START),
		.length		= IDE_IO_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF0 IO */
		.virtual	= CF0_IO_BASE,
		.pfn		= __phys_to_pfn(CF0_IO_START),
		.length		= CF0_IO_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF0 ATTR */
		.virtual	= CF0_ATTR_BASE,
		.pfn		= __phys_to_pfn(CF0_ATTR_START),
		.length		= CF0_ATTR_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF0 MEM */
		.virtual	= CF0_MEM_BASE,
		.pfn		= __phys_to_pfn(CF0_MEM_START),
		.length		= CF0_MEM_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF1 IO */
		.virtual	= CF1_IO_BASE,
		.pfn		= __phys_to_pfn(CF1_IO_START),
		.length		= CF0_IO_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF1 ATTR */
		.virtual	= CF1_ATTR_BASE,
		.pfn		= __phys_to_pfn(CF1_ATTR_START),
		.length		= CF1_ATTR_SIZE,
		.type		= MT_DEVICE
	},
	{	/* CF1 MEM */
		.virtual	= CF1_MEM_BASE,
		.pfn		= __phys_to_pfn(CF1_MEM_START),
		.length		= CF1_MEM_SIZE,
		.type		= MT_DEVICE
	}
#endif
};

void __init
mmsp2_map_io(void)
{
	printascii("mmsp2_map_io\n");
	iotable_init(mmsp2_io_desc, ARRAY_SIZE(mmsp2_io_desc));
}
