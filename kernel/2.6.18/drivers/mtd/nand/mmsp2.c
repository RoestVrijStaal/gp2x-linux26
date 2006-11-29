
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>
#include <asm/arch/hardware.h>
 
#ifdef CONFIG_MTD_NAND_MMSP2_HWECC
static int hardware_ecc = 1;
#else
static int hardware_ecc = 0;
#endif 

/* ==== nand api ==== */
static struct mtd_info mtd;
static struct nand_chip chip;

#ifdef CONFIG_MTD_PARTITIONS
static const struct mtd_partition partition_info[] = 
{
	{
		.name = "Bootloader",
		.size = 0x80000,
        .offset = 0,
		.mask_flags = MTD_WRITEABLE,  /* force read-only */
	},
	{
		.name = "Param",
		.size = 0x20000,
		.offset = MTDPART_OFS_APPEND,
	},
	{
		.name = "Kernel",
		.size = 0x100000,
		.offset = MTDPART_OFS_APPEND,
	},
	{
		.name = "Filesystem",
		.size = 0x1800000,
		.offset = MTDPART_OFS_APPEND,
	},
	{
		.name = "Extend",
		.size = MTDPART_SIZ_FULL,
		.offset = MTDPART_OFS_APPEND,
	}
};
#endif
 
static void mmsp2_nand_hwcontrol(struct mtd_info *mtd, int cmd,
				   unsigned int ctrl)
{
	if(cmd == NAND_CMD_NONE)
		return;
	
	if (ctrl & NAND_CLE) NFCMD = cmd;
	else NFADDR = cmd;
}

static void mmsp2_nand_select_chip(struct mtd_info *mtd, int chip)
{
	unsigned short mem = MEMNANDCTRL & 0xfffc;

	mem |= chip & 3;
	MEMNANDCTRL = mem;
}

static int mmsp2_nand_dev_ready(struct mtd_info *mtd)
{
	if ((MEMNANDCTRL & NAND_READY_DETECT) == NAND_READY_DETECT) 
	{
		while ( MEMNANDCTRL & NAND_READY_DETECT) 
			MEMNANDCTRL |=  NAND_READY_DETECT;
		return 1;
	}
    return 0;
}


/* ==== module API ==== */
static int __init 
mmsp2_nand_init(void)
{
	printk("MMSP2 NAND Driver\n");

	memzero(&mtd, sizeof(struct mtd_info));
	memzero(&chip, sizeof(struct nand_chip));
	
	mtd.priv = &chip;
	mtd.owner = THIS_MODULE;
	
	/* Set address of NAND IO lines */
	chip.IO_ADDR_R = (void *)NF_IO_BASE;
	chip.IO_ADDR_W = (void *)NF_IO_BASE;
	/* Reference hardware control function */
	chip.cmdfunc = NULL;
	chip.cmd_ctrl = mmsp2_nand_hwcontrol;
	chip.select_chip  = mmsp2_nand_select_chip;
	chip.dev_ready = mmsp2_nand_dev_ready;
	if(hardware_ecc)
		chip.ecc.mode = NAND_ECC_HW;
	else
		chip.ecc.mode = NAND_ECC_SOFT;
	/* Scan to find existance of the device */
	if (nand_scan (&mtd, 1)) 
		return -ENXIO;

#ifdef CONFIG_MTD_PARTITIONS
	add_mtd_partitions(&mtd, partition_info, ARRAY_SIZE(partition_info));
#endif

	return 0;
}

static void __exit 
mmsp2_nand_exit(void)
{
	nand_release(&mtd);
}

module_init(mmsp2_nand_init);
module_exit(mmsp2_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("");
