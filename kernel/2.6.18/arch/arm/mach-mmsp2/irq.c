/* You should provide the XXX_init_irq function here. This sets
 * up the interrupt controller. Interrupt mask and unmask func-
 * tions go here too.
 */


#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/mach/irq.h>

static void
mmsp2_ack_irq(unsigned int irq)
{
	unsigned int ofs = (1 << irq);

	INTMASK |= ofs;         // mask the interrupt
	SRCPEND = ofs;          // source pending flag clear
	INTPEND = ofs;          // interrupt pending flag clear
}

static void
mmsp2_mask_irq(unsigned int irq)
{
	printascii("irq mask %u\n",irq);
	INTMASK |= (1<<irq);
}

static void
mmsp2_unmask_irq(unsigned int irq)
{
	printascii("irq unmask %u\n",irq);
	INTMASK &= ~(1<<irq);
}


static struct irq_chip mmsp2_internal_chip = {
	.name = "MAIN",
	.ack = mmsp2_ack_irq,
	.mask = mmsp2_mask_irq,
	.unmask = mmsp2_unmask_irq,
};

void __init
mmsp2_init_irq(void)
{
	unsigned int irq;
	printascii("irqs\n");

	/* disable all interrupts */
	INTMASK = 0xffffffff;
	SRCPEND = 0xffffffff;
	INTPEND = 0xffffffff;
	
	/* register main interrupts */
	for (irq = 0; irq < MMSP2_IRQS; irq++) {
		set_irq_chip(irq, &mmsp2_internal_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID);
	}
	/* gpio interrupts */
	/* timer interrupts */
	/* uart interrupts */
}
