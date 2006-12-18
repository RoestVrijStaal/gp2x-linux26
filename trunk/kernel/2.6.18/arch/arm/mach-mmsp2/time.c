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
	unsigned long ticks_to_match, elapsed, usec;
	/* Get ticks before next timer match */
	ticks_to_match = TMATCH0 - TCOUNT;

	/* We need elapsed ticks since last match */
	elapsed = LATCH - ticks_to_match;

	/* Now convert them to usec */
	usec = (unsigned long)(elapsed * tick_nsec / 1000)/LATCH;
	return usec;
}

/*
 * IRQ handler for the timer
 */
static irqreturn_t
mmsp2_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	int next_match;
	
	write_seqlock(&xtime_lock);
	do {
		timer_tick(regs);
		/* clear match on timer 0 */
		TSTATUS = TCNT0;
		next_match = (TMATCH0 += LATCH);
	} while( (signed long)(next_match - TCOUNT) <= 8 );
	
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
	/* set initial match at 0 */
	TMATCH0 = 0x0;            
	/* clear status on all timers */
	TSTATUS = 0xffff;          
	/* diable all timers */
	TCONTROL &= ~(TIMER_EN);        
	/* Make irqs happen for the system timer */
	setup_irq(IRQ_TIMER_0, &mmsp2_timer_irq);
	/* to reset the TCOUNT register we have to 
	 * write anything wait a cycle and then write 0 
	 */
	TCOUNT = 1;
	{ int i; for (i = 10; i > 0; i--) ; }
	TCOUNT = 0;
	/* enable all timers */
	TCONTROL |= TIMER_EN;   
}

struct sys_timer mmsp2_timer = {
	.init		= mmsp2_timer_init,
	.offset		= mmsp2_gettimeoffset,
};
