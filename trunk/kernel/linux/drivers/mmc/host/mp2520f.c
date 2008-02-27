#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/delay.h>
#include <linux/dmaengine.h>

#include <asm/arch/hardware.h>
#include <asm/arch/generic.h>

/* TODO
 * handle pio and dma cases separated
 */

#define UDS printk("%s start\n", __FUNCTION__);
#define UDE printk("%s end\n", __FUNCTION__);

#define DRIVER_NAME "mmsp2_mmcsd"
#define DRIVER_VERSION "0.1"

#define CLOCK_MMC              10000000        /* 10 MHz */
#define CLOCK_SD               25000000        /* 25 MHz */

struct mmsp2_mmc_host {
	struct mmc_host 	*mmc;
	struct resource		*res; /* FIXME do we need it for something? */
	
	/* we duplicate this to be able to have it on the irq handler */
	unsigned char		bus_width;
	unsigned int		power_mode;
	unsigned int		clock;
	struct mmc_request	*req;
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	/* irq handling */
	int						irq;
	struct tasklet_struct	tasklet;
	/* dma handling */
	struct dma_client		*dma_client;
	/* register status */
	unsigned short int		data_status;
	unsigned short int		cmd_status;
	unsigned short int		fifo_status;
	/* cpu driven data */
	u8						*data_ptr;
};


static void mp2520f_start_command(struct mmsp2_mmc_host *mhost, struct mmc_command *cmd)
{
#if 0
	/* set also the 2nd MSB bit to 1 as the datasheet says */	
	SDICMDCON = (req->cmd->opcode & SDICMDCON_CMDINDEX_MSK) | (1 << 6);
	/* expects a respnse */
	if(req->cmd->flags & MMC_RSP_PRESENT)
	{
		/* receive timeout on response, response received */
		SDIINTENB1 |= SDIINTENB1_TOUTMSK | SDIINTENB1_CMDRCVMSK; 
		SDICMDCON |= SDICMDCON_HOSTWAITS;
	}
	else
	{
		/* command sent interrupt */
		SDIINTENB0 |= SDIINTENB0_CMDSNTMSK;
	}	
	/* long response */
	if(req->cmd->flags &  MMC_RSP_136)
		SDICMDCON |= SDICMDCON_HOSTRCV;
	/* set arguments */
	SDICMDARG = req->cmd->arg;
	/* abort command (CMD12,CMD52) */
	/*if(req->cmd->opcode == MMC_STOP_TRANSMISSION)
		SDICMDCON |= SDICMDCON_ABTCMD;*/
	/* start sending the command */
	SDICMDCON |= SDICMDCON_CMDOPST;
	SDIDATCON = sdidatcon;
	printk("command %x sent with data control %x and interrupts %x %x\n", SDICMDCON, sdidatcon, SDIINTENB0, SDIINTENB1);
#endif
}

static void mp2520f_start_data(struct mmsp2_mmc_host *mhost, struct mmc_data *data)
{
#if 0
	unsigned int sdidatcon = 0;
	/* we have to setup again the bus width, 
		 * because the trigger of the data transfer 
		 * must be done at the end, after the command send
		 */
		if(host->bus_width == MMC_BUS_WIDTH_4)
			sdidatcon = SDIDATCON_WIDE;
	SDIBSIZE = req->data->blksz;
			/* TODO use DMA */
			//sdidatcon |= SDIDATCON_DMAMODE;
			/* number of blocks, Rx/Tx data, */
			printk("%d blocks of size %d = %d\n", req->data->blocks, req->data->blksz, req->data->blocks * req->data->blksz);
			sdidatcon |= (req->data->blocks & 0xff) | SDIDATCON_BLKMODE;
			if (req->data->flags & MMC_DATA_WRITE)
			{
				/* write after response */
				sdidatcon |= SDIDATCON_TARSP | SDIDATCON_DATMODE_TS;	
			}
			if (req->data->flags & MMC_DATA_READ)
			{
				/* read after command */
				sdidatcon |= SDIDATCON_RACMD | SDIDATCON_DATMODE_RS;	
			}
			SDIINTENB1 |= SDIINTENB1_DCNT0MSK;
#endif
}

/* ==== MMC/SD API ==== */
static void mmsp2_mmc_request(struct mmc_host *mmc, struct mmc_request *req)
{
	struct mmsp2_mmc_host *host = mmc_priv(mmc);	

	UDS
	
	host->req = req;
	host->cmd = req->cmd;
	host->data = req->data;
		
	
	if(req->data)
	{
		mp2520f_start_data(host, req->data);
	}
	mp2520f_start_command(host, req->cmd);
	
	UDE
}


