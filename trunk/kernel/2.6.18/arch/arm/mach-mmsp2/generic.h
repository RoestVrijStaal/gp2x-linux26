#ifndef GENERIC_H_
#define GENERIC_H_

/* ==== Common Architecture Functions ==== */
extern void __init mmsp2_map_io(void);
extern void __init mmsp2_init_irq(void);
extern struct sys_timer mmsp2_timer;
/**
 * Gpio Functions
 */
extern void mmsp2_gpio_mode(unsigned short group, unsigned short pin, unsigned short mode);
/**
 * Clock Functions 
 */
unsigned long mmsp2_get_fclk(void);
unsigned long mmsp2_get_uclk(void);
unsigned long mmsp2_get_aclk(void);
unsigned long mmsp2_get_pclk(void);
/* ==== Platform Device Structures ==== */

/**
 * LCD Device Struct 
 */
struct mmsp2_platform_lcd
{
	int max_width; 		/* max width of the lcd resolution */
	int max_height; 	/* max height of the lcd resolution */
	int (*init)(void);	/* initialize the lcd */
};

#endif /* GENERIC_H_ */