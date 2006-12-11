#ifndef GENERIC_H_
#define GENERIC_H_

/* ==== Common Architecture Functions ==== */
extern void __init mmsp2_map_io(void);
extern void __init mmsp2_init(void);
extern void __init mmsp2_init_irq(void);
extern struct sys_timer mmsp2_timer;
/**
 * Gpio Functions
 */
extern void mmsp2_gpio_mode(unsigned short int group, unsigned short int pin, unsigned short int mode);
extern void mmsp2_gpio_write(unsigned short int group, unsigned short int pin, unsigned short int data);

/**
 * Clock Functions 
 */
unsigned long mmsp2_get_fclk(void);
unsigned long mmsp2_get_uclk(void);
unsigned long mmsp2_get_aclk(void);
unsigned long mmsp2_get_pclk(void);



#endif /* GENERIC_H_ */