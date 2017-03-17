#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

Uint16 const ascii_tab[]={48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70};
char const ascii_tab1[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void lrc_count(Uint16 number,char * pt)//number 为要发送的字符总数
{
	Uint16 temp=0,i;

	for(i=1;i<(number-2);i++)
	{
		temp=*(pt+i)+temp;
	}
	temp=temp&0x0ff;
	temp=0xff-temp+1;
	*(pt+number-2)=ascii_tab1[temp>>4&0x0f];
	*(pt+number-1)=ascii_tab1[temp&0x0f];
}

Uint16 lrc_check(Uint16 number,char * pt)//number 为接受到的字符总数
{
	Uint16 temp=0,i;

	for(i=1;i<(number-2);i++)
	{
		temp=*(pt+i)+temp;
	}
	temp=temp&0x0ff;
	temp=0xff-temp+1;

	if(*(pt+number-2)==ascii_tab1[temp>>4&0x0f])
	{
		if( *(pt+number-1)==ascii_tab1[temp&0x0f]) return OK;
		else return ERROR;
	}
	else return ERROR;
}

int16 ascii_hex(char * pt)
{
    Uint16 temp[4],i;

	for(i=0;i<4;i++)
	{
		if(*(pt+7+i)<58)temp[i]=*(pt+7+i)-48;
		else temp[i]=*(pt+7+i)-55;
	}
	i=temp[0]<<12 | temp[1]<<8 | temp[2]<<4 | temp[3];
	return i;
}

void get_Concentration(float * out)//返回为HEX的浓度值
{
	Uint16 i,temp;
	char send_buffer[20];
	char receive_buffer[40];

	if(flw.flag==1) *out=0;
	for(i=0;i<40;i++)receive_buffer[i]=0;
	send_buffer[0]  = ':';
	send_buffer[1]  = ascii_tab1[plat.pur_add>>4&0x0f];
	send_buffer[2]  = ascii_tab1[plat.pur_add&0x0f];
	send_buffer[3]  = '0';
	send_buffer[4]  = '3';
	send_buffer[5]  = '0';
	send_buffer[6]  = '0';
	send_buffer[7]  = '0';
	send_buffer[8]  = 'A';
	send_buffer[9]  = '0';
	send_buffer[10] = '0';
	send_buffer[11] = '0';
	send_buffer[12] = '1';
	send_buffer[13] = '0';
	send_buffer[14] = '0';
	send_buffer[15] = '\r';
	send_buffer[16] = '\n';
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)scia_send_byte(send_buffer[i]);
	do
	{
		receive_buffer[0]=scia_receive_byte();
	}
	while(receive_buffer[0]!=':');

	for(i=1;i<15;i++)
	{
		receive_buffer[i]=scia_receive_byte();
	}
    
	temp=lrc_check(13,receive_buffer);
	if(temp==OK)
	{
		*out= ascii_hex(receive_buffer);
		*out*=0.1;
	}
	else 
	{
		*out=0;
	}
}

Uint16 get_zero(void)//为实际测量的零点（需要通零气）
{
	Uint16 i,temp;
	char send_buffer[20];
	char receive_buffer[40];

	send_buffer[0]  = ':'  ;
	send_buffer[1]  = ascii_tab1[plat.pur_add>>4&0x0f];
	send_buffer[2]  = ascii_tab1[plat.pur_add&0x0f];
	send_buffer[3]  = '0'  ;
	send_buffer[4]  = '3'  ;
	send_buffer[5]  = '0'  ;
	send_buffer[6]  = '0'  ;
	send_buffer[7]  = '0'  ;
	send_buffer[8]  = '4'  ;
	send_buffer[9]  = '0'  ;
	send_buffer[10] = '0'  ;
	send_buffer[11] = '0'  ;
	send_buffer[12] = '1'  ;
	send_buffer[13] = '0'  ;
	send_buffer[14] = '0'  ;
	send_buffer[15] = '\r' ;
	send_buffer[16] = '\n' ;
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)scia_send_byte(send_buffer[i]);
	do
	{
		receive_buffer[0]=scia_receive_byte();
	}
	while(receive_buffer[0]!=':');

	for(i=1;i<15;i++)
	{
		receive_buffer[i]=scia_receive_byte();
	}
	temp=lrc_check(13,receive_buffer);
	if(temp==OK)
	{
		return 0;
	}
	else 
	{
		return 1;
	}    
}

