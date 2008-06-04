#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <asm/arch/hardware.h>

/**
 * UIO driver for the multilayer controller, the mapeable areas are:
 * 0: registers
 * 1: video
 * 2: osd
 * 3: rgb
 * 
 */

#define DRIVER_NAME "mp25xxf_mlc"
#define DRIVER_VERSION "0.1"

struct mlc
{
	/* keep the uio_info here */
	/* store every dma memory allocated */
};

/*============================================================================*
 *                                 Platform                                   * 
 *============================================================================*/
static int mp25xxf_mlc_probe(struct platform_device *pdev)
{
	struct uio_info *info;
	struct resource *r;
	int ret;
	unsigned int size;
	dma_addr_t map_dma;

	printk(KERN_INFO "[MP25XXF] MLC Driver\n");
	
	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r)
	{
		ret = -ENXIO;
		goto out_get;
	}
	
	/* MLC registers */ 
	if (!request_mem_region(r->start, r->end - r->start + 1, pdev->name))
	{
		ret = -ENXIO;
		goto out_get;
	}	 
	info->mem[0].addr = r->start;
	info->mem[0].size = r->end - r->start + 1;
	info->mem[0].memtype = UIO_MEM_PHYS;
	info->mem[0].internal_addr = ioremap(info->mem[0].addr, info->mem[0].size);

	if (!info->mem[0].internal_addr)
	{
	                printk(KERN_ERR "Failed to remap mem 0\n");
	                ret = -ENOMEM;
	                goto out_get;
	}
	/* Map the layers */
	/* TODO check what mach are we using to guess the size */
	/* video plane 0 */
	/* video plane 1 */
	/* osd */
	/* rgb */

	size = 320 * 240 * 3; // 320x240@24bpp
	info->mem[1].internal_addr = dma_alloc_writecombine(&pdev->dev, size,
	                &map_dma, GFP_KERNEL);
	info->mem[1].addr = map_dma;
	info->mem[1].size = size;
	info->mem[1].memtype = UIO_MEM_PHYS;
	printk("MAPPED %x %x %x\n", map_dma, info->mem[1].addr, info->mem[1].internal_addr);
	/* low address */
	__REGW(0xc000290e) = map_dma & 0xffff;
	__REGW(0xc0002912) = map_dma & 0xffff;
	/* high address */
	__REGW(0xc0002910) = map_dma >> 16;
	__REGW(0xc0002914) = map_dma >> 16;
	
	printk("VALUE = %x %x\n", *(unsigned int *)(info->mem[0].internal_addr + 0x8e),
				*(unsigned int *)(info->mem[0].internal_addr + 0x92));
	//msleep(100);
	//memset(addr, 0, size);
	
	info->version = DRIVER_VERSION;
	info->name = DRIVER_NAME;

	if (uio_register_device(&pdev->dev, info))
	{
		ret = -ENODEV;
		goto out_unmap;
	}

	platform_set_drvdata(pdev, info);

	return 0;
	
out_unmap:
	release_mem_region(r->start, r->end - r->start + 1);
out_get:
	kfree(info);
	return ret;
}

static int mp25xxf_mlc_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL); 
	uio_unregister_device(info);
	
	kfree (info);
	return 0;
}

#ifdef CONFIG_PM
static int mp25xxf_mlc_suspend(struct platform_device *dev, pm_message_t state)
{
	
}

static int mp25xxf_mlc_resume(struct platform_device *dev)
{

}

#else
#define mp25xxf_mlc_suspend  NULL
#define mp25xxf_mlc_resume   NULL
#endif /* CONFIG_PM */


static struct platform_driver mp25xxf_mlc_driver = {
	.driver		= 
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mp25xxf_mlc_probe,
	.remove		= mp25xxf_mlc_remove,
	.suspend	= mp25xxf_mlc_suspend,
	.resume		= mp25xxf_mlc_resume,
};
/*============================================================================*
 *                                  Module                                    * 
 *============================================================================*/
static int __init mp25xxf_mlc_init_module(void)
{
	return platform_driver_register(&mp25xxf_mlc_driver);
}

static void __exit mp25xxf_mlc_exit_module(void)
{
	platform_driver_unregister(&mp25xxf_mlc_driver);
}

module_init(mp25xxf_mlc_init_module);
module_exit(mp25xxf_mlc_exit_module);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("");
