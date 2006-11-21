#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/protocol.h>
#include <linux/delay.h>

#include <asm/arch/hardware.h>

#define CLOCK_MMC              15000000        /* 15 MHz */
#define CLOCK_SD               25000000        /* 25 MHz */

struct mmsp2_mmc_host {
	struct mmc_host		*mmc;
	struct resource		*res;
	int			irq;
};

/* MMC API
 *  SET IOS
     This configures the device to put it into the correct mode and clock speed
     required.

   MCI REQUEST
     MCI request processes the commands sent in the mmc_request structure. This
     can consist of a processing command and a stop command in the case of
     multiple block transfers.

     There are three main types of request, commands, reads and writes.

     Commands are straight forward. The command is submitted to the controller and
     the request function returns. When the controller generates an interrupt to indicate
     the command is finished, the response to the command are read and the mmc_request_done
     function called to end the request.

     Reads and writes work in a similar manner to normal commands but involve the PDC (DMA)
     controller to manage the transfers.

     A read is done from the controller directly to the scatterlist passed in from the request.
     Due to a bug in the controller, when a read is completed, all the words are byte
     swapped in the scatterlist buffers.

     The sequence of read interrupts is: ENDRX, RXBUFF, CMDRDY

     A write is slightly different in that the bytes to write are read from the scatterlist
     into a dma memory buffer (this is in case the source buffer should be read only). The
     entire write buffer is then done from this single dma memory buffer.

     The sequence of write interrupts is: ENDTX, TXBUFE, NOTBUSY, CMDRDY

   GET RO
     Gets the status of the write protect pin, if available.
 */

static void mmsp2_mmc_request(struct mmc_host *mmc, struct mmc_request *req)
{
	printk("request\n");
	
}


static void mmsp2_mmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	printk("set ios\n");
	switch (ios->power_mode) 
	{
		case MMC_POWER_OFF:
		case MMC_POWER_UP:
		case MMC_POWER_ON:
		default:
		break;
	}
	
}

static struct mmc_host_ops mmsp2_mmc_ops = {
	.request	= mmsp2_mmc_request,
	.set_ios	= mmsp2_mmc_set_ios,
	//int	(*get_ro)(struct mmc_host *host);
};

/* hardware interface */
static void mmsp2_mmc_init(void)
{
	#if 0
	SDIDCON = 0;
	SDIPRE = MMCSD_PCLK/(MMCSD_INICLK) -1;
	SDICARG = 0;
    SDICCON = 0;
	SDICSTA = 0xffff;
	SDIBSIZE = 512;
	SDICON = 0;
	SDIDSTA = 0xffff;
	SDIFSTA = 0xffff;
	SDIDAT = 0;
	SDIIMSK1 = 0;
	SDIIMSK0 = 0;
	SDIDTIMERL = 0xffff;
	SDIDTIMERH = 0x0;
	#endif
}

/* platform device API */
static int mmsp2_mmc_probe(struct platform_device *pdev)
{
	struct mmc_host *mmc;
	struct mmsp2_mmc_host *host = NULL;
	struct resource *r;
	int ret = 0, irq;

	printk(KERN_INFO "[MMSP2] mmc/sd driver\n");

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq == NO_IRQ)
		return -ENXIO;

	r = request_mem_region(r->start, r->end + r->start + 1, pdev->name);
	if (!r)
		return -EBUSY;

	mmc = mmc_alloc_host(sizeof(struct mmsp2_mmc_host), &pdev->dev);
	if (!mmc) {
		ret = -ENOMEM;
		goto out;
	}

	mmc->ops = &mmsp2_mmc_ops;
	host = mmc_priv(mmc);
	host->mmc = mmc;
	host->res = r;
	host->irq = irq;
	
	//mmsp2_mmc_init();
	
	
	mmc_add_host(mmc);

	return 0;
out:
	return ret;
}

static int mmsp2_mmc_remove(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (mmc) {

	}
	return 0;
}

#ifdef CONFIG_PM
static int mmsp2_mmc_suspend(struct platform_device *dev, pm_message_t state)
{
	
}

static int mmsp2_mmc_resume(struct platform_device *dev)
{

}
#else
#define mmsp2_mmc_suspend  NULL
#define mmsp2_mmc_resume   NULL
#endif /* CONFIG_PM */

static struct platform_driver mmsp2_mmc_driver = {
	.driver		= 
	{
		.name	= "mmsp2_mmcsd",
		.owner	= THIS_MODULE,
	},
	.probe		= mmsp2_mmc_probe,
	.remove		= mmsp2_mmc_remove,
	.suspend	= mmsp2_mmc_suspend,
	.resume		= mmsp2_mmc_resume,
};

static int __init mmsp2_init(void)
{
	return platform_driver_register(&mmsp2_mmc_driver);
}

static void __exit mmsp2_exit(void)
{
	platform_driver_unregister(&mmsp2_mmc_driver);
}

module_init(mmsp2_init);
module_exit(mmsp2_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
