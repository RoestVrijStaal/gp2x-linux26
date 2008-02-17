/*
 * 
 * TODO:
 * - If we have support for the serial console, the initialization is done
 * before the data from the platform devices is obtained, how to solve this?
 * declare the ports with static data and don't use the platform device resources?
 * 
 */

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
#include <asm/arch/uart.h>

#define DRIVER_NAME "mmsp2_uart"
#define DEVICE_NAME "ttySM"
#define DRIVER_VERSION "0.1"

#define UDS printk("%s start\n", __FUNCTION__);
#define UDE printk("%s end\n", __FUNCTION__);


struct mmsp2_uart_port {
	struct uart_port	port;
	int 			txirq;
	int 			rxirq;
	unsigned char 		rx_claimed;
	unsigned char 		tx_claimed;
	
};

#if defined(CONFIG_SERIAL_MP25XXF_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#define SERIAL_MP25XXF_MAJOR	204
#define MINOR_START		41

 
/* ==== console API ==== */
#ifdef CONFIG_SERIAL_MP25XXF_CONSOLE

static struct uart_driver mmsp2_uart_drv;
static struct mmsp2_uart_port mmsp2_uart_ports[NR_PORTS];

static void mmsp2_console_putchar(struct uart_port *port, int ch)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	
	while((FSTATUSx(mport->port.mapbase) & FSTATUS_TX_FIFO_FULL))
		barrier();
	THBx(mport->port.mapbase) = ch;
}

/*
 * Interrupts are disabled on entering
 */
static void
mmsp2_console_write(struct console *co, const char *s, unsigned int count)
{
	struct mmsp2_uart_port *mport = &mmsp2_uart_ports[co->index];
	uart_console_write(&mport->port, s, count, mmsp2_console_putchar);
}

/*
 * If the port was already initialised (eg, by a boot loader),
 * try to determine the current setup.
 */
static void __init
mmsp2_console_get_options(struct mmsp2_uart_port *sport, int *baud,
			   int *parity, int *bits)
{

}

static int __init
mmsp2_console_setup(struct console *co, char *options)
{
	struct mmsp2_uart_port *mport;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	printk("console setup %d!!!\n", co->index);
	if (co->index == -1 || co->index >= NR_PORTS)
		co->index = 0;
	
	mport = &mmsp2_uart_ports[co->index];
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
#if 0	
	else
		mmsp2_console_get_options(port, &baud, &parity, &bits);
#endif
	return uart_set_options(&mport->port, co, baud, parity, bits, flow);
}


static struct console mmsp2_console = {
	.name		= DEVICE_NAME,
	.write		= mmsp2_console_write,
	.device		= uart_console_device,
	.setup		= mmsp2_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &mmsp2_uart_drv,
};

static int __init mmsp2_console_init(void)
{
	/* imx_init_ports(); */
	printk("console init!!!\n");
	register_console(&mmsp2_console);
	return 0;
}

console_initcall(mmsp2_console_init);

#define MP25XXF_CONSOLE	&mmsp2_console
#else
#define MP25XXF_CONSOLE	NULL
#endif /* CONFIG_SERIAL_MP25XXF_CONSOLE */

static void mp25xxf_uart_start_tx(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;

	/* enable interrupt */
	if (!mport->tx_claimed)
	{
		enable_irq(mport->txirq);
		mport->tx_claimed = 1;
	}

}

static void mp25xxf_uart_stop_tx(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	
	/* disable interrupt */
	if (mport->tx_claimed)
	{
		disable_irq(mport->txirq);
		mport->tx_claimed = 0;
	}

}

static void mp25xxf_uart_stop_rx(struct uart_port *port)
{
	/* disable interrupt */
	UDS
	UDE
}

/* ==== interrupts ==== */
static irqreturn_t mmsp2_rx_int(int irq, void *dev_id)
{
	return IRQ_HANDLED;
}

