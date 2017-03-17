#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

struct MEASURE_DATA_0 store_data;
struct FM_PARAMETER parameter;

union _UnFloat
 {
  Uint16 byData[2];
  float fData;
 }unFloat;

void fm22l16_wr(Uint32 address,Uint16 data)
{
    *((Uint16 *)(0x00200000+address))=data;
}

Uint16 fm22l16_rd(Uint32 address)
{
     Uint16 i;
     i=*((Uint16 *)(0x00200000+address)); 
     return i; 
}

void fm22l16_float_wr(Uint32 address,float data)
{
	unFloat.fData = data;
	*((Uint16 *)(0x00200000+address))   = unFloat.byData[0];
	*((Uint16 *)(0x00200000+address+1)) = unFloat.byData[1];
}

float fm22l16_float_rd(Uint32 address)
{
	unFloat.byData[0] = *((Uint16 *)(0x00200000+address));
	unFloat.byData[1] = *((Uint16 *)(0x00200000+address+1));
    return unFloat.fData ;
}

void fm22l16_wrs(Uint32 address,Uint16 *source,Uint16 number)
{
    Uint16 m=address,i;
    for (i=0;i<number;i++)
    {
        fm22l16_wr(m,*source);
        m++;
        source++;
    }

}

void fm22l16_rds(Uint32 address,Uint16 *destination,Uint16 number)
{
    Uint16 m=address,i;
    for (i=0;i<number;i++)
    {
        *destination=fm22l16_rd(m);
        m++;
        destination++;
    }
}		 

void fm22l16_float_wrs(Uint32 address,float *source,Uint16 number)
{
    Uint16 m=address,i;
    for (i=0;i<number;i++)
    {
        fm22l16_float_wr(m,*source);
        m = m + 2 ;
        source++;
    }

}

void fm22l16_float_rds(Uint32 address,float *destination,Uint16 number)
{
    Uint16 m=address,i;
    for (i=0;i<number;i++)
    {
        *destination=fm22l16_float_rd(m);
        m = m + 2 ;
        destination++;
    }
}
/************************************************************************************
存储方式：
1.  从地址0开始存储数据，如NUMBER＝300，那么数存储在第299个位置
2.  存满300个数后，将最前面的数据覆盖，后面的数据依次前移一位，最后的数据存储最新的


************************************************************************************/
Uint16 const colortab[4][4]=
{
0x8410,0x0000,0x0000,0x0000,
0x0000,0x8410,0x0000,0x0000,
0x0000,0x0000,0x8410,0x0000,
0x0000,0x0000,0x0000,0x8410,
}; 

void display_1(Uint16 x)
{
    display_number(M64,0xffff,colortab[x][0],1,60+36,80,store_data.number[0]);
	display_number(M64,0xffff,colortab[x][1],1,60+36+36,80,store_data.number[1]);
	display_number(M64,0xffff,colortab[x][2],1,60+36+36+36,80,store_data.number[2]);
	display_number(M64,0xffff,colortab[x][3],1,60+36+72+36,80,store_data.number[3]);

}

