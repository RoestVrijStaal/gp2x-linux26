/* You should provide the XXX_init_irq function here. This sets
 * up the interrupt controller. Interrupt mask and unmask func-
 * tions go here too.
 */


#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/hardware.h>
#include <asm/irq.h>

#include <asm/mach/irq.h>

#if 0 
#define DEBUG_IRQ(fmt...)	printk(fmt)
#else
#define DEBUG_IRQ(fmt...)	do { } while (0)
#endif

static void
mmsp2_main_ack_irq(unsigned int irq)
{
	unsigned int ofs = (1 << irq);

	DEBUG_IRQ("irq ack %u\n",irq);
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
	DEBUG_IRQ("TCOUNT %d INTPEND 0x%x SRCPEND 0x%x\n", TCOUNT, INTPEND, SRCPEND);
	while (mask) 
	{
		if (mask & 1) 
		{
			DEBUG_IRQ("handling irq %d\n", irq);
			desc_handle_irq(irq, desc, regs);
		}
		irq++;
		desc++;
		mask >>= 1;
	}
	/* do an aknownledge of the interrupt */
	/* source pending flag clear */
	SRCPEND = (1 << IRQ_TIMER);          
	/* interrupt pending flag clear */
	INTPEND = (1 << IRQ_TIMER);          
	DEBUG_IRQ("TCOUNT %d INTPEND 0x%x SRCPEND 0x%x\n", TCOUNT, INTPEND, SRCPEND);
}

static void
mmsp2_timer_ack_irq(unsigned int irq)
{
	DEBUG_IRQ("timer ack\n");
	TINTEN &= ~(1 << (irq - IRQ_TIMER0)); 	
	TSTATUS = (1 << (irq - IRQ_TIMER0)); 
}

static void
mmsp2_timer_mask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer mask\n");
	TINTEN &= ~(1 << (irq - IRQ_TIMER0)); 	
}

static void
mmsp2_timer_unmask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer unmask\n");
	TINTEN |= (1 << (irq - IRQ_TIMER0)); 	
}

static struct irq_chip mmsp2_timer_chip = {
	.name = "TIMER",
	.ack = mmsp2_timer_ack_irq,
	.mask = mmsp2_timer_mask_irq,
	.unmask = mmsp2_timer_unmask_irq,
};

#define GPIO_EVT(i)             (*(volatile unsigned short *)io_p2v(0xc0001100 + 2*(i)))
#define GPIO_INTENB(i)  (*(volatile unsigned short *)io_p2v(0xc00010e0 + 2*(i)))

void __init
mmsp2_init_irq(void)
{
	unsigned int irq;
	
	/* disable all interrupts */
	INTMASK = 0xffffffff;
	/* set IRQ mode to all interrupts */
	INTMOD = 0x00000000;
	
	/* disable rotation */
	IPRIORITY = 0x0;        
	/* clear status on all timers */
	TSTATUS = 0xffff;          
	/* clear tx/rx on UART */
	UINTSTAT = 0xffff;
	
	for(irq=0; irq<15; irq++)
        {
                GPIO_INTENB(irq) = 0;             /* disable all GPIO interrupts */
                GPIO_EVT(irq)     = 0xffff;       /* clear all GPIO pending registers */
        }
	
	
	SRCPEND = 0xffffffff;
	INTPEND = 0xffffffff;
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
			set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
			break;
		}
	}
	/* gpio interrupts */
	/* timer interrupts */
	for (irq = IRQ_TIMER0; irq <= IRQ_TIMER3; irq++) 
	{
		set_irq_chip(irq, &mmsp2_timer_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
	set_irq_chained_handler(IRQ_TIMER, mmsp2_timer_demux_handler);
	/* uart interrupts */
}
