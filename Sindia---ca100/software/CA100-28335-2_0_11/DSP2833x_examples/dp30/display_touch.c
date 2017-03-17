#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

const Uint16 lib_mode[6][3] =
{ 0x20, 0x82, 0x00, 0x21, 0x82, 0x01, 0x22, 0x81, 0x02, 0x23, 0x81, 0x03, 0x24,
		0x81, 0x07, 0x2A, 0x81, 0x15, };

void scib_loopback_init()
{
	// SCIA
	// in the InitSysCtrl() function

	SciaRegs.SCICCR.all = 0x0066; // 1 stop bit,  No loopback
								  //  parity-EVEN,7 char bits,
								  // async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all = 0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA = 0;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;
	SciaRegs.SCIHBAUD = 0x07;
	SciaRegs.SCILBAUD = 0xa0; //2400
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	SciaRegs.SCIFFTX.all = 0x8000;
	SciaRegs.SCIFFRX.all = 0x0000;
	SciaRegs.SCIFFCT.all = 0x00;

	SciaRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

	// SCIB
	// in the InitSysCtrl() function

	ScibRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
								  // No parity,8 char bits,
								  // async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all = 0x0003;
	ScibRegs.SCICTL2.bit.TXINTENA = 1;
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
	ScibRegs.SCIHBAUD = 0x0000;
	ScibRegs.SCILBAUD = 0x0027; //0x0027;115200
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	ScibRegs.SCIFFTX.all = 0xC02f;
	ScibRegs.SCIFFRX.all = 0x002a;
	ScibRegs.SCIFFCT.all = 0x00;

	ScibRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;

	//SCIC

	ScicRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
								  // No parity,8 char bits,
								  // async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all = 0x0003;
	ScicRegs.SCICTL2.bit.TXINTENA = 1;
	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
	ScicRegs.SCIHBAUD = 0x0001;
	ScicRegs.SCILBAUD = 0x00e8; //9600
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	ScicRegs.SCIFFTX.all = 0xC02a;
	ScicRegs.SCIFFRX.all = 0x002a;
	ScicRegs.SCIFFCT.all = 0x00;

	ScicRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;

}
void scia_send_byte(Uint16 a)
{
	Uint16 x;
//	cc();
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
//	cc();
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

void cc(void)
{
	Uint32 i;
	instruction_cnt++;
	if (instruction_cnt > 25)
	{
		instruction_cnt = 0;
		for (i = 0; i < 100000; i++)
			; //1000000---500ms
	}

}

// Transmit a character from the SCI'
void scib_xmit(Uint16 a)
{
	Uint16 x;
//	cc();
	do
	{
		x = ScibRegs.SCIFFTX.all & 0x1f00;
		x = x >> 8;
	} while (x >= 15);
	ScibRegs.SCITXBUF = a;
}
void print_string(char *dat)
{
	Uint16 x1, x2, i;
	Uint16 x;

	x1 = strlen(dat);

	for (i = 0; i < x1; i++)
	{
		x2 = *(dat++);
		do
		{
			x = ScibRegs.SCIFFTX.all & 0x1f00;
			x = x >> 8;
		} while (x >= 15);
		ScibRegs.SCITXBUF = x2;
	}
	//  UART0_SendByte(0x0d);
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
	Uint16 tmp;
	cc();

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

	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
	tmp=scia_receive_byte();
}


void color_change(Uint16 front, Uint16 back)
{
	Uint16 i;
	cc();
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
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x52);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);
}

