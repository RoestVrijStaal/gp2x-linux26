#include <asm/arch/hardware.h>
#include <asm/arch/irqs.h>
.macro  disable_fiq
.endm

.macro  get_irqnr_and_base, irqnr, irqstat, base, tmp
		mov     r4, #NIO_BASE	@ virtual address of IRQ registers
		orr     r4, r4, #0x00000800
		ldr     \irqnr, [r4, #0x8]      @ read INTMSK
		ldr     \irqstat, [r4, #0x10]   @ read INTPND
		bics    \irqstat, \irqstat, \irqnr
		bics    \irqstat, \irqstat, \irqnr
		beq     1002f
		mov     \irqnr, #0
1001:	tst     \irqstat, #1
		bne     1002f                   @ found IRQ
		add     \irqnr, \irqnr, #1
		mov     \irqstat, \irqstat, lsr #1
		cmp     \irqnr, #32             @ irq : 0~31
		bcc     1001b
1002:
.endm
