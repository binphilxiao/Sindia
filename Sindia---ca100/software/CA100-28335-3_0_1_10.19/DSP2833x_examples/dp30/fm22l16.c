#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void fm22l16_wr(Uint32 address,Uint16 data)
{
	*((Uint16 *)(0x00200000+address))=data;
}

Uint16 fm22l16_rd(Uint32 address)
{
	Uint16 i;
	i=*((Uint16 *)(0x00200000+address)); 
	return i; 
}

void fm22l16_wrs(Uint32 address,Uint16 *source,Uint16 number)
{
	Uint16 m=address,i;
	for (i=0;i<number;i++)
	{
		fm22l16_wr(m,*source);
		m++;
		source++;
	}
}

void fm22l16_rds(Uint32 address,Uint16 *destination,Uint16 number)
{
	Uint16 m=address,i;
	for (i=0;i<number;i++)
	{
		*destination=fm22l16_rd(m);
		m++;
		destination++;
	}
}		































