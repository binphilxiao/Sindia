#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File



//#define MAXCOOL 0xa37//0x740//(2.2)0x794//(2.3)0x7e8//(2.5A)0x83d//0x8d6//0xa37(3.0A)
//#define MINCOOL 0x105//0.3A



void dac7632(Uint32 chanl,Uint16 dat)
{
/*    Uint32 i,x;
    Uint32 j;
    
    if (chanl>2) return;
    
    LDREG(1);CSD(0);LDAC(0);

    j=((chanl<<23)&0x800000)|dat;
    for (i=0x800000;i!=0;i=i/2)
    {
        DACL(0);
        if (j&i) DAOUT(1);
        else DAOUT(0);
        for (x=0;x<2;x++);
        DACL(1);
       for (x=0;x<2;x++);
    }
    LDREG(0);
    for (x=0;x<5;x++);
    LDREG(1);
    DACL(1);
    DAOUT(1);
	CSD(1);LDAC(1);
	*/
}
void  MSpiIni(void)
{ 
/*
    LDREG(1);
    DACL(1); 
    DAOUT(1);
    dac7632(0,0);
    dac7632(1,0);
*/
}
