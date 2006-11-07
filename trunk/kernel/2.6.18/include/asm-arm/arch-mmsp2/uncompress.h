/*
 * This file is included by arch/arm/boot/compressed/misc.c
 * (which,among other things,outputs the Uncompressing
 * Linux message). You are required to provide two functions,
 * arch_decomp_setup to setup the UART, and puts for out-
 * putting the decompression message to the UART.
 */


/*
 * Hardware-specific routine to put a char to the
 * debug UART, This assumes the serial port has already been
 * initialized by the bootloader.  We search for the first enabled
 * port in the most probable order.  If you didn't setup a port in
 * your bootloader then nothing will appear (which might be desired).
 */

#define UART0           ((volatile unsigned short *)0xc0001200)
#define UART1           ((volatile unsigned short *)0xc0001220)
#define UART2           ((volatile unsigned short *)0xc0001240)
#define UART3           ((volatile unsigned short *)0xc0001260)

#define UART UART0

/*
 * This does not append a newline
 */
static void putc(int c)
{
	while (!(UART[4] & 0x4));
	UART[8] = c;
}
static inline void flush(void)
{

}

/* Hardware-specific routine to put a string to
 * setup the UART mentioned above.
 */
#define arch_decomp_setup()

/* Watchdog is not used for most ARM Linux implementations */
#define arch_decomp_wdog()


