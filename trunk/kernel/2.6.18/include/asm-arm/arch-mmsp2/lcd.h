#ifndef LCD_H_
#define LCD_H_

struct mmsp2_platform_lcd
{
	int width;
	int height;
	int bpp;
	int hsync_len;
	int vsync_len;
	int (*init)(void);
};

#endif /*LCD_H_*/
