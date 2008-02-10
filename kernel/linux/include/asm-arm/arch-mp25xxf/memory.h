#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef CONFIG_MP25XXF_SHADOW_ENABLE
        #define PHYS_OFFSET     UL(0x00000000)
#else
        #define PHYS_OFFSET     UL(0xa0000000)
#endif

#define DRAM_START 			PHYS_OFFSET
#define DRAM_LENGTH 		(64*1024*1024) /* 64MB */

#define __virt_to_bus(x)        (x - PAGE_OFFSET + PHYS_OFFSET)
#define __bus_to_virt(x)        (x - PHYS_OFFSET + PAGE_OFFSET)

#endif /*MEMORY_H_*/
