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

#define DRIVER_NAME "mp25xxf_uart"
#define DEVICE_NAME "ttySM"
#define DRIVER_VERSION "0.1"

#define UDS printk("%s start\n", __FUNCTION__);
#define UDE printk("%s end\n", __FUNCTION__);


struct mp25xxf_uart_port {
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

static struct uart_driver mp25xxf_uart_drv;
static struct mp25xxf_uart_port mp25xxf_uart_ports[NR_PORTS];

static void mp25xxf_console_putchar(struct uart_port *port, int ch)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
	
	while((FSTATUSx(mport->port.mapbase) & FSTATUS_TX_FIFO_FULL))
		barrier();
	THBx(mport->port.mapbase) = ch;
}

/*
 * Interrupts are disabled on entering
 */
static void
mp25xxf_console_write(struct console *co, const char *s, unsigned int count)
{
	struct mp25xxf_uart_port *mport = &mp25xxf_uart_ports[co->index];
	uart_console_write(&mport->port, s, count, mp25xxf_console_putchar);
}

/*
 * If the port was already initialised (eg, by a boot loader),
 * try to determine the current setup.
 */
static void __init
mp25xxf_console_get_options(struct mp25xxf_uart_port *sport, int *baud,
			   int *parity, int *bits)
{

}

static int __init
mp25xxf_console_setup(struct console *co, char *options)
{
	struct mp25xxf_uart_port *mport;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	printk("console setup %d!!!\n", co->index);
	if (co->index == -1 || co->index >= NR_PORTS)
		co->index = 0;
	
	mport = &mp25xxf_uart_ports[co->index];
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
#if 0	
	else
		mp25xxf_console_get_options(port, &baud, &parity, &bits);
#endif
	return uart_set_options(&mport->port, co, baud, parity, bits, flow);
}


static struct console mp25xxf_console = {
	.name		= DEVICE_NAME,
	.write		= mp25xxf_console_write,
	.device		= uart_console_device,
	.setup		= mp25xxf_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &mp25xxf_uart_drv,
};

static int __init mp25xxf_console_init(void)
{
	/* imx_init_ports(); */
	printk("console init!!!\n");
	register_console(&mp25xxf_console);
	return 0;
}

console_initcall(mp25xxf_console_init);

#define MP25XXF_CONSOLE	&mp25xxf_console
#else
#define MP25XXF_CONSOLE	NULL
#endif /* CONFIG_SERIAL_MP25XXF_CONSOLE */

static void mp25xxf_uart_start_tx(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	/* enable interrupt */
	if (!mport->tx_claimed)
	{
		enable_irq(mport->txirq);
		mport->tx_claimed = 1;
	}

}

static void mp25xxf_uart_stop_tx(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
	
	/* disable interrupt */
	if (mport->tx_claimed)
	{
		disable_irq(mport->txirq);
		mport->tx_claimed = 0;
	}

}

static void mp25xxf_uart_stop_rx(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	/* disable interrupt */
	if (mport->rx_claimed)
	{
		disable_irq(mport->rxirq);
		mport->rx_claimed = 0;
	}
}

static void
mp25xxf_serial_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;
	unsigned short ucon;

	spin_lock_irqsave(&port->lock, flags);

	ucon = UCONx(port->mapbase);
	if (break_state)
		ucon |= MP2530_UCON_SBREAK;
	else
		ucon &= ~MP2530_UCON_SBREAK;

	UCONx(port->mapbase) = ucon;
	spin_unlock_irqrestore(&port->lock, flags);
}

