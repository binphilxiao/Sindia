#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//void scia_send_byte(Uint16 a)

//Uint16 scia_receive_byte(void)

Uint16 const ascii_tab[]={48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70};
char const ascii_tab1[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char send_buffer[20];
char receive_buffer[40];
Uint16 module_address;

void lrc_count(Uint16 number)//number 为要发送的字符总数
{
    Uint16 temp=0,i;

    for(i=1;i<(number-2);i++)
	{
	    temp=send_buffer[i]+temp;
	}
	temp=temp&0x0ff;
	temp=0xff-temp+1;
	send_buffer[number-2]=ascii_tab1[temp>>4&0x0f];
	send_buffer[number-1]=ascii_tab1[temp&0x0f];
}

Uint16 lrc_check(Uint16 number)//number 为接受到的字符总数
{
    Uint16 temp=0,i;

    for(i=1;i<(number-2);i++)
	{
	    temp=receive_buffer[i]+temp;
	}
	temp=temp&0x0ff;
	temp=0xff-temp+1;

	if(receive_buffer[number-2]==ascii_tab1[temp>>4&0x0f])
	{
	   if( receive_buffer[number-1]==ascii_tab1[temp&0x0f]) return OK;
	   else return ERROR;
	}
	else return ERROR;
}

int16 ascii_hex(void)
{
    Uint16 temp[4],i;

	for(i=0;i<4;i++)
	{
	    if(receive_buffer[7+i]<58)temp[i]=receive_buffer[7+i]-48;
		else temp[i]=receive_buffer[7+i]-55;
	}
	i=temp[0]<<12 | temp[1]<<8 | temp[2]<<4 | temp[3];
	return i;
}

float get_Concentration(void)//返回为HEX的浓度值
{
    Uint16 i,temp;
    char m;
	float tt;

    if(flow_flag==1)return 0.0;
	for(i=0;i<40;i++)receive_buffer[i]=0;
    send_buffer[0]  = ':';
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
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
    
    temp=lrc_check(13);
	if(temp==OK)
	{
	    tt= ascii_hex();
		tt=0.1*tt;
	//	if(tt<0.0)tt=0.0;
	    return tt;
	}
    else 
    {
        return 0.0;
    }
}

float get_Concentration1(void)//返回为HEX的浓度值 含有负数
{
    Uint16 i,temp;char m;
	float tt;
    if(flow_flag==1)return 0.0;
	for(i=0;i<40;i++)receive_buffer[i]=0;
    send_buffer[0]  = ':' ;
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
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
    
    temp=lrc_check(13);
	if(temp==OK)
	{
	    tt= ascii_hex();
		tt=0.1*tt;
	    return tt;
	}
    else 
    {
        return 0.0;
    }
}

Uint16 get_zero(void)//为实际测量的零点（需要通零气）
{
    Uint16 i,temp;float tt;

    send_buffer[0]  = ':'  ;
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
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
    temp=lrc_check(13);
	if(temp==OK)
	{
	    tt= ascii_hex();
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

    temp1[0]=receive_buffer[7];
	temp1[1]=receive_buffer[8];
	temp1[2]=receive_buffer[9];
	temp1[3]=receive_buffer[10];

    send_buffer[0]  = ':'  ;
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
    for(i=0;i<17;i++)
    {
    	scia_send_byte(send_buffer[i]);
    }
    
    send_buffer[0]  = ':'  ;
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
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
    temp=lrc_check(13);
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

    send_buffer[0]  = ':'  ;
    send_buffer[1]  = ascii_tab1[module_address>>4&0x0f];
	send_buffer[2]  = ascii_tab1[module_address&0x0f];
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
    lrc_count(15);
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
    temp=lrc_check(13);
	if(temp==OK)
	{
	    temp= ascii_hex();
	    return 1;
	}
    else 
    {
        return 0;
    }    
}

Uint16 get_address(Uint16 module)//返回为0---错误 1---正确
{
    Uint16 i,temp;char m;Uint32 j=0;

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
    lrc_count(15);
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
    
    temp=lrc_check(13);
	if(temp==OK)return 1;
    else return 0;
}

void purity_test(void)
{
    Uint16 i,temp;Uint32 j;

	color_change(0x001f,0x0000);cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
	display_ascii(M16,0x07e0,0x0000,1,5,30,"正在搜索纯度模块......");
	for(i=0;i<256;i++)
	{
	    display_number(M16,0x07e0,0x0000,1,180,30,i);
	    temp=get_address(i);
		if(temp==1)
		{
		    display_ascii(M16,0xf800,0x0000,1,5,50,"找到模块!");
		    module_address=i;
			fm22l16_wr(ADD_PUR,i);
			for(j=0;j<1000000;j++);
			return;
		}
	}
}

char *t_report_cn4={"GPA100 试验报告: \r\n"};

Uint16 gpa100_data_to_usb(struct MEASURE_DATA_0 *data)
{
    Uint16 tmp[240],j,i,k,m;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++)tmp[j]=t_report_cn4[j];
    for(j=0;j<20;j++)tmp[j+19]=t_dot_cn[j];
    for(j=0;j<11;j++)tmp[j+39]=t_parameter_cn[j];

    for(j=0;j<5;j++)tmp[j+50]=t_number_cn[j];
    for(j=0;j<4;j++)tmp[j+55]=data->number[j]+0x30;
    tmp[59]='\r';tmp[60]='\n'; //编号


    for(j=0;j<5;j++)tmp[j+61]=t_data_cn[j];//日期
    tmp[66]=0x30+2;
	tmp[67]=0x30+0;
	i=data->clock[6]>>4&0x0f;
	tmp[68]=i+0x30;
	i=data->clock[6]&0x0f;
    tmp[69]=i+0x30;
    tmp[70]='.';
	i=data->clock[5]>>4&0x0f;
	tmp[71]=i+0x30;
	i=data->clock[5]&0x0f;
    tmp[72]=i+0x30;
    tmp[73]='.';
	i=data->clock[4]>>4&0x0f;
    tmp[74]=i+0x30;
	i=data->clock[4]&0x0f;
    tmp[75]=i+0x30;
	tmp[76]='\r';tmp[77]='\n';

	for(j=0;j<5;j++)tmp[j+78]=t_time_cn[j];//时间
	i=data->clock[2]>>4&0x0f;
	tmp[83]=i+0x30;
	i=data->clock[2]&0x0f;
    tmp[84]=i+0x30;
    tmp[85]=':';
	i=data->clock[1]>>4&0x0f;
	tmp[86]=i+0x30;
	i=data->clock[1]&0x0f;
    tmp[87]=i+0x30;
    tmp[88]='\r';tmp[89]='\n';

    for(j=0;j<5;j++)tmp[j+90]=t_temp_cn[j];//温度
    if(data->c_temp>=0){k=(Uint16)(data->c_temp*10.0);tmp[95]='+';}
	else 
	{
	    k=(Uint16)(-data->c_temp*10.0);tmp[95]='-';
	}
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[96]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[97]=i;tmp[98]='.';
	i=k&0x0f;i=i+0x30;tmp[99]=i;tmp[100]='C';
	tmp[101]='\r';tmp[102]='\n';

    for(j=0;j<5;j++)tmp[j+103]=t_humid_cn[j];//湿度
	k=(Uint16)(data->c_humid*10.0);
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[108]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[109]=i;tmp[110]='.';
	i=k&0x0f;i=i+0x30;tmp[111]=i;tmp[112]='%';
	tmp[113]='\r';tmp[114]='\n';

    for(j=0;j<5;j++)tmp[j+115]=t_flow_cn[j];//流量
	tmp[120]=*(flow_table1[data->in8]);
	tmp[121]=*(flow_table1[data->in8]+1);
    tmp[122]='L';tmp[123]='/';tmp[124]='h';
    tmp[125]='\r';tmp[126]='\n';

	for(j=0;j<20;j++)tmp[j+127]=t_dot_cn[j];
    for(j=0;j<11;j++)tmp[j+147]=t_result_cn[j];

    for(j=0;j<5;j++)tmp[j+158]=t_gpa_cn[j];//纯度
	tmp[163]=' ';
    k=(Uint16)(data->gpa_calib_value*100.0);
	k=HEX4BCD(k);
	i=k>>16&0x0f;i=i+0x30;tmp[164]=i;
	i=k>>12&0x0f;i=i+0x30;tmp[165]=i;
	i=k>>8&0x0f;i=i+0x30;tmp[166]=i;
	tmp[167]='.';
	i=k>>4&0x0f;i=i+0x30;tmp[168]=i;
	i=k&0x0f;i=i+0x30;tmp[169]=i;tmp[170]='%';
	tmp[171]='\r';tmp[172]='\n';

    i=fm22l16_rd(ADD_USB_NUM);
    i++;if(i>300)i=0;	
    fm22l16_wr(ADD_USB_NUM,i);	 
    usb_creat_file(i,"GPA-",&tmp[0],173);
    
    return 1;
}

void gpa100_data_to_printer(struct MEASURE_DATA_0 *data)
{
    Uint16 i,j,g,m;
    int16 tt1;
    
    scic_xmit(0x1b);//反向打印
    scic_xmit(0x63);
    scic_xmit(0x00);

    scic_xmit(0x1b);
    scic_xmit(0x31);
    scic_xmit(0x01);
    
    print_string_c("GPA100测试报告:");//标题
    scic_xmit(0x0d);

    print_string_c("编号:");//编号
    for(j=0;j<4;j++)
    {
        g=data->number[j]+0x30;
        scic_xmit(g);
    }
    scic_xmit(0x0d);

    print_string_c("时间:");//时间
    scic_xmit(0x30+2);
    scic_xmit(0x30+0);
    for(j=6;j>3;j--)
    {
        g=data->clock[j];
        i=g>>4&0x0f;
        scic_xmit(0x30+i);
        i=g&0x0f;
        scic_xmit(0x30+i);
        if (j>4)scic_xmit(0x2e);
    }
    scic_xmit(0x20);
    for(j=2;j>0;j--)
    {
        g=data->clock[j];
        i=g>>4&0x0f;
        scic_xmit(0x30+i);
        i=g&0x0f;
        scic_xmit(0x30+i);
        if (j==2) scic_xmit(0x3a);
    } 
    scic_xmit(0x0d);
    
    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);
    
    print_string_c("测试参数:");//测试条件：
 
    scic_xmit(0x0d);

	print_string_c("温度:");
	for(i=0;i<3;i++) scic_xmit(0x20);
    if(data->c_temp>=0.0){scic_xmit(0x2b);i=data->c_temp*10.0;}
	else {scic_xmit(0x2d);i=(-data->c_temp*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("℃"); 
    for(i=0;i<1;i++) scic_xmit(0x20); 
    scic_xmit(0x0d);

	print_string_c("湿度:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	i=data->c_humid*10.0;
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("%"); 
    scic_xmit(0x0d);

/*    
    print_string_c("气体:");
    if (data->gas==0){print_string_c("SF6");}
    else if (data->gas==1){print_string_c("N2 ");}
    else if (data->gas==2){print_string_c("CO2");}
    for(i=0;i<4;i++) scic_xmit(0x20);
*/

    print_string_c("流量:");
	for(i=0;i<3;i++) scic_xmit(0x20);
    print_string_c(flow_table1[data->in8]);print_string_c("L/H");
    scic_xmit(0x0d);

    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);
    
    print_string_c("测试结果:");
    scic_xmit(0x0d);
    
    print_string_c("纯度:");
	for(i=0;i<3;i++) scic_xmit(0x20);
    i=data->gpa_calib_value*10.0;
	i=HEX4BCD(i);
	j=i>>12&0x0f;scic_xmit(0x30+j);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("%"); 
    scic_xmit(0x0d);
    
    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);

//    print_string_c("***SINDIA Instrument***");
//    scic_xmit(0x0d);
    print_string_c("www.sindia.cn");
    for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);
}

