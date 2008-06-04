#ifndef MP25XXF_REGS_MLC_H_
#define MP25XXF_REGS_MLC_H_
/*============================================================================*
 *                        Multi Layer Controller (MLC)                        * 
 *============================================================================*/
#define MP25XXF_MLC_START	0xc0002880
#define MP25XXF_MLC_END		0xc00029c0
#define MP25XXF_MLC_BASE	io_p2v(MP25XXF_MLC_START)

/* Overlay Control Register */
#define MLC_OVLAY_CNTR 				__REGW(MLC_START + 0x0)
//[15:14] R/W Reserved Must be 0 2’b0
#define MLC_OVLAY_CNTR_DISP_FLD_POL		(1 << 13) 	/* When Field Mode, Select Polarity of Field Signal from Display Sync. Generation */ 
#define MLC_OVLAY_CNTR_DISP_GAMM_BYPATH 	(1 << 12) 	/* Select Bypath of RGB Gamma Table */
#define MLC_OVLAY_CNTR_DISP_SWAP 		(1 << 11) 	/* Select Priority of OSD and Sub-Picture */ 
//[10] R/W Reserved Must be 0 0
#define MLC_OVLAY_CNTR_DISP_CURSOR 		(1 << 9) 	/* Cursor Enable / Disable */
#define MLC_OVLAY_CNTR_DISP_SUBPICTURE 		(1 << 8) 	/* Sub-Picture Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_OSD 		(1 << 7) 	/* OSD Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL5EN 		(1 << 6) 	/* Region 5 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL4EN 		(1 << 5) 	/* Region 4 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL3EN 		(1 << 4) 	/* Region 3 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL2EN 		(1 << 3) 	/* Region 2 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_SDL1EN 		(1 << 2) 	/* Region 1 Enable/ Disable */
#define MLC_OVLAY_CNTR_DISP_VLBON 		(1 << 1) 	/* Video Plane B Enable/Disable at Video(Y/Cb/Cr) Layer */
#define MLC_OVLAY_CNTR_DISP_VLAON 		(1 << 1) 	/* Video Plane A Enable/Disable at Video(Y/Cb/Cr) Layer */

/* Effect of Video Image */
#define MLC_YUV_EFECT 				__REGW(MLC_START + 0x2)
/* [15:10] Reserved */
#define MLC_YUV_EFECT_VLB_BT 			(1 << 9)
#define MLC_YUV_EFECT_VLB_TP_MR_H 		(1 << 8)
#define MLC_YUV_EFECT_VLB_TP_MR_V 		(1 << 7)
#define MLC_YUV_EFECT_VLB_BT_MR_H 		(1 << 6)
#define MLC_YUV_EFECT_VLB_BT_MR_V 		(1 << 5)
#define MLC_YUV_EFECT_VLA_BT 			(1 << 4)
#define MLC_YUV_EFECT_VLA_TP_MR_H 		(1 << 3)
#define MLC_YUV_EFECT_VLA_TP_MR_V 		(1 << 2)
#define MLC_YUV_EFECT_VLA_BT_MR_H 		(1 << 1)
#define MLC_YUV_EFECT_VLA_BT_MR_V 		(1 << 0)

/* Video Image Control Register */
#define MLC_YUV_CNTL 				__REGW(MLC_START + 0x4)
/* [15:3] Reserved */
#define MLC_YUV_CNTL_SC2DP			(1 << 12) /* Input data path for Video Region A */
#define MLC_YUV_CNTL_SC2DP_EXTMEM		(0) /* External Memory */
#define MLC_YUV_CNTL_SC2DP_SCALE		(1) /* Scaler */
#define MLC_YUV_CNTL_FDC2DP			(1 << 11) /* Input data path for Video Region A */
#define MLC_YUV_CNTL_FDC2DP_EXTMEM		(0) /* External Memory */
#define MLC_YUV_CNTL_FDC2DP_CONV		(1) /* Frame Dimensin Converter */
#define MLC_YUV_CNTL_PRIO			(1 << 10) /* Region Priority */
#define MLC_YUV_CNTL_PRIO_REGIONA		(0)
#define MLC_YUV_CNTL_PRIO_REGIONB		(0)
/* [9] Reserved */
#define MLC_YUV_CNTL_VLB_TP_STEP 		(3 << 7)
#define MLC_YUV_CNTL_VLB_BT_STEP 		(3 << 5)
#define MLC_YUV_CNTL_VLA_TP_STEP 		(3 << 2)
#define MLC_YUV_CNTL_VLA_BT_STEP 		(3 << 0)
#define MLC_YUV_CNTL_NOSKIP			(0)
#define MLC_YUV_CNTL_SKIP1			(1)
#define MLC_YUV_CNTL_SKIP2			(2)
#define MLC_YUV_CNTL_SKIP3			(3)