void store_0(void)
{
    Uint16 temp,*pt1,*pt2;Uint32 k1,k2,i,j;

	
    color_change(0x001f,0x0000);cls();u_key_cnt=0;
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);

    for(j=0;j<4;j++)store_data.number[j]=0;
	display_ascii(M16,0xffff,0x001f,1,5,2,"存储");
    display_ascii(M16,0x07e0,0x0000,1,10,40,"请输入试验编号：");
    display_ascii(M16,0xffff,0x001f,1,0,222," 取消       移位        修改       确认");
	i=0;display_1(0);key_clr();km=1;
	while(keynew[3]==0 && keynew[0]==0)
    {
	    u_key_draw();
        if(keynew[1]==1)
		{
		    key_clr();
		    i++;if(i>3)i=0;display_1(i);
		}
		else if(keynew[2]==1)
		{
		    key_clr();
		    store_data.number[i]++;
		    if(store_data.number[i]>9)store_data.number[i]=0;display_1(i);
		}
    }
    if(keynew[0]==1)
    {
	    key_clr();
        display_ascii(M16,0xf800,0x0000,1,10,180,"数据没有保存！");
		for(j=0;j<1000000;j++);
        return;
    }
	if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在保存数据！");
		for(j=0;j<1000000;j++);
    }
    for(j=0;j<4;j++)hold_data.number[j]=store_data.number[j];
	temp=fm22l16_rd(ADD_CNT);
	if(temp<300)temp++;
	fm22l16_wr(ADD_CNT,temp);
	store_number=temp;

    if(hold_flag==1)
	{
	    pt1=(Uint16 *)&store_data;
		pt2=(Uint16 *)&hold_data;
		for(i=0;i<47;i++)
       {
            *pt1=*pt2;
            pt2++;
            pt1++;
        }
	}

	if(temp<300)
	{
	    pt1=(Uint16 *)&store_data;
		k1=0;
        for(j=0;j<47;j++)
        {
	        k1=ADD_BASE+ADD_TAB*(temp-1)+j;
            fm22l16_wr(k1,*pt1);
            pt1++;
	    }
    }
	else
	{
	    for(j=1;j<300;j++)
        {
		    k2=ADD_BASE+ADD_TAB*j;
			k1=ADD_BASE+ADD_TAB*(j-1);
		    for(i=0;i<47;i++)
			{
	            temp=fm22l16_rd(k2+i);
                fm22l16_wr(k1+i,temp);
            }
	    }
        pt1=(Uint16 *)&store_data;
		k1=0;
        for(j=0;j<47;j++)
        {
	        k1=ADD_BASE+ADD_TAB*(temp-1)+j;
            fm22l16_wr(k1,*pt1);
            pt1++;
	    }
	}

}

/************************************************************************************
删除方式：
1.  删除当前数据，后面的数据前移一位，总数减一//mode=0;
2.  全部删除//mode=1;
3.  x为从0到299的数
4.  返回值为当前进入时的查询页数
************************************************************************************/

Uint16 delete(Uint16 x)
{
    Uint16 temp,number,mode=0;
    Uint32 k1,k2,i,j;
  
    number=fm22l16_rd(ADD_CNT);
	if(number<=1){fm22l16_wr(ADD_CNT,0);return 0;}
    if(x>number-1)return 0;

	
    color_change(0x001f,0x0000);u_key_cnt=0;cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);

    display_ascii(M16,0xffff,0x001f,1,5,2,"删除");
    display_ascii(M16,0x07e0,0x0000,1,10,40,"请确认删除对象");
    display_ascii(M16,0xffff,0x001f,1,0,222," 取消                   当前       全部");
	i=0;key_clr();km=1;
	while(keynew[0]==0 && keynew[2]==0 && keynew[3]==0)
		{
			if (keynew[1]) key_clr();
		u_key_draw();
		};
    
    if(keynew[0]==1)
    {
	    key_clr();
        return x;
    }
	if(keynew[2]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在删除当前数据！");mode=0;
		for(j=0;j<1000000;j++);
    }
    if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在删除全部数据！");mode=1;
		for(j=0;j<1000000;j++);
    }



    if(mode==0)
	{
	    for(j=x;j<(number-1);j++)
        {
		    k2=ADD_BASE+ADD_TAB*j;
			k1=ADD_BASE+ADD_TAB*(j+1);
		    for(i=0;i<47;i++)
			{
	            temp=fm22l16_rd(k1+i);
                fm22l16_wr(k2+i,temp);
            }
	    }
		number--;fm22l16_wr(ADD_CNT,number);
		if(x==number)return (x-1);
		else return x;
	}
	else
	{
	    fm22l16_wr(ADD_CNT,0);return 0;
	}
}
char *t_report_cn ={"DPA1000试验报告: \r\n"};
char *t_report_cn1={"CA100  试验报告: \r\n"};
char *t_report_cn2={"DA100  试验报告: \r\n"};
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
char *t_hf_cn= {"CO :"};
char *t_gpa_cn={"纯度:"};
char *t_dot_cn={"------------------\r\n"};


