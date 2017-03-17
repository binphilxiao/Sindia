#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
//#define KEY_ALL (GpioDataRegs.GPBDAT.all&0x1f00)>>21

Uint16 keynew[6],key_x,key_y,databuffer[16],keyflag;

void key_clr(void)
{
	Uint16 i;
	for (i = 0; i < 6; i++)
		keynew[i] = 0;
	u_key_enter = 0;
}

void key_get(void)
{
    //Uint16 key_x,key_y;

    key_x=databuffer[2]<<8|databuffer[3];
    key_y=databuffer[4]<<8|databuffer[5];

	if(key_y<=240 && key_y>=210)
	{
	    if(key_x<80)keynew[0]=1;
		else if(key_x<160)keynew[1]=1;
		else if(key_x<240)keynew[2]=1;
		else if(key_x<320)keynew[3]=1;
		spk_start();
	}
	else if(key_y<=60  && key_x<=290 )
	{
		keynew[5]=1;
		spk_start();
	}
}