Uint16 gpa100_data_out(struct MEASURE_DATA_0 *data,Uint16 cnt)
{
    Uint32 j;
	
    color_change(0x001f,0x0000);u_key_cnt=0;cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    
	display_ascii(M16,0xffff,0x001f,1,5,2,"处理");
    display_ascii(M16,0x07e0,0x0000,1,10,40,"请确认数据处理方式");
    display_ascii(M16,0xffff,0x001f,1,0,222," 取消       删除        打印        U盘");
    key_clr();km=1;
	while(keynew[0]==0 && keynew[2]==0 && keynew[3]==0&& keynew[1]==0){u_key_draw();};

    if(keynew[0]==1)
    {
	    key_clr();
        return cnt;
    }
	else if(keynew[2]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在打印当前数据！");
		for(j=0;j<1000000;j++);
		gpa100_data_to_printer(data);return cnt;
    }
    else if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在传输全部数据！");
		for(j=0;j<1000000;j++);
		gpa100_data_to_usb(data);return cnt;
    }
	else if(keynew[1]==1)
    {
	    key_clr();j=delete(cnt);return j;
    }
    return cnt;
}

void gpa100_display_datalib(Uint16 x)
{
    Uint16 i,k[4];Uint32 number;
    char s[17];

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
     
    display_num_ascii1(M16,0xffff,0x001f,1,220,2,number-x,"/");
    display_number(M16,0xffff,0x001f,1,270,2,number);
	display_ascii(M16,0xffff,0x001f,1,300,2,"页");
    read_out_fram(x);

	k[0]=store_data.number[0]<<12 & 0xf000;//编号
	k[1]=store_data.number[1]<<8 & 0x0f00;
	k[2]=store_data.number[2]<<4 & 0x00f0;
	k[3]=store_data.number[3] & 0x000f;
    k[0]=k[0]+k[1]+k[2]+k[3];i=BCD4HEX(k[0]);
	display_ascii(M16,0,0,1,53+8,51,"        ");
    display_number(M16,0xffff,0x0000,1,53+8,51,i);

    s[0]=0x30+2;//日期
	s[1]=0x30+0;
	i=store_data.clock[6]>>4&0x0f;
	s[2]=i+0x30;
	i=store_data.clock[6]&0x0f;
    s[3]=i+0x30;
    s[4]='.';
	i=store_data.clock[5]>>4&0x0f;
	s[5]=i+0x30;
	i=store_data.clock[5]&0x0f;
    s[6]=i+0x30;
    s[7]='.';
	i=store_data.clock[4]>>4&0x0f;
    s[8]=i+0x30;
	i=store_data.clock[4]&0x0f;
    s[9]=i+0x30;
    s[10]='\0';
	display_ascii(M16,0xffff,0x0000,1,53+8,51+26,&s[0]);


	i=store_data.clock[2]>>4&0x0f;//时间
	s[0]=i+0x30;
	i=store_data.clock[2]&0x0f;
    s[1]=i+0x30;
    s[2]=':';
	i=store_data.clock[1]>>4&0x0f;
	s[3]=i+0x30;
	i=store_data.clock[1]&0x0f;
    s[4]=i+0x30;
    s[5]='\0';  
    display_ascii(M16,0xffff,0x0000,1,53+8,51+52,&s[0]);

    display_fp_ascii(M16,0xffff,0x0000,1,53,129,store_data.c_temp,"℃");//温度和湿度
    display_fp_ascii(M16,0xffff,0x0000,1,53,129+26,store_data.c_humid,"%");//x<999.999

    display_ascii(M16,0xffff,0x0000,1,53+8,129+26+26,flow_table1[store_data.in8]);
    display_ascii(M16,0xffff,0x0000,1,53+24+8,129+26+26,"L/h");

	//display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51,store_data.gpa_calib_value," %");
	display_gpa(M16,0xffff,0x0000,1,150+48,51,store_data.gpa_calib_value," %");
} 

