#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//ʪ������ʵʱ��ʾ����
void ha100_do(Uint16 flag)
{
    Uint16 color; 
    float jh2o;Uint16 i,tem;

    if(flag==0)color=0xffff;
	else color=0xf800;
    

	fp32_ad_get(H2O);
    jh2o=ad8401_value[0]*(4.096/65536.0);
    jh2o=0.5*jh2o+0.5*h2oold;
    h2oold=jh2o;
    jh2o=25.0*jh2o-80.0;
	if(h2o_1.flag==0x5555)
	{
	    if(jh2o<=h2o.calib_dot1)h2o_calib_value=h2o.calib_k1[2]*jh2o*jh2o+h2o.calib_k1[1]*jh2o+h2o.calib_k1[0];
	    else h2o_calib_value=h2o.calib_k2[2]*jh2o*jh2o+h2o.calib_k2[1]*jh2o+h2o.calib_k2[0];
	}
	else h2o_calib_value=jh2o;
	//display_ascii(M24,0,0x0000,1,110,38,"            ");
    display_fp_ascii(M24,color,0x0000,1,110,38,h2o_calib_value,"��      "); 

	ha100_ppm_do(h2o_calib_value*10.0);
}

//ʪ�����ݻ�ͼ���� 1����/3��
void ha100_line(void)
{

}

//����������ﵽ240����ʱ��������������120���㣬��������ˢ�£�����120��������ݣ���ʼ���121��ʼ--
void ha100_redo(void)
{

}

//ʪ�����ݱ�����ѯ
//void ha100_search(void)
//{

//}
//����ɢ��ʪ�������ݴ��ڽṹ������У������������ݲ���������ʹ�ӡ��U�����
void ha100_data_get(struct TESTDATA *data)
{

}

//ʪ�����ݱ���
void ha100_store(void)
{

}

char *t_report_cn3={"HA100  ���鱨��: \r\n"};

