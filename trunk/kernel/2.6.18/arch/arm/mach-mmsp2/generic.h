#ifndef GENERIC_H_
#define GENERIC_H_

extern void __init mmsp2_map_io(void);
extern void __init mmsp2_init_irq(void);

extern struct sys_timer mmsp2_timer;

#endif /* GENERIC_H_ */