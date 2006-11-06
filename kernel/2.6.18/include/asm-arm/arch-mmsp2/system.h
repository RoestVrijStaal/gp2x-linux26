#ifndef SYSTEM_H_
#define SYSTEM_H_

static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode)
{
	switch (mode) 
	{
		/* software reset */
		case 's':
		/* Jump into ROM at address 0 */
		cpu_reset(0);
		break;

		/* hardware reset */
		case 'h':
		/* TODO */
		break;
	}
}

#endif /*SYSTEM_H_*/
