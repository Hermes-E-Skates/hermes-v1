/*
 * bqMaximo_Ctrl_G2553.h
 *
 * bq769x0 CRC code example header file
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


#define BQMAXIMO 8

#define CRC_KEY 7

#define DISABLE_INT asm(" BIC #8,SR")

#define DELAY_LIMIT 0xffff

#define SYS_STAT 0
#define CELLBAL1 1
#define CELLBAL2 2
#define CELLBAL3 3
#define SYS_CTRL1 4
#define SYS_CTRL2 5
#define PROTECT1 0x06
#define PROTECT2 0x07
#define PROTECT3 0x08
#define OV_TRIP 0x09
#define UV_TRIP 0x0A
#define VC1_HI_BYTE 0x0C
#define VC1_LO_BYTE 0x0D
#define VC2_HI_BYTE 0x0E
#define VC2_LO_BYTE 0x0F
#define VC5_HI_BYTE 0x14
#define VC5_LO_BYTE 0x15
#define TS1_HI 0x2C
#define TS1_LO 0x2D
#define CC_HI 0x32
#define CC_LO 0x33

#define ADCGAIN1 0x50
#define ADCOFFSET 0x51
#define ADCGAIN2 0x59

#define SCD_DELAY_50us		0x0
#define SCD_DELAY_100us		0x1
#define SCD_DEALY_200us		0x2
#define SCD_DELAY_400us		0x3

#define SCD_THRESH_44mV_22mV	0
#define SCD_THRESH_67mV_33mV	1
#define SCD_THRESH_89mV_44mV	2
#define SCD_THRESH_111mV_56mV	3
#define SCD_THRESH_133mV_67mV	4
#define SCD_TRHESH_155mV_68mV	5
#define SCD_THRESH_178mV_89mV	6
#define SCD_THRESH_200mV_100mV	7

#define OCD_DEALY_10ms		0x0
#define OCD_DELAY_20ms		0x1
#define OCD_DELAY_40ms		0x2
#define OCD_DELAY_80ms		0x3
#define OCD_DELAY_160ms		0x4
#define OCD_DELAY_320ms		0x5
#define OCD_DELAY_640ms		0x6
#define OCD_DELAY_1280ms	0x7

#define OCD_THRESH_17mV_8mV		0
#define OCD_THRESH_22mV_11mV	1
#define OCD_THRESH_28mV_14mV	2
#define OCD_THRESH_33mV_17mV	3
#define OCD_THRESH_39mV_19mV	4
#define OCD_THRESH_44mV_22mV	5
#define OCD_THRESH_50mV_25mV	6
#define OCD_THRESH_56mV_28MV	7
#define OCD_THRESH_61mV_31mV	8
#define OCD_THRESH_67mV_33mV	9
#define OCD_THRESH_72mV_36mV	0xA
#define OCD_THRESH_78mV_39mV	0xB
#define OCD_THRESH_83mV_42mV	0xC
#define OCD_THRESH_89mV_44mV	0xD
#define OCD_THRESH_94mV_47mV	0xE
#define OCD_THRESH_100mV_50mV	0xF

#define UV_DELAY_1s				0
#define UV_DELAY_4s				1
#define UV_DELAY_8s				2
#define UV_DELAY_16s			3

#define OV_DELAY_1s				0
#define OV_DELAY_2s				1
#define OV_DELAY_4s				2
#define OV_DELAY_8s				3

#define OV_THRESH_BASE			0x2008
#define UV_THRESH_BASE			0x1000
#define OV_STEP					0x10
#define UV_STEP					0x10

#define ADCGAIN_BASE			365

#define LOW_BYTE(Data)			(unsigned char)(0xff & Data)
#define HIGH_BYTE(Data)			(unsigned char)(0xff & (Data >> 8))

union SysStatus_t
{
	struct
	{
		unsigned char OCD : 1;
		unsigned char SCD : 1;
		unsigned char OV : 1;
		unsigned char UV : 1;
		unsigned char OVRD_ALERT : 1;
		unsigned char DEVICE_XREADY : 1;
		unsigned char WAKE : 1;
		unsigned char CC_READY : 1;
	}StatusBit;
	unsigned char StatusByte;
};

union SysCtrl1_t
{
	struct
	{
		unsigned char SHUT_B : 1;
		unsigned char SHUT_A : 1;
		unsigned char RSVD1 : 1;
		unsigned char TEMP_SEL : 1;
		unsigned char ADC_EN : 1;
		unsigned char RSVD2 : 2;
		unsigned char LOAD_PRESENT : 1;
	}SysCtrl1Bit;
	unsigned char SysCtrl1Byte;
};

union SysCtrl2_t
{
	struct
	{
		unsigned char CHG_ON : 1;
		unsigned char DSG_ON : 1;
		unsigned char WAKE_T : 2;
		unsigned char WAKE_EN : 1;
		unsigned char CC_ONESHOT : 1;
		unsigned char CC_EN : 1;
		unsigned char DELAY_DIS : 1;
	}SysCtrl2Bit;
	unsigned char SysCtrl2Byte;
};

union CellBal1_t
{
	struct
	{
		unsigned char RSVD : 3;
		unsigned char CB5 : 1;
		unsigned char CB4 : 1;
		unsigned char CB3 : 1;
		unsigned char CB2 : 1;
		unsigned char CB1 : 1;
	}CellBal1Bit;
	unsigned char CellBal1Byte;
};
