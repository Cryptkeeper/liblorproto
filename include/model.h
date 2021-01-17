// AUTOMATICALLY GENERATED, DO NOT MODIFY THIS FILE!
// LOR_DeviceFile.txt sha1sum: b0b1a5d5aa63b8df9523eb1b3e62512309ff8499
// Built Sun Jan 17 00:30:17 CST 2021
// git: Nick Krecklow <hello@github.nklow.com>
// generate_model_h.sh

/*
 * MIT License
 * 
 * Copyright (c) 2020 Nick Krecklow
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 */
#ifndef LIGHTORAMA_MODEL_H
#define LIGHTORAMA_MODEL_H

#define LOR_MODEL_CTB08_VER_1_19 25 /* 0x19 */
#define LOR_MODEL_CTB08_VER_2_1A 26 /* 0x1a */
#define LOR_MODEL_CTB08_VER_3_1B 27 /* 0x1b */
#define LOR_MODEL_CTB08S_VER_1_1E 30 /* 0x1e */
#define LOR_MODEL_CTB08S_VER_4_1F 31 /* 0x1f */
#define LOR_MODEL_CMB24D_VER_1_23 35 /* 0x23 */
#define LOR_MODEL_INPPUP_VER_1_28 40 /* 0x28 */
#define LOR_MODEL_SRVPUP_VER_1_2E 46 /* 0x2e */
#define LOR_MODEL_LOR600_VER_1_32 50 /* 0x32 */
#define LOR_MODEL_LC16_VER_4_33 51 /* 0x33 */
#define LOR_MODEL_LOR600S_VER_1_37 55 /* 0x37 */
#define LOR_MODEL_LOR600D_VER_1_37 55 /* 0x37 */
#define LOR_MODEL_ELD16_VER_4_3D 61 /* 0x3d */
#define LOR_MODEL_PIXIE2_VER_1_46 70 /* 0x46 */
#define LOR_MODEL_PIXIE2P_VER_1_47 71 /* 0x47 */
#define LOR_MODEL_PIXIE4_VER_1_4B 75 /* 0x4b */
#define LOR_MODEL_PIXIE4P_VER_1_4C 76 /* 0x4c */
#define LOR_MODEL_PIXIE8_VER_1_50 80 /* 0x50 */
#define LOR_MODEL_PIXIE8P_VER_1_51 81 /* 0x51 */
#define LOR_MODEL_PIXIE16_VER_1_55 85 /* 0x55 */
#define LOR_MODEL_PIXIE16P_VER_1_56 86 /* 0x56 */
#define LOR_MODEL_PIXIE4DMX_VER_1_5A 90 /* 0x5a */
#define LOR_MODEL_MC_P_VER_1_64 100 /* 0x64 */
#define LOR_MODEL_MC_P_VER_1_65 101 /* 0x65 */
#define LOR_MODEL_MC_PS_VER_1_6E 110 /* 0x6e */
#define LOR_MODEL_MC_PD_VER_1_78 120 /* 0x78 */
#define LOR_MODEL_MC_PD_VER_2_79 121 /* 0x79 */
#define LOR_MODEL_CCB100_VER_1_7B 123 /* 0x7b */
#define LOR_MODEL_CCF_VER_1_7D 125 /* 0x7d */
#define LOR_MODEL_F50RGB_VER_1_7E 126 /* 0x7e */
#define LOR_MODEL_F50UV_VER_1_7F 127 /* 0x7f */
#define LOR_MODEL_CTB08D_VER_1_82 130 /* 0x82 */
#define LOR_MODEL_CTB08D_VER_2_83 131 /* 0x83 */
#define LOR_MODEL_CTB08D_VER_3_84 132 /* 0x84 */
#define LOR_MODEL_CTB08D_VER_4_85 133 /* 0x85 */
#define LOR_MODEL_CTB08D_VER_5_86 134 /* 0x86 */
#define LOR_MODEL_CTB08D_VER_6_87 135 /* 0x87 */
#define LOR_MODEL_CTB16D_VER_1_8C 140 /* 0x8c */
#define LOR_MODEL_CTB16D_VER_2_8D 141 /* 0x8d */
#define LOR_MODEL_CTB16D_VER_3_8E 142 /* 0x8e */
#define LOR_MODEL_CTB16D_VER_4_8F 143 /* 0x8f */
#define LOR_MODEL_32LD_G3_VER_1_90 144 /* 0x90 */
#define LOR_MODEL_CTB16PC_VER_4_96 150 /* 0x96 */
#define LOR_MODEL_CTB16PC_VER_5_97 151 /* 0x97 */
#define LOR_MODEL_CTB16PC_VER_6_98 152 /* 0x98 */
#define LOR_MODEL_CTB16PC_VER_7_99 153 /* 0x99 */
#define LOR_MODEL_16PC_G3_VER_1_9A 154 /* 0x9a */
#define LOR_MODEL_CTB08G3_VER_1_9D 157 /* 0x9d */
#define LOR_MODEL_CMB16_VER_1_A0 160 /* 0xa0 */
#define LOR_MODEL_CMB16_VER_2_A1 161 /* 0xa1 */
#define LOR_MODEL_CMB16_VER_3_A2 162 /* 0xa2 */
#define LOR_MODEL_DMX1000_VER_1_A5 165 /* 0xa5 */
#define LOR_MODEL_CTB04G3_VER_1_A7 167 /* 0xa7 */
#define LOR_MODEL_MCP32_VER_1_AA 170 /* 0xaa */
#define LOR_MODEL_DIO32_VER_1_AF 175 /* 0xaf */
#define LOR_MODEL_CR150D_VER_1_B4 180 /* 0xb4 */
#define LOR_MODEL_CR150D_VER_2_B5 181 /* 0xb5 */
#define LOR_MODEL_SD08_VER_1_B9 185 /* 0xb9 */
#define LOR_MODEL_BOOTLOADER_16D_CA 202 /* 0xca */
#define LOR_MODEL_BOOTLOADER_MP3_CB 203 /* 0xcb */
#define LOR_MODEL_BOOTLOADER_RF_CC 204 /* 0xcc */
#define LOR_MODEL_BOOTLOADER_16D_CD 205 /* 0xcd */
#define LOR_MODEL_BOOTLOADER_CB16_CE 206 /* 0xce */
#define LOR_MODEL_BOOTLOADER_MMP3_CF 207 /* 0xcf */
#define LOR_MODEL_BOOTLOADER_DMX_D0 208 /* 0xd0 */
#define LOR_MODEL_BOOTLOADER_CMB24_D1 209 /* 0xd1 */
#define LOR_MODEL_BOOTLOADER_08S_D2 210 /* 0xd2 */
#define LOR_MODEL_BOOTLOADER_DIO32_D3 211 /* 0xd3 */
#define LOR_MODEL_BOOTLOADER_RB150_D4 212 /* 0xd4 */
#define LOR_MODEL_BOOTLOADER_SD08_D5 213 /* 0xd5 */
#define LOR_MODEL_BOOTLOADER_16DV7_D6 214 /* 0xd6 */
#define LOR_MODEL_BOOTLOADER_PC_V2_D7 215 /* 0xd7 */
#define LOR_MODEL_BOOTLOADER_MP3P_D8 216 /* 0xd8 */
#define LOR_MODEL_BOOTLOADER_C32G3_D9 217 /* 0xd9 */
#define LOR_MODEL_BOOTLOADER_PCG3_DA 218 /* 0xda */
#define LOR_MODEL_BOOTLOADER_LC16_DC 220 /* 0xdc */
#define LOR_MODEL_BOOTLOADER_ELD16_DD 221 /* 0xdd */
#define LOR_MODEL_BOOTLOADER_SPUP_DE 222 /* 0xde */
#define LOR_MODEL_BOOTLOADER_IPUP_DF 223 /* 0xdf */
#define LOR_MODEL_BOOTLOADER_CCF_E0 224 /* 0xe0 */
#define LOR_MODEL_BOOTLOADER_CCB_E1 225 /* 0xe1 */
#define LOR_MODEL_BOOTLOADER_MPG3_E2 226 /* 0xe2 */
#define LOR_MODEL_BOOTLOADER_RFV5_E3 227 /* 0xe3 */
#define LOR_MODEL_BOOTLOADER_08G3_E4 228 /* 0xe4 */
#define LOR_MODEL_BOOTLOADER_UMG3_E5 229 /* 0xe5 */
#define LOR_MODEL_BOOTLOADER_08G3_E6 230 /* 0xe6 */
#define LOR_MODEL_BOOTLOADER_PIX02_E8 232 /* 0xe8 */
#define LOR_MODEL_BOOTLOADER_PIX04_E9 233 /* 0xe9 */
#define LOR_MODEL_BOOTLOADER_PIX08_EA 234 /* 0xea */
#define LOR_MODEL_BOOTLOADER_PIX16_EB 235 /* 0xeb */

#endif //LIGHTORAMA_MODEL_H
