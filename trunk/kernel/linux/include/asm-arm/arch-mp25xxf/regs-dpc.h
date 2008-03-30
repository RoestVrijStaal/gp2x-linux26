#ifndef MP25XXF_REGS_DPC_H_
#define MP25XXF_REGS_DPC_H_

/*============================================================================*
 *                         Display Controller (DPC)                           * 
 *============================================================================*/
#define MP25XXF_DPC_START 0xc0002800


/**
 * Display Controller Registers
 * 
 */

#define DPC_CLKCNTL 		__REGW(0xc0002848) 	/* Clock Control */
#define DPC_HS_WIDTH 		__REGW(0xc000281a) 	/* Horizontal Sync Width */
#define DPC_HS_STR 			__REGW(0xc000281c) 	/* Horizontal Sync Start */
#define DPC_HS_END 			__REGW(0xc000291e) 	/* Horizontal Sync End */

#if 0
/* DPC Control Register */
#define DPC_CNTL 		__REGW(MP25XXF_DPC_START + 0x0)
[15:13] W Reserved Must be 0 3’b0
[12] R/W PAL NTSC/PAL Mode for ENC=0 0 : NTSC 1 : PAL 0
[11] R/W CISCYNC Get sync signal from ISP 0
[10] R/W HDTV TV Type for ENC=0 0 : SDTV 1 : HDTV 0
[9] R/W ENC External Device Mode 0 : DAC 1 : TV Encoder 0
[8] R/W TVMODE TV Mode enable 0 : LCD 1 : TV 0
[7:6] R/W DOT
Data Output Type
0 : RGB 1 : Multiplexed RGB(MRGB)
2 : YCbCr 3 : Reserved
2’b0
[5] R/W INTERACE TV Interface Mode 0 : Progressive mode 1 : Interlace mode 0
[4] R/W SYNCCBCR Add sync level for Cb and Cr in DAC mode(ENC=0). 0
[3] R/W ESAVEN
EAV/SAV Insertion Enable.
If ENC = 1, EAV/SAV is inserted to Data output.
If ENC = 0, Sync signals are created using the level of output value instead of the EAV/SAV.
0
[2:1] R/W DOF
Data Output Format
DOF 0 1 2 3
RGB 4:4:4 5:6:5 6:6:6 8:8:8
MRGB 5:5:5 5:6:5 888a 888b
YCbCr CCIR656 CCIR601 3x8 Mode 3x 10Mode
2’b0
[0] R/W ENB DPC Enable 0b

/* PAD Control Register */
#define DPC_FPICNTL 			__REGW(MP25XXF_PDC_START + 0x2) 

[15:13] R/W CHPHASE Select the phase for CLKH
3’b000 : Normal
3’b001 : +5 unit delay with inversion.
3’b010 : +6 unit delay with inversion.
3’b011 : +7 unit delay with inversion.
3’b100 : +1 unit delay.
3’b101 : +2 unit delay.
3’b110 : +3 unit delay.
3’b111 : +4 unit delay.
3’b0
[12] R/W CHMSKBNK Mask pixel clock during blank area. 0
[11:3] - Reserved -
[2:0] W Reserved Must be 0 3’b0

/* PAD Polarity Control1 Register */
#define DPC_FPIPOL1 			__REGW(MP25XXF_PDC_START + 0x4)
[15:10] - Reserved. -
[9] R/W FLDPOL Polarity control for field signal. 0
[8] R/W POLPOL Polarity control for POL / o_Y[0] PAD 0
[7] R/W XDOFFPOL Polarity control for XDOFF_BLANK / o_Y[1] PAD 0
[6] R/W PSPOL Polarity control for PS / o_Cb[0] PAD 0
[5] R/W CVPOL Polarity control for CLKV / o_Cb[1] PAD 0
[4] W Reserved Must be 0 0
[3] R/W FGPOL Polarity control for FG / o_Cr[0] PAD 0
[2] R/W DEPOL Polarity control for DE / o_Cr[1] PAD 0
[1] R/W HSPOL Polarity control for HSYNC PAD 0
[0] R/W VSPOL Polarity control for VSYNC PAD 0
/* PAD Polarity Control2 Register */ 
#define DPC_FPIPOL2 			__REGW(MP25XXF_PDC_START + 0x6)

