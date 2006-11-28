#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/dmaengine.h>

#define DRIVER_NAME "mmsp2_dma"
#define DRIVER_VERSION "0.1"

struct mmsp2_dma_device 
{
	struct dma_device common;
	/* irq handling */
	int						irq;
	struct tasklet_struct	tasklet;
};

struct mmsp2_dma_chan
{
	struct dma_chan common;
};

/* ==== DMA engine API ==== */


/* ==== IRQ handling ==== */
static irqreturn_t mmsp2_dma_irq(int irq, void *devid, struct pt_regs *regs)
{
	struct mmsp2_dma_device *device = devid;

#if 0	
	printk("IRQ dma irq cmd status %hx\n", SDICMDSTA);
	printk("IRQ dma irq data status %hx\n", SDIDATSTA);
	printk("IRQ dma irq fifo status %hx\n", SDIFSTA);
#endif 
	/* tasklet_schedule(&device->tasklet); */
	
	return IRQ_HANDLED;
}

/*
static void mmsp2_dma_cmd_end(struct mmsp2_mmc_host *host)
{
	
}
*/

/* ==== platform device API ==== */
static int mmsp2_dma_probe(struct platform_device *pdev)
{
	struct mmsp2_dma_device *device;
	int ret = 0, irq;

	printk(KERN_INFO "[MMSP2] DMA Controller initializing\n");

	device = kzalloc(sizeof(struct mmsp2_dma_device), GFP_KERNEL);
	if (!device)
		return -ENOMEM;
	
	irq = platform_get_irq(pdev, 0);
	if (irq == NO_IRQ)
	{
		ret = -ENXIO;
		goto err_irq;
	}
	/* set dma functions */
#if 0
	INIT_LIST_HEAD(&device->common.channels);
	enumerate_dma_channels(device);

	device->common.device_alloc_chan_resources = ioat_dma_alloc_chan_resources;
	device->common.device_free_chan_resources = ioat_dma_free_chan_resources;
	device->common.device_memcpy_buf_to_buf = ioat_dma_memcpy_buf_to_buf;
	device->common.device_memcpy_buf_to_pg = ioat_dma_memcpy_buf_to_pg;
	device->common.device_memcpy_pg_to_pg = ioat_dma_memcpy_pg_to_pg;
	device->common.device_memcpy_complete = ioat_dma_is_complete;
	device->common.device_memcpy_issue_pending = ioat_dma_memcpy_issue_pending;
	printk(KERN_INFO "Intel(R) I/OAT DMA Engine found, %d channels\n",
		device->common.chancnt);
#endif
	/* tasklet_init(&device->tasklet, mmsp2_dma_tasklet_fnc, (unsigned long)device); */
	ret = request_irq(device->irq, mmsp2_dma_irq, 0, DRIVER_NAME, device);
	if (ret)
		goto err_rirq;
	
	platform_set_drvdata(pdev, device);
	dma_async_device_register(&device->common);
	return 0;

err_rirq:
	free_irq(device->irq, device);
err_irq:
	kfree(device);
	return ret;
}

static int mmsp2_dma_remove(struct platform_device *pdev)
{
	struct mmsp2_dma_device *device = platform_get_drvdata(pdev);
	
	platform_set_drvdata(pdev, NULL);

	/* TODO remove dma engine? */
	if (device); 

	return 0;
}

#ifdef CONFIG_PM
static int mmsp2_dma_suspend(struct platform_device *dev, pm_message_t state)
{
	
}

static int mmsp2_dma_resume(struct platform_device *dev)
{

}
#else
#define mmsp2_dma_suspend  NULL
#define mmsp2_dma_resume   NULL
#endif /* CONFIG_PM */

static struct platform_driver mmsp2_dma_driver = {
	.driver		= 
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mmsp2_dma_probe,
	.remove		= mmsp2_dma_remove,
	.suspend	= mmsp2_dma_suspend,
	.resume		= mmsp2_dma_resume,
};

/* module API */
static int __init mmsp2_dma_init(void)
{
	return platform_driver_register(&mmsp2_dma_driver);
}

static void __exit mmsp2_dma_exit(void)
{
	platform_driver_unregister(&mmsp2_dma_driver);
}

module_init(mmsp2_dma_init);
module_exit(mmsp2_dma_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_VERSION(DRIVER_VERSION);
