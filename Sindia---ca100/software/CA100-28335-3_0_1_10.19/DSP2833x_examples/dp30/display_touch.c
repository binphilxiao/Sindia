#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//单精度浮点转换成字符串
//范围-9999.99--9999.99
char float_char[9];

const Uint16 lib_mode[6][3] =
{ 0x20, 0x82, 0x00, 0x21, 0x82, 0x01, 0x22, 0x81, 0x02, 0x23, 0x81, 0x03, 0x24,
		0x81, 0x07, 0x2A, 0x81, 0x15, };

void scia_send_byte(Uint16 a)
{
	Uint16 x;
	do
	{
		x = SciaRegs.SCICTL2.all & 0x80; //SciaRegs.SCIFFTX.all&0x1f00;
		//x=x>>8;
	} while (x == 0);
	SciaRegs.SCITXBUF = a;
}

Uint16 scia_status(void)
{
	Uint16 x;

	x = SciaRegs.SCIFFTX.all & 0x1f00;
	x = x >> 8;
	return x;
}

Uint16 scia_receive_byte(void)
{
	Uint16 x;
	Uint32 i = 0;
	do
	{
		x = SciaRegs.SCIRXST.all & 0x40; //SciaRegs.SCIFFRX.all&0x1f00;
		//x=x>>8;
		i++;
	} while (x == 0 && i < 50000);
	if (i >= 50000)
		return 0;
	x = SciaRegs.SCIRXBUF.all;
	return x;
}

void dis_delay(void)
{
	Uint32 i;
	for (i = 0; i < 100000; i++);
}

// Transmit a character from the SCI'
void scib_xmit(Uint16 a)
{
	Uint16 x;
	ScibRegs.SCITXBUF = a;
	do
	{
		//x = ScibRegs.SCIFFTX.all & 0x1f00;
		//x = x >> 8;
		x=ScibRegs.SCICTL2.bit.TXEMPTY;
	} while (x ==0);
	
}
void print_string(char *dat)
{
	Uint16 x1, x2, i;
	Uint16 x;

	x1 = strlen(dat);

	for (i = 0; i < x1; i++)
	{
		x2 = *(dat++);
		ScibRegs.SCITXBUF = x2;
		do
		{
			//x = ScibRegs.SCIFFTX.all & 0x1f00;
			//x = x >> 8;
			x=ScibRegs.SCICTL2.bit.TXEMPTY;
		} while (x == 0);	
	}
}

void scic_xmit(Uint16 a)
{
	Uint32 x;
	ScicRegs.SCITXBUF = a;
	for (x = 0; x < 10000; x++)
		;
}
void print_string_c(char *dat)
{
	Uint16 x1, x2, i;
	Uint32 x;

	x1 = strlen(dat);

	for (i = 0; i < x1; i++)
	{
		x2 = *(dat++);
		scic_xmit(x2);
		for (x = 0; x < 10000; x++)
			;
	}
//	scic_xmit(0x0D);
	//  UART0_SendByte(0x0d);
}

// Initalize the SCI FIFO
void scib_fifo_init()
{
//   ScibRegs.SCIFFTX.all=0xE040;
//    ScibRegs.SCIFFRX.all=0x204f;
//    ScibRegs.SCIFFCT.all=0x0;

}


void calib_touch(void)
{
	scib_xmit(0xaa);
	scib_xmit(0xe4);
	scib_xmit(0x55);
	scib_xmit(0xaa);
	scib_xmit(0x5a);
	scib_xmit(0xa5);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}


void color_change(Uint16 front, Uint16 back)
{
	Uint16 i;
	scib_xmit(0xaa);
	scib_xmit(0x40);
	i = front >> 8 & 0x0ff;
	scib_xmit(i);
	i = front & 0x0ff;
	scib_xmit(i);
	i = back >> 8 & 0x0ff;
	scib_xmit(i);
	i = back & 0x0ff;
	scib_xmit(i);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}

void cls(void)
{
	scib_xmit(0xaa);
	scib_xmit(0x52);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
	dis_delay();
}

