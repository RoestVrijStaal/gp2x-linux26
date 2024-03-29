#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/string.h>

#include <asm/hardware.h>

#include <asm/mach/map.h>


/* gpio functions */
void mmsp2_gpio_mode(unsigned short int group, unsigned short int pin, unsigned short int mode)
{
	unsigned short int value;
	unsigned short int offset; 
	
	
	if(group > GPIOO)
	{
		printk(KERN_ERR "[mmsp2] group out of range %u (%s)\n", group, __FUNCTION__);
		return;	
	}
	if(pin > 16)
	{
		printk(KERN_ERR "[mmsp2] pin out of range %u (%s)\n", pin, __FUNCTION__);
		return;
	}
	/* the mode (input, output, alt1, alt2) */
	/* FIXME there are register which have a different bit offset when reading */
	if(pin > 8)
	{
		offset = 2*(pin-8);
		printk("gpio read %x %x\n", GPIOxALTFNHI(group), GPIOxALTFNLOW(group));
		value = GPIOxALTFNHI(group) & ~(0x3 << offset);
		value |= (mode & 0x3) << offset;
		printk("gpio %x\n", value);
		GPIOxALTFNHI(group) = value;
	}
	else
	{
		offset = 2*pin;
		printk("gpio read %x %x\n", GPIOxALTFNHI(group), GPIOxALTFNLOW(group));
		value = GPIOxALTFNLOW(group) & ~(0x3 << offset);
		value |= (mode & 0x3) << offset;
		printk("gpio %x\n", value);
		GPIOxALTFNLOW(group) = value;
	}
	/* event type (rising/falling edge, high/low level) */
	/* pull up */
	/* interrupt */
}

EXPORT_SYMBOL(mmsp2_gpio_mode);

void mmsp2_gpio_write(unsigned short int group, unsigned short int pin, unsigned short int data)
{
	unsigned short int value;

	printk("GPIO%d %x\n", group, GPIOxOUT(group));
	value = GPIOxOUT(group) & ~(1 << pin);
	value |= (data << pin); 
	GPIOxOUT(group) = value;
	printk("GPIO%d %x\n", group, GPIOxOUT(group));
}

EXPORT_SYMBOL(mmsp2_gpio_write);

/* clock functions
 * clock = (m * Fin) / (p * s)
 * m = M + 8
 * p = P + 2
 * s = 2 ^ S
 *
 * Fin = Frequency Input (7372800 Hz)
 * S : Output frequency scaler
 * M : VCO frequency scaler
 * P : Input frequency scaler
 */

static inline unsigned long calc_clk(unsigned short v)
{
        unsigned long m, p, s;
        m = GET_MDIV(v); p = GET_PDIV(v); s = GET_SDIV(v);

        return ( ((m + 8) * CLOCK_TICK_RATE) / ((p + 2) * (1 << s)) );
}

unsigned long mmsp2_get_fclk(void)
{
        return calc_clk(FPLLVSETREG);      //return 199065600;
}

unsigned long mmsp2_get_uclk(void)
{
        return calc_clk(UPLLVSETREG);      //return 95000000;
}

unsigned long mmsp2_get_aclk(void)
{
        return calc_clk(APLLVSETREG);      //return 95000000;
}

unsigned long mmsp2_get_pclk(void)
{
        unsigned long c, d, b, p;

        c = calc_clk(FPLLVSETREG);
        d = c / (GET_DDIV - 1); // DCLK
        b = c / 2; // BCLK
        p = b / 2; // PCLK
        return p;
}

EXPORT_SYMBOL(mmsp2_get_fclk);
EXPORT_SYMBOL(mmsp2_get_uclk);
EXPORT_SYMBOL(mmsp2_get_aclk);
EXPORT_SYMBOL(mmsp2_get_pclk);