/* Scale Factor Register of Region A */
#define MLC_YUVA_TP_HSC 			__REGW(MLC_START + 0x6)

/* Scale Factor Register of Region A */
#define  MLC_YUVA_BT_HSC			__REGW(MLC_START + 0x8)

/* Scale Factor Register of Region A */
#define MLC_YUVA_TP_VSCL 			__REGW(MLC_START + 0xa)
#define MLC_YUVA_TP_VSCH 			__REGW(MLC_START + 0xc)

/* Scale Factor Register of Region A */
#define MLC_YUVA_BT_VSCL 			__REGW(MLC_START + 0xe)
#define MLC_YUVA_BT_VSCH 			__REGW(MLC_START + 0x10)

/* Horizontal Pixel Width Register of Region A */
#define MLC_YUVA_TP_PXW 			__REGW(MLC_START + 0x12)

/* Horizontal Pixel Width Register of Region A */
#define MLC_YUVA_BT_PXW 			__REGW(MLC_START + 0x14)

/* Coordinate Register of Region A */
#define MLC_YUVA_STX				__REGW(MLC_START + 0x16)

/* Coordinate Register of Region A */
#define MLC_YUVA_ENDX 				__REGW(MLC_START + 0x18)

/* Coordinate Register of Region A */
#define MLC_YUVA_TP_STY				__REGW(MLC_START + 0x1a)

/* Coordinate Register of Region A */
#define MLC_YUVA_TP_ENDY			__REGW(MLC_START + 0x1c)

/* Coordinate Register of Region A */
#define MLC_YUVA_BT_ENDY 			__REGW(MLC_START + 0x1e)

/* Source Address of Region A */
#define MLC_YUVA_TP_OADRL			__REGW(MLC_START + 0x20)
#define MLC_YUVA_TP_OADRH 			__REGW(MLC_START + 0x22)

/* Source Address of Region A */
#define MLC_YUVA_TP_EADRL			__REGW(MLC_START + 0x24)
#define MLC_YUVA_TP_EADRH 			__REGW(MLC_START + 0x26)

/* Source Address of Region A */
#define MLC_YUVA_BT_OADRL			__REGW(MLC_START + 0x28)
#define MLC_YUVA_BT_OADRH 			__REGW(MLC_START + 0x2a)

/* Source Address of Region A */
#define MLC_YUVA_BT_EADRL			__REGW(MLC_START + 0x2c)
#define MLC_YUVA_BT_EADRH 			__REGW(MLC_START + 0x2e)

/* Scale Factor Register of Region B */
#define MLC_YUVB_TP_HSC 			__REGW(MLC_START + 0x30) 

/* Scale Factor Register of Region B */
#define MLC_YUVB_BT_HSC 			__REGW(MLC_START + 0x32)

/* Scale Factor Register of Region B */
#define MLC_YUVB_TP_VSCL 			__REGW(MLC_START + 0x34)
#define MLC_YUVB_TP_VSCH 			__REGW(MLC_START + 0x36)

/* Scale Factor Register of Region B */
#define MLC_YUVB_BT_VSCL 			__REGW(MLC_START + 0x38)
#define MLC_YUVB_BT_VSCH 			__REGW(MLC_START + 0x3a)

/* Horizontal Pixel Width Register of Region B */
#define MLC_YUVB_TP_PXW 			__REGW(MLC_START + 0x3c)

/* Horizontal Pixel Width Register of Region B */
#define MLC_YUVB_BT_PXW 			__REGW(MLC_START + 0x3e)

/* Coordinate Register of Region B */
#define MLC_YUVB_STX				__REGW(MLC_START + 0x40)

/* Coordinate Register of Region B */
#define MLC_YUVB_ENDX 				__REGW(MLC_START + 0x42)

/* Coordinate Register of Region B */
#define MLC_YUVB_TP_STY				__REGW(MLC_START + 0x44)

/* Coordinate Register of Region B */
#define MLC_YUVB_TP_ENDY			__REGW(MLC_START + 0x46)

/* Coordinate Register of Region B */
#define MLC_YUVB_BT_ENDY 			__REGW(MLC_START + 0x48)