Uint16 ha100_data_to_usb(struct MEASURE_DATA_0 *data)
{
    Uint16 tmp[240],j,i,k,m;

	i=usb_check();if(i!=1)return 0;

	for(j=0;j<19;j++)tmp[j]=t_report_cn3[j];
    for(j=0;j<20;j++)tmp[j+19]=t_dot_cn[j];
    for(j=0;j<11;j++)tmp[j+39]=t_parameter_cn[j];

    for(j=0;j<5;j++)tmp[j+50]=t_number_cn[j];
    for(j=0;j<4;j++)tmp[j+55]=data->number[j]+0x30;
    tmp[59]='\r';tmp[60]='\n'; //���


    for(j=0;j<5;j++)tmp[j+61]=t_data_cn[j];//����
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

	for(j=0;j<5;j++)tmp[j+78]=t_time_cn[j];//ʱ��
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

    for(j=0;j<5;j++)tmp[j+90]=t_temp_cn[j];//�¶�
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

    for(j=0;j<5;j++)tmp[j+103]=t_humid_cn[j];//ʪ��
	k=(Uint16)(data->c_humid*10.0);
	k=HEX4BCD(k);
	i=k>>8&0x0f;i=i+0x30;tmp[108]=i;
	i=k>>4&0x0f;i=i+0x30;tmp[109]=i;tmp[110]='.';
	i=k&0x0f;i=i+0x30;tmp[111]=i;tmp[112]='%';
	tmp[113]='\r';tmp[114]='\n';

    for(j=0;j<5;j++)tmp[j+115]=t_flow_cn[j];//����
	tmp[120]=*(flow_table1[data->in8]);
	tmp[121]=*(flow_table1[data->in8]+1);
    tmp[122]='L';tmp[123]='/';tmp[124]='h';
    tmp[125]='\r';tmp[126]='\n';

	for(j=0;j<20;j++)tmp[j+127]=t_dot_cn[j];
    for(j=0;j<11;j++)tmp[j+147]=t_result_cn[j];

    for(j=0;j<5;j++)tmp[j+158]=t_dew_cn[j];//¶��
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

    for(j=0;j<5;j++)tmp[j+171]=t_h2o_cn[j];//ˮ��
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
	for(j=0;j<5;j++)tmp[j+186]=t_h2o_cn[j];//ˮ��

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



   i=fm22l16_rd(ADD_USB_NUM);
   i++;if(i>300)i=0;	
   fm22l16_wr(ADD_USB_NUM,i);	 
   usb_creat_file(i,"HA--",&tmp[0],207);
    
   return 1;
}
void ha100_data_to_printer(struct MEASURE_DATA_0 *data)
{
    Uint16 i,j,g,m;
    int16 tt1;
    
    scic_xmit(0x1b);//�����ӡ
    scic_xmit(0x63);
    scic_xmit(0x00);

    scic_xmit(0x1b);
    scic_xmit(0x31);
    scic_xmit(0x01);
    
    print_string_c("HA100���Ա���:");//����
    scic_xmit(0x0d);

    print_string_c("���:");//���
    for(j=0;j<4;j++)
    {
        g=data->number[j]+0x30;
        scic_xmit(g);
    }
    scic_xmit(0x0d);

    print_string_c("ʱ��:");//ʱ��
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
    
    print_string_c("���Բ���:");//����������
 
    scic_xmit(0x0d);

	print_string_c("�¶�:");
	for(i=0;i<3;i++) scic_xmit(0x20);
    if(data->c_temp>=0.0){scic_xmit(0x2b);i=data->c_temp*10.0;}
	else {scic_xmit(0x2d);i=(-data->c_temp*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("��"); 
    for(i=0;i<1;i++) scic_xmit(0x20); 
    scic_xmit(0x0d);

	print_string_c("ʪ��:");
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
    print_string_c("����:");
    if (data->gas==0){print_string_c("SF6");}
    else if (data->gas==1){print_string_c("N2 ");}
    else if (data->gas==2){print_string_c("CO2");}
    for(i=0;i<4;i++) scic_xmit(0x20);
*/

    print_string_c("����:");
	for(i=0;i<3;i++) scic_xmit(0x20);
    print_string_c(flow_table1[data->in8]);print_string_c("L/H");
    scic_xmit(0x0d);

    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);
    
    print_string_c("���Խ��:");
    scic_xmit(0x0d);
    
    print_string_c("¶��:");
	for(i=0;i<3;i++) scic_xmit(0x20);
	if(data->h2o_calib_value>=0.0){scic_xmit(0x2b);i=data->h2o_calib_value*10.0;}
	else {scic_xmit(0x2d);i=(-data->h2o_calib_value*10.0);}
	i=HEX4BCD(i);
	j=i>>8&0x0f;scic_xmit(0x30+j);
	j=i>>4&0x0f;scic_xmit(0x30+j);
	scic_xmit(0x2e);
	j=i&0x0f;scic_xmit(0x30+j);
    print_string_c("��"); 
    scic_xmit(0x0d);
    
    print_string_c("ʪ��:");
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

    print_string_c("����:");
    for(i=0;i<3;i++) scic_xmit(0x20);

    if(data->h2oppm20>6033.0)
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

    for(j=0;j<24;j++) scic_xmit(0x2e);
    scic_xmit(0x0d);

//    print_string_c("***SINDIA Instrument***");
//    scic_xmit(0x0d);
    print_string_c("www.sindia.cn");
    for(i=0;i<10;i++) scic_xmit(0x0a);
	scic_xmit(0x0d);


}

Uint16 ha100_data_out(struct MEASURE_DATA_0 *data,Uint16 cnt)
{
    Uint32 j;
	
    color_change(0x001f,0x0000);u_key_cnt=0;cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    
	display_ascii(M16,0xffff,0x001f,1,5,2,"����");
    display_ascii(M16,0x07e0,0x0000,1,10,40,"��ȷ�����ݴ���ʽ");
    display_ascii(M16,0xffff,0x001f,1,0,222," ȡ��       ɾ��        ��ӡ        U��");
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
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"���ڴ�ӡ��ǰ���ݣ�");
		for(j=0;j<1000000;j++);
		ha100_data_to_printer(data);return cnt;
    }
    else if(keynew[3]==1)
    {
	    key_clr();
        display_ascii(M16,0x0fe0,0x0000,1,10,180,"���ڴ���ȫ�����ݣ�");
		for(j=0;j<1000000;j++);
		ha100_data_to_usb(data);return cnt;
    }
	else if(keynew[1]==1)
    {
	    key_clr();j=delete(cnt);return j;
    }
    return cnt;
}