//成功---1,失败---0
Uint16 data_to_usb(struct MEASURE_DATA *data)
{
    Uint16 tmp[240],j,i,k;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++)tmp[j]=t_report_cn[j];
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

    for(j=0;j<5;j++)tmp[j+158]=t_dew_cn[j];//露点
    if(data->h2o_calib_value>=0){k=(Uint16)(data->h2o_calib_value*10.0);tmp[163]='+';}
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

	for(j=0;j<5;j++)tmp[j+207]=t_so2_cn[j];//SO2
    k=(Uint16)(data->so2_calib_value*10.0);
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[212]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[213]=i;
	tmp[214]='.';
	i=k&0x0f;i=i+0x30;tmp[215]=i;
	tmp[216]='P';tmp[217]='P';tmp[218]='M';
	tmp[219]='\r';tmp[220]='\n';

	for(j=0;j<5;j++)tmp[j+221]=t_h2s_cn[j];//SO2
    k=(Uint16)(data->h2s_calib_value*10.0);
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[226]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[227]=i;
	tmp[228]='.';
	i=k&0x0f;i=i+0x30;tmp[229]=i;
	tmp[230]='P';tmp[231]='P';tmp[232]='M';
	tmp[233]='\r';tmp[234]='\n';

    i=fm22l16_rd(ADD_USB_NUM);
    i++;if(i>300)i=0;	
    fm22l16_wr(ADD_USB_NUM,i);	 
	usb_creat_file(i,"DPA_",&tmp[0],235);
    
	return 1;
}
void data_to_printer(struct MEASURE_DATA *data)
{
    Uint16 i,j,g;
//    int16 tt1;
    
    scic_xmit(0x1b);//反向打印
    scic_xmit(0x63);
    scic_xmit(0x00);

    scic_xmit(0x1b);
    scic_xmit(0x31);
    scic_xmit(0x01);
    
    print_string_c("DPA1000测试报告:");//标题
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

    if(data->h2oppm20>=6033.0)
	{
	    print_string_c(">6033uL/L");
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

    print_string_c("SO2:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	if(data->so2_calib_value>=0.0){scic_xmit(0x2b);i=data->so2_calib_value*10.0;}
	else {scic_xmit(0x2d);i=(-data->so2_calib_value*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("ppm"); 
    scic_xmit(0x0d);

	print_string_c("H2S:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	if(data->h2s_calib_value>=0.0){scic_xmit(0x2b);i=data->h2s_calib_value*10.0;}
	else {scic_xmit(0x2d);i=(-data->h2s_calib_value*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("ppm"); 
    scic_xmit(0x0d);

    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);

//    print_string_c("***SINDIA Instrument***");
//    scic_xmit(0x0d);
    print_string_c("www.sindia.cn");
    for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);


}


Uint16 data_to_usb_0(struct MEASURE_DATA_0 *data)
{
    Uint16 tmp[300],j,i,k,m,ca_v2;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++)tmp[j]=t_report_cn1[j];
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

	if (sub_ver!=1)
	{
	    for(j=0;j<5;j++)tmp[j+158]=t_dew_cn[j];//露点
	    if(data->h2o_calib_value>=0){k=(Uint16)(data->h2o_calib_value*10.0);tmp[163]='+';}
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
	  ca_v2=0;
  }
  else if(sub_ver==1)
  {
      ca_v2=49;
	  //tmp[158]='\r';tmp[159]='\n';
  }

  for(j=0;j<5;j++)tmp[j+207-ca_v2]=t_gpa_cn[j];//纯度

  k=(Uint16)(data->gpa_calib_value*10.0);
  k=HEX4BCD(k);
  i=k>>12&0x0f;i=i+0x30;tmp[212-ca_v2]=i;
  i=k>>8&0x0f;i=i+0x30;tmp[213-ca_v2]=i;
  i=k>>4&0x0f;i=i+0x30;tmp[214-ca_v2]=i;
  tmp[215-ca_v2]='.';
  i=k&0x0f;i=i+0x30;tmp[216-ca_v2]=i;
  tmp[217-ca_v2]='%';
  tmp[218-ca_v2]='\r';tmp[219-ca_v2]='\n';

  for(m=0;m<sensor_number;m++)
  {
     if(sensor[m]==0xa0)
     {
	    for(j=0;j<5;j++)tmp[j+220+14*m-ca_v2]=t_h2s_cn[j];//H2S
		if (data->h2s_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->h2s_calib_value*10.0);
		k=HEX4BCD(k);
		i=k>>8&0x0f;i=i+0x30;tmp[225+14*m-ca_v2]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[226+14*m-ca_v2]=i;
		tmp[227+14*m-ca_v2]='.';
		i=k&0x0f;i=i+0x30;tmp[228+14*m-ca_v2]=i;
		tmp[229+14*m-ca_v2]='P';tmp[230+14*m-ca_v2]='P';tmp[231+14*m-ca_v2]='M';
		tmp[232+14*m-ca_v2]='\r';tmp[233+14*m-ca_v2]='\n';
		
     }
     else if(sensor[m]==0xa4)
     {
		for(j=0;j<5;j++)tmp[j+220+14*m-ca_v2]=t_so2_cn[j];//SO2
		if (data->so2_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->so2_calib_value*10.0);
		k=HEX4BCD(k);
		i=k>>8&0x0f;i=i+0x30;tmp[225+14*m-ca_v2]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[226+14*m-ca_v2]=i;
		tmp[227+14*m-ca_v2]='.';
		i=k&0x0f;i=i+0x30;tmp[228+14*m-ca_v2]=i;
		tmp[229+14*m-ca_v2]='P';tmp[230+14*m-ca_v2]='P';tmp[231+14*m-ca_v2]='M';
		tmp[232+14*m-ca_v2]='\r';tmp[233+14*m-ca_v2]='\n';
      }
    else if(sensor[m]==0xa8)
    {
		for(j=0;j<4;j++)tmp[j+220+14*m-ca_v2]=t_hf_cn[j];//CO
		if (data->hf_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->hf_calib_value*10.0);
		k=HEX4BCD(k);
        i=k>>12&0x0f;i=i+0x30;tmp[224+14*m-ca_v2]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[225+14*m-ca_v2]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[226+14*m-ca_v2]=i;
		tmp[227+14*m-ca_v2]='.';
		i=k&0x0f;i=i+0x30;tmp[228+14*m-ca_v2]=i;
		tmp[229+14*m-ca_v2]='P';tmp[230+14*m-ca_v2]='P';tmp[231+14*m-ca_v2]='M';
		tmp[232+14*m-ca_v2]='\r';tmp[233+14*m-ca_v2]='\n';
     }
   }
   i=fm22l16_rd(ADD_USB_NUM);
   i++;if(i>300)i=0;	
   fm22l16_wr(ADD_USB_NUM,i);	 
   	usb_creat_file(i,"CA--",&tmp[0],220+14*sensor_number-ca_v2);
    
   return 1;
}
void data_to_printer_0(struct MEASURE_DATA_0 *data)
{
    Uint16 i,j,g,m;
    int16 tt1;
    
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

	if (sub_ver!=1)
	{
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
	}

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

	for(m=0;m<sensor_number;m++)
	{
	  if(sensor[m]==0xa0)
	  {
          print_string_c("H2S:");
          for(i=0;i<3;i++) scic_xmit(0x20);
    	  if(data->h2s_calib_value>=0.0)
    	  {
	    	  	scic_xmit(0x2b);
				if (data->h2s_calib_value>=999.9) i=9999;
				else
	    	  	i=data->h2s_calib_value*10.0;
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
	  else if(sensor[m]==0xa4)
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
	  else if(sensor[m]==0xa8)
	  {
          print_string_c("CO :");
          for(i=0;i<2;i++) scic_xmit(0x20);
    	  if(data->hf_calib_value>=0.0)
    	  {
    	  		scic_xmit(0x2b);
				if (data->hf_calib_value>=999.9) i=9999;
				else
    	  		i=data->hf_calib_value*10.0;
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

//    print_string_c("***SINDIA Instrument***");
//    scic_xmit(0x0d);
    print_string_c("www.sindia.cn");
    for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);


}
//打印或U盘数据输出
Uint16 data_out(struct MEASURE_DATA_0 *data,Uint16 cnt)
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
		data_to_printer_0(data);return cnt;
    }
    else if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在传输全部数据！");
		for(j=0;j<1000000;j++);
		data_to_usb_0(data);return cnt;
    }
	else if(keynew[1]==1)
    {
	    key_clr();j=delete(cnt);return j;
    }
    return cnt;
}
Uint16 data_out_0(struct MEASURE_DATA_0 *data,Uint16 cnt)
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
		data_to_printer_0(data);return cnt;
    }
    else if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在传输全部数据！");
		for(j=0;j<1000000;j++);
		data_to_usb_0(data);return cnt;
    }
	else if(keynew[1]==1)
    {
	    key_clr();j=delete(cnt);return j;
    }
    return cnt;
}

void read_out_fram(Uint32 x)
{
    Uint32 j,k;Uint16 *pt;

    pt=(Uint16 *)&store_data;
    k=0;
    for(j=0;j<47;j++)
    {
	    k=ADD_BASE+ADD_TAB*(x)+j;
        *pt=fm22l16_rd(k);
        pt++;
    }
}

void display_datalib(Uint16 x)
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

    display_fp_ascii(M16,0xffff,0x0000,1,150+48,51,store_data.h2o_calib_value," ℃");

    if(store_data.h2oppm<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+26,store_data.h2oppm,"uL/L");
	else display_ascii(M16,0xffff,0x0000,1,150+48,51+26,">6033uL/L");

    if(store_data.h2oppm20<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+52,store_data.h2oppm20,"uL/L,20C");
    else display_ascii(M16,0xffff,0x0000,1,150+48,51+52,">6033uL/L");

    display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+78,store_data.so2_calib_value,"PPM");
    display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+104,store_data.h2s_calib_value,"PPM");


} 


