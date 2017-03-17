#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

char *flow_table[] =
{ "00 ", "05 ", "10 ", "15 ", "20 ", "25 ", "30 ", "35 ", "40 ", "45 ", "50 " };

void dump(unsigned int val)
{
	if (val) BPL(0);
	else BPL(1);
}

void valve(unsigned int ch,unsigned int val)
{
	if (ch==1)
	{
		if (val) KG1(1);
		else KG1(0);
	}
	else if (ch==2)
	{
		if (val) KG2(1);
		else KG2(0);
	}
	else if (ch==3)
	{
		if (val) KG3(1);
		else KG3(0);
	}
}

//pipleline   1:大浓度SO2气路吹气   2:小浓度SO2吹气
void gas_clean(Uint16 val)
{
	if (val==0)
	{
		valve(1,1);
		valve(2,1);
		valve(3,0);
	 	dump(0);
	}
	else if (val==1)
	{
		valve(1,1);
		valve(2,1);
		valve(3,0);
	 	dump(1);
	}
	else if (val==2)
	{
		valve(1,1);
		valve(2,0);
		valve(3,0);
		dump(1);
	}
}

char *t_report_cn1={"CA100  试验报告: \r\n"};
char *t_report_cn2={"DA100  试验报告: \r\n"};
char *t_report_cn4={"GPA100 试验报告: \r\n"};
char *t_report_cn3={"HA100  试验报告: \r\n"};
char *t_parameter_cn={"测量参数 \r\n"};
char *t_number_cn={"编号:"};
char *t_data_cn={"日期:"};
char *t_time_cn={"时间:"};
char *t_temp_cn={"温度:"};
char *t_humid_cn={"湿度:"};
char *t_flow_cn={"流量:"};

char *t_result_cn={"测量结果 \r\n"};
char *t_dew_cn={"露点:"};
char *t_h2o_cn={"微水:"};
char *t_so2_cn={"SO2:"};
char *t_h2s_cn={"H2S:"};
char *t_hf_cn={"CO :"};
char *t_gpa_cn={"纯度:"};
char *t_dot_cn={"------------------\r\n"};