static void mmsp2_mmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct mmsp2_mmc_host *host = mmc_priv(mmc);
	
	
	UDS
	printk("set ios clk: %u vdd: %u bmode: %u chips: %u power: %u bwidth: %u\n", ios->clock, ios->vdd, ios->bus_mode, ios->chip_select, ios->power_mode, ios->bus_width);
	/* bus width */
	if(ios->bus_width == MMC_BUS_WIDTH_4) 
		SDIDATCON |= SDIDATCON_WIDE;
	else
		SDIDATCON &= ~SDIDATCON_WIDE;
	host->bus_width = ios->bus_width;
	
	if(ios->clock && ios->clock != host->clock) 
	{
		unsigned long pclk;
		/* clk = PCLK / (SDIPRE + 1) => SDIPRE = PCLK/clk - 1 */
		/* TODO check mode, to set up the clock correctly */
		pclk = mmsp2_get_pclk();
		SDIPRE = pclk/ios->clock - 1;
		
		printk("setting clock to %lu\n", pclk/ios->clock - 1);
		host->clock = ios->clock;
	}
	/* power mode */
	if(host->power_mode != ios->power_mode) 
	{
		switch (ios->power_mode) 
		{
			case MMC_POWER_OFF:
			/* disable clock */
			SDICON = 0x0;
			/* reset data control */
			SDIDATCON = 0x0;
			/* disable interrupts */
			SDIINTENB1 = 0x0;
			SDIINTENB0 = 0x0;
			/* timeouts to 0 */
			SDIDTIMERL = 0x0;
			SDIDTIMERH = 0x0;
			/* block size to 0 */
			SDIBSIZE = 0x0;
        	printk("power off\n");
			break;
		
			case MMC_POWER_UP:
			/* FIFO reset, Byte Order, receive interrupts */
        	SDICON = SDICON_FRESET | SDICON_BYTE_BE | SDICON_ENCLK; // | SDICON_RCVIOINT;
         	/* Set timeout count */
        	SDIDTIMERL = 0xffff;
        	SDIDTIMERH = 0x007f;
        	/* receive response timeout, response receive,  */
        	//SDIINTENB1 = SDIINTENB1_TOUTMSK; // | SDIINTENB1_CMDRCVMSK; */
        	//SDIINTENB1 = 0xf000;
        	/* command sent interrupt */
			//SDIINTENB0 = SDIINTENB0_CMDSNTMSK;
        	printk("power up\n");
			break;
		
			case MMC_POWER_ON:
			/* enable the clock */
        	SDICON |= SDICON_ENCLK;
			/* Wait for power-up :  74 clocks delay 74/CLOCK_TICK_RATE ~ 10.03e-6*/
        	udelay(11);
        	printk("power on\n");
			break;
			
			default:
			break;
		}
		host->power_mode = ios->power_mode;
	}
	UDE
}

static struct mmc_host_ops mmsp2_mmc_ops = {
	.request	= mmsp2_mmc_request,
	.set_ios	= mmsp2_mmc_set_ios,
	//int	(*get_ro)(struct mmc_host *host);
};

static void mmsp2_mmc_request_end(struct mmsp2_mmc_host *host, struct mmc_request *req)
{
	UDS
	host->req = NULL;
	host->cmd = NULL;
	host->data = NULL;
	/* clear previous status */
	host->cmd_status = 0;
	host->data_status = 0;
	host->fifo_status = 0;
	mmc_request_done(host->mmc, req);
	UDE
}

/* ==== IRQ handling ==== */
/* just clear the interrupt let the tasklet handle the rest */
static irqreturn_t mmsp2_mmc_irq(int irq, void *devid)
{
	struct mmsp2_mmc_host *host = devid;

#if 1	
	printk("IRQ mmc irq cmd status %hx\n", SDICMDSTA);
	printk("IRQ mmc irq data status %hx\n", SDIDATSTA);
	printk("IRQ mmc irq fifo status %hx\n", SDIFSTA);
#endif
	/* clear interrupt status bit (to not generate more interrupts) */
	//SDICMDSTA &= SDICMDSTA_CMDSENT;
	host->cmd_status |= SDICMDSTA;
	SDICMDSTA &= SDICMDSTA;
	SDIDATSTA &= SDIDATSTA;
	SDIFSTA &= SDIFSTA;
	tasklet_schedule(&host->tasklet);
	
	return IRQ_HANDLED;
}

