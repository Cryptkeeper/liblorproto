// AUTOMATICALLY GENERATED FILE, DO NOT MODIFY!
// YOUR CHANGES WILL BE OVERWRITTEN BY SCRIPT:
// tools/generate_model_files.py
#ifndef LORPROTO_MODEL_H
#define LORPROTO_MODEL_H

#include <stdbool.h>

typedef int LorModel;

#define LOR_MODEL_CTB08_V1_19                ((LorModel) 25) /* 0x19 */
#define LOR_MODEL_CTB08_V2_1A                ((LorModel) 26) /* 0x1a */
#define LOR_MODEL_CTB08_V3_1B                ((LorModel) 27) /* 0x1b */
#define LOR_MODEL_CTB08S_V1_1E               ((LorModel) 30) /* 0x1e */
#define LOR_MODEL_CTB08S_V4_1F               ((LorModel) 31) /* 0x1f */
#define LOR_MODEL_CMB24D_V1_23               ((LorModel) 35) /* 0x23 */
#define LOR_MODEL_INPPUP_V1_28               ((LorModel) 40) /* 0x28 */
#define LOR_MODEL_SRVPUP_V1_2E               ((LorModel) 46) /* 0x2e */
#define LOR_MODEL_LOR600_V1_32               ((LorModel) 50) /* 0x32 */
#define LOR_MODEL_LC16_V4_33                 ((LorModel) 51) /* 0x33 */
#define LOR_MODEL_LOR600S_V1__LOR600D_V1_37  ((LorModel) 55) /* 0x37 */
#define LOR_MODEL_ELD16_V4_3D                ((LorModel) 61) /* 0x3d */
#define LOR_MODEL_PIXIE2_V1_46               ((LorModel) 70) /* 0x46 */
#define LOR_MODEL_PIXIE2P_V1_47              ((LorModel) 71) /* 0x47 */
#define LOR_MODEL_PIXIE4_V1_4B               ((LorModel) 75) /* 0x4b */
#define LOR_MODEL_PIXIE4P_V1_4C              ((LorModel) 76) /* 0x4c */
#define LOR_MODEL_PIXIE8_V1_50               ((LorModel) 80) /* 0x50 */
#define LOR_MODEL_PIXIE8P_V1_51              ((LorModel) 81) /* 0x51 */
#define LOR_MODEL_PIXIE16_V1_55              ((LorModel) 85) /* 0x55 */
#define LOR_MODEL_PIXIE16P_V1_56             ((LorModel) 86) /* 0x56 */
#define LOR_MODEL_PIXIE4DMX_V1_5A            ((LorModel) 90) /* 0x5a */
#define LOR_MODEL_MC_P_V1_64                 ((LorModel)100) /* 0x64 */
#define LOR_MODEL_MC_P_V1_65                 ((LorModel)101) /* 0x65 */
#define LOR_MODEL_MC_PS_V1_6E                ((LorModel)110) /* 0x6e */
#define LOR_MODEL_MC_PD_V1_78                ((LorModel)120) /* 0x78 */
#define LOR_MODEL_MC_PD_V2_79                ((LorModel)121) /* 0x79 */
#define LOR_MODEL_CCB100_V1_7B               ((LorModel)123) /* 0x7b */
#define LOR_MODEL_CCF_V1_7D                  ((LorModel)125) /* 0x7d */
#define LOR_MODEL_F50RGB_V1_7E               ((LorModel)126) /* 0x7e */
#define LOR_MODEL_F50UV_V1_7F                ((LorModel)127) /* 0x7f */
#define LOR_MODEL_CTB08D_V1_82               ((LorModel)130) /* 0x82 */
#define LOR_MODEL_CTB08D_V2_83               ((LorModel)131) /* 0x83 */
#define LOR_MODEL_CTB08D_V3_84               ((LorModel)132) /* 0x84 */
#define LOR_MODEL_CTB08D_V4_85               ((LorModel)133) /* 0x85 */
#define LOR_MODEL_CTB08D_V5_86               ((LorModel)134) /* 0x86 */
#define LOR_MODEL_CTB08D_V6_87               ((LorModel)135) /* 0x87 */
#define LOR_MODEL_CTB16D_V1_8C               ((LorModel)140) /* 0x8c */
#define LOR_MODEL_CTB16D_V2_8D               ((LorModel)141) /* 0x8d */
#define LOR_MODEL_CTB16D_V3_8E               ((LorModel)142) /* 0x8e */
#define LOR_MODEL_CTB16D_V4_8F               ((LorModel)143) /* 0x8f */
#define LOR_MODEL_32LD_G3_V1_90              ((LorModel)144) /* 0x90 */
#define LOR_MODEL_CTB16PC_V4_96              ((LorModel)150) /* 0x96 */
#define LOR_MODEL_CTB16PC_V5_97              ((LorModel)151) /* 0x97 */
#define LOR_MODEL_CTB16PC_V6_98              ((LorModel)152) /* 0x98 */
#define LOR_MODEL_CTB16PC_V7_99              ((LorModel)153) /* 0x99 */
#define LOR_MODEL_16PC_G3_V1_9A              ((LorModel)154) /* 0x9a */
#define LOR_MODEL_CTB08G3_V1_9D              ((LorModel)157) /* 0x9d */
#define LOR_MODEL_CMB16_V1_A0                ((LorModel)160) /* 0xa0 */
#define LOR_MODEL_CMB16_V2_A1                ((LorModel)161) /* 0xa1 */
#define LOR_MODEL_CMB16_V3_A2                ((LorModel)162) /* 0xa2 */
#define LOR_MODEL_DMX1000_V1_A5              ((LorModel)165) /* 0xa5 */
#define LOR_MODEL_CTB04G3_V1_A7              ((LorModel)167) /* 0xa7 */
#define LOR_MODEL_MCP32_V1_AA                ((LorModel)170) /* 0xaa */
#define LOR_MODEL_DIO32_V1_AF                ((LorModel)175) /* 0xaf */
#define LOR_MODEL_CR150D_V1_B4               ((LorModel)180) /* 0xb4 */
#define LOR_MODEL_CR150D_V2_B5               ((LorModel)181) /* 0xb5 */
#define LOR_MODEL_SD08_V1_B9                 ((LorModel)185) /* 0xb9 */
#define LOR_MODEL_BOOTLOADER_16D_CA          ((LorModel)202) /* 0xca */
#define LOR_MODEL_BOOTLOADER_MP3_CB          ((LorModel)203) /* 0xcb */
#define LOR_MODEL_BOOTLOADER_RF_CC           ((LorModel)204) /* 0xcc */
#define LOR_MODEL_BOOTLOADER_16D_CD          ((LorModel)205) /* 0xcd */
#define LOR_MODEL_BOOTLOADER_CB16_CE         ((LorModel)206) /* 0xce */
#define LOR_MODEL_BOOTLOADER_MMP3_CF         ((LorModel)207) /* 0xcf */
#define LOR_MODEL_BOOTLOADER_DMX_D0          ((LorModel)208) /* 0xd0 */
#define LOR_MODEL_BOOTLOADER_CMB24_D1        ((LorModel)209) /* 0xd1 */
#define LOR_MODEL_BOOTLOADER_08S_D2          ((LorModel)210) /* 0xd2 */
#define LOR_MODEL_BOOTLOADER_DIO32_D3        ((LorModel)211) /* 0xd3 */
#define LOR_MODEL_BOOTLOADER_RB150_D4        ((LorModel)212) /* 0xd4 */
#define LOR_MODEL_BOOTLOADER_SD08_D5         ((LorModel)213) /* 0xd5 */
#define LOR_MODEL_BOOTLOADER_16DV7_D6        ((LorModel)214) /* 0xd6 */
#define LOR_MODEL_BOOTLOADER_PC_V2_D7        ((LorModel)215) /* 0xd7 */
#define LOR_MODEL_BOOTLOADER_MP3P_D8         ((LorModel)216) /* 0xd8 */
#define LOR_MODEL_BOOTLOADER_C32G3_D9        ((LorModel)217) /* 0xd9 */
#define LOR_MODEL_BOOTLOADER_PCG3_DA         ((LorModel)218) /* 0xda */
#define LOR_MODEL_BOOTLOADER_LC16_DC         ((LorModel)220) /* 0xdc */
#define LOR_MODEL_BOOTLOADER_ELD16_DD        ((LorModel)221) /* 0xdd */
#define LOR_MODEL_BOOTLOADER_SPUP_DE         ((LorModel)222) /* 0xde */
#define LOR_MODEL_BOOTLOADER_IPUP_DF         ((LorModel)223) /* 0xdf */
#define LOR_MODEL_BOOTLOADER_CCF_E0          ((LorModel)224) /* 0xe0 */
#define LOR_MODEL_BOOTLOADER_CCB_E1          ((LorModel)225) /* 0xe1 */
#define LOR_MODEL_BOOTLOADER_MPG3_E2         ((LorModel)226) /* 0xe2 */
#define LOR_MODEL_BOOTLOADER_RFV5_E3         ((LorModel)227) /* 0xe3 */
#define LOR_MODEL_BOOTLOADER_08G3_E4         ((LorModel)228) /* 0xe4 */
#define LOR_MODEL_BOOTLOADER_UMG3_E5         ((LorModel)229) /* 0xe5 */
#define LOR_MODEL_BOOTLOADER_08G3_E6         ((LorModel)230) /* 0xe6 */
#define LOR_MODEL_BOOTLOADER_PIX02_E8        ((LorModel)232) /* 0xe8 */
#define LOR_MODEL_BOOTLOADER_PIX04_E9        ((LorModel)233) /* 0xe9 */
#define LOR_MODEL_BOOTLOADER_PIX08_EA        ((LorModel)234) /* 0xea */
#define LOR_MODEL_BOOTLOADER_PIX16_EB        ((LorModel)235) /* 0xeb */

bool lorGetModelName(LorModel model, const char **name);

#define lorGetMaxModel() ((LorModel)235)

#endif //LORPROTO_MODEL_H