static void __init mmsp2_show_clk(void)
{
        unsigned long v, c, m, p, s;

        v = FPLLVSETREG;
        m = GET_MDIV(v); p = GET_PDIV(v); s = GET_SDIV(v);
        c = ( ((m + 8) * CLOCK_TICK_RATE) / ((p + 2) * (1 << s)) );
        printk(KERN_INFO "[mmsp2] FCLK: %9lu Hz, M = 0x%lx P = %lu S = %lu (%lx)\n", c, m, p, s, v);

        v = UPLLVSETREG;
        m = GET_MDIV(v); p = GET_PDIV(v); s = GET_SDIV(v);
        c = ( ((m + 8) * CLOCK_TICK_RATE) / ((p + 2) * (1 << s)) );
        printk(KERN_INFO "[mmsp2] UCLK: %9lu Hz, M = 0x%lx P = %lu S = %lu (%lx)\n", c, m, p, s, v);

        v = APLLVSETREG;
        m = GET_MDIV(v); p = GET_PDIV(v); s = GET_SDIV(v);
        c = ( ((m + 8) * CLOCK_TICK_RATE) / ((p + 2) * (1 << s)) );
        printk(KERN_INFO "[mmsp2] ACLK: %9lu Hz, M = 0x%lx P = %lu S = %lu (%lx)\n", c, m, p, s, v);

        c = mmsp2_get_pclk();
        printk(KERN_INFO "[mmspw] PCLK: %9lu Hz\n", c);
}

/*============================================================================*
 *                                 SD/MMC                                     * 
 *============================================================================*/
