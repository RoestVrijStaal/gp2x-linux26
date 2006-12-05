#ifndef LCD_H_
#define LCD_H_

struct mmsp2_platform_lcd
{
	int max_width;
	int max_height;
	int (*init)(void);
};

#endif /*LCD_H_*/
