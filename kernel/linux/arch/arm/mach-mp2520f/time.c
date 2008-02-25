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

/**
 * TODO use new clocksource/event API
 * 
 */


/* Use timer 1 as system timer */

int gok = 0;

/*
 * Returns number of us since last clock interrupt.  Note that interrupts
 * will have been disabled by do_gettimeoffset()
 */
static unsigned long mmsp2_gettimeoffset(void)
{
	unsigned long ticks_to_match, elapsed, usec;
	short int tcount;
	
	printk("gettimeoffset\n");
	gok++;
/*	if (gok > 500)
	{
		printk("gokkk\n");
	}*/
	/* Get ticks before next timer match */
	tcount = TCOUNT;
	if (tcount > TMATCH0)
		ticks_to_match = 65536 - tcount;
	else
		ticks_to_match = tcount;
	/* We need elapsed ticks since last match */
	//elapsed = LATCH - ticks_to_match;

	/* Now convert them to usec */
	//usec = (ticks_to_match * tick_nsec / 1000);
	ticks_to_match = TMATCH0 - TCOUNT;
	elapsed = LATCH - ticks_to_match;
	usec = (unsigned long)(elapsed * tick_nsec / 1000)/LATCH;
	if (gok)
		printk("%ld %u\n", usec, tcount);
	return usec;
}

int num = 0;

/*
 * IRQ handler for the timer
 */
static irqreturn_t
mmsp2_timer_interrupt(int irq, void *dev_id)
{
	int next_match;
	
	write_seqlock(&xtime_lock);
	
	//printk("timer tick\n");
	num++;
#if 0
/*	if (num > 100)
		printk("timer tick %d\n", num);*/
	do {
		timer_tick();
		//printk("timer tick n\n");
		/* clear match on timer 0 */
		TSTATUS = TCNT0;
		next_match = (TMATCH0 += LATCH);
	} while( (signed long)(next_match - TCOUNT) <= 8 );
#endif
	timer_tick();
	write_sequnlock(&xtime_lock);
	printk("tick\n");
	return IRQ_HANDLED;
}

static struct irqaction mmsp2_timer_irq = {
	.name		= "MMSP2 Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= mmsp2_timer_interrupt,
};

static void __init mmsp2_timer_setup(void)
{
	/* set initial match at 100 */
	TMATCH0 = 100;
	/* clear status on all timers */
	TSTATUS = 0xffff;          
	/* diable all timers */
	TCONTROL &= ~(TIMER_EN);        
	
	/* to reset the TCOUNT register we have to 
	 * write anything wait a cycle and then write 0 
	 */

	TCOUNT = 0;
	{ int i; for (i = 8; i > 0; i--) ; }
	TCOUNT = 1;
	/* enable all timers */
	TCONTROL |= TIMER_EN;
}
/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static void __init mmsp2_timer_init(void)
{
	mmsp2_timer_setup();
	setup_irq(IRQ_TIMER_0, &mmsp2_timer_irq);
	printk("Timer setup\n");
}

struct sys_timer mmsp2_timer = {
	.init		= mmsp2_timer_init,
	.offset		= mmsp2_gettimeoffset,
#if 0
	.resume 	= mmsp2_timer_setup,
#endif
};