void search(void)
{
    Uint16 keycnt,i;Uint32 number,h;

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	keycnt=number-1;	

search1:
    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	
    color_change(0x001f,0x0000); cls();km=1;u_key_cnt=0;
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

    display_ascii(M16,0x07e0,0x0000,1,150,25+26,"露点:");
    display_ascii(M16,0x07e0,0x0000,1,150,25+52,"微水:");  
    display_ascii(M16,0x07e0,0x0000,1,150,25+78,"微水:");  
    display_ascii(M16,0x07e0,0x0000,1,150,25+104,"SO2 :");  
    display_ascii(M16,0x07e0,0x0000,1,150,25+130,"H2S :");  

    for(h=0;h<200000;h++);
    key_clr();
    display_datalib(keycnt);

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
			    keycnt=data_out(&store_data,keycnt);
				for(h=0;h<200000;h++);
			    goto search1;
            }
            key_clr();
            display_datalib(keycnt);
          }  
    }
   
}

void display_datalib_0(Uint16 x)
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



	if (sub_ver==0)
	{
	    display_fp_ascii(M16,0xffff,0x0000,1,150+48,51,store_data.h2o_calib_value," ℃");
    	display_ascii(M16,0xffff,0x0000,1,150+48,51+26,"               ");
	    if(store_data.h2oppm<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+26,store_data.h2oppm,"uL/L");
		else display_ascii(M16,0xffff,0x0000,1,150+48,51+26,">6033uL/L");
	    display_ascii(M16,0xffff,0x0000,1,150+48,51+52,"               ");
	    if(store_data.h2oppm20<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+52,store_data.h2oppm20,"uL/L,20C");
	    else display_ascii(M16,0xffff,0x0000,1,150+48,51+52,"*********");
	}
	if  (sub_ver==1) display_gpa(M16,0xffff,0x0000,1,150+48,51+26,store_data.gpa_calib_value," %");
	if  (sub_ver==0) display_gpa(M16,0xffff,0x0000,1,150+48,51+78,store_data.gpa_calib_value," %");

	for(i=0;i<sensor_number;i++)
	{
        if(store_data.sensor[i]==0xa0)
        {
			if (sub_ver==1)
			{
				display_ascii(M16,0x07e0,0x0000,1,150,51+26+i*26+26,"H2S :");  
	            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+26+i*26+26,store_data.h2s_calib_value,"PPM");
			}
			if (sub_ver==0)
			{
			    display_ascii(M16,0x07e0,0x0000,1,150,51+78+i*26+26,"H2S :");  
	            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+78+i*26+26,store_data.h2s_calib_value,"PPM");
			}
        }
        else if(store_data.sensor[i]==0xa4)
        {
			if (sub_ver==1)
			{
			    display_ascii(M16,0x07e0,0x0000,1,150,51+26+i*26+26,"SO2 :");  
	            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+26+i*26+26,store_data.so2_calib_value,"PPM");
			}
			if (sub_ver==0)
			{
			    display_ascii(M16,0x07e0,0x0000,1,150,51+78+i*26+26,"SO2 :");  
	            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+78+i*26+26,store_data.so2_calib_value,"PPM");
			}
        }
		else if(store_data.sensor[i]==0xa8)
        {
			if (sub_ver==1)
			{
			    display_ascii(M16,0x07e0,0x0000,1,150,51+26+i*26+26,"CO  :");  
	            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+26+i*26+26,store_data.hf_calib_value,"PPM");
			}
        }
	}

} 

