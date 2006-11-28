#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "mmsp2_dma"
#define DRIVER_VERSION "0.1"

/* platform device API */
static int mmsp2_dma_probe(struct platform_device *pdev)
{
	//dma_async_device_register(&device->common);
	printk(KERN_INFO "[MMSP2] DMA Controller initializing\n");
	
	return 0;
}

static int mmsp2_dma_remove(struct platform_device *pdev)
{

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
