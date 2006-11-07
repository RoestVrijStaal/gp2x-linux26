#ifndef IRQS_H_
#define IRQS_H_

#define IRQ_TIMER		5 /* Timer -> Timer 0,1,2,3 */
#define MAIN_IRQ_END	(IRQ_TIMER + 1)
/* TIMER sub irqs */
#define TIMER_SUB_IRQ_BASE	MAIN_IRQ_END	
#define IRQ_TIMER_SUB(x)    (TIMER_SUB_IRQ_BASE + (x))
#define IRQ_TIMER0			IRQ_TIMER_SUB(0)
#define IRQ_TIMER1			IRQ_TIMER_SUB(1)
#define IRQ_TIMER2			IRQ_TIMER_SUB(2)
#define IRQ_TIMER3			IRQ_TIMER_SUB(3)
#define TIMER_SUB_IRQ_END	IRQ_TIMER_SUB(4)


#endif /*IRQS_H_*/
