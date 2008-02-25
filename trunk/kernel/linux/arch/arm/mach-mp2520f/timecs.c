#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/time.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/leds.h>
#include <asm/irq.h>
#include <asm/mach/time.h>

#define DEBUG

static struct clock_event_device clockevent_mp2520f;
static enum clock_event_mode clockevent_mode = CLOCK_EVT_MODE_UNUSED;

unsigned long mmsp2_get_pclk(void);

/*
 * IRQ handler for the timer
 */
static irqreturn_t
mp2520f_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &clockevent_mp2520f;

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction mp2520f_timer_irq = {
	.name		= "MP2520F Timer 1",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= mp2520f_timer_interrupt,
};

/* clockevent interface */

static int mp2520f_set_next_event(unsigned long evt,
				  struct clock_event_device *unused)
{
	TMATCH0 = TCOUNT + evt;
	return (int32_t)(TMATCH0 - TCOUNT) < 0 ? -ETIME : 0;
}

#ifdef DEBUG
static const char *clock_event_mode_label[]={
	[CLOCK_EVT_MODE_PERIODIC] = "CLOCK_EVT_MODE_PERIODIC",
	[CLOCK_EVT_MODE_ONESHOT]  = "CLOCK_EVT_MODE_ONESHOT",
	[CLOCK_EVT_MODE_SHUTDOWN] = "CLOCK_EVT_MODE_SHUTDOWN",
	[CLOCK_EVT_MODE_UNUSED]   = "CLOCK_EVT_MODE_UNUSED"
};
#endif /*DEBUG*/

static void mp2520f_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{
	unsigned long flags;

	/*
	 * The timer interrupt generation is disabled at least
	 * for enough time to call mp2520f_set_next_event()
	 */
	local_irq_save(flags);
	/* Disable interrupt in GPT module */
	TINTEN &= ~1;
	if (mode != clockevent_mode) {
		/* Set event time into far-far future */
		TMATCH0 = TCOUNT - 3;
		/* Clear pending interrupt */
		TSTATUS &= 1;
	}

#ifdef DEBUG
	printk(KERN_INFO "mp2520f_set_mode: changing mode from %s to %s\n",
		clock_event_mode_label[clockevent_mode], clock_event_mode_label[mode]);
#endif /*DEBUG*/

	/* Remember timer mode */
	clockevent_mode = mode;
	local_irq_restore(flags);

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		printk(KERN_ERR "mp2520f_set_mode: Periodic mode is not supported for i.MX\n");
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		/*
		 * Do not put overhead of interrupt enable/disable into
		 * mp2520f_set_next_event(), the core has about 4 minutes
		 * to call mp2520f_set_next_event() or shutdown clock after
		 * mode switching
		 */
		local_irq_save(flags);
		TINTEN |= 1;
		TCONTROL |= TIMER_EN;
		local_irq_restore(flags);
		break;
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_RESUME:
		/* Left event sources disabled, no more interrupts appears */
		break;
	}
}

static struct clock_event_device clockevent_mp2520f = {
	.name		= "mp2520f_timer1",
	.features	= CLOCK_EVT_FEAT_ONESHOT,
	.shift		= 32,
	.set_mode	= mp2520f_set_mode,
	.set_next_event	= mp2520f_set_next_event,
	.rating		= 200,
};

static int __init mp2520f_clockevent_init(void)
{
	clockevent_mp2520f.mult = div_sc(mmsp2_get_pclk(), NSEC_PER_SEC,
					clockevent_mp2520f.shift);
	clockevent_mp2520f.max_delta_ns =
		clockevent_delta2ns(0xfffffffe, &clockevent_mp2520f);
	clockevent_mp2520f.min_delta_ns =
		clockevent_delta2ns(0xf, &clockevent_mp2520f);

	clockevent_mp2520f.cpumask = cpumask_of_cpu(0);

	clockevents_register_device(&clockevent_mp2520f);

	return 0;
}

/* 
 * clocksource interface
 */
cycle_t mp2520f_get_cycles(void)
{
	return TCOUNT;
}

static struct clocksource clocksource_mp2520f = {
	.name 		= "mp2520f_timer1",
	.rating		= 200,
	.read		= mp2520f_get_cycles,
	.mask		= 0xFFFFFFFF,
	.shift 		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static int __init mp2520f_clocksource_init(void)
{
	clocksource_mp2520f.mult =
		clocksource_hz2mult(mmsp2_get_pclk(), clocksource_mp2520f.shift);
	clocksource_register(&clocksource_mp2520f);

	return 0;
}


/*
 * Set up timer hardware into expected mode and state.
 */
static void __init mp2520f_timer_hardware_init(void)
{
	/*
	 * Initialise to a known state (all timers off, and timing reset)
	 */
	/* clear status on all timers */
	TSTATUS = 0xffff;          
	/* diable all timers */
	TCONTROL &= ~(TIMER_EN);   
}


static void __init mp2520f_timer_init(void)
{
	mp2520f_timer_hardware_init();
	mp2520f_clocksource_init();

	mp2520f_clockevent_init();

	/*
	 * Make irqs happen for the system timer
	 */
	setup_irq(IRQ_TIMER_0, &mp2520f_timer_irq);
}

struct sys_timer mp2520f_timer = {
	.init		= mp2520f_timer_init,
};
