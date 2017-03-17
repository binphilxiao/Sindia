#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//#define SCL1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO3=1):(GpioDataRegs.GPASET.bit.GPIO3=1)
//#define SDA1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO4=1):(GpioDataRegs.GPASET.bit.GPIO4=1)

/**********************************************************************************************
说明：见RECORD.C

**********************************************************************************************/



void delay2(void)
{
	 Uint16 j;
     for(j=0;j<2;j++);
}

//#define   RX8025_ADD 0x64           
           



void  DelayNS1(Uint32  dly);


void start1(void)// 启动I2C总线子程序 
{
     
     SCL1(0);
     delay2();
   	 SDA1(1);
     delay2();
     SCL1(1);
     delay2();
     SDA1(0);
     delay2();
   	 SCL1(0);	
     delay2();
}
void stop1()// 停止I2C总线 
{
     SCL1(0);
	 delay2();
     SDA1(0);
     delay2();	
     SCL1(1);	
     delay2();
     SDA1(1);
     delay2();
     SCL1(0);	
     DelayNS1(1);
}

void ack1 (void)
{
     SDA1(0);
     delay2();	
     SCL1(1);	
     delay2();
     SCL1(0);	
     delay2();
}

void nack1(void)
{
    SCL1(0);
	delay2();

    SDA1(1);
    delay2();
    SCL1(1);
    delay2();
    SCL1(0);
	delay2();
}

Uint16  inbyte1(void)
{
    Uint16 i,data=0,k;
    SCL1(0);
	SDA1(1);
	EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO4=0;
    EDIS;
    delay2();
	for(i=0;i<8;i++)
	{
	    data=data<<1;
	    SCL1(0);
	    delay2();
	    k=GpioDataRegs.GPADAT.bit.GPIO4;
	    if (k==0) data=data & 0x0fe;
	    else data=data|0x01;
	    SCL1(1);
	}
	EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO4=1;
    EDIS;
	SCL1(0);
    return(data);
}

void outbyte1(Uint16 outdata) 
{
    Uint16 i,k,tt;
	tt=outdata&0x00ff;
    SCL1(0);
    for(i=0x80;i!=0;i=i/2)
    {
       k= tt&i;
       if (k==0) SDA1(0);
       else SDA1(1);
       delay2();
       SCL1(1);
       delay2();
       SCL1(0);
    }
}
void fm24cl04_write(Uint16 device,Uint16 add,Uint16 wbyte)
{
	start1();
	outbyte1(device);
	nack1();
//	outbyte1(add>>8);
//	nack1();
	outbyte1(add);
	nack1();
	outbyte1(wbyte);
	nack1();
	stop1();	
}


Uint16 fm24cl04_read(Uint16 device,Uint16 add)
{
	Uint16 temp;
	start1();
	outbyte1(device);
	nack1();
//	outbyte1(add>>8);
//	nack1();
	outbyte1(add);
	nack1();
	start1();
	outbyte1(device+1);
	nack1();
	temp=inbyte1();
	nack1();
	stop1();
	return(temp);
}

void fm24cl04_test(void)
{
/*
    Uint16 temp,tt[20];

	for(temp=0;temp<20;temp++)
	{
	    fm24cl04_write(SO2_EE_ADD,temp,temp);
		fm24cl04_write(H2S_EE_ADD,temp,20+temp);
		fm24cl04_write(HF_EE_ADD,temp,40+temp);
	}

	for(temp=0;temp<20;temp++)
	{
	    tt[temp]=fm24cl04_read(SO2_EE_ADD,temp);
	}

	for(temp=0;temp<20;temp++)
	{
	    tt[temp]=fm24cl04_read(H2S_EE_ADD,temp);
	}
	for(temp=0;temp<20;temp++)
	{
	    tt[temp]=fm24cl04_read(HF_EE_ADD,temp);
	}
*/
}

void  DelayNS1(Uint32  dly)
{  Uint32  i;

   for(; dly>0; dly--) 
      for(i=0; i<50000; i++);
}
	

/*
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
*/