void display_num_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, Uint16 num, char *strings)
{
	Uint16 i, j;
	Uint32 t;

	cc();

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

	cc();

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
//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, char *strings)
{
	Uint16 i;
	cc();

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
//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

//显示小数点后面一位，不显示＋
void display_fp_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j, h;
	float k, s;

	if (x1 > 100.0)
		s = 100.0;
	else if (x1 >= -100.0)
		s = x1;
	else
		s = -100.0;
	cc();

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

	if (s >= 0.0)
		scib_xmit(0x20);
	else
		scib_xmit(0x2d);

	if (s < 0.0)
	{
		i = -s;
		k = -s;
		h = -s;
	}
	else
	{
		i = s;
		k = s;
		h = s;
	}
	i = HEX4BCD(i);
	//   j=i>>8&0x0f;

	j = i >> 4 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
	}
	else
		scib_xmit(0x20);
	j = i & 0x0f;
	scib_xmit(j + 0x30);

	scib_xmit(0x2e);
	i = (Uint16) ((k - (float) h) * 1000.0);
	if( i == 99 || i == 199 || i == 299 || i == 399 || i == 499 || i == 599 || i == 699 || i == 799 || i == 899)
	{
		i = i + 1 ;
	}
	i = HEX4BCD(i);
	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);
	if (dots > 2)
	{
		j = i >> 4 & 0x0f;
		scib_xmit(j + 0x30);
	}

	print_string(strings);

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

//显示小数点后面三位，不显示＋
void display_fp4(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j, h, n;
	float k;
	cc();

	n = 0;
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

//    if (x1>=0.0)scib_xmit(0x2b);
//	else scib_xmit(0x2d);

	if (x1 < 0.0)
	{
		i = -x1;
		k = -x1;
		h = -x1;
	}
	else
	{
		i = x1;
		k = x1;
		h = x1;
	}
	i = HEX4BCD(i);
	j = i >> 12 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
		j = i >> 8 & 0x0f;
		scib_xmit(j + 0x30);
		j = i >> 4 & 0x0f;
		scib_xmit(j + 0x30);
		j = i & 0x0f;
		scib_xmit(j + 0x30);
		n = 4;
	}
	else
	{
		j = i >> 8 & 0x0f;
		if (j)
		{
			scib_xmit(j + 0x30);
			j = i >> 4 & 0x0f;
			scib_xmit(j + 0x30);
			j = i & 0x0f;
			scib_xmit(j + 0x30);
			n = 3;
		}
		else
		{
			j = i >> 4 & 0x0f;
			if (j)
			{
				scib_xmit(j + 0x30);
				n = 2;
			}
			else
				n = 1;
			j = i & 0x0f;
			scib_xmit(j + 0x30);
		}
	}
	scib_xmit(0x2e);
	i = (Uint16) ((k - (float) h) * 1000.0);
	i = HEX4BCD(i);
	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);
	if (n < 3)
		j = i >> 4 & 0x0f;
	scib_xmit(j + 0x30);
	if (n < 4)
	{
		j = i & 0x0f;
		scib_xmit(j + 0x30);
	}

	if (n == 1)
	{
		scib_xmit(0x20);
	}
	print_string(strings);

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

//显示小数点后面三位，显示＋
void display_fp_number(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1) //x<999.999
{
	Uint16 i, j, h, n;
	float k;
	cc();

	n = 0;
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

	if (x1 >= 0.0)
		scib_xmit(0x2b);
	else
		scib_xmit(0x2d);

	if (x1 < 0.0)
	{
		i = -x1;
		k = -x1;
		h = -x1;
	}
	else
	{
		i = x1;
		k = x1;
		h = x1;
	}
	i = HEX2BCD(i);
	j = i >> 8 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
		j = i >> 4 & 0x0f;
		scib_xmit(j + 0x30);
		j = i & 0x0f;
		scib_xmit(j + 0x30);
		n = 3;
	}
	else
	{
		j = i >> 4 & 0x0f;
		if (j)
		{
			scib_xmit(j + 0x30);
			n = 2;
		}
		else
			n = 1;
		j = i & 0x0f;
		scib_xmit(j + 0x30);
	}

	scib_xmit(0x2e);
	i = (Uint16) ((k - (float) h) * 1000.0);
	i = HEX2BCD(i);
	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);
	j = i >> 4 & 0x0f;
	scib_xmit(j + 0x30);
	if (n != 3)
	{
		j = i & 0x0f;
		scib_xmit(j + 0x30);
	}

	if (n == 1)
	{
		scib_xmit(0x20);
	}
