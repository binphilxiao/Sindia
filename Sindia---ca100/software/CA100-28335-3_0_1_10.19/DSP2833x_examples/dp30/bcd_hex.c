#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/********** HEX转BCD******/
/***bcd_data(<0x255,>0)***/
Uint16 BCD2HEX(Uint16 bcd_data)
{
    Uint16 temp;
    temp=(((bcd_data&0x0f00)>>8)*100)+(((bcd_data&0x00f0)>>4)*10)+(bcd_data&0x000f);
  //  temp=(_iror_(bcd_data&0x0f00,8)*100)+(_iror_(bcd_data&0x00f0,4)*10)+(bcd_data&0x000f); //the function of c51 is slower
    return temp;
}

/********** HEX转BCD******/
/***hex_data(<0xff,>0)****/

Uint16 HEX2BCD(Uint16 hex_data)
{
    Uint16 bcd_data;
    Uint16 temp;
    temp=hex_data%100;
    bcd_data=((Uint16)hex_data)/100<<8;
    bcd_data=bcd_data|temp/10<<4;
    bcd_data=bcd_data|temp%10;
    return bcd_data;
}
/********** HEX转BCD******/
/***bcd_data(<0x65535,>0)***/
Uint16 BCD4HEX(Uint32 bcd_data)
{
    Uint16 temp;
    temp=  (((bcd_data&0x000f0000)>>16)*10000)
          +(((bcd_data&0x0000f000)>>12)*1000)
          +(((bcd_data&0x00000f00)>>8)*100)
          +(((bcd_data&0x000000f0)>>4)*10)
          +  (bcd_data&0x0000000f);
    return temp;
}
/********** HEX转BCD******/
/***hex_data(<0xff,>0)****/

Uint32 HEX4BCD(Uint16 hex_data)
{
    Uint32 bcd_data;
    Uint16 temp;
    temp=hex_data%10000;
    bcd_data=((unsigned long)hex_data)/10000<<16;
    bcd_data=bcd_data|((temp/1000)<<12);
    temp=hex_data%1000;
    bcd_data=bcd_data|((temp/100)<<8);
    temp=hex_data%100;
    bcd_data=bcd_data|((temp/10)<<4);
    bcd_data=bcd_data|temp%10;
    return bcd_data;
}

/***********************************************************
单精度浮点转换为字符串
支持正负数值
小数点后显示两位
字符串长度不定
不显示'+'  号
************************************************************/
void sflt_to_str(float in, char * out)
{
	Uint32 i,flag=0;
	if (in<0) {*out='-';in*=-1;out++;}
	in*=100;
	i=(Uint32)in;
	if (i>=1000000) i%=1000000;
	if (i<1000000) 
	{
		*out=(char)(i/100000)+48;
		if (*out>48) {out++;flag=1;}
		i%=100000;
	}
	if (i<100000) 
	{
		*out=(char)(i/10000)+48;
		if (flag==1 || *out>48) {out++;flag=1;}
		i%=10000;
	}
	if (i<10000) 
	{
		*out=(char)(i/1000)+48;
		if (flag==1 || *out>48) {out++;flag=1;}
		i%=1000;
	}
	if (i<1000) {*out=(char)(i/100)+48;out++;i%=100;i%=100;}
	*out='.';out++;
	*out=(char)(i/10)+48;i%=10;out++;
	*out=(char)(i/1)+48;out++;
	*out='\0';
}

void uint32_to_str(Uint32 in, char *out)
{
	Uint16 flag=0;

	if (in>=1000000) in%=1000000;

	*out=(char)(in/100000)+48;
	if (*out>48) {out++;flag=1;}
	in%=100000;

	*out=(char)(in/10000)+48;
	if (flag==1 || *out>48) {out++;flag=1;}
	in%=10000;

	*out=(char)(in/1000)+48;
	if (flag==1 || *out>48) {out++;flag=1;}
	in%=1000;

	*out=(char)(in/100)+48;
	if (flag==1 || *out>48) {out++;flag=1;}
	in%=100;

	*out=(char)(in/10)+48;
	if (flag==1 || *out>48) {out++;flag=1;}
	in%=10;

	*out=(char)(in)+48;
	if (flag==1 || *out>48) {out++;flag=1;}

	*out='\0';
}
/***********************************************************
格式化sflt_to_str后输出的字符串
字符串长度6
当有'-' 和'.' 字符时只能显示4位有效数字
字符串不够长时补空格
************************************************************/
void format_fpstr(char * in, Uint16 len)
{
	Uint16 i;
	for (i=0;i<len;i++)
		if (*(in+i)=='\0') break;
	for(;i<len;i++)
		*(in+i)=' ';
	*(in+len)='\0';
}

float str_to_sflt(char * in)
{
	float out,sign,dot;
	Uint16 i;

	i=0;
	sign=1;//表示符号
	dot=1;//表示小数点后第几位，每次除以10

	while(*(in+i)!='\0')
	{
		if (*(in+i)=='-') sign=-1;
		if (*(in+i)=='+') sign= 1;
		else if (*(in+i)=='.') dot=0.1;
		else if (*(in+i)>='0' && *(in+i)<='9' )	
			if (dot==1)
				out=out*10+(*(in+i)-'0');
			else
			{
				out+=(*(in+i)-'0')*dot;
				dot/=10;
			}
		i++;
	}
	out*=sign;
	return out;
}