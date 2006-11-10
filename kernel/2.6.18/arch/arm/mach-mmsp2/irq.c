/* You should provide the XXX_init_irq function here. This sets
 * up the interrupt controller. Interrupt mask and unmask func-
 * tions go here too.
 */


#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/mach/irq.h>

#if 1 
#define DEBUG_IRQ(fmt...)	printk(fmt)
#else
#define DEBUG_IRQ(fmt...)	do { } while (0)
#endif

static void
mmsp2_main_ack_irq(unsigned int irq)
{
	unsigned int ofs = (1 << irq);

	DEBUG_IRQ("irq ack %u\n",irq);
	INTMASK |= ofs;         // mask the interrupt
	SRCPEND = ofs;          // source pending flag clear
	INTPEND = ofs;          // interrupt pending flag clear
}

static void
mmsp2_main_mask_irq(unsigned int irq)
{
	DEBUG_IRQ("irq mask %u\n",irq);
	INTMASK |= (1<<irq);
}

static void
mmsp2_main_unmask_irq(unsigned int irq)
{
	DEBUG_IRQ("irq unmask %u\n",irq);
	INTMASK &= ~(1<<irq);
}


static struct irq_chip mmsp2_main_chip = {
	.name = "MAIN",
	.ack = mmsp2_main_ack_irq,
	.mask = mmsp2_main_mask_irq,
	.unmask = mmsp2_main_unmask_irq,
};

static void
mmsp2_timer_demux_handler(unsigned int irq, struct irqdesc *desc,
			struct pt_regs *regs)
{
	unsigned short mask;
	
	
	mask = TSTATUS;
	irq = IRQ_TIMER0;
	desc = irq_desc + irq;
	DEBUG_IRQ("irq demux 0x%hx\n", mask);
	while (mask) {
		if (mask & 1) {
			DEBUG_IRQ("handling irq %d\n", irq);
			desc_handle_irq(irq, desc, regs);
		}
		irq++;
		desc++;
		mask >>= 1;
	}
}

static void
mmsp2_timer_ack_irq(unsigned int irq)
{
	DEBUG_IRQ("timer ack %u\n",irq);
	TINTEN &= ~(1 << (irq - IRQ_TIMER0)); 	
	TSTATUS = (1 << (irq - IRQ_TIMER0)); 
}

static void
mmsp2_timer_mask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer mask %u\n",irq);
	TINTEN &= ~(1 << (irq - IRQ_TIMER0)); 	
}

static void
mmsp2_timer_unmask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer unmask %u %u\n",irq,  1 << (irq - IRQ_TIMER0));
	TINTEN |= (1 << (irq - IRQ_TIMER0)); 	
}

static struct irq_chip mmsp2_timer_chip = {
	.name = "TIMER",
	.ack = mmsp2_timer_ack_irq,
	.mask = mmsp2_timer_mask_irq,
	.unmask = mmsp2_timer_unmask_irq,
};

void __init
mmsp2_init_irq(void)
{
	unsigned int irq;
	
	/* disable all interrupts */
	INTMASK = 0xffffffff;
	SRCPEND = 0xffffffff;
	INTPEND = 0xffffffff;
	
	IPRIORITY = 0x0;        /* disable rotation */
	TSTATUS = 0xffff;          /* clear status on all timers */

	/* register main interrupts */
	for (irq = 0; irq < MMSP2_IRQS; irq++) 
	{
		switch(irq)
		{
			/* reserved IRQS */
			case 3:
			case 4:
			case 21:
			case 22:
			break;
			
			default:
			set_irq_chip(irq, &mmsp2_main_chip);
			set_irq_handler(irq, do_level_IRQ);
			set_irq_flags(irq, IRQF_VALID);
			break;
		}
	}
	/* gpio interrupts */
	/* timer interrupts */
	for (irq = IRQ_TIMER0; irq <= IRQ_TIMER3; irq++) 
	{
		set_irq_chip(irq, &mmsp2_timer_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID);
	}
	set_irq_chained_handler(IRQ_TIMER, mmsp2_timer_demux_handler);
	/* uart interrupts */
}
