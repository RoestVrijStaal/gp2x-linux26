#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>

#include <asm/arch/hardware.h>

#define DRIVER_NAME "mmsp2_uart"
#define DRIVER_VERSION "0.1"

struct mmsp2_uart_port {
	struct uart_port	port;
	struct timer_list	timer;
	unsigned int		old_status;
	int					txirq, rxirq, rtsirq;
};



#if defined(CONFIG_SERIAL_IMX_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#define SERIAL_MMSP2_MAJOR	204
#define MINOR_START		41

/* ==== interrupts ==== */
static irqreturn_t mmsp2_rx_int(int irq, void *dev_id, struct pt_regs *regs)
{
	return IRQ_HANDLED;
}

static irqreturn_t mmsp2_tx_int(int irq, void *dev_id, struct pt_regs *regs)
{
	return IRQ_HANDLED;
} 

/* ==== console API ==== */
#ifdef CONFIG_SERIAL_MMSP2_CONSOLE

static void mmsp2_console_putchar(struct uart_port *port, int ch)
{

}

/*
 * Interrupts are disabled on entering
 */
static void
mmsp2_console_write(struct console *co, const char *s, unsigned int count)
{

}

/*
 * If the port was already initialised (eg, by a boot loader),
 * try to determine the current setup.
 */
static void __init
mmsp2_console_get_options(struct imx_port *sport, int *baud,
			   int *parity, int *bits)
{

}

static int __init
mmsp2_console_setup(struct console *co, char *options)
{
	
}

static struct uart_driver mmsp2_uart_drv;
static struct console mmsp2_console = {
	.name		= "ttyMMSP2",

	.write		= mmsp2_console_write,
	.device		= uart_console_device,
	.setup		= mmsp2_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= mmsp2_uart_drv,
};

static int __init mmsp2_console_init(void)
{
	/* imx_init_ports(); */
	register_console(&mmsp2_console);
	return 0;
}

console_initcall(mmsp2_console_init);

#define MMSP2_CONSOLE	&mmsp2_console
#else
#define MMSP2_CONSOLE	NULL
#endif

/* ==== UART API ==== */

/*
 * check if the transfer buffer register is empty
 */
static unsigned int
mmsp2_uart_tx_empty(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	return (TRSTATUS(mport->port.membase) & TRSTATUS_TRANSMITTER_EMPTY ? TIOCSER_TEMT:0);
}

static void 
mmsp2_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	unsigned short int	mcon = MCONx(mport->port.membase);

	if(mctrl & TIOCM_RTS)
		mcon |= MCON_RTS_ACTIVE; 
	else
		mcon &= ~MCON_RTS_ACTIVE; 
        
	if(mctrl & TIOCM_DTR)
		mcon |= MCON_DTR_ACTIVE;
	else
		mcon &= ~MCON_DTR_ACTIVE;

	MCONx(mport->port.membase) = mcon;
}

static unsigned int 
mmsp2_uart_get_mctrl(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	unsigned short int	mstatus = MSTATUSx(mport->port.membase);
	unsigned int tmp = 0;
	
	/* TODO */
	
	return tmp;
}
/*
 * power management
 */
static void mmsp2_uart_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
	
}



static struct uart_ops mmsp2_uart_ops = 
{
	.tx_empty		= mmsp2_uart_tx_empty,
	.set_mctrl		= mmsp2_uart_set_mctrl,
	.get_mctrl		= mmsp2_uart_get_mctrl,
#if 0
	.stop_tx		= mmsp2_uart_stop_tx,
	.start_tx		= mmsp2_uart_start_tx,
	
	.stop_rx		= mmsp2_uart_stop_rx,
	.enable_ms		= mmsp2_uart_enable_ms,
	.break_ctl		= mmsp2_uart_break_ctl,
	.startup		= mmsp2_uart_startup,
	.shutdown		= mmsp2_uart_shutdown,
	.set_termios	= mmsp2_uart_set_termios,
#endif
	.pm				= mmsp2_uart_pm,
#if 0
	.type			= mmsp2_uart_type,
	.release_port	= mmsp2_uart_release_port,
	.request_port	= mmsp2_uart_request_port,
	.config_port	= mmsp2_uart_config_port,
	.verify_port	= mmsp2_uart_verify_port,
#endif
};

static struct mmsp2_uart_port mmsp2_uart_ports[] = 
{
	[0] =  
	{
		.port	= {
			.type		= PORT_MMSP2,
			.iotype		= UPIO_MEM,
			/*.membase	= (void *)IMX_UART1_BASE,
			.mapbase	= IMX_UART1_BASE, /* FIXME */
			/*.irq		= UART1_MINT_RX, */
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
	},
	[1] =  
	{
		.port	= {
			.type		= PORT_MMSP2,
			.iotype		= UPIO_MEM,
			/*.membase	= (void *)IMX_UART1_BASE,
			.mapbase	= IMX_UART1_BASE, /* FIXME */
			/*.irq		= UART1_MINT_RX, */
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
	},	
	[2] =  
	{
		.port	= {
			.type		= PORT_MMSP2,
			.iotype		= UPIO_MEM,
			/*.membase	= (void *)IMX_UART1_BASE,
			.mapbase	= IMX_UART1_BASE, /* FIXME */
			/*.irq		= UART1_MINT_RX, */
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
	},
	[3] =  
	{
		.port	= {
			.type		= PORT_MMSP2,
			.iotype		= UPIO_MEM,
			/*.membase	= (void *)IMX_UART1_BASE,
			.mapbase	= IMX_UART1_BASE, /* FIXME */
			/*.irq		= UART1_MINT_RX, */
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
	},
};

static struct uart_driver mmsp2_uart_drv = {
	.owner          = THIS_MODULE,
	.driver_name    = DRIVER_NAME,
	.dev_name       = "ttyMMSP2",
	.major          = SERIAL_MMSP2_MAJOR,
	.minor          = MINOR_START,
	.nr             = ARRAY_SIZE(mmsp2_uart_ports),
	.cons           = MMSP2_CONSOLE,
};

/* ==== platform device API ==== */
static int mmsp2_uart_probe(struct platform_device *pdev)
{
	printk("uart probe with id %d\n", pdev->id);
	uart_add_one_port(&mmsp2_uart_drv, &mmsp2_uart_ports[pdev->id].port);
	platform_set_drvdata(pdev, &mmsp2_uart_ports[pdev->id]);
	
	return 0;
}

static int mmsp2_uart_remove(struct platform_device *pdev)
{
	struct mmsp2_uart_port *mport = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (mport)
		uart_remove_one_port(&mmsp2_uart_drv, &mport->port);
	return 0;
}

static struct platform_driver mmsp2_uart_driver = {
	.driver		= 
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mmsp2_uart_probe,
	.remove		= mmsp2_uart_remove,
};

/* ==== module API ==== */
static int __init mmsp2_uart_init(void)
{
	int ret = 0;
	
	printk(KERN_INFO "[MMSP2] Serial driver\n");
	ret = uart_register_driver(&mmsp2_uart_drv);
	if (ret)
		return ret;
	ret = platform_driver_register(&mmsp2_uart_driver);
	if (ret)
		uart_unregister_driver(&mmsp2_uart_drv);
	
	return ret;
}

static void __exit mmsp2_uart_exit(void)
{
	platform_driver_unregister(&mmsp2_uart_driver);
}

module_init(mmsp2_uart_init);
module_exit(mmsp2_uart_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_VERSION(DRIVER_VERSION);