/* ==== interrupts ==== */
static irqreturn_t mp25xxf_rx_int(int irq, void *dev_id)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)dev_id;
	struct uart_port *port = &mport->port;
	struct tty_struct *tty = port->info->tty;
	unsigned int ch;
	unsigned int flag;
	int max_count = 64;
	unsigned short estatus;
	
	//printk("handling interrupt %d %x\n", irq, ESTATUSx(port->mapbase));
	while (max_count-- > 0)
	{
		/* check how many characters are on the fifo */
		//printk("%d \n", FSTATUSx(port->mapbase));
		if (!(FSTATUSx(port->mapbase) & FSTATUS_RX_FIFO_COUNT))
		{
			//printk("quitting\n");
			break;
		}
	        if (port->flags & UPF_CONS_FLOW)
	        {
	        	
	        }

		/*
		 * insert the character into the buffer 
		 */
		ch = RHBx(port->mapbase);
		flag = TTY_NORMAL;
		port->icount.rx++;
		estatus = ESTATUSx(port->mapbase);

		if (unlikely(estatus & ESTATUS_ANY))
		{
			/*
			 * check for break 
			 */
			if (estatus & ESTATUS_BREAK_DETECT)
			{
				port->icount.brk++;
				if (uart_handle_break(port))
					goto ignore_char;
			}

			if (estatus & ESTATUS_FRAME_ERROR)
				port->icount.frame++;
			if (estatus & ESTATUS_OVERRUN_ERROR)
				port->icount.overrun++;

			/* check our flags */
			/* FIXME also set them */
			estatus &= port->read_status_mask;

			if (estatus & ESTATUS_BREAK_DETECT)
				flag = TTY_BREAK;
			else if (estatus & ESTATUS_PARITY_ERROR)
				flag = TTY_PARITY;
			else if (estatus & (ESTATUS_FRAME_ERROR | ESTATUS_OVERRUN_ERROR))
				flag = TTY_FRAME;
		}
		if (uart_handle_sysrq_char(port, ch))
			goto ignore_char;
		//printk("receiving char %c\n", ch);
		uart_insert_char(port, estatus, ESTATUS_OVERRUN_ERROR, ch, flag);
ignore_char:
		continue;
	}
	tty_flip_buffer_push(tty);
	
	return IRQ_HANDLED;
}

static irqreturn_t mp25xxf_tx_int(int irq, void *dev_id)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)dev_id;
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

	while (!uart_circ_empty(xmit) && count-- > 0)
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
mp25xxf_uart_tx_empty(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
	
	UDS
	return (TRSTATUSx(mport->port.mapbase) & TRSTATUS_TRANSMITTER_EMPTY ? TIOCSER_TEMT:0);
}

static void 
mp25xxf_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
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
mp25xxf_uart_get_mctrl(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
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
static void mp25xxf_uart_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
	UDS
	UDE
}

static int mp25xxf_uart_startup(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;
	int ret = 0;
	
	/* register the interrupts */
	ret = request_irq(mport->txirq, mp25xxf_tx_int, 0, DEVICE_NAME, mport);
	if (ret != 0) {
		printk("cannot get irq %d\n", mport->txirq);
		return ret;
	}
	ret = request_irq(mport->rxirq, mp25xxf_rx_int, 0, DEVICE_NAME, mport);
	if (ret != 0) {
		free_irq(mport->txirq, mport);
		printk("cannot get irq %d\n", mport->rxirq);
		return ret;
	}
	mport->tx_claimed = 1;
	mport->rx_claimed = 1;

	return ret;
}

static void mp25xxf_uart_shutdown(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	if (mport->tx_claimed)
	{
		free_irq(mport->txirq, mport);
		mport->tx_claimed = 0;
	}

	if (mport->rx_claimed)
	{
		free_irq(mport->rxirq, mport);
		mport->rx_claimed = 0;
	}
}

static void
mp25xxf_uart_set_termios(struct uart_port *port, struct termios *termios,
		   struct termios *old)
{
	UDS
	UDE
}



static const char *mp25xxf_uart_type(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	return mport->port.type == PORT_MP25XXF ? "MP25XXF" : NULL;
}

/*
 * Release the memory region(s) being used by 'port'.
 */
static void mp25xxf_uart_release_port(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	release_mem_region(mport->port.mapbase, MP25XXF_UART_SIZE);
}

/*
 * Request the memory region(s) being used by 'port'.
 */
static int mp25xxf_uart_request_port(struct uart_port *port)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	return request_mem_region(mport->port.mapbase, MP25XXF_UART_SIZE,
			"mp25xxf-uart") != NULL ? 0 : -EBUSY;
}

/*
 * Configure/autoconfigure the port.
 */
static void mp25xxf_uart_config_port(struct uart_port *port, int flags)
{
	struct mp25xxf_uart_port *mport = (struct mp25xxf_uart_port *)port;

	if (flags & UART_CONFIG_TYPE &&
	    mp25xxf_uart_request_port(&mport->port) == 0)
		mport->port.type = PORT_MP25XXF;
}