void search_0(void)
{
    Uint16 keycnt,i;Uint32 number,h;

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	keycnt=number-1;	

search1:
    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	
    color_change(0x001f,0x0000); cls();km=1;u_key_cnt=0;
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

	if (sub_ver==1)
	{
   	 	display_ascii(M16,0x07e0,0x0000,1,150,25+52,"纯度:");  
	}
	if (sub_ver==0)
	{
	    display_ascii(M16,0x07e0,0x0000,1,150,25+26,"露点:");
		display_ascii(M16,0x07e0,0x0000,1,150,25+52,"微水:");  
	    display_ascii(M16,0x07e0,0x0000,1,150,25+78,"微水:");  
	    display_ascii(M16,0x07e0,0x0000,1,150,25+104,"纯度:");  
	}


    for(h=0;h<200000;h++);
    key_clr();
    display_datalib_0(keycnt);

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
			    keycnt=data_out_0(&store_data,keycnt);
				for(h=0;h<200000;h++);
			    goto search1;
            }
            key_clr();
            display_datalib_0(keycnt);
          }  
    }
   
}
Uint16 data_to_usb_1(struct MEASURE_DATA_0 *data)
{
    Uint16 tmp[300],j,i,k,m;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++)tmp[j]=t_report_cn2[j];
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



  for(m=0;m<sensor_number;m++)
  {
     if(sensor[m]==0xa0)
     {
	    for(j=0;j<5;j++)tmp[j+158+14*m]=t_h2s_cn[j];//H2S
		if (data->h2s_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->h2s_calib_value*10.0);
		k=HEX4BCD(k);
		i=k>>12&0x0f;i=i+0x30;tmp[162+14*m]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[163+14*m]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[164+14*m]=i;
		tmp[165+14*m]='.';
		i=k&0x0f;i=i+0x30;tmp[166+14*m]=i;
		tmp[167+14*m]='P';tmp[168+14*m]='P';tmp[169+14*m]='M';
		tmp[170+14*m]='\r';tmp[171+14*m]='\n';
     }
     else if(sensor[m]==0xa4)
     {
	    for(j=0;j<5;j++)tmp[j+158+14*m]=t_so2_cn[j];//H2S
		if (data->so2_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->so2_calib_value*10.0);
		k=HEX4BCD(k);
		i=k>>12&0x0f;i=i+0x30;tmp[162+14*m]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[163+14*m]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[164+14*m]=i;
		tmp[165+14*m]='.';
		i=k&0x0f;i=i+0x30;tmp[166+14*m]=i;
		tmp[167+14*m]='P';tmp[168+14*m]='P';tmp[169+14*m]='M';
		tmp[170+14*m]='\r';tmp[171+14*m]='\n';

      }
    else if(sensor[m]==0xa8)
    {
        for(j=0;j<4;j++)tmp[j+158+14*m]=t_hf_cn[j];//CO
		if (data->hf_calib_value>=999.9) k=9999;
		else
 	    k=(Uint16)(data->hf_calib_value*10.0);
		k=HEX4BCD(k);
        i=k>>12&0x0f;i=i+0x30;tmp[162+14*m]=i;
		i=k>>8&0x0f;i=i+0x30;tmp[163+14*m]=i;
		i=k>>4&0x0f;i=i+0x30;tmp[164+14*m]=i;
		tmp[165+14*m]='.';
		i=k&0x0f;i=i+0x30;tmp[166+14*m]=i;
		tmp[167+14*m]='P';tmp[168+14*m]='P';tmp[169+14*m]='M';
		tmp[170+14*m]='\r';tmp[171+14*m]='\n';

     }
   }
   i=fm22l16_rd(ADD_USB_NUM);
   i++;if(i>300)i=0;	
   fm22l16_wr(ADD_USB_NUM,i);	 
   usb_creat_file(i,"DA--",&tmp[0],158+14*sensor_number);
    
   return 1;
}
void data_to_printer_1(struct MEASURE_DATA_0 *data)
{
    Uint16 i,j,g,m;
    int16 tt1;
    
    scic_xmit(0x1b);//反向打印
    scic_xmit(0x63);
    scic_xmit(0x00);

    scic_xmit(0x1b);
    scic_xmit(0x31);
    scic_xmit(0x01);
    
    print_string_c("DA100测试报告:");//标题
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
	for(i=0;i<3;i++)
	{
		scic_xmit(0x20);
	}
    print_string_c(flow_table1[data->in8]);
    print_string_c("L/H");
    scic_xmit(0x0d);

    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);
    
    print_string_c("测试结果:");
    scic_xmit(0x0d);