[15:8] R/W GREENPOL Polarity control for Green / Y [7:0]. 8’b0
[7:0] R/W REDPOL Polarity control for Red / Cr [7:0]. 8’b0

/* PAD Polarity Control3 Register */ 
#define DPC_FPIPOL3 			__REGW(MP25XXF_PDC_START + 0x8)

[15:8] - Reserved -
[7:0] R/W BLUEPOL Polarity control for Blue / Cb [7:0]. 8’b0

/* PAD Enable Control1 Register */ 
#define DPC_FPIATV1 			__REGW(MP25XXF_DPC_START + 0xa)
[15:9] - Reserved. -
[8] R/W POLATV Enable control for POL / o_Y[0] PAD 0
[7] R/W XDOFFATV Enable control for XDOFF_BLANK / o_Y[1] PAD 0
[6] R/W PSATV Enable control for PS / o_Cb[0] PAD 0
[5] R/W CHATV Enable control for CLKH PAD 0
[4] R/W CVATV Enable control for CLKV / o_Cb[1] PAD 0
[3] R/W FGATV Enable control for FG / o_Cr[0] PAD 0
[2] R/W DEATV Enable control for DE / o_Cr[1] PAD 0
[1] R/W HSATV Enable control for HSYNC PAD 0
[0] R/W VSATV Enable control for VSYNC PAD 0

/* PAD Enable Control2 Register */ 
#define DPC_FPIATV2 			__REGW(MP25XXF_DPC_START + 0xc)
[15:0] R/W VD1ATV Enable control for VD[15:0]. 16’b0
/* PAD Enable Control3 Register */ 
#define DPC_FPIATV3 			__REGW(MP25XXF_DPC_START + 0xe)
[15:8] - Reserved -
[7:0] R/W VD2ATV Enable control for VD[23:16]. 8’b0
Reserved Register
Address : C000 2810h ~ C000 2814h
[15:0] - Reserved -
/* Active Width Register */
#define DPC_X_MAX			__REGW(MP25XXF_DPC_START + 0x16)
[15:12] - Reserved -
[11:0] R/W T12(XMAX) Active data width – 1. 12’d719
/* Active Height Register */
#define DPC_Y_MAX 			__REGW(MP25XXF_DPC_START + 0x18)
[15:12] - Reserved -
[11:0] R/W T13(YMAX) Active data height – 1. 12’d479

/* HSYNC Width Register */
#define DPC_HS_WIDTH 			__REGW(MP25XXF_DPC_START + 0x1a)
[15:11] - Reserved -
[10] W Reserved Must be 1 1
[9:8] R/W HSPADCFG HSYNC PAD Configuration.
2’b00 : HSYNC pin outputs horizontal sync signal(HSYNC).
2’b01 : HSYNC pin outputs the latch pulse(LP).
2’b10 : HSYNC pin outputs the DE PAD(DE/STH)
2’b11 : Reserved
2’b0
[7:0] R/W T2(HSWID) HSYNC Width – 1 8’b0
/* HSYNC Start Register */
#define DPC_HS_STR 			__REGW(MP25XXF_DPC_START + 0x1c)
[15:8] - Reserved -
[7:0] R/W T8(HSSTR) HSYNC Start Position – 1. 8’b0

/* HSYNC End Register */
#define DPC_HS_END 			__REGW(MP25XXF_DPC_START + 0x1e)

[15:8] - Reserved -
[7:0] R/W T7(HSEND) HSYNC End position – 1. 8’b0


