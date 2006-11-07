/* You should provide the XXX_init_irq function here. This sets
 * up the interrupt controller. Interrupt mask and unmask func-
 * tions go here too.
 */


#include <linux/init.h>

void __init
mmsp2_init_irq(void)
{
	printascii("irqs\n");
}