Uint16 usb_page(struct MEASURE_DATA *data)
{
	Uint16 tmp[300],j,i,k,m;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++) tmp[j]=t_report_cn1[j];
	for(j=0;j<20;j++) tmp[j+19]=t_dot_cn[j];
	for(j=0;j<11;j++) tmp[j+39]=t_parameter_cn[j];
	
	for(j=0;j<5;j++)tmp[j+50]=t_number_cn[j];//编号
	for(j=0;j<4;j++) tmp[j+55]=data->number[j]+0x30;
	tmp[59]='\r';tmp[60]='\n'; //编号

	for(j=0;j<5;j++) tmp[j+61]=t_data_cn[j];//日期
	tmp[66]=0x30+2;
	tmp[67]=0x30+0;
	i=data->clk.year/10;
	tmp[68]=i+0x30;
	i=data->clk.year%10;
	tmp[69]=i+0x30;
	tmp[70]='.';
	i=data->clk.month/10;
	tmp[71]=i+0x30;
	i=data->clk.month%10;
	tmp[72]=i+0x30;
	tmp[73]='.';
	i=data->clk.date/10;
	tmp[74]=i+0x30;
	i=data->clk.date%10;
	tmp[75]=i+0x30;
	tmp[76]='\r';tmp[77]='\n';

	for(j=0;j<5;j++)tmp[j+78]=t_time_cn[j];//时间
	i=data->clk.hour/10;
	tmp[83]=i+0x30;
	i=data->clk.hour%10;
	tmp[84]=i+0x30;
	tmp[85]=':';
	i=data->clk.minute/10;
	tmp[86]=i+0x30;
	i=data->clk.minute%10;
	tmp[87]=i+0x30;
	tmp[88]='\r';tmp[89]='\n';

	for(j=0;j<5;j++)tmp[j+90]=t_temp_cn[j];//温度
	if(data->c_temp>=0)
	{
		k=(Uint16)(data->c_temp*10.0);tmp[95]='+';
	}
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
	//tmp[120]=*(flow_table1[data->flw]);
	//tmp[121]=*(flow_table1[data->flw]+1);
	tmp[122]='L';tmp[123]='/';tmp[124]='h';
	tmp[125]='\r';tmp[126]='\n';

	for(j=0;j<20;j++)tmp[j+127]=t_dot_cn[j];
	for(j=0;j<11;j++)tmp[j+147]=t_result_cn[j];

	for(j=0;j<5;j++)tmp[j+158]=t_dew_cn[j];//露点
	if(data->h2o_calib_value>=0)
	{
		k=(Uint16)(data->h2o_calib_value*10.0);tmp[163]='+';
	}
	else 
	{
		k=(Uint16)(-data->h2o_calib_value*10.0);tmp[163]='-';
	}
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[164]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[165]=i;tmp[166]='.';
	i=k&0x0f;i=i+0x30;tmp[167]=i;tmp[168]='C';
	tmp[169]='\r';tmp[170]='\n';

	for(j=0;j<5;j++)tmp[j+171]=t_h2o_cn[j];//水分
	if(data->h2oppm<6033.0)
	{
		k=(Uint16)(data->h2oppm);
		k=HEX4BCD(k);
		i=k>>12&0x0f;i=i+0x30;tmp[176]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[177]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[178]=i;
		i=k&0x0f;i=i+0x30;tmp[179]=i;
		tmp[180]='u';tmp[181]='L';tmp[182]='/';tmp[183]='L';
		tmp[184]='\r';tmp[185]='\n';
	}
	else
	{
		tmp[176]='*';
		tmp[177]='*';
		tmp[178]='*';
		tmp[179]='*';
		tmp[180]='u';tmp[181]='L';tmp[182]='/';tmp[183]='L';
		tmp[184]='\r';tmp[185]='\n';
	}
	for(j=0;j<5;j++)tmp[j+186]=t_h2o_cn[j];//水分

	if(data->h2oppm20<6033.0)
	{
		k=(Uint16)(data->h2oppm20);
		k=HEX4BCD(k);
		i=k>>12&0x0f;i=i+0x30;tmp[191]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[192]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[193]=i;
		i=k&0x0f;i=i+0x30;tmp[194]=i;
		tmp[195]='u';tmp[196]='L';tmp[197]='/';tmp[198]='L';
		tmp[199]='(';tmp[200]='+';tmp[201]='2';tmp[202]='0';tmp[203]='C';tmp[204]=')';
		tmp[205]='\r';tmp[206]='\n';
	}
	else
	{
		tmp[191]='*';
		tmp[192]='*';
		tmp[193]='*';
		tmp[194]='*';
		tmp[195]='u';tmp[196]='L';tmp[197]='/';tmp[198]='L';
		tmp[199]='-';tmp[200]='-';tmp[201]='-';tmp[202]='-';tmp[203]='-';tmp[204]='-';
		tmp[205]='\r';tmp[206]='\n';
	}

	for(j=0;j<5;j++)tmp[j+207]=t_gpa_cn[j];//纯度
	k=(Uint16)(data->pur_calib_value*10.0);
	k=HEX4BCD(k);
	i=k>>12&0x0f;i=i+0x30;tmp[212]=i;
	i=k>>8&0x0f;i=i+0x30;tmp[213]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[214]=i;
	tmp[215]='.';
	i=k&0x0f;i=i+0x30;tmp[216]=i;
	tmp[217]='%';
	tmp[218]='\r';tmp[219]='\n';

	for(m=0;m<plat.sensor_number;m++)
	{
		if(plat.sensor[m]==0xa0)
		{
			for(j=0;j<5;j++)tmp[j+220+14*m]=t_h2s_cn[j];//H2S
			if (data->h2s_calib_value>=999.9) k=9999;
			else
			k=(Uint16)(data->h2s_calib_value*10.0);
			k=HEX4BCD(k);
			i=k>>8&0x0f;i=i+0x30;tmp[225+14*m]=i;
			i=k>>4&0x0f;i=i+0x30;tmp[226+14*m]=i;
			tmp[227+14*m]='.';
			i=k&0x0f;i=i+0x30;tmp[228+14*m]=i;
			tmp[229+14*m]='P';tmp[230+14*m]='P';tmp[231+14*m]='M';
			tmp[232+14*m]='\r';tmp[233+14*m]='\n';		
		}
		else if(plat.sensor[m]==0xa4)
		{
			for(j=0;j<5;j++)tmp[j+220+14*m]=t_so2_cn[j];//SO2
			if (data->so2_calib_value>=999.9) k=9999;
			else
			k=(Uint16)(data->so2_calib_value*10.0);
			k=HEX4BCD(k);
			i=k>>8&0x0f;i=i+0x30;tmp[225+14*m]=i;
			i=k>>4&0x0f;i=i+0x30;tmp[226+14*m]=i;
			tmp[227+14*m]='.';
			i=k&0x0f;i=i+0x30;tmp[228+14*m]=i;
			tmp[229+14*m]='P';tmp[230+14*m]='P';tmp[231+14*m]='M';
			tmp[232+14*m]='\r';tmp[233+14*m]='\n';
		}
		else if(plat.sensor[m]==0xa8)
		{
			for(j=0;j<4;j++)tmp[j+220+14*m]=t_hf_cn[j];//CO
			if (data->hf_calib_value>=999.9) k=9999;
			else
			k=(Uint16)(data->hf_calib_value*10.0);
			k=HEX4BCD(k);
			i=k>>12&0x0f;i=i+0x30;tmp[224+14*m]=i;
			i=k>>8&0x0f;i=i+0x30;tmp[225+14*m]=i;
			i=k>>4&0x0f;i=i+0x30;tmp[226+14*m]=i;
			tmp[227+14*m]='.';
			i=k&0x0f;i=i+0x30;tmp[228+14*m]=i;
			tmp[229+14*m]='P';tmp[230+14*m]='P';tmp[231+14*m]='M';
			tmp[232+14*m]='\r';tmp[233+14*m]='\n';
		}
	}
	i=fm22l16_rd(ADD_USB_NUM);
	i++;if(i>300)i=0;	
	fm22l16_wr(ADD_USB_NUM,i);	 
	usb_creat_file(i,"CA--",&tmp[0],220+14*plat.sensor_number);
    
	return 1;
}

