#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define   RX8025_ADD 0x64           

void rx8025_clock_init(void)
{
	writebyte(0xE0,0x22);
	delay_iicstop(1);
	writebyte(0xf0,0x20);
	delay_iicstop(1);
	writebyte(0x70,0);
	delay_iicstop(1);
}

void rx8025_time_set(struct CLOCK *c)
{
	writebyte(0x00,c->second);		
	delay_iicstop(1);
	writebyte(0x10,c->minute);		
	delay_iicstop(1);
	writebyte(0x20,c->hour);		
	delay_iicstop(1);
}

void rx8025_date_set(struct CLOCK *c)
{
	writebyte(0x30,c->week);		
	delay_iicstop(1);
	writebyte(0x40,c->date);		
	delay_iicstop(1);
	writebyte(0x50,c->month);		
	delay_iicstop(1);
	writebyte(0x60,c->year);		
	delay_iicstop(1);
}

void rx8025_clock_read(struct CLOCK *c)
{
	Uint16 i;
	Uint16 i2c[7];
   
	for (i=0;i<7;i++) i2c[i]=Creadbyte(i*16);
	
	c->second=i2c[0];
	c->minute=i2c[1];//&0x7f;
	c->hour=i2c[2];//&0x3f;
	c->week=i2c[3];//&0x07;
	c->date=i2c[4];//&0x3f;
	c->month=i2c[5];//&0x1f;
	c->year=i2c[6];

	c->second%=60;
	c->minute%=60;//&0x7f;
	c->hour%=24;//&0x3f;
	c->week%=7;//&0x07;
	c->date%=32;//&0x3f;
	if (c->date==0) c->date=1;
	c->month%=13;//&0x1f;
	if (c->month==0) c->month=1;
	c->year%=100;//&0x3f;

	if (c->year>99) c->year=99;
	if (c->month>12) c->year=12;
	else if (c->month<1) c->year=1;
	if (c->date>31) c->year=31;
	else if (c->date<1) c->year=1;
	if (c->hour>23) c->year=23;
	if (c->minute>59) c->year=59;
	if (c->second>59) c->year=59;
}









