#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define TMP_REG 0
#define CFG_REG 1
#define TLOW_REG 2
#define THIGH_REG 3

void tmp275_init(Uint16 ch)
{
	Uint16 ret,add;
	Uint16 tmp;

	if (ch==H2S) add=0x92;
	else if (ch==SO2) add=0x96;
	else if (ch==CO) add=0x9e;

	tmp=0x01;//SHUT DOWN MODE DISABLE
	tmp|=0x00;//TM MODE SET TO 0
	tmp|=0x00;//POL MODE SET TO 0
	tmp|=0x18;//F1 F0  SET TO 0   6 CONSECUTIVE FAULTS
	tmp|=0x60;//R1 R0  SET TO 0  12bits RESOLUTION 220ms  
	tmp|=0x00;//One shot

	ret=fm24cl04_write(add, CFG_REG, tmp);
}

void start_tmp275(add)
{
	Uint16 ret,cfg;
	ret=fm24cl04_read(add, CFG_REG, &cfg);
	cfg|=0x80;
	ret=fm24cl04_write(add, CFG_REG, cfg);
}


float get_sensortmp(Uint16 ch)
{
	Uint16 add;
	int ret[2];
	float tmp;
	Uint32 i,j;
	
	if (ch==H2S) add=0x92;
	else if (ch==SO2) add=0x96;
	else if (ch==CO) add=0x9e;

	start_tmp275(add);
	for(i=0;i<220;i++)
		for(j=0;j<1846;j++);

	fm24cl04_rds(add, TMP_REG, (Uint16*)ret, 2);
	ret[0]= ((ret[0]<<8) + ret[1])>>4;
	tmp=(float)ret[0]/16.0;
	
	return tmp;
}