static void mmsp2_mmc_cmd_end(struct mmsp2_mmc_host *host)
{
	UDS
	printk("TSK mmc irq cmd status %x %x\n", host->cmd_status, SDICMDSTA);
	host->cmd_status |= SDICMDSTA;
	
	/* error cheking
	 * MMC_ERR_FIFO	3
	 * MMC_ERR_FAILED	4
	 * MMC_ERR_INVALID	5
	 */
	
	/* wait for command to complete (just a check it shouldnt happen)*/
	//while(SDICMDSTA & SDICMDSTA_CMDON);
	//host->cmd->error = MMC_ERR_NONE;
	/*if(host->cmd_status & SDICMDSTA_CMDTOUT)
		host->cmd->error |= MMC_ERR_TIMEOUT;*/
	/* FIXME my card gives a CRC when reading the OCR */
	/*if(host->cmd_status & SDICMDSTA_RSPCRC)
		host->cmd->error |= MMC_ERR_BADCRC;*/
	/* check for a response */
	if((host->cmd->flags & MMC_RSP_PRESENT) && (!host->cmd->error))
	{
		/* wait for response end */
		//while(!(SDICMDSTA & SDICMDSTA_RSPFIN));
		if(host->cmd->flags & MMC_RSP_136)
		{
			host->cmd->resp[0] = SDIRSP0 | (SDIRSP1 << 16);
			host->cmd->resp[1] = SDIRSP2 | (SDIRSP3 << 16);
			host->cmd->resp[2] = SDIRSP4 | (SDIRSP5 << 16);
			host->cmd->resp[3] = SDIRSP6 | (SDIRSP7 << 16);
					
		}
		else
			host->cmd->resp[0] = SDIRSP0 | (SDIRSP1 << 16);	
	}
	if(host->cmd->error)
		printk("ERROR %d\n", host->cmd->error);
	/* clear status bits */
	SDICMDSTA = SDICMDSTA;
	printk("TSK mmc irq cmd status %x %x\n", host->cmd_status, SDICMDSTA);
	UDE
}

static void mmsp2_mmc_data_end(struct mmsp2_mmc_host *host)
{
	UDS
	printk("TSK mmc irq data status %x %x\n", host->data_status, SDIDATSTA);
	printk("TSK mmc irq fifo status %x %x\n", host->fifo_status, SDIFSTA);
	host->data_status = SDIDATSTA;
	host->fifo_status = SDIFSTA;
		
	/* TODO put the above in one function */
	//host->data->error = MMC_ERR_NONE;
	/* TODO parse data errors */
	/* data errors */
	/*if(host->data_status & SDIDATSTA_DATTOUT)
		host->data->error |= MMC_ERR_TIMEOUT;*/
		
	//host->data_ptr = (u8*)(page_address(host->data->sg->page) + host->data->sg->offset);
	if(host->data->flags & MMC_DATA_WRITE)
	{
		printk("write data\n");
	}
	else
	{
		int cnt = 0;
		int bytes = host->data->blksz * host->data->blocks;
		
		printk("read data\n");
		#if 0	
		while(bytes > cnt)
		{
			unsigned short int sdifsta = SDIFSTA;
			if(sdifsta & (SDIFSTA_RFDET | SDIFSTA_RFHALF | SDIFSTA_RFLAST))
			{
				printk("remaining block number %d of data %d\n", SDIDATCNT & SDIDATCNT_BLKNUMCNT, SDIDATCNT & SDIDATCNT_BLKCNT);
				/* TODO needs dma first */
				*(host->data_ptr + cnt++) = SDIDAT;
				*(host->data_ptr + cnt++) = SDIDAT;
				*(host->data_ptr + cnt++) = SDIDAT;
				*(host->data_ptr + cnt++) = SDIDAT;
				if(host->bus_width == MMC_BUS_WIDTH_4)
				{
					int i;
					for(i=0; i<8; i++)
					{
						*(host->data_ptr + cnt++) = SDIDAT;
						*(host->data_ptr + cnt++) = SDIDAT;
						*(host->data_ptr + cnt++) = SDIDAT;
						*(host->data_ptr + cnt++) = SDIDAT;
					}						
				}
			}
		}
		#endif
		printk("read %d bytes fifo status %x\n", cnt, SDIFSTA);
	}
	/* reset fifo */
	SDICON |= SDICON_FRESET;
	/* clear data and fifo status */
	SDIDATSTA &= SDIDATSTA;
	SDIFSTA &= SDIFSTA;
	UDE
}

