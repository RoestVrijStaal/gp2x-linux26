#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <asm/arch/lcd.h>
#include <asm/arch/hardware.h>

#define DRIVER_NAME "mmsp2_fb"
#define DRIVER_VERSION "0.1"

struct mmsp2fb_info
{
	struct mmsp2_platform_lcd 	*pdata;
	struct device				*dev;
	/* dma video memory */
	dma_addr_t					map_dma;
	u_char *					map_cpu;
	u_int						map_size;
};

/* helper functions */
static int __init mmsp2fb_init_fbinfo(struct fb_info *fbi)
{
	struct mmsp2fb_info *mfbi = fbi->par;
	
	/* setup default parameters */
	printk("%s\n", __FUNCTION__);
	
	fbi->pseudo_palette = kmalloc( sizeof(u32) * 16, GFP_KERNEL);
	if (!fbi->pseudo_palette)
		return -ENOMEM;
	
	/* fixed info */
	strlcpy(fbi->fix.id, DRIVER_NAME, sizeof(fbi->fix.id));
	fbi->fix.type 			= FB_TYPE_PACKED_PIXELS;
	fbi->fix.visual 		= FB_VISUAL_TRUECOLOR;
	fbi->fix.type_aux		= 0;
	fbi->fix.xpanstep		= 0;
	fbi->fix.ypanstep		= 0;
	fbi->fix.ywrapstep		= 0;
	fbi->fix.accel			= FB_ACCEL_NONE;
	fbi->fix.smem_len 		= mfbi->pdata->width * mfbi->pdata->height * 3; /* 24 bits */
	
	/* variable info */
	fbi->var.xres 			= mfbi->pdata->width;
	fbi->var.xres_virtual	= mfbi->pdata->width;
	fbi->var.yres 			= mfbi->pdata->height;
	fbi->var.yres_virtual	= mfbi->pdata->height;
	fbi->var.bits_per_pixel	= mfbi->pdata->bpp;
	fbi->var.hsync_len 		= mfbi->pdata->hsync_len;
	fbi->var.vsync_len 		= mfbi->pdata->vsync_len;
	
	return 0;
}

static inline u_int chan_to_field(u_int chan, struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;
	return chan << bf->offset;
}

static int __init mmsp2fb_map_video_memory(struct fb_info *fbi)
{
	struct mmsp2fb_info *mfbi = fbi->par;

	mfbi->map_size = PAGE_ALIGN(fbi->fix.smem_len);
	mfbi->map_cpu = dma_alloc_writecombine(mfbi->dev, mfbi->map_size,
					&mfbi->map_dma,GFP_KERNEL);

	if(!mfbi->map_cpu)
		return -ENOMEM;
		
	fbi->screen_base = mfbi->map_cpu;
	fbi->fix.smem_start = mfbi->map_dma;
	/* clear the fb memory */
	memset(mfbi->map_cpu, 0xf, mfbi->map_size);
	
	printk("cpu %p dma %p\n",mfbi->map_cpu, mfbi->map_dma); 

	return 0;
}

/* ==== FB API ==== */
static int
mmsp2_fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
		   u_int trans, struct fb_info *info)
{
	int ret = 0;
	u_int val;
	
	printk("%s\n", __FUNCTION__);
	switch (info->fix.visual) 
	{
		case FB_VISUAL_TRUECOLOR:
		/*
		 * 12 or 16-bit True Colour.  We encode the RGB value
		 * according to the RGB bitfield information.
		 */
			if (regno < 16) {
				u32 *pal = info->pseudo_palette;

				val  = chan_to_field(red, &info->var.red);
				val |= chan_to_field(green, &info->var.green);
				val |= chan_to_field(blue, &info->var.blue);

				pal[regno] = val;
				ret = 0;
			}
		break;
	}
	return ret;
}


static int
mmsp2_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}


static int mmsp2_fb_set_par(struct fb_info *fbi)
{
	struct mmsp2fb_info *mfbi = fbi->par;
	printk("%s\n", __FUNCTION__);


	/* set the fb address on the multi layer controller */	
	MLC_STL_OADRL = mfbi->map_dma & 0xffff; 
	MLC_STL_OADRH = mfbi->map_dma >> 16; 
	MLC_STL_EADRL = mfbi->map_dma & 0xffff; 
	MLC_STL_EADRH = mfbi->map_dma >> 16; 
	return 0;
}

static int mmsp2_fb_blank(int blank, struct fb_info *info)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

static struct fb_ops mmsp2fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= mmsp2_fb_check_var,
	.fb_set_par		= mmsp2_fb_set_par,
	.fb_setcolreg	= mmsp2_fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_blank		= mmsp2_fb_blank,
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
	struct fb_info *fbi;
	struct mmsp2fb_info *mfbi;
	struct mmsp2_platform_lcd *pdata;
	struct resource *res;
	int ret = 0;
	
	pdata = pdev->dev.platform_data;
	if(!pdata)
		return -EINVAL;
	
	fbi = framebuffer_alloc(sizeof(struct mmsp2fb_info), &pdev->dev);
	if(!fbi)
		return -ENOMEM;
	
	mfbi = fbi->par;
	memset(mfbi, 0, sizeof(struct mmsp2fb_info));
	mfbi->dev = &pdev->dev; 
	mfbi->pdata = pdata;
	
	platform_set_drvdata(pdev, fbi);
	/* TODO request the irq */
	/* default fb info */
	ret = mmsp2fb_init_fbinfo(fbi);
	if(ret < 0)
		goto failed_init;
		
	ret = mmsp2fb_map_video_memory(fbi);
	if(ret) 
	{
		dev_err(&pdev->dev, "Failed to allocate video RAM: %d\n", ret);
		ret = -ENOMEM;
		goto failed_map;
	}
	/* initialize the lcd */
	mfbi->pdata->init();
	/* setup the display controller */
	/* setup the mixer layer controller */
	/* register the fb */
	fbi->fbops	= &mmsp2fb_ops;
	/* TODO colormap ??? */
	#if 0
	ret = fb_alloc_cmap(&fbi->cmap, 1 << 24, 0);
	if (ret < 0)
	{
		dev_err(&pdev->dev, "Failed to alloc console map\n");
		goto failed_cmap;
	}
	#endif
	ret = register_framebuffer(fbi);
	if(ret < 0) 
	{
		dev_err(&pdev->dev, "Failed to register framebuffer\n");
		goto failed_register;
	}
	/* FIXME Now we should do the registers not before or it will get on failed state */

	return 0;	
failed_init:
failed_cmap:
failed_map:	
failed_register:
	return ret;
}

static int mmsp2_fb_remove(struct platform_device *pdev)
{
	struct fb_info *fbi = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (fbi) 
	{
		framebuffer_release(fbi);
	}
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