/* dynamically mapped devices */
/* FIXME do we need the first resource? */
static struct resource mmsp2_mmcsd_resources[] = 
{
	[0] = {
		.start	= MP25XXF_MMC_START,
		.end	= MP25XXF_MMC_END,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SD,
		.end	= IRQ_SD,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device mmsp2_mmcsd_device = {
	.name			= "mmsp2_mmcsd",
	.id				= -1,
	.num_resources	= ARRAY_SIZE(mmsp2_mmcsd_resources),
	.resource		= mmsp2_mmcsd_resources,
};

/*============================================================================*
 *                                   UART                                     * 
 *============================================================================*/
static struct resource mp25xxf_uart0_resources[] = 
{
	[0] = {
		.start	= IRQ_UART_RXD0,
		.end	= IRQ_UART_RXD0,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_UART_TXD0,
		.end	= IRQ_UART_TXD0,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_UART_ERROR0,
		.end	= IRQ_UART_ERROR0,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_UART_MODEM0,
		.end	= IRQ_UART_MODEM0,
		.flags	= IORESOURCE_IRQ,
	},
	[4] = {
		.start	= MP25XXF_UART0_START,
		.end	= MP25XXF_UART0_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mp25xxf_uart0_device = {
	.name			= "mp25xxf_uart",
	.id				= 0,
	.num_resources	= ARRAY_SIZE(mp25xxf_uart0_resources),
	.resource		= mp25xxf_uart0_resources,
};

static struct resource mp25xxf_uart1_resources[] = 
{
	[0] = {
		.start	= IRQ_UART_RXD1,
		.end	= IRQ_UART_RXD1,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_UART_TXD1,
		.end	= IRQ_UART_TXD1,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_UART_ERROR1,
		.end	= IRQ_UART_ERROR1,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_UART_MODEM1,
		.end	= IRQ_UART_MODEM1,
		.flags	= IORESOURCE_IRQ,
	},
	[4] = {
		.start	= MP25XXF_UART1_START,
		.end	= MP25XXF_UART1_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mp25xxf_uart1_device = {
	.name			= "mp25xxf_uart",
	.id				= 1,
	.num_resources	= ARRAY_SIZE(mp25xxf_uart1_resources),
	.resource		= mp25xxf_uart1_resources,
};

static struct resource mp25xxf_uart2_resources[] = 
{
	[0] = {
		.start	= IRQ_UART_RXD2,
		.end	= IRQ_UART_RXD2,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_UART_TXD2,
		.end	= IRQ_UART_TXD2,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_UART_ERROR2,
		.end	= IRQ_UART_ERROR2,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_UART_MODEM2,
		.end	= IRQ_UART_MODEM2,
		.flags	= IORESOURCE_IRQ,
	},
	[4] = {
		.start	= MP25XXF_UART2_START,
		.end	= MP25XXF_UART2_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mp25xxf_uart2_device = {
	.name			= "mp25xxf_uart",
	.id				= 2,
	.num_resources	= ARRAY_SIZE(mp25xxf_uart2_resources),
	.resource		= mp25xxf_uart2_resources,
};

static struct resource mp25xxf_uart3_resources[] = 
{
	[0] = {
		.start	= IRQ_UART_RXD3,
		.end	= IRQ_UART_RXD3,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_UART_TXD3,
		.end	= IRQ_UART_TXD3,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_UART_ERROR3,
		.end	= IRQ_UART_ERROR3,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_UART_MODEM3,
		.end	= IRQ_UART_MODEM3,
		.flags	= IORESOURCE_IRQ,
	},
	[4] = {
		.start	= MP25XXF_UART3_START,
		.end	= MP25XXF_UART3_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mp25xxf_uart3_device = {
	.name			= "mp25xxf_uart",
	.id				= 3,
	.num_resources	= ARRAY_SIZE(mp25xxf_uart3_resources),
	.resource		= mp25xxf_uart3_resources,
};


static struct resource mmsp2_dma_resources[] = 
{
	[0] = {
		.start	= IRQ_DMA,
		.end	= IRQ_DMA,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device mmsp2_dma_device = {
	.name		= "mmsp2_dma",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mmsp2_dma_resources),
	.resource	= mmsp2_dma_resources,
};

/*============================================================================*
 *                        Multi Layer Controller (MLC)                        * 
 *============================================================================*/
static struct resource mp25xxf_mlc_resources[] = 
{
	[0] = {
		.start	= MP25XXF_MLC_START,
		.end	= MP25XXF_MLC_END,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mp25xxf_mlc_device = {
	.name		= "mp25xxf_mlc",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mp25xxf_mlc_resources),
	.resource	= mp25xxf_mlc_resources,
	.dev = {
		.coherent_dma_mask	=  0xffffffffUL,
	}
};

static struct platform_device *mmsp2_devices[] __initdata = {
	&mmsp2_mmcsd_device,
	&mp25xxf_uart0_device,
	&mp25xxf_uart1_device,
	&mp25xxf_uart2_device,
	&mp25xxf_uart3_device,
	&mp25xxf_mlc_device,
	&mmsp2_dma_device,
};

/* statically mapped devices */
static struct map_desc mmsp2_io_desc[] __initdata = {
	{	/* Normal IO */
		.virtual	= MMSP2_NIO_BASE,
		.pfn		= __phys_to_pfn(MMSP2_NIO_START),
		.length		= MMSP2_NIO_SIZE,
		.type		= MT_DEVICE
	},
	{	/* Fast IO */
		.virtual	= FIO_BASE,
		.pfn		= __phys_to_pfn(FIO_START),
		.length		= FIO_SIZE,
		.type		= MT_DEVICE
	}
#ifdef CONFIG_MTD_NAND_MMSP2
	,{
		.virtual	= NF_IO_BASE,
		.pfn		= __phys_to_pfn(NF_IO_START),
		.length		= NF_IO_SIZE,
		.type		= MT_DEVICE	
	}
#endif
	#if 0
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

void __init mmsp2_map_io(void)
{
	/* static mapping */
	iotable_init(mmsp2_io_desc, ARRAY_SIZE(mmsp2_io_desc));
	mmsp2_show_clk();
}

int __init mmsp2_init(void)
{
	/* dynamic mapping */
	return platform_add_devices(mmsp2_devices, ARRAY_SIZE(mmsp2_devices));
}