//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_number(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, Uint16 x1) //x<999.999
{
	Uint16 i, j;
	Uint32 t;
	cc();

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
{ "日", "一", "二", "三", "四", "五", "六", "日", "日", "日"};
void display_clock(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y)
{
	Uint16 i;
	rx8025_clock_read();
	cc();

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
	i = clock.year >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.year & 0x0f;
	scib_xmit(0x30 + i);
	print_string(".");
	i = clock.month >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.month & 0x0f;
	scib_xmit(0x30 + i);
	print_string(".");
	i = clock.date >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.date & 0x0f;
	scib_xmit(0x30 + i);
	print_string(" ");

	i = clock.hour >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.hour & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = clock.minute >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.minute & 0x0f;
	scib_xmit(0x30 + i);
	print_string(":");
	i = clock.second >> 4 & 0x0f;
	scib_xmit(0x30 + i);
	i = clock.second & 0x0f;
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
	cc();

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

void display_dot(Uint16 x, Uint16 y)
{
	Uint16 i;
	cc();

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

void display_jx(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
	Uint16 i;
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x59);
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

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_jxy(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
	Uint16 i;
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x5b);
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

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_jx_clear(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
	Uint16 i;
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x5a);
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

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_line(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
	Uint16 i;
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x56);
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

	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void display_picture(Uint16 number, Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2,
		Uint16 x, Uint16 y)
{
	Uint16 i;
	cc();

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

}
void display_picture_all(Uint16 number)
{
	cc();

	scib_xmit(0xaa);
	scib_xmit(0x70);
	scib_xmit(number); //x
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

void picture_store(Uint16 number)
{
	cc();

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

void dw_int1(void)
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
        scib_xmit(0xcc);
        scib_xmit(0x33);
        scib_xmit(0xc3);
        scib_xmit(0x3c);

}

//显示小数点后面一位，不显示＋，最大99.9
void display_fp_ascii_ppm(Uint16 dots, Uint16 front, Uint16 back,
		Uint16 switch1, Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j, h;
	float k, s;

	if (x1 > 99.99)
		s = 99.99;
	else if (x1 >= 0.0)
		s = x1;
	else
		s = 0.0;
	cc();

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
	j = i >> 12 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
	}
	else
		scib_xmit(0x20);
	;

	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);
	scib_xmit(0x2e);

	j = i >> 4 & 0x0f;
	scib_xmit(j + 0x30);
	//   j=i&0x0f;
//	scib_xmit(j+0x30);

	print_string(strings);

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}

//显示小数点后面一位，不显示＋,最大999。9
void display_fp_ascii_ppm1(Uint16 dots, Uint16 front, Uint16 back,
		Uint16 switch1, Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j, h;
	float k, s;

	if (x1 > 999.99)
		s = 999.99;
	else if (x1 >= 0.0)
		s = x1;
	else
		s = 0.0;
	cc();

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

	i = (Uint16) (s * 10.0);
	i = HEX4BCD(i);
	j = i >> 12 & 0x0f;
	if (j)
	{
		scib_xmit(j + 0x30);
	}
	else
		scib_xmit(0x20);
	;

	j = i >> 8 & 0x0f;
	scib_xmit(j + 0x30);

	j = i >> 4 & 0x0f;
	scib_xmit(j + 0x30);
	scib_xmit(0x2e);
	j = i & 0x0f;
	scib_xmit(j + 0x30);

	print_string(strings);

//	scib_xmit(i+0x30);
	scib_xmit(0xcc);
	scib_xmit(0x33);
	scib_xmit(0xc3);
	scib_xmit(0x3c);

}
//显示小数点后面二位，不显示＋,最大999。9
void display_gpa(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,
		Uint16 x, Uint16 y, float x1, char *strings) //x<999.999
{
	Uint16 i, j, h;
	float k, s;

	if (x1 > 999.99)
		s = 999.99;
	else if (x1 >= 0.0)
		s = x1;
	else
		s = 0.0;
	cc();

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

