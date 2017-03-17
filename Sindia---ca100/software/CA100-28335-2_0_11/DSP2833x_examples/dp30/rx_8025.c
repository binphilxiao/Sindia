#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//#define SCL(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO33=1):(GpioDataRegs.GPBSET.bit.GPIO33=1)
//#define SDA(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO32=1):(GpioDataRegs.GPBSET.bit.GPIO32=1)


void SCL(Uint16 x)
{
    if (x==0)GpioDataRegs.GPBCLEAR.bit.GPIO33=1;
	else GpioDataRegs.GPBSET.bit.GPIO33=1;
}

void SDA(Uint16 x)
{
    if (x==0)GpioDataRegs.GPBCLEAR.bit.GPIO32=1;
	else GpioDataRegs.GPBSET.bit.GPIO32=1;
}

void delay(void)
{
	 Uint16 j;
     for(j=0;j<2;j++);
}

#define   RX8025_ADD 0x64           

void  DelayNS(Uint32  dly);

struct CLOCK clock;
Uint16 i2c[20];

void start(void)// 启动I2C总线子程序 
{
     
     SCL(0);
     delay();
   	 SDA(1);
     delay();
     SCL(1);
     delay();
     SDA(0);
     delay();
   	 SCL(0);	
     delay();
}
void stop()// 停止I2C总线 
{
     SCL(0);
	 delay();
     SDA(0);
     delay();	
     SCL(1);	
     delay();
     SDA(1);
     delay();
     SCL(0);	
     DelayNS(1);
}

void ack (void)
{
     SDA(0);
     delay();	
     SCL(1);	
     delay();
     SCL(0);	
     delay();
}

void nack(void)
{
    SCL(0);
	delay();

    SDA(1);
    delay();
    SCL(1);
    delay();
    SCL(0);
	delay();
}

Uint16  inbyte(void)
{
    Uint16 i,data=0,k;
    SCL(0);
	SDA(1);
	EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO32=0;
    EDIS;
    delay();
	for(i=0;i<8;i++)
	{
	    data=data<<1;
	    SCL(0);
	    delay();
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
       delay();
       SCL(1);
       delay();
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

void Creadbytes (Uint16 add,Uint16 number)
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
	    i2c[i]=inbyte();
	    ack();
	} 
	i2c[number-1]=inbyte();
	nack(); 
	stop();
}

void  DelayNS(Uint32  dly)
{  Uint32  i;

   for(; dly>0; dly--) 
      for(i=0; i<50000; i++);
}
	
void rx8025_clock_inti(void)
{
    i2c[0]=0;
    writebyte(0xE0,0x22);
    DelayNS(1);
	writebyte(0xf0,0x20);
	DelayNS(1);
    writebyte(0x70,0);
	DelayNS(1);
}

void rx8025_time_set(Uint16 second,Uint16 minute,Uint16 hour)
{
    writebyte(0x00,second);		
    DelayNS(1);
    writebyte(0x10,minute);		
    DelayNS(1);
    writebyte(0x20,hour);		
    DelayNS(1);
}

void rx8025_date_set(Uint16 week,Uint16 date,Uint16 month,Uint16 year)
{
    writebyte(0x30,week);		
    DelayNS(1);
    writebyte(0x40,date);		
    DelayNS(1);
    writebyte(0x50,month);		
    DelayNS(1);
    writebyte(0x60,year);		
    DelayNS(1);
}

void rx8025_clock_read(void)
{
    Uint16 i;
   
    for (i=0;i<7;i++) i2c[i]=Creadbyte(i*16);
    clock.second=i2c[0];
    clock.minute=i2c[1];//&0x7f;
    clock.hour=i2c[2];//&0x3f;
    clock.week=i2c[3];//&0x07;
    clock.date=i2c[4];//&0x3f;
    clock.month=i2c[5];//&0x1f;
    clock.year=i2c[6];
}









