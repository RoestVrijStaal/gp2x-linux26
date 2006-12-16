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

struct mmsp2_port {
	struct uart_port	port;
	struct timer_list	timer;
	unsigned int		old_status;
};

static struct mmsp2_port mmsp2_ports[] = 
{
	
	
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

static struct uart_driver imx_reg;
static struct console mmsp2_console = {
	.name		= "ttyMMSP2",
/*
	.write		= imx_console_write,
	.device		= uart_console_device,
	.setup		= imx_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &mmsp2_uart_drv,
*/
};

#define MMSP2_CONSOLE	&mmsp2_console
#else
#define MMSP2_CONSOLE	NULL
#endif

static struct uart_driver mmsp2_uart_drv = {
	.owner          = THIS_MODULE,
	.driver_name    = DRIVER_NAME,
	.dev_name       = "ttyMMSP2",
	.major          = SERIAL_IMX_MAJOR,
	.minor          = MINOR_START,
	.nr             = ARRAY_SIZE(imx_ports),
	.cons           = MMSP2_CONSOLE,
};

static struct uart_ops mmsp2_uart_ops = {
/*
	.pm		= s3c24xx_serial_pm,
	.tx_empty	= s3c24xx_serial_tx_empty,
	.get_mctrl	= s3c24xx_serial_get_mctrl,
	.set_mctrl	= s3c24xx_serial_set_mctrl,
	.stop_tx	= s3c24xx_serial_stop_tx,
	.start_tx	= s3c24xx_serial_start_tx,
	.stop_rx	= s3c24xx_serial_stop_rx,
	.enable_ms	= s3c24xx_serial_enable_ms,
	.break_ctl	= s3c24xx_serial_break_ctl,
	.startup	= s3c24xx_serial_startup,
	.shutdown	= s3c24xx_serial_shutdown,
	.set_termios	= s3c24xx_serial_set_termios,
	.type		= s3c24xx_serial_type,
	.release_port	= s3c24xx_serial_release_port,
	.request_port	= s3c24xx_serial_request_port,
	.config_port	= s3c24xx_serial_config_port,
	.verify_port	= s3c24xx_serial_verify_port,
*/
};


/* ==== platform device API ==== */
static int mmsp2_uart_probe(struct platform_device *pdev)
{
	uart_add_one_port(&imx_reg, &imx_ports[dev->id].port);
	platform_set_drvdata(dev, &imx_ports[dev->id]);
	return 0;

}

static int mmsp2_uart_remove(struct platform_device *pdev)
{
	struct mmsp2_port *sport = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	if (sport)
		uart_remove_one_port(&imx_reg, &sport->port);
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
	return platform_driver_register(&mmsp2_uart_driver);
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
