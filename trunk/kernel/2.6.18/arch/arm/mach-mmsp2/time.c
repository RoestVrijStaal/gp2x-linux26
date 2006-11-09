#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/time.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/time.h>

/* Use timer 1 as system timer */

/*
 * Returns number of us since last clock interrupt.  Note that interrupts
 * will have been disabled by do_gettimeoffset()
 */
static unsigned long mmsp2_gettimeoffset(void)
{
	printascii("gettimeoffset\n");
	return 1000000;
}

/*
 * IRQ handler for the timer
 */
static irqreturn_t
mmsp2_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	printascii("timer interrupt\n");
	write_seqlock(&xtime_lock);

	TSTATUS = TCNT0;
	
	timer_tick(regs);
	write_sequnlock(&xtime_lock);

	return IRQ_HANDLED;
}

static struct irqaction mmsp2_timer_irq = {
	.name		= "MMSP2 Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= mmsp2_timer_interrupt,
};

/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static void __init mmsp2_timer_init(void)
{
	int error;
	printascii("timer init\n");
	TMATCH0 = 0;            /* set initial match at 0 */
	TSTATUS = 0xf;          /* clear status on all timers */
	TCONTROL &= ~(TIMER_EN);        /* diable all timers */
	/* Make irqs happen for the system timer */
	error = setup_irq(IRQ_TIMER0, &mmsp2_timer_irq);
	printk("error %d\n", error);
	TCOUNT = 0;
	TCONTROL |= TIMER_EN;   /* enable all timers */
}

struct sys_timer mmsp2_timer = {
	.init		= mmsp2_timer_init,
	.offset		= mmsp2_gettimeoffset,
};
