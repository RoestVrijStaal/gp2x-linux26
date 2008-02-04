#include <linux/init.h>
#include <linux/kernel.h>

static int __init mmsp2_dma_init(void)
{
	return 0;
}

core_initcall(mmsp2_dma_init);
