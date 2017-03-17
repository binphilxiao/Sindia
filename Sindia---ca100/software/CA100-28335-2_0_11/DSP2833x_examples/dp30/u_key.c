#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define I6  GpioDataRegs.GPBDAT.bit.GPIO56;
#define I7  GpioDataRegs.GPBDAT.bit.GPIO57;
#define I8  GpioDataRegs.GPBDAT.bit.GPIO58;

Uint16 kp1,kp2,kp3,kk1,kk2,kk3,kf,km;
Uint16 u_key_cnt,u_key_enter;

Uint16 key_alarm_cnt,key_alarm_flag;

void spk_start(void)
{
	key_alarm_cnt=0;
	key_alarm_flag=1;
	SPK(1);
}

void u_key_ini(void)
{
	Uint16 k;

	kp1=I8;
	kp2=I7;
	kp3=I6;
	kk1=0;
	kk2=0;
//	kk3=0;
	kf=0;
}


void u_key_get(void)
{
	Uint16 k;

	k=I8;
	if(k!=kp1)kk1=1;
	else kk1=0;
	k=I7;
	if(k!=kp2)
    		kk2=1;
	else
    		kk2=0;

	if((kk1+kk2)==2)
		u_key_ini();
	else if((kk1+kk2)==1 && kf==0)
	{
		if(kk1)
		{ 
			if(u_key_cnt==0)u_key_cnt=3;
			else u_key_cnt--;
			spk_start();km=1;
		}
		else 
		{ 
			u_key_cnt++;
			if(u_key_cnt>3)u_key_cnt=0;
			spk_start();km=1;
		}
		kf=1;
	}

	k=I6;
	if(k==1)kk3=0;
	
	if(k==0 && u_key_enter==0 &&kk3==0)
	{
		u_key_enter=1;
		spk_start();
		keynew[u_key_cnt]=1;
		kk3=1;
	}

}

Uint16 const zbtab[4]={0,88,184,271};
void u_key_draw(void)
{
    Uint16 i;
    if(km==0)return;
	color_change(0x001f,0x001f);
	for(i=0;i<4;i++)display_jx(zbtab[i],221,zbtab[i]+48,239);
	color_change(0xf800,0x001f);
	display_jx(zbtab[u_key_cnt],221,zbtab[u_key_cnt]+48,239);
    km=0;
}


