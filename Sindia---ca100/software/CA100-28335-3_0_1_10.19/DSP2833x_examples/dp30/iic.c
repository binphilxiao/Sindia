#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define SCL1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO3=1):(GpioDataRegs.GPASET.bit.GPIO3=1)
#define SDA1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO4=1):(GpioDataRegs.GPASET.bit.GPIO4=1)

void nop(void)
{
	 Uint16 j;
     for(j=0;j<2;j++);
}

void  DelayNS1(Uint32  dly)
{  Uint32  i;

   for(; dly>0; dly--) 
      for(i=0; i<50000; i++);
}

void start1(void)// 启动I2C总线子程序 
{
	SDA1(1);
	SCL1(1);
	nop();
	
	SDA1(0);
	nop();
	
	SCL1(0);
	nop();
}
void stop1()// 停止I2C总线 
{
	SCL1(0);
	SDA1(0);
	nop();	
	
	SCL1(1);	
	nop();
	
	SDA1(1);
	nop();
	
	DelayNS1(1);
}

void put_ack1 (Uint16 ack)
{
	SCL1(0);
	nop();
	
	if (ack==0) SDA1(0);
	else SDA1(1);
	nop();	
	
	SCL1(1);	
	nop();
	
	SCL1(0);	
	nop();
}

Uint16 get_ack1(void)
{
	Uint16 i;
	
	SCL1(0);
	nop();
	
	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO4=0;
	EDIS;
	
	SCL1(1);	
	nop();
	i=GpioDataRegs.GPADAT.bit.GPIO4;
	SCL1(0);	
	nop();

	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO4=1;
	EDIS;

	return i;
}

Uint16 inbyte1(void)
{
	Uint16 i,data=0,k;
	
	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO4=0;
	EDIS;
	
	for(i=0;i<8;i++)
	{
		nop();
		SCL1(0);
		nop();
		SCL1(1);
		nop();
		data=data<<1;
		k=GpioDataRegs.GPADAT.bit.GPIO4;
		if (k==0) data=data & 0x0fe;
		else data=data|0x01;
		SCL1(0);
		nop();
	}

	EALLOW;
	GpioCtrlRegs.GPADIR.bit.GPIO4=1;
	EDIS;
	
	return(data);
}

void outbyte1(Uint16 outdata) 
{
	Uint16 i,k,tt;
	
	tt=outdata&0x00ff;
	
	for(i=0x80;i!=0;i=i/2)
	{
		nop();
	       k= tt&i;
	       if (k==0) SDA1(0);
	       else SDA1(1);
	       nop();
	       SCL1(1);
	       nop();
		SCL1(0);
		nop();
	}
}

#define SCL(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO33=1):(GpioDataRegs.GPBSET.bit.GPIO33=1)
#define SDA(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO32=1):(GpioDataRegs.GPBSET.bit.GPIO32=1)

void delay_iic(void)
{
	Uint16 j;
	for(j=0;j<2;j++);
}

void  delay_iicstop(Uint32  dly)
{  
	Uint32  i;

	for(; dly>0; dly--) 
	for(i=0; i<50000; i++);
}

void start(void)// 启动I2C总线子程序 
{
	SCL(0);
	delay_iic();
	SDA(1);
	delay_iic();
	SCL(1);
	delay_iic();
	SDA(0);
	delay_iic();
	SCL(0);	
	delay_iic();
}
void stop()// 停止I2C总线 
{
	SCL(0);
	delay_iic();
	SDA(0);
	delay_iic();	
	SCL(1);	
	delay_iic();
	SDA(1);
	delay_iic();
	SCL(0);	
	delay_iicstop(1);
}

void ack (void)
{
	SDA(0);
	delay_iic();	
	SCL(1);	
	delay_iic();
	SCL(0);	
	delay_iic();
}

void nack(void)
{
	SCL(0);
	delay_iic();
	SDA(1);
	delay_iic();
	SCL(1);
	delay_iic();
	SCL(0);
	delay_iic();
}

Uint16  inbyte(void)
{
	Uint16 i,data=0,k;
	SCL(0);
	SDA(1);
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32=0;
	EDIS;
	delay_iic();
	for(i=0;i<8;i++)
	{
		data=data<<1;
		SCL(0);
		delay_iic();
		k=GpioDataRegs.GPBDAT.bit.GPIO32;
		if (k==0) data=data & 0x0fe;
		else data=data|0x01;
		SCL(1);
	}
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32=1;
	EDIS;
	SCL(0);
	return(data);
}

void outbyte(Uint16 outdata) 
{
	Uint16 i,k;
	SCL(0);
	for(i=0x80;i!=0;i=i/2)
	{
	       k= outdata&i;
	       if (k==0) SDA(0);
	       else SDA(1);
	       delay_iic();
	       SCL(1);
	       delay_iic();
	       SCL(0);
	}
}

void writebyte(Uint16 add,Uint16 wbyte)
{
	start();
	outbyte(0x64);
	nack();
//	outbyte(add>>8);
//	nack();
	outbyte(add);
	nack();
	outbyte(wbyte);
	nack();
	stop();	
}

void Cwritebyte(Uint16 add,Uint16 wbyte)
{
	start();
	outbyte(0x64);
	nack();
	outbyte(add);
	nack();
	outbyte(wbyte);
	nack();
	stop();	
}

Uint16 Creadbyte (Uint16 add)
{
	Uint16 temp;
	start();
	outbyte(0x64);
	nack();
	outbyte(add);
	nack();
	start();
	outbyte(0x65);
	nack();
	temp=inbyte();
	nack();
	stop();
	return(temp);
}

Uint16 readbyte (Uint16 add)
{
	Uint16 temp;
	start();
	outbyte(0x64);
	nack();
//	outbyte(add>>8);
//	nack();
	outbyte(add);
	nack();
	start();
	outbyte(0x65);
	nack();
	temp=inbyte();
	nack();
	stop();
	return(temp);
}

void writebytes(Uint16 add,Uint16 *arrow,Uint16 number)
{
    Uint16 i;
	start();
	outbyte(0x64);
	nack();
	outbyte(add);
	nack();
	for(i=0;i<number;i++)
	{
		outbyte(*arrow);arrow++;
		nack();
	}    
	stop();	
}

void Creadbytes (Uint16 add,Uint16 number,Uint16 *i2c)
{
	Uint16 i;
	start();
	outbyte(0x64);
	nack();
	outbyte(add);
	nack();
	start();
	outbyte(0x65);
	nack();
	for(i=0;i<number-1;i++)
	{
		*(i2c+i)=inbyte();
		ack();
	} 
	*(i2c+number-1)=inbyte();
	nack(); 
	stop();
}