/* Source Address of Region B */
#define MLC_YUVB_TP_OADRL			__REGW(MLC_START + 0x4a)
#define MLC_YUVB_TP_OADRH 			__REGW(MLC_START + 0x4c)

/* Source Address of Region B (MLC_YUVB_TP_EADRL, MLC_YUVB_TP_EADRH) */
#define MLC_YUVB_TP_EADRL			__REGW(MLC_START + 0x4e)
#define MLC_YUVB_TP_EADRH 			__REGW(MLC_START + 0x50)

/* Source Address of Region B (MLC_YUVB_BT_OADRL, MLC_YUVB_BT_OADRH) */
#define MLC_YUVB_BT_OADRL			__REGW(MLC_START + 0x52)
#define MLC_YUVB_BT_OADRH 			__REGW(MLC_START + 0x54)

/* Source Address of Region B (MLC_YUVB_BT_EADRL, MLC_YUVB_BT_EADRH) */
#define MLC_YUVB_BT_EADRL			__REGW(MLC_START + 0x56)
#define MLC_YUVB_BT_EADRH 			__REGW(MLC_START + 0x58)

/* Still Image Control Register */
#define MLC_STL_CNTL 				__REGW(MLC_START + 0x5a)
#define MLC_STL_CNTL_BPP 			(3 << 9)
#define MLC_STL_CNTL_BPP_4 			(0)
#define MLC_STL_CNTL_BPP_8 			(1)
#define MLC_STL_CNTL_BPP_16 			(2)
#define MLC_STL_CNTL_BPP_24 			(3)
#define MLC_STL_CNTL_5ACT 			(1 << 8)
#define MLC_STL_CNTL_4ACT 			(1 << 6)
#define MLC_STL_CNTL_3ACT 			(1 << 4)
#define MLC_STL_CNTL_2ACT 			(1 << 2)
#define MLC_STL_CNTL_1ACT 			(1 << 0)

/* Mix/ Mux Control Register */
#define MLC_STL_MIXMUX 				__REGW(MLC_START + 0x5c)
#define MLC_STL_MIXMUX_5			(3 << 8)
#define MLC_STL_MIXMUX_4			(3 << 6)
#define MLC_STL_MIXMUX_3			(3 << 4)
#define MLC_STL_MIXMUX_2			(3 << 2)
#define MLC_STL_MIXMUX_1			(3 << 0)
#define MLC_STL_MIXMUX_STL			(0)
#define MLC_STL_MIXMUX_CKEY			(1)
#define MLC_STL_MIXMUX_ABLEND			(2)

/* Alpha Blending Value Register */
#define MLC_STL_ALPHAL 				__REGW(MLC_START + 0x5e)
#define MLC_STL_ALPHAH_3 			(15 << 8)
#define MLC_STL_ALPHAH_2 			(15 << 4)
#define MLC_STL_ALPHAH_1 			(15 << 0)
/* Alpha Blending Value Register */
#define MLC_STL_ALPHAH 				__REGW(MLC_START + 0x60)
#define MLC_STL_ALPHAH_5 			(15 << 4)
#define MLC_STL_ALPHAH_4 			(15 << 0)

/* Coordinate Register of Still Image(RGB) Layer 1 */
#define MLC_STL1_STX 				__REGW(MLC_START + 0x62)
#define MLC_STL1_ENDX 				__REGW(MLC_START + 0x64)
#define MLC_STL1_STY 				__REGW(MLC_START + 0x66)
#define MLC_STL1_ENDY 				__REGW(MLC_START + 0x68)
/* Coordinate Register of Still Image(RGB) Layer 2 */
#define MLC_STL2_STX 				__REGW(MLC_START + 0x6a)
#define MLC_STL2_ENDX 				__REGW(MLC_START + 0x6c)
#define MLC_STL2_STY 				__REGW(MLC_START + 0x6e)
#define MLC_STL2_ENDY 				__REGW(MLC_START + 0x70)
/* Coordinate Register of Still Image(RGB) Layer 3 */
#define MLC_STL3_STX 				__REGW(MLC_START + 0x72)
#define MLC_STL3_ENDX 				__REGW(MLC_START + 0x74)
#define MLC_STL3_STY 				__REGW(MLC_START + 0x76)
#define MLC_STL3_ENDY 				__REGW(MLC_START + 0x78)
/* Coordinate Register of Still Image(RGB) Layer 4 */
#define MLC_STL4_STX 				__REGW(MLC_START + 0x7a)
#define MLC_STL4_ENDX 				__REGW(MLC_START + 0x7c)
#define MLC_STL4_STY 				__REGW(MLC_START + 0x7e)
#define MLC_STL4_ENDY 				__REGW(MLC_START + 0x80)

