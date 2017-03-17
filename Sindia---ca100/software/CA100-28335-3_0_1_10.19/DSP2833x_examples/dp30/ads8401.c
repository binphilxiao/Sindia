#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void ad_select(Uint16 chl)
{
	if (chl&0x01) ANSA0(1);
	else ANSA0(0);
	if (chl&0x02) ANSA1(1);
	else ANSA1(0);
	if (chl&0x04) ANSA2(1);
	else ANSA2(0);
	if (chl&0x08) ANSA3(1);
	else ANSA3(0);
	if (chl&0x10) ANSA4(1);
	else ANSA4(0);
}

void fp32_ad_get(Uint16 chl, float * out)
{
	Uint16 i,m;
	Uint32 m1[10],k;

	ad_select(chl);
	for(m=0;m<100;m++);
	for(i=0;i<10;i++)
	{
		*((Uint16 *)0x004000)=0;
		for(m=0;m<1;m++);
		m1[i]=*((Uint16 *)0x004001)&0x0ffff;     
	}
	ad_select(7);
	for(i=0;i<10;i++)
	{
		for(m=0;m<9;m++)
		{
			if (m1[m]>m1[m+1])
			{
				k=m1[m];m1[m]=m1[m+1];m1[m+1]=k;
			}
		}      
	}
	*out=((float)(m1[3]+m1[4]+m1[5]+m1[6]+m1[7]+m1[2]))*0.1666667;
}
