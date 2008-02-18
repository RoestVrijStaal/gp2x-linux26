#ifndef MP25XXF_REGS_DMA_H_
#define MP25XXF_REGS_DMA_H_

/* DMA Command Register 0 */
#define DMACOM0 			__REGW(0xc0000200)
#define DMACOM0_BURSTSIZE 		(3 << 14)
#define DMACOM0_BURSTSIZE_NOBURST 	(0)
#define DMACOM0_BURSTSIZE_4WORD  	(1)
#define DMACOM0_BURSTSIZE_8WORD  	(2)
#define DMACOM0_SRCADDRINC 		(1 << 13)
#define DMACOM0_FLOWSRC 		(1 << 12)
#define DMACOM0_SRCMEMFMT 		(3 << 10)
#define DMACOM0_SRCMEMFMT_WORD 		(0)
#define DMACOM0_SRCMEMFMT_LS 		(1)
#define DMACOM0_SRCMEMFMT_MS 		(2)
#define DMACOM0_SRCPERIWD 		(3 << 8)
#define DMACOM0_SRCPERIWD_BYTE		(0)
#define DMACOM0_SRCPERIWD_HALFWORD	(1)
#define DMACOM0_TRGADDRINC 		(1 << 5)
#define DMACOM0_FLOWTRG 		(1 << 4)
#define DMACOM0_TRGMEMFMT 		(3 << 2)
#define DMACOM0_TRGMEMFMT_WORD 		(0)
#define DMACOM0_TRGMEMFMT_LS 		(1)
#define DMACOM0_TRGMEMFMT_MS 		(2)
#define DMACOM0_TRGPERIWD 		(3 << 0)
#define DMACOM0_TRGPERIWD_BYTE 		(0)
#define DMACOM0_TRGPERIWD_HALFWORD	(1)

/* DMA Command Register 1 */
#define DMACOM1 			__REGW(0xc0000202)
#define DAMCOM1_SRCADDRSTEP 		(255 << 8)
#define DAMCOM1_TRGADDRSTEP 		(255 << 0)

/* DMA Command Register 2 */
#define DMACOM2 			__REGW(0xc0000204)

/* DMA Control/Status Register */
#define DMACONS 			__REGW(0xc0000206)
#define DMACONS_DMARUN 			(1 << 10)
#define DMACONS_ENDIRQEN 		(1 << 9)
#define DMACONS_FLYBYS 			(1 << 4)
#define DMACONS_FLYBYT	 		(1 << 3)
#define DMACONS_REQPEND 		(1 << 2)
#define DMACONS_ENDINTR 		(1 << 1)

/* DMA Source / Target Address Register */ 
#define SRCLADDR 			__REGW(0xc0000208)
#define SRCHADDR 			__REGW(0xc000020a)

/* DMA Source / Target Address Register */
#define TRGLADDR 			__REGW(0xc000020c)
#define TRGHADDR 			__REGW(0xc000020e)

/* DMA Interrupt Status Register */
/* TODO is the address correct? */
#define DMAINT 				__REGW(0xc0000000)

/* DMA Channel Source I/O Request Map Register */
#define DCH0SRM 			__REGW(0xc0000100)
#define DCH0SRM_VALID 			(1 << 6)
#define DCH0SRM_NUM 			(64 << 0)

#define DCH1SRM 			__REGW(0xc0000104)
#define DCH1SRM_VALID 			(1 << 6)
#define DCH1SRM_NUM 			(64 << 0)

#define DCH2SRM 			__REGW(0xc0000108)
#define DCH2SRM_VALID 			(1 << 6)
#define DCH2SRM_NUM 			(64 << 0)

#define DCH3SRM 			__REGW(0xc000010c)
#define DCH3SRM_VALID 			(1 << 6)
#define DCH3SRM_NUM 			(64 << 0)

#define DCH4SRM 			__REGW(0xc0000110)
#define DCH4SRM_VALID 			(1 << 6)
#define DCH4SRM_NUM 			(64 << 0)

#define DCH5SRM 			__REGW(0xc0000114)
#define DCH5SRM_VALID 			(1 << 6)
#define DCH5SRM_NUM 			(64 << 0)

#define DCH6SRM 			__REGW(0xc0000118)
#define DCH6SRM_VALID 			(1 << 6)
#define DCH6SRM_NUM 			(64 << 0)

#define DCH6SRM 			__REGW(0xc0000118)
#define DCH6SRM_VALID 			(1 << 6)
#define DCH6SRM_NUM 			(64 << 0)

/* TODO until 15 */

#define DCH0TRM 			__REGW(0xc0000102)
#define DCH0TRM_VALID 			(1 << 6)
#define DCH0TRM_NUM 			(64 << 0)

/* TODO until 15 */

#endif