static irqreturn_t mmsp2_tx_int(int irq, void *dev_id)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)dev_id;
	struct uart_port *port = &mport->port;
	struct circ_buf *xmit = &port->info->xmit;
	int count = 256;

	if (port->x_char)
	{
		THBx(port->mapbase) = port->x_char;

		port->icount.tx++;
		port->x_char = 0;
		goto out;
	}

	/*
	 * if there isnt anything more to transmit, or the uart is now stopped, disable the uart and exit 
	 */

	if (uart_circ_empty(xmit) || uart_tx_stopped(port))
	{
		mp25xxf_uart_stop_tx(port);
		goto out;
	}

	/*
	 * try and drain the buffer... 
	 */

	while (!uart_circ_empty(xmit) && count--> 0)
	{
		if ((FSTATUSx(port->mapbase)) & FSTATUS_TX_FIFO_FULL)
			break;

		THBx(port->mapbase) = xmit->buf[xmit->tail];
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		mp25xxf_uart_stop_tx(port);

out:
	return IRQ_HANDLED;
}

/* ==== UART API ==== */

/*
 * check if the transfer buffer register is empty
 */
static unsigned int
mmsp2_uart_tx_empty(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	
	UDS
	return (TRSTATUSx(mport->port.mapbase) & TRSTATUS_TRANSMITTER_EMPTY ? TIOCSER_TEMT:0);
}

static void 
mmsp2_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	unsigned short int	mcon;
	
	UDS
	mcon = MCONx(mport->port.mapbase);

	
	if(mctrl & TIOCM_RTS)
		mcon |= MCON_RTS_ACTIVE; 
	else
		mcon &= ~MCON_RTS_ACTIVE; 
        
	if(mctrl & TIOCM_DTR)
		mcon |= MCON_DTR_ACTIVE;
	else
		mcon &= ~MCON_DTR_ACTIVE;

	MCONx(mport->port.mapbase) = mcon;
	UDE
}

static unsigned int 
mmsp2_uart_get_mctrl(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	unsigned short int	mstatus;
	unsigned int tmp = 0;

	UDS
	mstatus = MSTATUSx(mport->port.mapbase);
	if(mstatus & MSTATUS_DSR)
		tmp |= TIOCM_DSR;
	if(mstatus & MSTATUS_CTS)
		tmp |= TIOCM_CTS;
	if(mstatus & MSTATUS_RI)
		tmp |= TIOCM_RI;
	if(mstatus & MSTATUS_DCD)
		tmp |= TIOCM_CAR;
	UDE
	return tmp;
}

/*
 * power management
 */
static void mmsp2_uart_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
	UDS
	UDE
}

static int mmsp2_uart_startup(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;
	int ret = 0;
	
	/* register the interrupts */
	ret = request_irq(mport->txirq, mmsp2_tx_int, 0, DEVICE_NAME, mport);
	if (ret != 0) {
		printk("cannot get irq %d\n", mport->txirq);
		return ret;
	}
	ret = request_irq(mport->rxirq, mmsp2_rx_int, 0, DEVICE_NAME, mport);
	if (ret != 0) {
		free_irq(mport->txirq, mport);
		printk("cannot get irq %d\n", mport->rxirq);
		return ret;
	}
	mport->tx_claimed = 1;
	mport->rx_claimed = 1;

	return ret;
}

static void mmsp2_uart_shutdown(struct uart_port *port)
{
	UDS
	UDE
}

static void
mmsp2_uart_set_termios(struct uart_port *port, struct termios *termios,
		   struct termios *old)
{
	UDS
	UDE
}



static const char *mmsp2_uart_type(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;

	return mport->port.type == PORT_MP25XXF ? "MP25XXF" : NULL;
}

/*
 * Release the memory region(s) being used by 'port'.
 */
static void mmsp2_uart_release_port(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;

	release_mem_region(mport->port.mapbase, MP25XXF_UART_SIZE);
}

/*
 * Request the memory region(s) being used by 'port'.
 */
static int mmsp2_uart_request_port(struct uart_port *port)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;

	return request_mem_region(mport->port.mapbase, MP25XXF_UART_SIZE,
			"mmsp2-uart") != NULL ? 0 : -EBUSY;
}

/*
 * Configure/autoconfigure the port.
 */
static void mmsp2_uart_config_port(struct uart_port *port, int flags)
{
	struct mmsp2_uart_port *mport = (struct mmsp2_uart_port *)port;

	if (flags & UART_CONFIG_TYPE &&
	    mmsp2_uart_request_port(&mport->port) == 0)
		mport->port.type = PORT_MP25XXF;
}

