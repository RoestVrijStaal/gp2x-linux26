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

/* timer interrupts handling */
static void
mmsp2_timer_demux_handler(unsigned int irq, struct irqdesc *desc,
			struct pt_regs *regs)
{
	unsigned short mask;
	
	mask = TSTATUS;
	irq = IRQ_TIMER_0;
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
	/* FIXME why this? do an aknownledge of the interrupt */
	/* source pending flag clear */
	SRCPEND = (1 << IRQ_TIMER);          
	/* interrupt pending flag clear */
	INTPEND = (1 << IRQ_TIMER);          
	DEBUG_IRQ("TCOUNT %d INTPEND 0x%x SRCPEND 0x%x\n", TCOUNT, INTPEND, SRCPEND);
}

static void
mmsp2_timer_ack_irq(unsigned int irq)
{
	DEBUG_IRQ("timer ack %d\n", irq);
	TSTATUS = (1 << (irq - IRQ_TIMER_0));
}

static void
mmsp2_timer_mask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer mask %d\n", irq);
	TINTEN &= ~(1 << (irq - IRQ_TIMER_0));
}

static void
mmsp2_timer_unmask_irq(unsigned int irq)
{
	DEBUG_IRQ("timer unmask %d\n", irq);
	TINTEN |= (1 << (irq - IRQ_TIMER_0));
}

static struct irq_chip mmsp2_timer_chip = {
	.name = "TIMER",
	.ack = mmsp2_timer_ack_irq,
	.mask = mmsp2_timer_mask_irq,
	.unmask = mmsp2_timer_unmask_irq,
};

/* uart interrupts handling */
static void
mmsp2_uart_demux_handler(unsigned int irq, struct irqdesc *desc,
			struct pt_regs *regs)
{
	unsigned short mask;
	
	mask = INTSTATREG;
	irq = IRQ_UART_TXD0;
	desc = irq_desc + irq;
	DEBUG_IRQ("INTSTAT 0x%x INTPEND 0x%x SRCPEND 0x%x\n", mask, INTPEND, SRCPEND);
	
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
	SRCPEND = (1 << IRQ_UART);          
	/* interrupt pending flag clear */
	INTPEND = (1 << IRQ_UART);          
}

static void
mmsp2_uart_ack_irq(unsigned int irq)
{
	DEBUG_IRQ("uart ack %d\n", irq);
	INTSTATREG = (1 << (irq - IRQ_UART_TXD0));
}

static void
mmsp2_uart_mask_irq(unsigned int irq)
{
	unsigned short int port, type;
	
	port = irq >> 2;
	type = irq & 0x3;
	DEBUG_IRQ("uart mask %d %d %d\n", irq, port, type);
	switch(type)
	{
		/* tx interrupt */
		case 0:
		UCONx(port) &= ~0xC;
		break;
		/* rx interrupt */
		case 1:
		UCONx(port)&= ~0x3;
		break;
		/* error interrupt */
		case 2:
		UCONx(port) &= ~0x40;
		break;
		/* modem interrupt */
		case 3:
		UMODx(port) &= ~0x8; 
		break;	
	}
}

static void
mmsp2_uart_unmask_irq(unsigned int irq)
{
	unsigned short int port, type;
	
	port = irq >> 2;
	type = irq & 0x3;
	DEBUG_IRQ("uart unmask %d %d %d\n", irq, port, type);
	switch(type)
	{
		/* tx interrupt */
		case 0:
		UCONx(port) |= 0xC;
		break;
		/* rx interrupt */
		case 1:
		UCONx(port) |= 0x3;
		break;
		/* error interrupt */
		case 2:
		UCONx(port) |= 0x40;
		break;
		/* modem interrupt */
		case 3:
		UMODx(port) |= 0x8; 
		break;	
	}
}

static struct irq_chip mmsp2_uart_chip = {
	.name = "UART",
	.ack = mmsp2_uart_ack_irq,
	.mask = mmsp2_uart_mask_irq,
	.unmask = mmsp2_uart_unmask_irq,
};

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
	/* clear status on UART */
	INTSTATREG = 0xffff;
	
	/* TODO disable all gpio interrupts */
	
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
	for (irq = IRQ_TIMER_START; irq <= IRQ_TIMER_END; irq++) 
	{
		set_irq_chip(irq, &mmsp2_timer_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
	set_irq_chained_handler(IRQ_TIMER, mmsp2_timer_demux_handler);
	/* uart interrupts */
	for (irq = IRQ_UART_START; irq <= IRQ_UART_END; irq++) 
	{
		set_irq_chip(irq, &mmsp2_uart_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
	set_irq_chained_handler(IRQ_UART, mmsp2_uart_demux_handler);
}