void display_num_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, Uint16 num, char *strings)
{
	Uint16 i, j;
	Uint32 t;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b
	t = HEX4BCD(num);
	j = t >> 16 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
		j = t >> 12 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 8 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 4 & 0x0f;
		scib_xmit(j + 0x30);
		j = t & 0x0f;
		scib_xmit(j + 0x30);
	}
	else
	{
		j = t >> 12 & 0x0f;
		if (j)
		{
			scib_xmit(j + 0x30);
			j = t >> 8 & 0x0f;
			scib_xmit(j + 0x30);
			j = t >> 4 & 0x0f;
			scib_xmit(j + 0x30);
			j = t & 0x0f;
			scib_xmit(j + 0x30);
		}
		else
		{
			j = t >> 8 & 0x0f;
			if (j)
			{
				scib_xmit(j + 0x30);
				j = t >> 4 & 0x0f;
				scib_xmit(j + 0x30);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
			else
			{
				j = t >> 4 & 0x0f;
				if (j)
					scib_xmit(j + 0x30);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
		}
	}

	print_string(strings);
//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}
void display_num_ascii1(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, Uint16 num, char *strings)
{
	Uint16 i, j;
	Uint32 t;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b
	t = HEX4BCD(num);
	j = t >> 16 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
		j = t >> 12 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 8 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 4 & 0x0f;
		scib_xmit(j + 0x30);
		j = t & 0x0f;
		scib_xmit(j + 0x30);
	}
	else
	{
		scib_xmit(0x20);
		j = t >> 12 & 0x0f;
		if (j)
		{
			scib_xmit(j + 0x30);
			j = t >> 8 & 0x0f;
			scib_xmit(j + 0x30);
			j = t >> 4 & 0x0f;
			scib_xmit(j + 0x30);
			j = t & 0x0f;
			scib_xmit(j + 0x30);
		}
		else
		{
			scib_xmit(0x20);
			j = t >> 8 & 0x0f;
			if (j)
			{
				scib_xmit(j + 0x30);
				j = t >> 4 & 0x0f;
				scib_xmit(j + 0x30);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
			else
			{
				scib_xmit(0x20);
				j = t >> 4 & 0x0f;
				if (j)
					scib_xmit(j + 0x30);
				else
					scib_xmit(0x20);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
		}
	}

	print_string(strings);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, char *strings)
{
	Uint16 i;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b
	print_string(strings);

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

	dis_delay();

}

void display_fp_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1, Uint16 bit, char *strings) //x<999.999
{
	char tmp[20];
	char * pt;

	sflt_to_str(x1,tmp);
	format_fpstr(tmp,bit);
	
	pt=tmp;
	pt+=bit;

	while(*strings!='\0')
	{
		*pt=*strings;
		pt++;
		strings++;
	}
	*pt='\0';
	display_ascii(dots, front, back, switch1, x, y, tmp);
}

void display_number(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, Uint16 x1) //x<999.999
{
	Uint16 i, j;
	Uint32 t;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b

	t = HEX4BCD(x1);
	j = t >> 16 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
		j = t >> 12 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 8 & 0x0f;
		scib_xmit(j + 0x30);
		j = t >> 4 & 0x0f;
		scib_xmit(j + 0x30);
		j = t & 0x0f;
		scib_xmit(j + 0x30);
	}
	else
	{
		j = t >> 12 & 0x0f;
		if (j)
		{
			scib_xmit(j + 0x30);
			j = t >> 8 & 0x0f;
			scib_xmit(j + 0x30);
			j = t >> 4 & 0x0f;
			scib_xmit(j + 0x30);
			j = t & 0x0f;
			scib_xmit(j + 0x30);
		}
		else
		{
			j = t >> 8 & 0x0f;
			if (j)
			{
				scib_xmit(j + 0x30);
				j = t >> 4 & 0x0f;
				scib_xmit(j + 0x30);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
			else
			{
				j = t >> 4 & 0x0f;
				if (j)
					scib_xmit(j + 0x30);
				j = t & 0x0f;
				scib_xmit(j + 0x30);
			}
		}
	}

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}
char *weektable[] =
{ "日", "一", "二", "三", "四", "五", "六"};
void display_clock(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y)
{
	Uint16 i;
	rx8025_clock_read(&clock);

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b

	scib_xmit(0x30 + 2);
	scib_xmit(0x30 + 0);
	i = (clock.year/10) & 0x0f;
	scib_xmit(0x30 + i);
	i =  (clock.year%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(".");
	i = (clock.month/10) & 0x0f;
	scib_xmit(0x30 + i);
	i = (clock.month%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(".");
	i = (clock.date/10)  & 0x0f;
	scib_xmit(0x30 + i);
	i = (clock.date%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(" ");

	i = (clock.hour/10)  & 0x0f;
	scib_xmit(0x30 + i);
	i = (clock.hour%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = (clock.minute/10) & 0x0f;
	scib_xmit(0x30 + i);
	i = (clock.minute%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = (clock.second/10) & 0x0f;
	scib_xmit(0x30 + i);
	i = (clock.second%10) & 0x0f;
	scib_xmit(0x30 + i);
	print_string(" 星期");
	i = clock.week & 0x0f;
	print_string(weektable[i]);

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}
void display_measure_time(Uint16 dots, Uint16 front, Uint16 back,
		Uint16 switch1, Uint16 x, Uint16 y)
{
	Uint16 i;
	Uint32 hour, minute, second;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b

	second = HEX2BCD(measure_time.second);
	minute = HEX2BCD(measure_time.minute);
	hour = HEX2BCD(measure_time.hour);

	i = hour >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = hour & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = minute >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = minute & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = second >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = second & 0x0f;
	scib_xmit(0x30 + i);

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void disp_background(Uint16 id)
{
	scib_xmit(0xaa);
	scib_xmit(0x70);
	scib_xmit(id);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}

void display_dot(Uint16 x, Uint16 y)
{
	Uint16 i;

	scib_xmit(0xaa);
	scib_xmit(0x51);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_picture(Uint16 number, Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2,
		Uint16 x, Uint16 y)
{
	Uint16 i;

	scib_xmit(0xaa);
	scib_xmit(0x71);
	scib_xmit(number); //x
	i = x1 >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x1 & 0x0ff;
	scib_xmit(i); //x
	i = y1 >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y1 & 0x0ff;
	scib_xmit(i); //y

	i = x2 >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x2 & 0x0ff;
	scib_xmit(i); //x
	i = y2 >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y2 & 0x0ff;
	scib_xmit(i); //y

	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

	dis_delay();

}

void picture_store(Uint16 number)
{
	scib_xmit(0xaa);
	scib_xmit(0xe2);
	scib_xmit(number); //x
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}

void dw_int(void)
{
        scib_xmit(0xaa);
        scib_xmit(0xe3);
        scib_xmit(0x55);
        scib_xmit(0xaa);
        scib_xmit(0x5a);
        scib_xmit(0xa5);
        scib_xmit(0x00);
        scib_xmit(0x07);
        scib_xmit(0x04);
        scib_xmit(0xdf);
        scib_xmit(0xcc);
        scib_xmit(0x33);
        scib_xmit(0xc3);
        scib_xmit(0x3c);
}

//显示小数点后面二位，不显示＋,最大999。9
void display_gpa(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j;
	float s;

	if (x1 > 999.99)
		s = 999.99;
	else if (x1 >= 0.0)
		s = x1;
	else
		s = 0.0;

	scib_xmit(0xaa);
	scib_xmit(0x98);
	i = x >> 8 & 0x0ff;
	scib_xmit(i); //x
	i = x & 0x0ff;
	scib_xmit(i); //x
	i = y >> 8 & 0x0ff;
	scib_xmit(i); //y
	i = y & 0x0ff;
	scib_xmit(i); //y
	i = lib_mode[dots][0];
	scib_xmit(i); //lib
	i = lib_mode[dots][1];
	if (switch1)
		i = i + 0x40;
	scib_xmit(i); //lib
	i = lib_mode[dots][2];
	scib_xmit(i); //dot
	i = front >> 8 & 0x0ff;
	scib_xmit(i); //color f
	i = front & 0x0ff;
	scib_xmit(i); //color f
	i = back >> 8 & 0x0ff;
	scib_xmit(i); //color b
	i = back & 0x0ff;
	scib_xmit(i); //color b

	i = (Uint16) (s * 100.0);
	i = HEX4BCD(i);
	j = i >> 16 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
	}
	else
		scib_xmit(0x20);
	;

	j = i >> 12 & 0x0f;
	scib_xmit(j + 0x30);

	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);
	scib_xmit(0x2e);
	j = i >> 4 & 0x0f;
	scib_xmit(j + 0x30);
	j = i & 0x0f;
	scib_xmit(j + 0x30);
	print_string(strings);

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void keyboard_flt( Uint16 x, Uint16 y, float * out)
{
	Uint16  keyboard_new;//键值
	Uint16 key_xx,key_yy;//上次按键位置
	Uint16 flag_key;//是否有按键按下标志
	float sign;//符号位
	float dot;//小树点位
	char tmp[10];


a1:	display_picture(63, 296, 84, 585, 405, x, y);
	
	flag_key = 0 ;
	sign=1.0;
	dot=0;
	keyboard_new=17;//17 is a not useful value
	key_xx=0;key_yy=0;
	key.x=0;key.y=0;
	*out=0;

	while( keyboard_new != 16 )
	{
		if( key_xx != key.x || key_yy != key.y )
		{
			flag_key = 1 ;
			if (key.y > y+66 && key.y < y+122)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 1;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 2;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 3;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 11;//f1
			}
			else if (key.y > y+126 && key.y < y+183)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 4;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 5;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 6;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 12;//f2
			}
			else if (key.y > y+188 && key.y < y+245)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 7;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 8;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 9;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 13;//clear
			}
			else if (key.y > y+250 && key.y < y+307)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 0;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 14;//dot
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 15;//sign
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 16;//return
			}
			key_yy = key.y ;
			key_xx = key.x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if( keyboard_new == 11)//backspace
			{
			}
			if( keyboard_new == 13)//clear
			{
				*out=0;
				dot=0;
				sign=1;
				goto a1;
			}
			else if(keyboard_new == 15)//sign
				sign*=-1;
			else if(keyboard_new==14 && dot==0)
				dot=0.1;
			else if (keyboard_new>=0 && keyboard_new<=9)
			{
				if (dot==0)
					*out=*out*10+keyboard_new;
				else 
				{
					*out+=keyboard_new*dot;
					dot/=10.0;
				}
			}
			//output_number;
		}
		sflt_to_str(*out*sign, tmp);
		format_fpstr(tmp ,6);
		display_ascii(M32, WHITE, 0xbefb, 1, x+20, y+17, tmp);
	}
	key_clr() ;
	*out*=sign;
}

void keyboard_int( Uint16 x, Uint16 y ,Uint32 *pass)
{
	Uint16  keyboard_new;//键值
	Uint16 key_xx,key_yy;//上次按键位置
	Uint16 flag_key;//是否有按键按下标志
	char tmp[10];

a1:	display_picture(63, 296, 84, 585, 405, x, y);
	
	flag_key = 0 ;
	keyboard_new=17;//17 is a not useful value
	key_xx=0;key_yy=0;
	key.x=0;key.y=0;
	*pass=0;

	while( keyboard_new != 16 )
	{
		if( key_xx != key.x || key_yy != key.y )
		{
			flag_key = 1 ;
			if (key.y > y+66 && key.y < y+122)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 1;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 2;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 3;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 11;//f1
			}
			else if (key.y > y+126 && key.y < y+183)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 4;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 5;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 6;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 12;//f2
			}
			else if (key.y > y+188 && key.y < y+245)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 7;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 8;
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 9;
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 13;//clear
			}
			else if (key.y > y+250 && key.y < y+307)
			{
				if(key.x > x+9 && key.x < x+70)
					keyboard_new = 0;
				else if(key.x > x+79 && key.x < x+141)
					keyboard_new = 14;//dot
				else if(key.x > x+148 && key.x < x+210)
					keyboard_new = 15;//sign
				else if(key.x > x+218 && key.x < x+280)
					keyboard_new = 16;//return
			}
			key_yy = key.y ;
			key_xx = key.x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if( keyboard_new == 13)//clear
			{
				*pass=0;
				goto a1;
			}
			else if (keyboard_new>=0 && keyboard_new<=9)
			{
				*pass=*pass*10+keyboard_new;
			}
		}
		uint32_to_str(*pass, tmp);
		format_fpstr(tmp ,6);
		display_ascii(M32, WHITE, 0xbefb, 1, x+20, y+17, tmp);
	}
	key_clr() ;
}
/*********************************************************
 SCI波特率计算方法：

 SCIBAUD= 系统频率/（32×（BRR+1）)-------BRR为寄存器内的值


 BRR=系统频率/（SCIBAUD×32）-1

 例：BRR=150*1000000/(115200*32)-1
 =40.69-1
 =39
 =0X0027

 ScicRegs.SCIHBAUD    =0x00;
 ScicRegs.SCILBAUD    =0x27;
 *************************************************************/