static struct uart_ops mmsp2_uart_ops = 
{
	.tx_empty		= mmsp2_uart_tx_empty,
	.set_mctrl		= mmsp2_uart_set_mctrl,
	.get_mctrl		= mmsp2_uart_get_mctrl,
	.stop_tx		= mp25xxf_uart_stop_tx,
	.start_tx		= mp25xxf_uart_start_tx,
	.stop_rx		= mp25xxf_uart_stop_rx,
#if 0
	.enable_ms		= mmsp2_uart_enable_ms,
	.break_ctl		= mmsp2_uart_break_ctl,
#endif
	.startup		= mmsp2_uart_startup,
	.shutdown		= mmsp2_uart_shutdown,
	.set_termios	= mmsp2_uart_set_termios,
	.pm				= mmsp2_uart_pm,
	.type			= mmsp2_uart_type,
	.release_port	= mmsp2_uart_release_port,
	.request_port	= mmsp2_uart_request_port,
	.config_port	= mmsp2_uart_config_port,
#if 0
	.verify_port	= mmsp2_uart_verify_port,
#endif
};

/* FIXME change this static declaration with a simpler one, using the platform
 * variables
 */
static struct mmsp2_uart_port mmsp2_uart_ports[] = 
{
	[0] =  
	{
		.port	= {
			.type		= PORT_MP25XXF,
			.iotype		= UPIO_MEM,
			.membase	= (void *)MP25XXF_UART0_START,
			.mapbase	= MP25XXF_UART0_START,
			.irq		= IRQ_UART_RXD0,
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
		.txirq = IRQ_UART_RXD0, 
		.rxirq = IRQ_UART_TXD0,
	},
	[1] =  
	{
		.port	= {
			.type		= PORT_MP25XXF,
			.iotype		= UPIO_MEM,
			.membase	= (void *)MP25XXF_UART1_START,
			.mapbase	= MP25XXF_UART1_START,
			.irq		= IRQ_UART_RXD1,
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
		.txirq = IRQ_UART_RXD1, 
		.rxirq = IRQ_UART_TXD1,
	},
	[2] =  
	{
		.port	= {
			.type		= PORT_MP25XXF,
			.iotype		= UPIO_MEM,
			.membase	= (void *)MP25XXF_UART2_START,
			.mapbase	= MP25XXF_UART2_START,
			.irq		= IRQ_UART_RXD2,
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
		.txirq = IRQ_UART_RXD2, 
		.rxirq = IRQ_UART_TXD2,
	},
	[3] =  
	{
		.port	= {
			.type		= PORT_MP25XXF,
			.iotype		= UPIO_MEM,
			.membase	= (void *)MP25XXF_UART3_START,
			.mapbase	= MP25XXF_UART3_START,
			.irq		= IRQ_UART_RXD3,
			.uartclk	= 16000000,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.ops		= &mmsp2_uart_ops,
			.line		= 0,
		},
		.txirq = IRQ_UART_RXD3, 
		.rxirq = IRQ_UART_TXD3,
	},
};

static struct uart_driver mmsp2_uart_drv = {
	.owner          = THIS_MODULE,
	.driver_name    = DRIVER_NAME,
	.dev_name       = DEVICE_NAME,
	.major          = SERIAL_MP25XXF_MAJOR,
	.minor          = MINOR_START,
	.nr             = NR_PORTS,
	.cons           = MP25XXF_CONSOLE,
};

/* ==== platform device API ==== */
static int mmsp2_uart_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct mmsp2_uart_port *mport;
	
	printk("uart probe with id %d\n", pdev->id);
	 
	mport = &mmsp2_uart_ports[pdev->id];
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		printk(KERN_ERR "failed to find memory resource for uart\n");
		return -EINVAL;
	}
	/* mport->port.membase = (void *)res->start;  FIXME */
	mport->port.mapbase = res->start;
	
	mport->port.irq = mport->rxirq = platform_get_irq(pdev, 0);
	mport->txirq = platform_get_irq(pdev, 1);
	/* TODO move the below to startup function */
	/* common values */
	mport->port.type = PORT_MP25XXF;
	mport->port.iotype = UPIO_MEM;
	mport->port.uartclk	= 16000000;
	mport->port.fifosize = 16;
	mport->port.flags = UPF_BOOT_AUTOCONF;
	mport->port.ops = &mmsp2_uart_ops;
	mport->port.line = pdev->id;
	
	/* register the interrupt hanlders */
	uart_add_one_port(&mmsp2_uart_drv, &mport->port);
	platform_set_drvdata(pdev, mport);
	
	printk("exit\n");
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
	
	printk(KERN_INFO "[MP25XXF] Serial driver\n");
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