void ha100_display_datalib(Uint16 x)
{
    Uint16 i,k[4];Uint32 number;
    char s[17];

    number=fm22l16_rd(ADD_CNT);
	if(number==0)return;
     
    display_num_ascii1(M16,0xffff,0x001f,1,220,2,number-x,"/");
    display_number(M16,0xffff,0x001f,1,270,2,number);
	display_ascii(M16,0xffff,0x001f,1,300,2,"ҳ");
    read_out_fram(x);

	k[0]=store_data.number[0]<<12 & 0xf000;//���
	k[1]=store_data.number[1]<<8 & 0x0f00;
	k[2]=store_data.number[2]<<4 & 0x00f0;
	k[3]=store_data.number[3] & 0x000f;
    k[0]=k[0]+k[1]+k[2]+k[3];i=BCD4HEX(k[0]);
	display_ascii(M16,0,0,1,53+8,51,"        ");
    display_number(M16,0xffff,0x0000,1,53+8,51,i);

    s[0]=0x30+2;//����
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


	i=store_data.clock[2]>>4&0x0f;//ʱ��
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

    display_fp_ascii(M16,0xffff,0x0000,1,53,129,store_data.c_temp,"��");//�¶Ⱥ�ʪ��
    display_fp_ascii(M16,0xffff,0x0000,1,53,129+26,store_data.c_humid,"%");//x<999.999

    display_ascii(M16,0xffff,0x0000,1,53+8,129+26+26,flow_table1[store_data.in8]);
    display_ascii(M16,0xffff,0x0000,1,53+24+8,129+26+26,"L/h");

	display_fp_ascii(M16,0xffff,0x0000,1,150+48,51,store_data.h2o_calib_value," ��");

	if(store_data.h2oppm<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+26,store_data.h2oppm,"uL/L");
	else display_ascii(M16,0xffff,0x0000,1,150+48,51+26,">6033uL/L");

    if(store_data.h2oppm20<6033.0)display_fp4(M16,0xffff,0x0000,1,150+48,51+52,store_data.h2oppm20,"uL/L,20C");
    else display_ascii(M16,0xffff,0x0000,1,150+48,51+52,">6033uL/L");
} 

void ha100_search(void)
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
    display_ascii(M16,0xffff,0x001f,1,5,2,"��ѯ");
	display_ascii(M16,0xffff,0x001f,1,0,222," �˳�       ��ҳ        ��ҳ       ����");

	display_ascii(M16,0xFFE0,0x0000,1,5,25,"��������");

    display_ascii(M16,0x07e0,0x0000,1,5,25+26,"���:");
    display_ascii(M16,0x07e0,0x0000,1,5,25+52,"����:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+78,"ʱ��:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+104,"�¶�:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+130,"ʪ��:");  
    display_ascii(M16,0x07e0,0x0000,1,5,25+156,"����:");  

	display_ascii(M16,0xFFE0,0x0000,1,150,25,"�������");

    display_ascii(M16,0x07e0,0x0000,1,150,25+26,"¶��:");
    display_ascii(M16,0x07e0,0x0000,1,150,25+52,"΢ˮ:");  
    display_ascii(M16,0x07e0,0x0000,1,150,25+78,"΢ˮ:");  
//    display_ascii(M16,0x07e0,0x0000,1,150,25+104,"SO2 :");  
//    display_ascii(M16,0x07e0,0x0000,1,150,25+130,"H2S :");  

    for(h=0;h<200000;h++);
    key_clr();
    ha100_display_datalib(keycnt);

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
			    keycnt=ha100_data_out(&store_data,keycnt);goto search01;
            }
            key_clr();
            ha100_display_datalib(keycnt);
          }  
    }
   
}









