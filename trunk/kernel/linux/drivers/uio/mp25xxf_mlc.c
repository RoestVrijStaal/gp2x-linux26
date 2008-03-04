#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>

#include <asm/arch/hardware.h>

#define DRIVER_NAME "mp25xxf_mlc"
#define DRIVER_VERSION "0.1"

/*============================================================================*
 *                                 Platform                                   * 
 *============================================================================*/
static int mp25xxf_mlc_probe(struct platform_device *pdev)
{
	struct uio_info *info;
	struct resource *r;

	printk(KERN_INFO "[MP25XXF] MLC Driver\n");
	
	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r)
		return -ENXIO;

	r = request_mem_region(r->start, r->end - r->start + 1, pdev->name);
	
	info->mem[0].addr = r->start;
	info->mem[0].size = r->end - r->start + 1;
	info->mem[0].memtype = UIO_MEM_PHYS;
	
	info->version = DRIVER_VERSION;
	info->name = DRIVER_NAME;

	if (uio_register_device(&pdev->dev, info))
		goto out_unmap;

	platform_set_drvdata(pdev, info);

	return 0;
out_unmap:
	//release_mem_region(r->start, r->end - r->start + 1);
	return -ENODEV;
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

MODULE_LICENSE("");
MODULE_AUTHOR("");