static void mmsp2_mmc_tasklet_fnc(unsigned long data)
{
	struct mmsp2_mmc_host *host = (struct mmsp2_mmc_host *)data;
	int valid = 0;
	
	UDS
	/* disable interrupts */
	SDIINTENB1 = 0x0;
	SDIINTENB0 = 0x0;
	
	if(host->data)
	{
		valid = 1;
		mmsp2_mmc_data_end(host);
	}
	if(host->cmd)
	{
		valid = 1;	
		mmsp2_mmc_cmd_end(host);	
	}
	if(host->req)
	{
		valid = 1;
		mmsp2_mmc_request_end(host, host->req);
	}
	printk("request end");
	if(valid)	printk(" OK!!!\n");
	else		printk(" NOOOO\n");
	
	UDE
}
/* ==== DMA handling */
/*static void mmsp2_mmc_dma_handler(struct dma_client *client, struct dma_chan *chan, enum dma_event event)
{
	
	
}*/

/* ==== platform device API ==== */
static int mmsp2_mmc_probe(struct platform_device *pdev)
{
	struct mmc_host *mmc;
	struct mmsp2_mmc_host *host = NULL;
	struct resource *r;
	int ret = 0, irq;

	printk(KERN_INFO "[MMSP2] MMC/SD driver\n");

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq == NO_IRQ)
		return -ENXIO;

	r = request_mem_region(r->start, r->end - r->start + 1, pdev->name);
	if (!r)
		return -EBUSY;

	mmc = mmc_alloc_host(sizeof(struct mmsp2_mmc_host), &pdev->dev);
	if (!mmc) 
	{
		ret = -ENOMEM;
		goto out;
	}

	mmc->ops = &mmsp2_mmc_ops;
	mmc->f_min = 400000; /* 400 Khz */
	mmc->f_max = 400000; /* 25Mhz in SD mode, 10 Mhz in MMC mode */
	mmc->caps = MMC_CAP_4_BIT_DATA; 
	mmc->ocr_avail = MMC_VDD_27_28 | MMC_VDD_29_30 | MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36; /* 26_36 mmc, 27_36 sd */
	
	host = mmc_priv(mmc);
	host->mmc = mmc;
	host->res = r;
	host->irq = irq;

	/* irq initialization */	
	tasklet_init(&host->tasklet, mmsp2_mmc_tasklet_fnc, (unsigned long)host);
	ret = request_irq(host->irq, mmsp2_mmc_irq, 0, DRIVER_NAME, host);
	if (ret)
		goto err_irq;
	/* dma initialization */
	/*host->dma_client = dma_async_client_register(mmsp2_mmc_dma_handler);
	if(!host->dma_client)
	{
		ret = -ENOMEM;
		goto err_dma;
	}*/
	/* FIXME what channel number? */
	//dma_async_client_chan_request(host->dma_client, 1);
	
	platform_set_drvdata(pdev, mmc);
	/* set GPIOL pin 0-5 to alt function 1 */ 
	mmsp2_gpio_mode(GPIOL, 0, GPIO_FN_ALT1);
	mmsp2_gpio_mode(GPIOL, 1, GPIO_FN_ALT1);
	mmsp2_gpio_mode(GPIOL, 2, GPIO_FN_ALT1);
	mmsp2_gpio_mode(GPIOL, 3, GPIO_FN_ALT1);
	mmsp2_gpio_mode(GPIOL, 4, GPIO_FN_ALT1);
	mmsp2_gpio_mode(GPIOL, 5, GPIO_FN_ALT1);
	
	mmc_add_host(mmc);
	return 0;

/* TODO */
err_dma:
err_irq:
	free_irq(host->irq, host);
out:
	if(mmc)
		mmc_free_host(mmc);
	release_resource(r);
	return ret;
}

static int mmsp2_mmc_remove(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (mmc) 
	{
		struct mmsp2_mmc_host *host = mmc_priv(mmc);
		/* irq shutdown */
		free_irq(host->irq, host);
		/* dma shutdown */
		//dma_async_client_unregister(host->dma_client);
		/* mmc shutdown */
		mmc_remove_host(mmc);
		mmc_free_host(mmc);
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
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mmsp2_mmc_probe,
	.remove		= mmsp2_mmc_remove,
	.suspend	= mmsp2_mmc_suspend,
	.resume		= mmsp2_mmc_resume,
};

/* ==== module API ==== */
static int __init mmsp2_mmc_init(void)
{
	return platform_driver_register(&mmsp2_mmc_driver);
}

static void __exit mmsp2_mmc_exit(void)
{
	platform_driver_unregister(&mmsp2_mmc_driver);
}

module_init(mmsp2_mmc_init);
module_exit(mmsp2_mmc_exit);

MODULE_DESCRIPTION("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_VERSION(DRIVER_VERSION);
