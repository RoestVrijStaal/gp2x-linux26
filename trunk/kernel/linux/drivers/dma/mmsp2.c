#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/dmaengine.h>

#include <asm/arch/hardware.h>
#include <asm/arch/dma.h>

/* The DMA engine API is being developed, there are new patches (2.6.19+) 
 * that make the new API fit in our needs, but before that, we'll use
 * our private method: arch/arm/mach-mmsp2/dma.c
 */

/* FIXME hardcoded */
#define DMA_REG_BASE 	io_p2v(0xc0000200)
#define DMA_IO_BASE 	io_p2v(0xc0000100)

#define DRIVER_NAME "mmsp2_dma"
#define DRIVER_VERSION "0.1"

#define to_mmsp2_chan(chan) container_of(chan, struct mmsp2_dma_chan, common)
#define to_mmsp2_device(dev) container_of(dev, struct mmsp2_dma_device, common)

struct mmsp2_dma_device 
{
	struct dma_device common;
	/* irq handling */
	int						irq;
	struct tasklet_struct	tasklet;
};

struct mmsp2_dma_chan
{
	struct dma_chan 			common;
	struct mmsp2_dma_device 	*device;
	void 						*reg_base; /* control registers */
	void 						*io_base;  /* io request registers */
	unsigned int				peripheric; /* which internal device, 0 in case of memory */
};

/* ==== local functions ==== */
static int enumerate_dma_channels(struct mmsp2_dma_device *device)
{
	int i;
	struct mmsp2_dma_chan *mmsp2_chan;

	device->common.chancnt = DMA_MMSP2_MAX_CHANNELS;
	
	for (i = 0; i < device->common.chancnt; i++) 
	{
		mmsp2_chan = kzalloc(sizeof(struct mmsp2_dma_chan), GFP_KERNEL);
		if (!mmsp2_chan) 
		{
			device->common.chancnt = i;
			break;
		}

		mmsp2_chan->device = device;
		/* FIXME hardcoded */
		mmsp2_chan->reg_base =  DMA_REG_BASE + (0x40 * i);
		mmsp2_chan->reg_base =  DMA_IO_BASE + (0x4 * i);
		/*INIT_LIST_HEAD(&mmsp2_chan->free_desc);
		INIT_LIST_HEAD(&mmsp2_chan->used_desc);*/
		
		/* This should be made common somewhere in dmaengine.c */
		mmsp2_chan->common.device = &device->common;
		mmsp2_chan->common.client = NULL;
		list_add_tail(&mmsp2_chan->common.device_node, &device->common.channels);
	}
	return device->common.chancnt;
}

/* ==== DMA engine API ==== */
static int mmsp2_dma_alloc_chan_resources(struct dma_chan *chan)
{
	printk("%s\n", __FUNCTION__);	
	return 1;
}

static void mmsp2_dma_memcpy_cleanup(struct mmsp2_dma_chan *mmsp2_chan);

static void mmsp2_dma_free_chan_resources(struct dma_chan *chan)
{
	printk("%s\n", __FUNCTION__);	

}


static dma_cookie_t do_mmsp2_dma_memcpy(struct mmsp2_dma_chan *mmsp2_chan,
                                       dma_addr_t dest,
                                       dma_addr_t src,
                                       size_t len)
{
	printk("%s\n", __FUNCTION__);	

}

static dma_cookie_t mmsp2_dma_memcpy_buf_to_buf(struct dma_chan *chan,
                                               void *dest,
                                               void *src,
                                               size_t len)
{
	printk("%s\n", __FUNCTION__);	

}


static dma_cookie_t mmsp2_dma_memcpy_buf_to_pg(struct dma_chan *chan,
                                              struct page *page,
                                              unsigned int offset,
                                              void *src,
                                              size_t len)
{
	printk("%s\n", __FUNCTION__);	

}

static dma_cookie_t mmsp2_dma_memcpy_pg_to_pg(struct dma_chan *chan,
                                             struct page *dest_pg,
                                             unsigned int dest_off,
                                             struct page *src_pg,
                                             unsigned int src_off,
                                             size_t len)
{
	printk("%s\n", __FUNCTION__);	

}

static enum dma_status mmsp2_dma_is_complete(struct dma_chan *chan,
                                            dma_cookie_t cookie,
                                            dma_cookie_t *done,
                                            dma_cookie_t *used)
{
	printk("%s\n", __FUNCTION__);	

}

static void mmsp2_dma_memcpy_issue_pending(struct dma_chan *chan)
{
	printk("%s\n", __FUNCTION__);	
}
/* ==== IRQ handling ==== */
static irqreturn_t mmsp2_dma_irq(int irq, void *devid, struct pt_regs *regs)
{
	struct mmsp2_dma_device *device = devid;
	printk("IRQ DMA in\n"); 
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
	/* initialize controller */
	/* set dma functions */
	INIT_LIST_HEAD(&device->common.channels);
	enumerate_dma_channels(device);

	device->common.device_alloc_chan_resources = mmsp2_dma_alloc_chan_resources;
	device->common.device_free_chan_resources = mmsp2_dma_free_chan_resources;
	device->common.device_memcpy_buf_to_buf = mmsp2_dma_memcpy_buf_to_buf;
	device->common.device_memcpy_buf_to_pg = mmsp2_dma_memcpy_buf_to_pg;
	device->common.device_memcpy_pg_to_pg = mmsp2_dma_memcpy_pg_to_pg;
	device->common.device_memcpy_complete = mmsp2_dma_is_complete;
	device->common.device_memcpy_issue_pending = mmsp2_dma_memcpy_issue_pending;
	printk(KERN_INFO "[MMSP2] DMA Controller with %d channels\n", device->common.chancnt);
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

static struct platform_driver mmsp2_dma_driver = 
{
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