void print_page(struct MEASURE_DATA *data)
{
	Uint16 i,j,g,m;
	
	scic_xmit(0x1b);//反向打印
	scic_xmit(0x63);
	scic_xmit(0x00);
	scic_xmit(0x1b);
	scic_xmit(0x31);
	scic_xmit(0x01);
    
	print_string_c("CA100测试报告:");//标题
	scic_xmit(0x0d);

	print_string_c("编号:");//编号
	for(j=0;j<4;j++)
	{
		g=data->number[j]+0x30;
		scic_xmit(g);
	}
	scic_xmit(0x0d);

	print_string_c("时间:");//时间
	
	scic_xmit(0x30+2);//年
	scic_xmit(0x30+0);
	g=data->clk.year;
	i=g/10;
	if (i>9) i=9;
	scic_xmit(0x30+i);
	i=g%10;
	scic_xmit(0x30+i);
	scic_xmit('.');

	g=data->clk.month;//月
	i=g/10;
	if (i>9) i=9;
	scic_xmit(0x30+i);
	i=g%10;
	scic_xmit(0x30+i);
	scic_xmit('.');

	g=data->clk.date;//日
	i=g/10;
	if (i>9) i=9;
	scic_xmit(0x30+i);
	i=g%10;
	scic_xmit(0x30+i);
	scic_xmit('.');

	scic_xmit(' ');

	g=data->clk.hour;//时
	i=g/10;
	if (i>9) i=9;
	scic_xmit(0x30+i);
	i=g%10;
	scic_xmit(0x30+i);
	scic_xmit(':');

	g=data->clk.minute;//分
	i=g/10;
	if (i>9) i=9;
	scic_xmit(0x30+i);
	i=g%10;
	scic_xmit(0x30+i);

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

	print_string_c("流量:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	print_string_c(flow_table[data->flw]);print_string_c("L/H");
	scic_xmit(0x0d);

	for(j=0;j<24;j++) scic_xmit(0x2e);
	scic_xmit(0x0d);
    
	print_string_c("测试结果:");
	scic_xmit(0x0d);
    
	print_string_c("露点:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	if(data->h2o_calib_value>=0.0){scic_xmit(0x2b);i=data->h2o_calib_value*10.0;}
	else {scic_xmit(0x2d);i=(-data->h2o_calib_value*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
	print_string_c("℃"); 
	scic_xmit(0x0d);
    
	print_string_c("湿度:");
	for(i=0;i<3;i++) scic_xmit(0x20);

	if(data->h2oppm>=6033.0)
	{
		print_string_c(">6033uL/L");
	}
	else if(data->h2oppm>=1000.0)
	{
		i=data->h2oppm;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm>=100.0)
	{
		i=data->h2oppm*10.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm>=10.0)
	{
		i=data->h2oppm*100.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm>=1.0)
	{
		i=data->h2oppm*1000.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm>=0.1)
	{
		i=data->h2oppm*10000.0;
		i=HEX4BCD(i);
		scic_xmit(0x20);
		scic_xmit(0x2e);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else 
	{
		i=data->h2oppm*100000.0;
		i=HEX4BCD(i);
		scic_xmit(0x20);
		scic_xmit(0x2e);
		scic_xmit(0x20);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}

	scic_xmit(0x0d);

	print_string_c("折算:");
	for(i=0;i<3;i++) scic_xmit(0x20);

	if(data->h2oppm>=6033.0)
	{
		print_string_c("******");
	}
	else if(data->h2oppm20>=1000.0)
	{
		i=data->h2oppm20;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm20>=100.0)
	{
		i=data->h2oppm20*10.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm20>=10.0)
	{
		i=data->h2oppm20*100.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm20>=1.0)
	{
		i=data->h2oppm20*1000.0;
		i=HEX4BCD(i);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		scic_xmit(0x2e);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else if(data->h2oppm20>=0.1)
	{
		i=data->h2oppm20*10000.0;
		i=HEX4BCD(i);
		scic_xmit(0x20);
		scic_xmit(0x2e);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	else 
	{
		i=data->h2oppm20*100000.0;
		i=HEX4BCD(i);
		scic_xmit(0x20);
		scic_xmit(0x2e);
		scic_xmit(0x20);
		j=i>>12&0x0f;scic_xmit(0x30+j);
		j=i>>8&0x0f;scic_xmit(0x30+j);
		j=i>>4&0x0f;scic_xmit(0x30+j);
		j=i&0x0f;scic_xmit(0x30+j);
		print_string_c("uL/L");
	}
	scic_xmit(0x0d);

	print_string_c("纯度:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	i=data->pur_calib_value*10.0;
    
	i=HEX4BCD(i);
	j=i>>12&0x0f;scic_xmit(0x30+j);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
	print_string_c("%"); 
	scic_xmit(0x0d);

	for(m=0;m<plat.sensor_number;m++)
	{
		if(plat.sensor[m]==0xa0)
		{
			print_string_c("H2S:");
			for(i=0;i<3;i++) scic_xmit(0x20);
			if(data->h2s_calib_value>=0.0)
			{
				scic_xmit(0x2b);
				if (data->h2s_calib_value>=999.9) i=9999;
				else i=data->h2s_calib_value*10.0;
			}
			else {scic_xmit(0x2d);i=(-data->h2s_calib_value*10.0);}
			i=HEX4BCD(i);
			j=i>>8&0x0f;scic_xmit(0x30+j);
			j=i>>4&0x0f;scic_xmit(0x30+j);
			scic_xmit(0x2e);
			j=i&0x0f;scic_xmit(0x30+j);
			print_string_c("ppm"); 
			scic_xmit(0x0d);
		}
		else if(plat.sensor[m]==0xa4)
		{
			print_string_c("SO2:");
			for(i=0;i<3;i++) scic_xmit(0x20);
			if(data->so2_calib_value>=0.0)
			{
				scic_xmit(0x2b);
				if (data->so2_calib_value>=999.9) i=9999;
				else
				i=data->so2_calib_value*10.0;
			}
			else {scic_xmit(0x2d);i=(-data->so2_calib_value*10.0);}
			i=HEX4BCD(i);
			j=i>>8&0x0f;scic_xmit(0x30+j);
			j=i>>4&0x0f;scic_xmit(0x30+j);
			scic_xmit(0x2e);
			j=i&0x0f;scic_xmit(0x30+j);
			print_string_c("ppm"); 
			scic_xmit(0x0d);
		}
		else if(plat.sensor[m]==0xa8)
		{
			print_string_c("CO :");
			for(i=0;i<2;i++) scic_xmit(0x20);
			if(data->hf_calib_value>=0.0)
			{
				scic_xmit(0x2b);
				if (data->hf_calib_value>=999.9) i=9999;
			 	else i=data->hf_calib_value*10.0;
			}
			else {scic_xmit(0x2d);i=(-data->hf_calib_value*10.0);}
			i=HEX4BCD(i);
			j=i>>12&0x0f;scic_xmit(0x30+j);
			j=i>>8&0x0f;scic_xmit(0x30+j);
			j=i>>4&0x0f;scic_xmit(0x30+j);
			scic_xmit(0x2e);
			j=i&0x0f;scic_xmit(0x30+j);
			print_string_c("ppm"); 
			scic_xmit(0x0d);
		}
	}

	for(j=0;j<24;j++) scic_xmit(0x2e);
	scic_xmit(0x0d);

	print_string_c("www.sindia.cn");
	for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);

}