/* VSYNC Control Register */ 
#define DPC_V_SYNC 			__REGW(MP25XXF_DPC_START + 0x20)
[15:8] R/W T10(VSSTR) VSYNC Start Position. 8’b0
[7] R/W VSFLDEN Vertical Sync Field Enable. 0
[6] R/W VSFLDPOL Vertical Sync Field polarity. 0
[5:4] R/W VSPADCFG VSYNC PAD Configuration.
2’b00 : FVSYNC pin outputs vertical sync signal(VSYNC).
2’b01 : FVSYNC pin outputs the start of First line(STV).
2’b10 : FVSYNC pin outputs the field.
2’b11 : Reserved
2’b0
[3:0] R/W T11(VSWID) VSYNC Width – 1. 4’b0
/* VSYNC End Register */
#define DPC_V_END 			__REGW(MP25XXF_DPC_START + 0x2)
[15] R/W NOSYMBNKFLD Select field has T9 – 1 Blank lines. 0
[14] R/W NOSYMBNKLINE Each field has not same lines for blank lines. 0
[13] R/W NOSYMATVFLD Select field has YMAX – 1 active lines 0
[12] R/W NOSYMATVLINE Each field has not same lines for active lines 0
[11] W Reserved Must be 1 1
[10] R/W POLSTRLVL Toggle start level of POL per every VBLANK. 0
[9] R/W SMPTE274M SMPTE 274M Interlace Flag. (If its value is ‘1” in Interlace Mode, the periods needed for the
first field and the second field vertical syncs are different. The second field period is 1H longer
than the first period.
0
[8:0] R/W T9(VSEND) VSYNC End position – 1 9’b0
Address : C000 2824h
[15:0] W Reserved Must be 0 -

/* DE Control Register */
#define DPC_DE 				__REGW(MP25XXF_DPC_START + 0x26)
[15:14] R/W DEPADCFG DE PAD Configuration.
Be caution. When DOF is 3x10 mode(DOF==3),DE outputs Cr[1].
DOF[1:0] DEPADCFG[1:0] SIGNAL
3 xx Cr[1]
xx 0 DE
xx 1 STH
xx 2 / 3 Reserved
2’b0
[13:12] W Reserved Must be 0 2’b0
[11:4] R/W T3(DESTR) DE Start position – 1. 8’b0
[3:0] R/W DEWID DE Delay
4’b0000 : 0 delay
4’b0001 : +1 delay, 4’b0010 : +2 delay, 4’b0011 : +3 delay
4’b1001 : - 1 delay, 4’b1010 : - 2 delay, 4’b1011 : - 3 delay
the other : Reserved
4’b0

/* S Control Register */
#define DPC_PS 				__REGW(MP25XXF_DPC_START + 0x28)
[15:12] R/W T31(PSEND) PS End position. 4’b0
[11:0] R/W T30(PSSTR) PS Start position.
It must be greater than 0 and less than or equal to T12(XMAX) 12’b0

/* FG Control Register */
#define DPC_FG 				__REGW(MP25XXF_DPC_START + 0x2a)
[15] R/W PSEN PS/FG/XDOFF Enable 0
[14:12] - Reserved -
[11:8] R/W XDFPLSWID(T32) XDOFF pulse width 4’b0
[7:4] - Reserved -
[3:0] R/W FGWID(T33) FG Width 4’b0

/* LP Control Register */ 
#define DPC_LP 				__REGW(MP25XXF_DPC_START + 0x2c)
Address : C000 282Ch
[15:14] - Reserved -
[13] R/W LPEN LP Enable. 0
[12] R/W LPMSKBNK Disable the LP during blank area 0
[11:8] R/W T5(LPSTR) LP start position 4’b0
[7:0] R/W T4(LPWID) LP Width 8’b0
/* CLKV Control1 Register */
#define DPC_CLKV1 			__REGW(MP25XXF_DPC_START + 0x2e)
[15:13] R/W CVDLY CLKV Delay, When CVBNKEN = 0, It is valid. A unit of it is a line. 3’b0
[12] R/W CVATVAREA CLKV Active area 0
[11:0] R/W T22(CVRISPOS) CLKV rising edge position.
When CVATVAREA = 0, it must be greater than 0 and less than or equal to T12(XMAX).
When CVATVAREA = 1, it must be less than or equal to T7(HSEND).
12’b0
/* CLKV Control2 Register */ 
#define DPC_CLKV2 			__REGW(MP25XXF_DPC_START + 0x30)
[15] R/W POLTGLEN Toggle start level of POL per every VSYNC 0
[14] - Reserved -
[13] R/W CVEN CLKV Enable. 0
[12] R/W CVBNKEN Enable CLKV during T9(VSEND) except T10(VSSTR). 0
[11:0] R/W T21(CVFALPOS)
CLKV falling edge position.
When CVATVAREA = 0, it must be less than T22(CVRISPOS).
When CVATVAREA = 1, it must be less than or equal to T12(XMAX).
12’b0

/* POL Control Register */
#define DPC_POL 			__REGW(MP25XXF_DPC_START + 0x32)

[15] R/W POLEN POL Enable 0
[14:13] R/W POLPERIOD POL alternating period. (1 ~ 4 HD) 2’b0
[12] R/W POLTGLAREA POL transition zone
0 : The transition is in a Active area.
1 : The transition is in a Blank area.
0
[11:0] R/W T20(POLTGLPOS) POL toggle position.
When POLTGLAREA = 0, it must be greater than 0 and less than or equal to T12(XMAX).
When POLTGLAREA = 1, it must be less than or equal to T7(HSEND).
12’b0
/* CIS Sync Control Register */
#define DPC_CISSYNC 			__REGW(MP25XXF_DPC_START + 0x34)
[15:10] - Reserved -
[9:0] R/W T30(CISDLY) CIS Sync Delay (1~1024 clocks) 10’b0
Reserved

Address : C000 2836h / C000 2838h

[15:0] W Reserved Must be 0 -
/* Luminance Blank Level Register */
#define DPC_Y_BLANK 			__REGW(MP25XXF_DPC_START + 0x3a)
[15:10] - Reserved -
[9:0] R/W L1(YBNKLVL) blank level for Luminance. 10’d252

/* Chrominance Blank Level Register */
#define DPC_C_BLANK 			__REGW(MP25XXF_DPC_START + 0x3c)
[15:10] - Reserved -
[9:0] R/W L2(CBNKLVL) blank level for Chrominance. 10’d212

/* Luminance Positive Sync Level Register */
#define DPC_YP_CSYNC 			__REGW(MP25XXF_DPC_START + 0x3e)
[15:10] - Reserved -
[9:0] R/W L3(YPOSSYNCLVL) Positive Sync level for the luminance 10’d748

/* Luminance Negative Sync Level Register */
#define DPC_YN_CSYNC 			__REGW(MP25XXF_DPC_START + 0x40)
[15:10] - Reserved -
[9:0] R/W L4(YNEGSYNCLVL) Negative Sync level for the luminance 10’d16

/* Chrominance Positive Sync Level Register */
#define  DPC_CP_CSYNC 			__REGW(MP25XXF_DPC_START + 0x42)
[15:10] - Reserved -
[9:0] R/W L5(CPOSSYNCLVL) Positive Sync level for the chrominance 10’d748

/* Chrominance Negative Sync Level Register */
#define DPC_CN_CSYNC 			__REGW(MP25XXF_DPC_START + 0x44)
Address : C000 2844h
[15:10] - Reserved -
[9:0] R/W L6(CNEGSYNCLVL) Negative Sync level for the chrominance 10’d276

/* Interrupt Controller Register */
#define DPC_INTR 			__REGW(MP25XXF_DPC_START + 0x46)
Address : C000 2846h
[15:6] W Reserved Must be 0 -
[5] R/W VSINTEN VSYNC Interrupt Enable 0
[4] R/W HSINTEN HSYNC Interrupt Enable 0
[3] R/W VSINTEDGE Interrupt position for VSYNC ( 0 : first edge, 1: second edge) 0
[2] R/W HSINTEDGE Interrupt position for HSYNC ( 0 : first edge, 1: second edge) 0
[1] R/W VSINTFLAG VSYNC Interrupt Status, Write ‘1’ to clear. 0
[0] R/W HSINTFLAG HSYNC Interrupt Status, Write ‘1’ to clear. 0

/* Clock Controller Register */
#define DPC_CLKCNTL 			__REGW(MP25XXF_DPC_START + 0x48)
[15:7] - Reserved -
[6:5] W Reserved Must be 0 2’b0
[4:3] R/W CLKSRC Display Source Clock
0 : ISP Clock Input : ICLKIN(GPIO_H[4])
1 : External Video Clock Input : VCLKIN(GPIO_H[5])
2 : PLL Clock : Display Clock from CLOCK and POWER MANAGER
3 : Reserved
2’b0
[2] R/W CLK2SEL CLK2 Selection ( 0: 1 CLK, 1:1/2 CLK ) 0
[1] R/W CLK1SEL CLK1 Selection ( 0: 1 CLK, 1:1/2 CLK ) 0
[0] R/W CLK1POL CLK1 polarity control. (0: Normal, 1: Inversion) 0

#endif
#endif /*MP25XXF_REGS_DPC_H_*/
