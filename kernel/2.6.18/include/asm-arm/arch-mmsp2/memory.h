#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef CONFIG_MMSP2_SHADOW_ENABLE
        #define PHYS_OFFSET     UL(0x00000000)
#else
        #define PHYS_OFFSET     UL(0xa0000000)
#endif

#define __virt_to_bus(x)        (x - PAGE_OFFSET + PHYS_OFFSET)
#define __bus_to_virt(x)        (x - PHYS_OFFSET + PAGE_OFFSET)

#endif /*MEMORY_H_*/