Uint16 set_zero(void)//返回为状态：OK,ERROR
{
	Uint16 i,temp,temp1[4];
	char send_buffer[20];
	char receive_buffer[40];

	temp1[0]=receive_buffer[7];
	temp1[1]=receive_buffer[8];
	temp1[2]=receive_buffer[9];
	temp1[3]=receive_buffer[10];

	send_buffer[0]  = ':'  ;
	send_buffer[1]  = ascii_tab1[plat.pur_add>>4&0x0f];
	send_buffer[2]  = ascii_tab1[plat.pur_add&0x0f];
	send_buffer[3]  = '0'  ;
	send_buffer[4]  = '6'  ;
	send_buffer[5]  = '0'  ;
	send_buffer[6]  = '0'  ;
	send_buffer[7]  = '4'  ;
	send_buffer[8]  = '7'  ;
	send_buffer[9]  = receive_buffer[7];
	send_buffer[10] = receive_buffer[8];
	send_buffer[11] = receive_buffer[9];
	send_buffer[12] = receive_buffer[10];
	send_buffer[13] = '0'  ;
	send_buffer[14] = '0'  ;
	send_buffer[15] = '\r' ;
	send_buffer[16] = '\n' ;
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)
	{
		scia_send_byte(send_buffer[i]);
	}
    
	send_buffer[0]  = ':'  ;
	send_buffer[1]  = ascii_tab1[plat.pur_add>>4&0x0f];
	send_buffer[2]  = ascii_tab1[plat.pur_add&0x0f];
	send_buffer[3]  = '0'  ;
	send_buffer[4]  = '3'  ;
	send_buffer[5]  = '0'  ;
	send_buffer[6]  = '0'  ;
	send_buffer[7]  = '4'  ;
	send_buffer[8]  = '7'  ;
	send_buffer[9]  = '0'  ;
	send_buffer[10] = '0'  ;
	send_buffer[11] = '0'  ;
	send_buffer[12] = '1'  ;
	send_buffer[13] = '0'  ;
	send_buffer[14] = '0'  ;
	send_buffer[15] = '\r' ;
	send_buffer[16] = '\n' ;
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)scia_send_byte(send_buffer[i]);
	do
	{
		receive_buffer[0]=scia_receive_byte();
	}
	while(receive_buffer[0]!=':');

	for(i=1;i<15;i++)
	{
		receive_buffer[i]=scia_receive_byte();
	}
	temp=lrc_check(13,receive_buffer);
	if(temp==OK)
	{
		if(temp1[0]==receive_buffer[7] && temp1[1]==receive_buffer[8] && temp1[2]==receive_buffer[9] && temp1[3]==receive_buffer[10])
		return OK;
		else return ERROR; 
	}
	else return ERROR;
  
}

Uint16 read_zero(void)//读取设置后的零点值，寄存器中设定好的
{
    Uint16 i,temp;
	char send_buffer[20];
	char receive_buffer[40];

	send_buffer[0]  = ':'  ;
	send_buffer[1]  = ascii_tab1[plat.pur_add>>4&0x0f];
	send_buffer[2]  = ascii_tab1[plat.pur_add&0x0f];
	send_buffer[3]  = '0'  ;
	send_buffer[4]  = '3'  ;
	send_buffer[5]  = '0'  ;
	send_buffer[6]  = '0'  ;
	send_buffer[7]  = '4'  ;
	send_buffer[8]  = '7'  ;
	send_buffer[9]  = '0'  ;
	send_buffer[10] = '0'  ;
	send_buffer[11] = '0'  ;
	send_buffer[12] = '1'  ;
	send_buffer[13] = '0'  ;
	send_buffer[14] = '0'  ;
	send_buffer[15] = '\r' ;
	send_buffer[16] = '\n' ;
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)
	{
		scia_send_byte(send_buffer[i]);
	}

	do
	{
		receive_buffer[0]=scia_receive_byte();
	}
	while(receive_buffer[0]!=':');

	for(i=1;i<15;i++)
	{
	receive_buffer[i]=scia_receive_byte();
	}
	temp=lrc_check(13,receive_buffer);
	if(temp==OK)
	{
		temp= ascii_hex(receive_buffer);
		return 1;
	}
	else 
	{
		return 0;
	}    
}

Uint16 get_address(Uint16 module)//返回为0---错误 1---正确
{
	Uint16 i,temp;
	Uint32 j=0;
	char send_buffer[20];
	char receive_buffer[40];

	for(i=0;i<40;i++)receive_buffer[i]=0;
	send_buffer[0]  = ':'  ;
	send_buffer[1]  = ascii_tab1[module>>4&0x0f] ;
	send_buffer[2]  = ascii_tab1[module&0x0f] ;
	send_buffer[3]  = '0'  ;
	send_buffer[4]  = '3'  ;
	send_buffer[5]  = '0'  ;
	send_buffer[6]  = '0'  ;
	send_buffer[7]  = '0'  ;
	send_buffer[8]  = 'A'  ;
	send_buffer[9]  = '0'  ;
	send_buffer[10] = '0'  ;
	send_buffer[11] = '0'  ;
	send_buffer[12] = '1'  ;
	send_buffer[13] = '0'  ;
	send_buffer[14] = '0'  ;
	send_buffer[15] = '\r' ;
	send_buffer[16] = '\n' ;
	lrc_count(15,send_buffer);
	for(i=0;i<17;i++)scia_send_byte(send_buffer[i]);
	do
	{
		receive_buffer[0]=scia_receive_byte();
		j++;
	}
	while(receive_buffer[0]!=':'&& j<10);
	if(j>=10) return 0;
	for(i=1;i<15;i++)
	{
		receive_buffer[i]=scia_receive_byte();
	}
    
	temp=lrc_check(13,receive_buffer);
	if(temp==OK)return 1;
	else return 0;
}

void purity_det(Uint16 x, Uint16 y, Uint16 id)
{
	Uint16 i,temp;Uint32 j;

	display_ascii(M32,WHITE,BLUE_GREEN,1,x,y,"正在搜索纯度模块......");
	temp=get_address(id);
	if(temp==1)
	{
		display_ascii(M32,WHITE,BLUE_GREEN,1,x,y,"纯度模块初始化成功                     ");
		plat.pur_add=id;
		for(j=0;j<1000000;j++);
		return;
	}
	for(i=0;i<256;i++)
	{
		display_number(M32,WHITE,BLUE_GREEN,1,x+400,y,i);
		temp=get_address(i);
		if(temp==1)
		{
			display_ascii(M32,WHITE,BLUE_GREEN,1,x,y,"纯度模块初始化成功                     ");
			plat.pur_add=i;
			fm22l16_wr(ADD_PUR,i);
			for(j=0;j<1000000;j++);
			return;
		}
	}
}