/*    
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

    if(data->h2oppm==0xffff)
	{
	    print_string_c(">6039uL/L");
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

    if(data->h2oppm20==0xffff)
	{
	    print_string_c(">6039uL/L");
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
*/
	for(m=0;m<sensor_number;m++)
	{
	  if(sensor[m]==0xa0)
	  {
          print_string_c("H2S:");
          for(i=0;i<3;i++) scic_xmit(0x20);
    	  if(data->h2s_calib_value>=0.0)
		  {
		  	scic_xmit(0x2b);
			if (data->h2s_calib_value>=999.9) i=9999;
			else
			i=data->h2s_calib_value*10.0;	
		  }
    	  else 
		  {
		  	scic_xmit(0x2d);
			i=(-data->h2s_calib_value*10.0);
		  }
          i=HEX4BCD(i);
		  j=i>>12&0x0f;scic_xmit(0x30+j);
    	  j=i>>8&0x0f;scic_xmit(0x30+j);
    	  j=i>>4&0x0f;scic_xmit(0x30+j);
    	  scic_xmit(0x2e);
    	  j=i&0x0f;scic_xmit(0x30+j);
          print_string_c("ppm"); 
          scic_xmit(0x0d);
	  }
	  else if(sensor[m]==0xa4)
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
		  j=i>>12&0x0f;scic_xmit(0x30+j);
    	  j=i>>8&0x0f;scic_xmit(0x30+j);
    	  j=i>>4&0x0f;scic_xmit(0x30+j);
    	  scic_xmit(0x2e);
    	  j=i&0x0f;scic_xmit(0x30+j);
          print_string_c("ppm"); 
          scic_xmit(0x0d);
	  }
	  else if(sensor[m]==0xa8)
	  {
          print_string_c("CO :");
          for(i=0;i<2;i++) scic_xmit(0x20);
    	  if(data->hf_calib_value>=0.0)
    	  {
    	  	scic_xmit(0x2b);
			if (data->hf_calib_value>=999.9) i=9999;
			else
    	  	i=data->hf_calib_value*10.0;
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

//    print_string_c("***SINDIA Instrument***");
//    scic_xmit(0x0d);
    print_string_c("www.sindia.cn");
    for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);


}