static struct uart_ops mp25xxf_uart_ops = 
{
	.tx_empty		= mp25xxf_uart_tx_empty,
	.set_mctrl		= mp25xxf_uart_set_mctrl,
	.get_mctrl		= mp25xxf_uart_get_mctrl,
	.stop_tx		= mp25xxf_uart_stop_tx,
	.start_tx		= mp25xxf_uart_start_tx,
	.stop_rx		= mp25xxf_uart_stop_rx,
#if 0
	.enable_ms		= mp25xxf_uart_enable_ms,
#endif
	.break_ctl		= mp25xxf_uart_break_ctl,
	.startup		= mp25xxf_uart_startup,
	.shutdown		= mp25xxf_uart_shutdown,
	.set_termios	= mp25xxf_uart_set_termios,
	.pm				= mp25xxf_uart_pm,
	.type			= mp25xxf_uart_type,
	.release_port	= mp25xxf_uart_release_port,
	.request_port	= mp25xxf_uart_request_port,
	.config_port	= mp25xxf_uart_config_port,
#if 0
	.verify_port	= mp25xxf_uart_verify_port,
#endif
};

/* FIXME change this static declaration with a simpler one, using the platform
 * variables
 */
static struct mp25xxf_uart_port mp25xxf_uart_ports[] = 
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
			.ops		= &mp25xxf_uart_ops,
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
			.ops		= &mp25xxf_uart_ops,
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
			.ops		= &mp25xxf_uart_ops,
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
			.ops		= &mp25xxf_uart_ops,
			.line		= 0,
		},
		.txirq = IRQ_UART_RXD3, 
		.rxirq = IRQ_UART_TXD3,
	},
};

static struct uart_driver mp25xxf_uart_drv = {
	.owner          = THIS_MODULE,
	.driver_name    = DRIVER_NAME,
	.dev_name       = DEVICE_NAME,
	.major          = SERIAL_MP25XXF_MAJOR,
	.minor          = MINOR_START,
	.nr             = NR_PORTS,
	.cons           = MP25XXF_CONSOLE,
};

/* ==== platform device API ==== */
static int mp25xxf_uart_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct mp25xxf_uart_port *mport;
	
	mport = &mp25xxf_uart_ports[pdev->id];
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
	mport->port.uartclk = 16000000;
	mport->port.fifosize = 16;
	mport->port.flags = UPF_BOOT_AUTOCONF;
	mport->port.ops = &mp25xxf_uart_ops;
	mport->port.line = pdev->id;
	
	/* register the interrupt hanlders */
	uart_add_one_port(&mp25xxf_uart_drv, &mport->port);
	platform_set_drvdata(pdev, mport);
	printk("(%d) %d %d\n", mport->port.line, mport->rxirq, mport->txirq);
	
	{
		unsigned short ucon = UCONx(mport->port.mapbase);
		printk("ucon = %x\n", ucon);
		ucon &= UCON_RECEIVE_MODE;
		ucon |= UCON_RECEIVE_MODE_IRQ;
	}
	
	return 0;
}

static int mp25xxf_uart_remove(struct platform_device *pdev)
{
	struct mp25xxf_uart_port *mport = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (mport)
		uart_remove_one_port(&mp25xxf_uart_drv, &mport->port);
	return 0;
}

static struct platform_driver mp25xxf_uart_driver = {
	.driver		= 
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mp25xxf_uart_probe,
	.remove		= mp25xxf_uart_remove,
};

/* ==== module API ==== */
static int __init mp25xxf_uart_init(void)
{
	int ret = 0;
	
	printk(KERN_INFO "[MP25XXF] Serial driver\n");
	ret = uart_register_driver(&mp25xxf_uart_drv);
	if (ret)
		return ret;
	ret = platform_driver_register(&mp25xxf_uart_driver);
	if (ret)
		uart_unregister_driver(&mp25xxf_uart_drv);
	
	return ret;
}

static void __exit mp25xxf_uart_exit(void)
{
	platform_driver_unregister(&mp25xxf_uart_driver);
}

module_init(mp25xxf_uart_init);
module_exit(mp25xxf_uart_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_VERSION(DRIVER_VERSION);
