#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "mmsp2_fb"
#define DRIVER_VERSION "0.1"


/* ==== FB API ==== */
static int
mmsp2_fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
		   u_int trans, struct fb_info *info)
{
	return 0;
}


static int
mmsp2_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	return 0;
}


static int mmsp2_fb_set_par(struct fb_info *info)
{
	return 0;
}

static int mmsp2_fb_blank(int blank, struct fb_info *info)
{
	return 0;
}

static struct fb_ops mmsp2_fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= mmsp2_fb_check_var,
	.fb_set_par	= mmsp2_fb_set_par,
	.fb_setcolreg	= mmsp2_fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_blank	= mmsp2_fb_blank,
};
/* ==== IRQ handling ==== */
/* just clear the interrupt let the tasklet handle the rest */
static irqreturn_t mmsp2_fb_irq(int irq, void *devid, struct pt_regs *regs)
{
	return IRQ_HANDLED;
}



/* ==== platform device API ==== */
static int mmsp2_fb_probe(struct platform_device *pdev)
{

	return 0;
}

static int mmsp2_fb_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver mmsp2_fb_driver = {
	.driver		= 
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mmsp2_fb_probe,
	.remove		= mmsp2_fb_remove,
};

/* ==== module API ==== */
static int __init mmsp2_fb_init(void)
{
	return platform_driver_register(&mmsp2_fb_driver);
}

static void __exit mmsp2_fb_exit(void)
{
	platform_driver_unregister(&mmsp2_fb_driver);
}

module_init(mmsp2_fb_init);
module_exit(mmsp2_fb_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_VERSION(DRIVER_VERSION);