Uint16 data_out_1(struct MEASURE_DATA_0 *data,Uint16 cnt)//数据输出，优盘或打印机
{
    Uint32 j;
	
    color_change(0x001f,0x0000);
    u_key_cnt=0;
    cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    
	display_ascii(M16,0xffff,0x001f,1,5,2,"处理");
    display_ascii(M16,0x07e0,0x0000,1,10,40,"请确认数据处理方式");
    display_ascii(M16,0xffff,0x001f,1,0,222," 取消       删除        打印        U盘");
    key_clr();
    km=1;
	while(keynew[0]==0 && keynew[2]==0 && keynew[3]==0&& keynew[1]==0)
	{
		u_key_draw();
	}

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
		data_to_printer_1(data);return cnt;
    }
    else if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"正在传输全部数据！");
		for(j=0;j<1000000;j++);
		data_to_usb_1(data);return cnt;
    }
	else if(keynew[1]==1)
    {
	    key_clr();j=delete(cnt);return j;
    }
    return cnt;
}

void display_datalib_1(Uint16 x)//显示传感器数据
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


	for(i=0;i<sensor_number;i++)
	{
        if(store_data.sensor[i]==0xa0)
        {
		    display_ascii(M16,0x07e0,0x0000,1,150,51+i*26,"H2S :");  
            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+i*26,store_data.h2s_calib_value,"PPM");
        }
        else if(store_data.sensor[i]==0xa4)
        {
		    display_ascii(M16,0x07e0,0x0000,1,150,51+i*26,"SO2 :");  
            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+i*26,store_data.so2_calib_value,"PPM");
        }
		else if(store_data.sensor[i]==0xa8)
        {
		    display_ascii(M16,0x07e0,0x0000,1,150,51+i*26,"CO  :");  
            display_fp_ascii_ppm1(M16,0xffff,0x0000,1,150+48,51+i*26,store_data.hf_calib_value,"PPM");
        }
	}

} 

void search_1(void)
{
    Uint16 keycnt,i;Uint32 number,h;

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
	keycnt=number-1;	

search1:
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

//    display_ascii(M16,0x07e0,0x0000,1,150,25+26,"露点:");
//    display_ascii(M16,0x07e0,0x0000,1,150,25+52,"微水:");  
//    display_ascii(M16,0x07e0,0x0000,1,150,25+78,"微水:");  
//    display_ascii(M16,0x07e0,0x0000,1,150,25+104,"SO2 :");  
//    display_ascii(M16,0x07e0,0x0000,1,150,25+130,"H2S :");  

    for(h=0;h<200000;h++);
    key_clr();
    display_datalib_1(keycnt);

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
			    keycnt=data_out_1(&store_data,keycnt);
				for(h=0;h<200000;h++);
			    goto search1;
            }
            key_clr();
            display_datalib_1(keycnt);
			i++;
          }  
    }
   
}
































