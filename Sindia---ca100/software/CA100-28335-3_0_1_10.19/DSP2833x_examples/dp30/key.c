#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

struct KEY key;

void key_clr(void)
{
	Uint16 i;
	for (i = 0; i < 8; i++)
		key.keynew[i] = 0;
	key.x=0;
	key.y=0;
}

void key_get(void)
{
	key.x=key.buffer[2]<<8|key.buffer[3];
	key.y=key.buffer[4]<<8|key.buffer[5];

	if(key.y<=3+97*4+94 && key.y>=3+97*4)
	{
		if        (key.x<=4+169*0+137 && key.x>=4+169*0) key.keynew[0]=1;
		else if(key.x<=4+169*1+137 && key.x>=4+169*1) key.keynew[1]=1;
		else if(key.x<=4+169*2+137 && key.x>=4+169*2) key.keynew[2]=1;
		else if(key.x<=4+169*3+137 && key.x>=4+169*3) key.keynew[3]=1;
		SPK(0);
	}
	else if(key.x<=4+169*3+137 && key.x>=4+169*3)
	{
		if        (key.y<=3+97*0+94 && key.y>=3+97*0) key.keynew[7]=1;
		else if(key.y<=3+97*1+94 && key.y>=3+97*1) key.keynew[6]=1;
		else if(key.y<=3+97*2+94 && key.y>=3+97*2) key.keynew[5]=1;
		else if(key.y<=3+97*3+94 && key.y>=3+97*3) key.keynew[4]=1;
		else if(key.y<=3+97*4+94 && key.y>=3+97*4) key.keynew[3]=1;
		SPK(0);
	}
}





