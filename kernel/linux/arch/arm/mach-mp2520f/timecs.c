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

/* Use timer 1 as system timer */
#define TIMER_BASE IMX_TIM1_BASE

static struct clock_event_device clockevent_mp2520f;
static enum clock_event_mode clockevent_mode = CLOCK_EVT_MODE_UNUSED;

/*
 * IRQ handler for the timer
 */
static irqreturn_t
mp2520f_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &clockevent_mp2520f;
	uint32_t tstat;
	irqreturn_t ret = IRQ_NONE;

	/* clear the interrupt */
	tstat = IMX_TSTAT(TIMER_BASE);
	IMX_TSTAT(TIMER_BASE) = 0;

	if (tstat & TSTAT_COMP) {
		evt->event_handler(evt);
		ret = IRQ_HANDLED;
	}

	return ret;
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
	unsigned long tcmp;

	tcmp = IMX_TCN(TIMER_BASE) + evt;
	IMX_TCMP(TIMER_BASE) = tcmp;

	return (int32_t)(tcmp - IMX_TCN(TIMER_BASE)) < 0 ? -ETIME : 0;
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
	IMX_TCTL(TIMER_BASE) &= ~TCTL_IRQEN;
	if (mode != clockevent_mode) {
		/* Set event time into far-far future */
		IMX_TCMP(TIMER_BASE) = IMX_TCN(TIMER_BASE) - 3;
		/* Clear pending interrupt */
		IMX_TSTAT(TIMER_BASE) &= ~TSTAT_COMP;
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
		IMX_TCTL(TIMER_BASE) |= TCTL_IRQEN;
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
	clockevent_mp2520f.mult = div_sc(mp2520f_get_perclk1(), NSEC_PER_SEC,
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
	return IMX_TCN(TIMER_BASE);
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
		clocksource_hz2mult(mp2520f_get_perclk1(), clocksource_mp2520f.shift);
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
	IMX_TCTL(TIMER_BASE) = 0;
	IMX_TPRER(TIMER_BASE) = 0;

	IMX_TCTL(TIMER_BASE) = TCTL_FRR | TCTL_CLK_PCLK1 | TCTL_TEN;
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