void gpa100_search(void)
{
    Uint16 keycnt,i;Uint32 number,h;

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	keycnt=number-1;	

search01:
    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	
    color_change(0x001f,0x0000); 
    cls();
    km=1;u_key_cnt=0;
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    display_ascii(M16,0xffff,0x001f,1,5,2,"查询");
	display_ascii(M16,0xffff,0x001f,1,0,222," 退出       上页        下页       处理");

	display_ascii(M16,0xFFE0,0x0000,1,5,25,"测量参数");

    display_ascii(M16,0x07e0,0x0000,1,5,25+26,"编号:");
    display_ascii(M16,0x07e0,0x0000,1,5,25+52,"日期:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+78,"时间:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+104,"温度:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+130,"湿度:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+156,"流量:");  

	display_ascii(M16,0xFFE0,0x0000,1,150,25,"测量结果");

    display_ascii(M16,0x07e0,0x0000,1,150,25+26,"纯度:");

    for(h=0;h<200000;h++);
    key_clr();
    gpa100_display_datalib(keycnt);

    while(keynew[0]==0)
    {
	      u_key_draw();
          if (keynew[1]+keynew[2]+keynew[3])
          {
            if (keynew[1]==1)
            {
                keycnt++;
                if (keycnt>(number-1)) keycnt=0;
            }
            else if (keynew[2]==1)
            {
                if (keycnt==0) keycnt=number-1;
				else keycnt--;
            }
            else if (keynew[3]==1)
            {
			    keycnt=gpa100_data_out(&store_data,keycnt);goto search01;
            }
            key_clr();
            gpa100_display_datalib(keycnt);
          }  
    }
}









