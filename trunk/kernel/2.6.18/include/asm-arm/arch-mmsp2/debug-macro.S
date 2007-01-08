#include "hardware.h"                
                .macro  addruart,rx
                mrc     p15, 0, \rx, c1, c0
                tst     \rx, #1                 @ MMU enabled?
                moveq   \rx, #MMSP2_NIO_START        @ physical
                movne   \rx, #MMSP2_NIO_BASE        @ virtual
                orr     \rx, \rx, #0x00001200
                .endm

                .macro  senduart,rd,rx
                strh    \rd, [\rx, #0x10]
                .endm

                .macro  busyuart,rd,rx
                nop
                .endm

                .macro  waituart,rd,rx
                mov     \rd, \rx
1001:           ldrh    \rd, [\rx, #0x8]
                and     \rd, \rd, #(1 << 2)
                tst     \rd, #(1 << 2)
                beq     1001b
                .endm