/* Color Key Register of Still Image (RGB) Layer */
#define MLC_STL_CKEY_GR 			__REGW(MLC_START + 0x82) 
#define MLC_STL_CKEY_GR_G 			(0xff << 8)
#define MLC_STL_CKEY_GR_R 			(0xff << 0)

/* Color Key Register of Still Image (RGB) Layer */
#define MLC_STL_CKEY_B 				__REGW(MLC_START + 0x84) 
#define MLC_STL_CKEY_B_B 			(0xff << 0)

/* Scale Factor Register of Still Image (RGB) Layer */
#define MLC_STL_HSC 				__REGW(MLC_START + 0x86) 

/* Scale Factor Register of Still Image (RGB) Layer */
#define MLC_STL_VSCL 				__REGW(MLC_START + 0x88)
#define MLC_STL_VSCH 				__REGW(MLC_START + 0x8a) 

/* Horizontal Width Register of Still Image(RGB) Layer */
#define MLC_STL_HW 				__REGW(MLC_START + 0x8c) /* [14:0] */

/* Source Address of Still Image (RGB) Layer */
#define MLC_STL_OADRL				__REGW(MLC_START + 0x8e)
#define MLC_STL_OADRH				__REGW(MLC_START + 0x90)
#define MLC_STL_EADRL				__REGW(MLC_START + 0x92)
#define MLC_STL_EADRH				__REGW(MLC_START + 0x94)

/* Source Address of OSD */
#define MLC_OSD_OADRL				__REGW(MLC_START + 0x96)
#define MLC_OSD_OADRH				__REGW(MLC_START + 0x98)
#define MLC_OSD_EADRL				__REGW(MLC_START + 0x9a)
#define MLC_OSD_EADRH				__REGW(MLC_START + 0x9c)

/* Hardware Cursor Control Register */
#define MLC_HWC_CNTR 				__REGW(MLC_START + 0x9e)
#define MLC_HWC_CNTR_FALPH 			(15 << 12) /* Foreground Alpha */
#define MLC_HWC_CNTR_BALPH 			(15 << 8) /* Background Alpha */
/* [7] Reserved */
#define MLC_HWC_CNTR_SZ 			(63 << 0) /* Cursor Size */
#define MLC_HWC_CNTR_SZ_32 			(0x20) /* 32x32 */
#define MLC_HWC_CNTR_SZ_64 			(0x40) /* 64x64 */

/* Hardware Cursor Horizontal Starting Position Register */
#define MLC_HWC_STX 				__REGW(MLC_START + 0xa0) /* [11:0] */

/* Hardware Cursor Vertical Starting Position Register (MLC_HWC_STY) */
#define MLC_HWC_STY 				__REGW(MLC_START + 0xa2) /* [11:0] */

/* Hardware Cursor Foreground Color(GR) Register */
#define MLC_HWC_FGR 				__REGW(MLC_START + 0xa4)
#define MLC_HWC_FGR_G 				(0xff << 8)
#define MLC_HWC_FGR_R 				(0xff << 0)

/* Hardware Cursor Foreground Color(B) Register */
#define MLC_HWC_FB 				__REGW(MLC_START + 0xa6)
#define MLC_HWC_FB_B 				(0xff << 0)

/* Hardware Cursor Background Color(GR) Register */
#define MLC_HWC_BGR 				__REGW(MLC_START + 0xa8)
#define MLC_HWC_BGR_G 				(0xff << 8)
#define MLC_HWC_BGR_R 				(0xff << 0)

/* Hardware Cursor Background Color(B) Register */
#define MLC_HWC_BB 				__REGW(MLC_START + 0xaa)
#define MLC_HWC_BB_B 				(0xff << 0)

/* Source Address of Hardware Cursor */
#define MLC_HWC_OADRL				__REGW(MLC_START + 0xac)
#define MLC_HWC_OADRH				__REGW(MLC_START + 0xae)
#define MLC_HWC_EADRL				__REGW(MLC_START + 0xb0)
#define MLC_HWC_EADRH				__REGW(MLC_START + 0xb2)

#endif /*MP25XXF_REGS_MLC_H_*/
