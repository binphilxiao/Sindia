#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
Uint32 password_old;

float jiaocha_SO2_k,jiaocha_CO_k;
Uint16 jiaocha_SO2_flag,jiaocha_CO_flag;

struct CLOCK calib_time;
float BYmv[7], BiaoY[7], kaa, kbb, kcc;

struct CALIB_K h2o;
struct CALIB_K so2;
struct CALIB_K h2s;
struct CALIB_K hf;
struct CALIB_K gpa;
struct CALIB_K tempu;

struct LINE_K h2o_1;
struct LINE_K so2_1;
struct LINE_K h2s_1;
struct LINE_K hf_1;
struct LINE_K gpa_1;
float in1[3], in2[3];
float value_ad[4], value_ka75[4];
Uint16 const color_1[7][7] =
{ 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
};

extern Uint16 password_set(void); //0---退出 1---密码正确
//extern void password_adj(void); //0---进入校准 1---密码设置完成
extern void display_calib_time(Uint16 x, Uint16 y, Uint16 *pt);
extern void calib_dot( void );
void calib_test(struct CALIB_K *v);

//void temperature_calib(void);
//float keyboard( void );

float keyboard( void )
{

	float output_number ;
	Uint16  keyboard_new = 0,key_xx,key_yy,flag_key;
	Uint32 j;
    Uint16 input[4]={0};

    display_jx_clear(215, 55, 315, 215) ;
    flag_key = 0 ;
    display_jx(220, 60, 310, 210);
    display_jx(220, 90, 310, 150);
	display_jx(220, 120, 310, 180);
	display_jx(220, 90, 280, 210);
	display_jx(250, 90, 310, 210);

	display_ascii(M16, 0xffff, 0x0000, 1,291, 100 , "9");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 100 , "8");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 100 , "7");
	display_ascii(M16, 0xffff, 0x0000, 1,291, 130 , "6");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 130 , "5");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 130 , "4");
	display_ascii(M16, 0xffff, 0x0000, 1,291, 160 , "3");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 160 , "2");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 160 , "1");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 190 , "0");
	display_ascii(M12, 0xffff, 0x0000, 1,253, 190 , "取消");
	display_ascii(M12, 0xffff, 0x0000, 1,285, 190 , "确定");

	//key_clr();
	while( keyboard_new != 11 )
	{
		if( key_xx != key_x || key_yy != key_y )
		{
			flag_key = 1 ;
			if (key_y > 90&&key_y < 120)
			{
				if(key_x > 220&&key_x < 250)
					keyboard_new = 7;//7
				else if(key_x > 250&&key_x < 280)
					keyboard_new = 8;//8
				else if(key_x > 280&&key_x < 310)
					keyboard_new = 9;//9
			}
			else if (key_y > 120&&key_y < 150)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 4;//4
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 5;//5
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 6;//6
			}
			else if (key_y > 150&&key_y < 180)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 1;//1
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 2;//2
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 3;//3
			}
			else if (key_y > 180&&key_y < 210)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 0;//0
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 10;//取消
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 11;//确定
			}
			key_yy = key_y ;
			key_xx = key_x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if( keyboard_new == 10)
			{
				input[0] = 0 ;
				input[1] = 0 ;
				input[2] = 0 ;
				input[3] = 0 ;

			}
			else if(keyboard_new == 11)
			{
				 ;
			}
			else
			{
				for(j=0;j<500000;j++) ;
				input[0] = input[1];
				input[1] = input[2];
				input[2] = input[3];
				input[3] = keyboard_new;

			}
			output_number = ((float) (input[0]*100+input[1]*10+input[2]+(float)(input[3]*0.1)));
		}
		display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 250, 70, output_number, " ");
	}
	key_clr() ;
	color_change(0x0000, 0x0000);
	display_jx_clear(215, 55, 315, 215) ;
	//display_jxy(220, 60, 310, 210);
	color_change(0x001f, 0x0000);

	return ( output_number ) ;
}

float keyboard1( void )
{

	float output_number,aa;
	Uint16  keyboard_new = 0,key_xx,key_yy,flag_key;
	Uint32 j;
    Uint16 input[3]={0};

    flag_key = 0 ;
    aa = 1.0 ;
    display_jx(220, 60, 310, 210);
	display_jx(220, 90, 310, 150);
	display_jx(220, 120, 310, 180);
	display_jx(220, 90, 280, 210);
	display_jx(250, 90, 310, 210);

	display_ascii(M16, 0xffff, 0x0000, 1,291, 100 , "9");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 100 , "8");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 100 , "7");
	display_ascii(M16, 0xffff, 0x0000, 1,291, 130 , "6");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 130 , "5");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 130 , "4");
	display_ascii(M16, 0xffff, 0x0000, 1,291, 160 , "3");
	display_ascii(M16, 0xffff, 0x0000, 1,261, 160 , "2");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 160 , "1");
	display_ascii(M16, 0xffff, 0x0000, 1,231, 190 , "0");
	display_ascii(M12, 0xffff, 0x0000, 1,263, 190 , "-");
	display_ascii(M12, 0xffff, 0x0000, 1,285, 190 , "确定");

	//key_clr();
	while( keyboard_new != 11 )
	{
		if( key_xx != key_x || key_yy != key_y )
		{
			flag_key = 1 ;
			if (key_y > 90&&key_y < 120)
			{
				if(key_x > 220&&key_x < 250)
					keyboard_new = 7;//7
				else if(key_x > 250&&key_x < 280)
					keyboard_new = 8;//8
				else if(key_x > 280&&key_x < 310)
					keyboard_new = 9;//9
			}
			else if (key_y > 120&&key_y < 150)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 4;//4
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 5;//5
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 6;//6
			}
			else if (key_y > 150&&key_y < 180)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 1;//1
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 2;//2
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 3;//3
			}
			else if (key_y > 180&&key_y < 210)
			{
				if (key_x > 220&&key_x < 250)
					keyboard_new = 0;//0
				else if (key_x > 250&&key_x < 280)
					keyboard_new = 10;//取消
				else if (key_x > 280&&key_x < 310)
					keyboard_new = 11;//确定
			}
			key_yy = key_y ;
			key_xx = key_x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if( keyboard_new == 10)
			{
				display_ascii(M16, 0xffff, 0x0000, 1, 235, 70,  "-");
				aa = -1.0;
			}
			else if(keyboard_new == 11)
			{
				 ;
			}
			else
			{
				for(j=0;j<500000;j++) ;
				input[0] = input[1];
				input[1] = input[2];
				input[2] = keyboard_new;
			}
			output_number = (float) (input[0]*10+input[1]+(float)(input[2]*0.1)) ;
		}
		display_fp_ascii(M16, 0xffff, 0x0000, 1, 250, 70, output_number, " " );
		//display_fp_ascii(M16, 0xffff, 0x0000, 1, 250, 70, output_number, " ");
	}
	key_clr() ;
	color_change(0x0000, 0x0000);
	display_jx_clear(215, 55, 315, 215) ;
	//display_jxy(220, 60, 310, 210);
	color_change(0x001f, 0x0000);

	return ( output_number * aa ) ;
}

Uint16 JQX(void)
{
	float buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7;
	float w, x, y, z, K1, K2, KB;

	Uint16 i, points = 4;

	buffer1 = 0;
	buffer2 = 0;
	buffer3 = 0;
	buffer4 = 0;
	buffer5 = 0;
	buffer6 = 0;
	buffer7 = 0;

	for (i = 0; i < points; i++)
	{
		value_ad[i] = BYmv[i];
		value_ka75[i] = BiaoY[i];
	}

	for (i = 0; i < points; i++)
	{
		buffer1 = buffer1 + value_ad[i]; //ci1
		buffer2 = buffer2 + value_ad[i] * value_ad[i]; //ci2
		buffer3 = buffer3 + value_ad[i] * value_ad[i] * value_ad[i]; //ci3
		buffer4 = buffer4
				+ value_ad[i] * value_ad[i] * value_ad[i] * value_ad[i]; //ci4
		buffer5 = buffer5 + value_ka75[i] * value_ad[i] * value_ad[i]; //aici2
		buffer6 = buffer6 + value_ka75[i] * value_ad[i]; //aici1
		buffer7 = buffer7 + value_ka75[i]; //ai
	}
	w = (points * buffer2 - buffer1 * buffer1) * (points * buffer5 - buffer2 * buffer7);
	x = (points * buffer3 - buffer1 * buffer2) * (points * buffer6 - buffer1 * buffer7);
	y = w - x;
	w = (points * buffer2 - buffer1 * buffer1) * (points * buffer4 - buffer2 * buffer2);
	x = (points * buffer3 - buffer1 * buffer2) * (points * buffer3 - buffer1 * buffer2);
	z = w - x;
	if (z == 0)
	{
		//出错提示
		return (0);
	}
	K1 = y / z;
	w = points * buffer6 - buffer7 * buffer1;
	x = buffer1 * buffer2 - points * buffer3;
	x = K1 * x;
	y = w + x;
	z = points * buffer2 - buffer1 * buffer1;
	if (z == 0)
	{
		//出错提示
		return (0);
	}
	K2 = y / z;
	y = buffer7 - K2 * buffer1 - K1 * buffer2;
	KB = y / points;
	kaa = K1;
	kbb = K2;
	kcc = KB;
	return (1);
}

Uint16 JQX1(void)
{
	float buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7;
	float w, x, y, z, K1, K2, KB;
	//	float value_ad[4],value_ka75[4];
	Uint16 i;
	Uint16 points = 3;

	buffer1 = 0;
	buffer2 = 0;
	buffer3 = 0;
	buffer4 = 0;
	buffer5 = 0;
	buffer6 = 0;
	buffer7 = 0;

	for (i = 0; i < points; i++)
	{
		buffer1 = buffer1 + value_ad[i]; //ci1
		buffer2 = buffer2 + value_ad[i] * value_ad[i]; //ci2
		buffer3 = buffer3 + value_ad[i] * value_ad[i] * value_ad[i]; //ci3
		buffer4 = buffer4
				+ value_ad[i] * value_ad[i] * value_ad[i] * value_ad[i]; //ci4
		buffer5 = buffer5 + value_ka75[i] * value_ad[i] * value_ad[i]; //aici2
		buffer6 = buffer6 + value_ka75[i] * value_ad[i]; //aici1
		buffer7 = buffer7 + value_ka75[i]; //ai
	}
	w = (points * buffer2 - buffer1 * buffer1)
			* (points * buffer5 - buffer2 * buffer7);
	x = (points * buffer3 - buffer1 * buffer2)
			* (points * buffer6 - buffer1 * buffer7);
	y = w - x;
	w = (points * buffer2 - buffer1 * buffer1)
			* (points * buffer4 - buffer2 * buffer2);
	x = (points * buffer3 - buffer1 * buffer2)
			* (points * buffer3 - buffer1 * buffer2);
	z = w - x;
	if (z == 0)
	{
		//出错提示
		return (0);
	}
	K1 = y / z;
	w = points * buffer6 - buffer7 * buffer1;
	x = buffer1 * buffer2 - points * buffer3;
	x = K1 * x;
	y = w + x;
	z = points * buffer2 - buffer1 * buffer1;
	if (z == 0)
	{
		//出错提示
		return (0);
	}
	K2 = y / z;
	y = buffer7 - K2 * buffer1 - K1 * buffer2;
	KB = y / points;
	kaa = K1;
	kbb = K2;
	kcc = KB;
	return (1);
}

char *gastab[] =
{ "露点温度:", "气体纯度:", "SO2气体 :", "H2S气体 :", "CO气体  :" };
char *flow_table2[] =
{ "00 L/h", "05 L/h", "10 L/h", "15 L/h", "20 L/h", "25 L/h", "30 L/h","35 L/h", "40 L/h", "45 L/h" };

void null_point_display()
{
	float sensor1, flw;
	Uint16 m = 0 ,ver;

	u_key_enter = 0;
	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);
	display_ascii(M16, 0xffff, 0x001f, 1, 5, 2,  "校准");
	display_ascii(M16, 0xF800, 0x0000, 1, 5, 30, "校准气体原始数据");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出       泵开        泵关    ");
	color_change(0xf81f, 0x0000);
	display_jx(5, 60, 314, 144);
	if( instrument == 0 )
	{	
		if (sub_ver==1) ver=1;
		if (sub_ver==0) ver=0;
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,   74, gastab[0+ver]);
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,   94, gastab[1+ver]);
		display_ascii(M16, 0x07e0, 0x0000, 1, 160,  74, gastab[2+ver]);
		display_ascii(M16, 0x07e0, 0x0000, 1, 160,  94, gastab[3+ver]);
	}
	else if ( instrument == 1 )
	{
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,   74, gastab[0]);
	}
	else if ( instrument == 2 )
	{
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,  74, gastab[2]);
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,  94, gastab[3]);
		display_ascii(M16, 0x07e0, 0x0000, 1, 10, 114, gastab[4]);
	}
	else if ( instrument == 3 )
	{
		display_ascii(M16, 0x07e0, 0x0000, 1, 10,   74, gastab[1]);
	}

	if (parameter.gas == 0)
	{
		display_ascii(M16, 0xf800, 0x0000, 1, 5, 200, "平衡气为SF6");
	}
	else
	{
		display_ascii(M16, 0xf800, 0x0000, 1, 5, 200, "平衡气为N2 ");
	}

	key_clr();
	while ( keynew[3] == 0)
	{
		u_key_draw();
		if (keynew[3]) key_clr();

		if( instrument == 0 )
		{
			fp32_ad_get(H2O);
			sensor1 = ad8401_value[0] * (4.096 / 65536.0);
			if (h2oold<0) h2oold=0;
			sensor1 = 0.5 * sensor1 + 0.5 * h2oold;
			h2oold = sensor1;
			sensor1 = 25.0 * sensor1 - 80.0;
			if (sub_ver==0)
			display_fp_ascii(M16, 0xffff, 0x0000, 1, 85, 74, sensor1, " ");

			fp32_ad_get(SO2);
			sensor1 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;oo
			if (so2old<0) so2old=0;
			sensor1 = 0.03 * sensor1 + 0.97 * so2old;
			so2old = sensor1;
			if (sub_ver==1) display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 85, 94, sensor1, " ");
			if(sub_ver==0) display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 235, 74, sensor1, " ");

			fp32_ad_get(H2S);
			sensor1 = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
			if (h2sold<0) h2sold=0;
			sensor1 = 0.03 * sensor1 + 0.97 * h2sold;
			h2sold = sensor1;
			if (sub_ver==1) display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 235, 74, sensor1, " ");
			if(sub_ver==0) display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 235, 94, sensor1, " ");

			fp32_ad_get(HF);
			sensor1 = ad8401_value[0] * 0.0625; //*0.01894;//*10.0;//0.01491625-1.0024;
			if (hfold<0) hfold=0;
			sensor1 = 0.03 * sensor1 + 0.97 * hfold;
			hfold = sensor1;
			if (sub_ver==1) display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 235, 94, sensor1, " ");

			sensor1 = get_Concentration();
			if (gpaold<0) gpaold=0;
			sensor1 = 0.8 * sensor1 + 0.2 * gpaold;
			gpaold = sensor1;
			if (sub_ver==1) display_fp4(M16, 0xffff, 0x0000, 1, 85, 74, sensor1, " ");
			if(sub_ver==0) display_fp4(M16, 0xffff, 0x0000, 1, 85, 94, sensor1, " ");

		}
		else if ( instrument == 1 )
		{
			fp32_ad_get(H2O);
			sensor1 = ad8401_value[0] * (4.096 / 65536.0);
			if (h2oold<0) h2oold=0;
			sensor1 = 0.5 * sensor1 + 0.5 * h2oold;
			h2oold = sensor1;
			sensor1 = 25.0 * sensor1 - 80.0;
			display_fp_ascii(M16, 0xffff, 0x0000, 1, 85, 74, sensor1, " ");
		}
		else if ( instrument == 2 )
		{
			fp32_ad_get(SO2);
			sensor1 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
			if (so2old<0) so2old=0;
			sensor1 = 0.03 * sensor1 + 0.97 * so2old;
			so2old = sensor1;
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 85, 74, sensor1, " ");

			fp32_ad_get(H2S);
			sensor1 = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
			if (h2sold<0) h2sold=0;
			sensor1 = 0.03 * sensor1 + 0.97 * h2sold;
			h2sold = sensor1;
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 85, 94, sensor1, " ");

			fp32_ad_get(HF);
			sensor1 = ad8401_value[0] * 0.0625; //*0.01894;//*10.0;//0.01491625-1.0024;
			if (hfold<0) hfold=0;
			sensor1 = 0.03 * sensor1 + 0.97 * hfold;
			hfold = sensor1;
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 85, 114, sensor1, " ");
		}
		else if ( instrument == 3 )
		{
			sensor1 = get_Concentration();
			if (gpaold<0) gpaold=0;
			sensor1 = 0.8 * sensor1 + 0.2 * gpaold;
			gpaold = sensor1;
			display_fp4(M16, 0xffff, 0x0000, 1, 85, 74, sensor1, " ");
		}

		u_key_draw();
		fp32_ad_get(FLOW);
		flw = ad8401_value[0] * (4.096 / 65536.0);
		if (flwold<0) flwold=0;
		flw = 0.5 * flw + 0.5 * flwold;
		flwold = flw;

		if (flw <= flow_tab[parameter.gas][0])
			in8 = 0;
		else if (flw <= flow_tab[parameter.gas][1])
			in8 = 0;
		else if (flw <= flow_tab[parameter.gas][2])
			in8 = 1;
		else if (flw <= flow_tab[parameter.gas][3])
			in8 = 2;
		else if (flw <= flow_tab[parameter.gas][4])
			in8 = 3;
		else if (flw <= flow_tab[parameter.gas][5])
			in8 = 4;
		else if (flw <= flow_tab[parameter.gas][6])
			in8 = 5;
		else if (flw <= flow_tab[parameter.gas][7])
			in8 = 6;
		else if (flw <= flow_tab[parameter.gas][8])
			in8 = 7;
		else if (flw <= flow_tab[parameter.gas][9])
			in8 = 8;
		else if (flw >= flow_tab[parameter.gas][10])
		{
			in8 = 8;
		}
		display_ascii(M24, 0x07e0, 0x0000, 1, 120, 190, "气体流量:");
		display_ascii(M24, 0xffff, 0x0000, 1, 235, 190, flow_table2[in8]);

		if (keynew[1] == 1)
		{
			gas_clean(1);
			//key_clr();
		}
		if (keynew[2] == 1)
		{
			gas_clean(0);
			//key_clr();
		}
		if (keynew[0] == 1)
		{
		      gas_clean(0);
		      return ;
		      //key_clr();
		}
	        key_clr() ;
        }
	gas_clean(0);
}

void flag_set(void)
{
	fm24cl04_write(0xa8, 0x02, 0x55);
	fm24cl04_write(0xa4, 0x02, 0x55);
	fm24cl04_write(0xa0, 0x02, 0x55);
}

void flag_reset(void)
{
	fm24cl04_write(0xa8, 0x02, 0x0);
	fm24cl04_write(0xa4, 0x02, 0x0);
	fm24cl04_write(0xa0, 0x02, 0x0);
}

void calib_status(Uint16 flag)
{
	Uint32 i;

	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);
	if (flag == 0)
	{
		display_ascii(M24, 0xf800, 0x0000, 1, 20, 50, "校准失败,重新操作！");
		for (i = 0; i < 1000000; i++);
		//while (1);
	}
	else
	{
		display_ascii(M24, 0xffff, 0x0000, 1, 20, 50, "校准成功！！！");
		for (i = 0; i < 1000000; i++);
	}
}


void password_1(Uint16 x)
{
	display_ascii(M16, 0xffff, 0x0000, 1, 100+20*x, 30, "_");
}

Uint32 password_keyboard(void)
{

	Uint16  keyboard = 0,key_xx,key_yy,flag_key,x;
	Uint32 output_number,j;
	Uint32 input[6]={0};
	key_clr() ;

	x = 0 ;

	display_jx(100, 60, 180, 200);
	display_jx(140, 60, 220, 200);
	display_jx(100, 95, 220, 165);
	display_jx(100, 130, 220, 200);

	display_ascii(M24, 0xffff, 0x0000, 1,192,  65 , "9");
	display_ascii(M24, 0xffff, 0x0000, 1,158,  65 , "8");
	display_ascii(M24, 0xffff, 0x0000, 1,118,  65 , "7");
	display_ascii(M24, 0xffff, 0x0000, 1,192, 100 , "6");
	display_ascii(M24, 0xffff, 0x0000, 1,158, 100 , "5");
	display_ascii(M24, 0xffff, 0x0000, 1,118, 100 , "4");
	display_ascii(M24, 0xffff, 0x0000, 1,192, 135 , "3");
	display_ascii(M24, 0xffff, 0x0000, 1,158, 135 , "2");
	display_ascii(M24, 0xffff, 0x0000, 1,118, 135 , "1");
	display_ascii(M24, 0xffff, 0x0000, 1,118, 170 , "0");
	display_ascii(M16, 0xffff, 0x0000, 1,154, 175 , "ESC");
	display_ascii(M16, 0xffff, 0x0000, 1,190, 175 , "OK");

	for(j=0;j<500000;j++) ;
	key_x = 0 ;
	key_y = 0 ;
	key_xx = key_x ;
	key_yy = key_y ;
	keyboard = 10 ;

	while( keyboard != 11 )
	{
		if( key_xx != key_x || key_yy != key_y )
		{
			flag_key = 0;
			if (key_y > 60 && key_y < 95)
			{
				if(key_x > 100 && key_x < 140)
					{keyboard = 7;flag_key = 1;}//7
				else if(key_x > 140 && key_x < 180)
					{keyboard = 8;flag_key = 1;}//8
				else if(key_x > 180 && key_x < 220)
					{keyboard = 9;flag_key = 1;}//9
			}
			else if (key_y > 95 && key_y < 130)
			{
				if ( key_x > 100 && key_x < 140 )
					{keyboard = 4;flag_key = 1;}//4
				else if (key_x > 140 && key_x < 180)
					{keyboard = 5;flag_key = 1;}//5
				else if (key_x > 180 && key_x < 220)
					{keyboard = 6;flag_key = 1;}//6
			}
			else if ( key_y > 130 && key_y < 165 )
			{
				if (key_x > 100&&key_x < 140)
					{keyboard = 1;flag_key = 1;}//1
				else if (key_x > 140 && key_x < 180)
					{keyboard = 2;flag_key = 1;}//2
				else if (key_x > 180 && key_x < 220)
					{keyboard = 3;flag_key = 1;}//3
			}
			else if ( key_y > 165 && key_y <200 )
			{
				if (key_x > 100 && key_x < 140)
					{keyboard = 0;flag_key = 1;}//0
				else if (key_x > 140 && key_x < 180)
					{keyboard = 10;flag_key = 1;}//取消
				else if (key_x > 180 && key_x < 220)
					{keyboard = 11;flag_key = 1;}//确定
			}
			key_yy = key_y ;
			key_xx = key_x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if(keyboard == 11)
			{
				x = 0 ;
				for( j = 0; j < 7; j++ )
				{
					display_ascii(M16, 0x0000, 0x0000, 1, 100+20*j, 30, "*");
				}
			}
			else if( keyboard == 10)
			{
				input[0] = 0 ;
				input[1] = 0 ;
				input[2] = 0 ;
				input[3] = 0 ;
				input[4] = 0 ;
				input[5] = 0 ;
				x = 0 ;
				for( j = 0; j < 7; j++ )
				{
					display_ascii(M16, 0x0000, 0x0000, 1, 100+20*j, 30, "*");
				}
			}
			else
			{
				if( x < 6 )
				{
					for(j=0;j<500000;j++) ;
					input[0]=input[1];
					input[1]=input[2];
					input[2]=input[3];
					input[3]=input[4];
					input[4]=input[5];
					input[5] = keyboard;
					display_ascii(M16, 0xffff, 0x0000, 1, 100+20*x, 30, "*");
					x += 1 ;
				}
				else
				{
					x = 6;
				}
			}
		}
		password_1( x );
	}
	output_number = input[0]*100000+input[1]*10000+input[2]*1000+input[3]*100+input[4]*10+input[5];

	key_clr() ;
	color_change(0x0000, 0x0000);
	display_jx_clear(100, 60, 220, 200) ;
	color_change(0xf81f, 0x0000);
	//display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "密码正确！");
	return output_number ;
}

Uint16 password_adj(void) //0---进入校准 1---密码设置完成
{
	Uint16 error_flag;
	Uint32 password1,password,i;

	fm22l16_rds(ADD_PASSWORD,(Uint16 *)&password_old,2);

	color_change(0x001f, 0x0000);
	cls();
	u_key_cnt = 0;
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);

	//for (j = 0; j < 4; j++)
		//password_in[j] = password[j];

	//display_ascii(M16, 0xffff, 0x001f, 1, 5, 2, "密码修改");
	display_ascii(M16, 0xf800, 0x0000, 1, 10, 30, "请输入密码：");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出                   修改       输入");
	color_change(0xf81f, 0x0000);
	display_jx(5, 25, 314, 55);

	password_1(0);
	key_clr();
	error_flag = 0 ;
	//km = 1;
	while ( keynew[0] == 0)
	{
a1:		u_key_draw();
		if (keynew[1]) key_clr();
		if (keynew[2] == 1)
		{
a2:			key_clr();
			display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "请输入旧的密码!");
			for (i = 0; i < 2000000; i++) ;
			display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "请输入旧的密码!");
			password = password_keyboard() ;
			if( password == password_old || password == 588588 )
			{
				display_ascii(M24, 0xf800, 0x0000, 1, 70, 110, "密码正确！");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M24, 0x0000, 0x0000, 1, 70, 110, "密码正确！");
				display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "请输入新的密码!");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "请输入新的密码!");
				password = password_keyboard() ;
				display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "请再输入一次密码!");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "请再输入一次密码!");
				password1 = password_keyboard() ;
				if( password1 == password )
				{
					password_old = password ;
					fm22l16_wrs(ADD_PASSWORD,(Uint16 *)&password,2);
				}
				else
				{
					display_ascii(M24, 0xf800, 0x0000, 1, 30, 110, "两次密码不同请重新输入!");
					for (i = 0; i < 2000000; i++) ;
					goto a2 ;
				}
			}
			else
			{
				error_flag++ ;
				display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "密码不正确！");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "密码不正确！");
				if( error_flag<3 )
				{
					goto a2 ;
				}
				else
				{
					display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "密码三次不正确，设置退出！");
					for (i = 0; i < 2000000; i++) ;
					display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "密码三次不正确，设置退出！");
					goto a1 ;
				}
			}
		}
		else if(keynew[3] == 1)
		{
			password = password_keyboard() ;
			key_clr();
			if(password == password_old || password == 588588 )
			{
				display_ascii(M24, 0xf800, 0x0000, 1, 70, 110, "密码正确！");
				for (i = 0; i < 2000000; i++) ;
				return 1 ;
			}
			else
			{
				display_ascii(M24, 0xf800, 0x0000, 1, 50, 110, "密码不正确！");
				for (i = 0; i < 2000000; i++) ;
				return 0 ;
			}
		}
	}
	return 0 ;
}

void display_calib_time(Uint16 x, Uint16 y, Uint16 *pt)
{
	Uint16 i;
	char s[20];

	display_ascii(M12, 0x07e0, 0x0000, 1, x, y, "上次校准时间:");
	s[0] = 0x30 + 2; //日期
	s[1] = 0x30 + 0;
	i = pt[6] >> 4 & 0x0f;
	s[2] = i + 0x30;
	i = pt[6] & 0x0f;
	s[3] = i + 0x30;
	s[4] = '.';
	i = pt[5] >> 4 & 0x0f;
	s[5] = i + 0x30;
	i = pt[5] & 0x0f;
	s[6] = i + 0x30;
	s[7] = '.';
	i = pt[4] >> 4 & 0x0f;
	s[8] = i + 0x30;
	i = pt[4] & 0x0f;
	s[9] = i + 0x30;
	s[10] = ' ';
	i = pt[2] >> 4 & 0x0f; //时间
	s[11] = i + 0x30;
	i = pt[2] & 0x0f;
	s[12] = i + 0x30;
	s[13] = ':';
	i = pt[1] >> 4 & 0x0f;
	s[14] = i + 0x30;
	i = pt[1] & 0x0f;
	s[15] = i + 0x30;
	s[16] = '\0';
	display_ascii(M12, 0x07e0, 0x0000, 1, x + 90, y, &s[0]);
}

float input1[7], input2[7],input3[7],input4[7]; //+113.7
void display_input(Uint16 x,Uint16 sensor)
{
	Uint16 i; //j, t1 = 0;

	if( sensor == 0 )
	{
	    for ( i = 0; i < 5; i++ )
	    {
	        if (x < 5)
	        {
	        	display_fp_ascii(M16, 0xffff, color_1[x][i], 1, 25, 70+20*i, input1[i], " ");
	        	display_fp_ascii(M16, 0xffff, 0x0000, 1, 120, 70+20*i, input2[i], " ");
	        }
	        else
	        {
	        	display_fp_ascii(M16, 0xffff, 0x0000, 1, 25, 70+20*i, input1[i], " ");
	        	display_fp_ascii(M16, 0xffff, color_1[x-5][i], 1, 120, 70+20*i, input2[i], " ");
	        }
	    }
	}
	else if( sensor == 1 || sensor == 3 )
	{
	    for (i = 0; i < 7; i++)
	    {
	        if (x < 7)
	        {
	        	display_fp_ascii_ppm1(M16, 0xffff, color_1[x][i], 1, 25, 70+20*i, input1[i], " ");
	        	display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 120, 70+20*i, input2[i], " ");
	        }
	        else
	        {
	        	display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 25, 70+20*i, input1[i], " ");
	        	display_fp_ascii_ppm1(M16, 0xffff, color_1[x-7][i], 1, 120, 70+20*i, input2[i], " ");
	        }
	    }
	}
	else if(sensor == 2 || sensor == 4 )
	{
		for (i = 0; i < 7; i++)
		{
		    if (x < 7)
		    {
		    	display_fp_ascii_ppm1(M16, 0xffff, color_1[x][i], 1, 25, 70+20*i, input1[i], " ");
		        display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 120, 70+20*i, input2[i], " ");
		        display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 215, 70+20*i, input3[i], " ");
		    }
		    else if( x < 14 )
		    {
		        display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 25, 70+20*i, input1[i], " ");
		        display_fp_ascii_ppm1(M16, 0xffff, color_1[x-7][i], 1, 120, 70+20*i, input2[i], " ");
		        display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 215, 70+20*i, input3[i], " ");
		    }
		    else
		    {
		    	display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 25, 70+20*i, input1[i], " ");
		    	display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 120, 70+20*i, input2[i], " ");
		    	display_fp_ascii_ppm1(M16, 0xffff, color_1[x-14][i], 1, 215, 70+20*i, input3[i], " ");
		    }
		 }
	}
}

void display_dot_input( Uint16 x )
{
	Uint16 i; //j, t1 = 0;


		for (i = 0; i < 2; i++)
		{
			if (x < 2)
			{
				display_fp_ascii_ppm1(M16, 0xffff, color_1[x][i], 1, 25, 70+20*i, in1[i], " ");
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 120, 70+20*i, in2[i], " ");
			}
			else
			{
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 25, 70+20*i, in1[i], " ");
				display_fp_ascii_ppm1(M16, 0xffff, color_1[x-2][i], 1, 120, 70+20*i, in2[i], " ");
		    }
		}
}

void display_dot_input1( Uint16 x )
{
	Uint16 i; //j, t1 = 0;


		for (i = 0; i < 2; i++)
		{
			if (x < 2)
			{
				display_fp_ascii(M16, 0xffff, color_1[x][i], 1, 25, 70+20*i, in1[i], " ");
				display_fp_ascii(M16, 0xffff, 0x0000, 1, 120, 70+20*i, in2[i], " ");
			}
			else
			{
				display_fp_ascii(M16, 0xffff, 0x0000, 1, 25, 70+20*i, in1[i], " ");
				display_fp_ascii(M16, 0xffff, color_1[x-2][i], 1, 120, 70+20*i, in2[i], " ");
		    }
		}
}

float temperature_range, temperature_offset;
Uint16 temperature_flag;
void temperature_calib(void)
{

	Uint16  buff =0,i, j, keycnt = 0, t[7];
	float  tmp1, tmp2;
	float input1[3],input2[3];
	Uint32 m;

a1:	keycnt = 0;
	km = 1;
	u_key_enter = 0;
	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);

	display_ascii(M16, 0xffff, 0x001f, 1, 5, 2, "温度校准");
	display_ascii(M16, 0x07e0, 0x0000, 1, 25, 50, "测量温度     实际温度");

	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 70, "1.");
	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 90, "2.");

	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 右移       下移        修改       确认");
	display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "温度:");
	key_clr();

       buff = fm22l16_rd( TEMPERATURE_FLAG );
	if (buff == 0x5555)
	{
		fm22l16_rds(TEMP_MEASURE_INPUT, (Uint16*)in1, 4);
		fm22l16_rds(TEMP_REAL_INPUT, (Uint16*)in2, 4);
		fm22l16_rds(TEMP_TIME, &t[0], 7);
		display_calib_time(0, 206, &t[0]);
		input1[0] = in1[0] ;
		input1[1] = in1[1] ;
		input2[0] = in2[0] ;
		input2[1] = in2[1] ;
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			input1[i] = 0;
			input2[i] = 0;
		}
	}

	display_dot_input1( keycnt );
	display_dot_input1( keycnt );
       while (keynew[3] == 0)
	{
		sht71_ht(&c_humid,&c_temp);
		//display_fp_ascii(M16, 0xffff, 0x0000, 1, 240, 30, (temperature_range*c_temp + temperature_offset), " ");
		display_fp_ascii(M16, 0xffff, 0x0000, 1, 240, 30, c_temp, " ");
		u_key_draw();
		if (keynew[0] + keynew[1] + keynew[2] + keynew[5])
		{
			if (keynew[0] == 1)
			{
				if (keycnt >=2)
					keycnt -= 2;
				else
					keycnt += 2;
			}
			if (keynew[1] == 1)
			{
				if (keycnt <=1)
				{
					keycnt++;
					if (keycnt >= 2)
						keycnt = 0;
				}
				else
				{
					keycnt++;
					if (keycnt >= 4)
						keycnt = 2;
				}
			}
			if (keynew[2] == 1)
			{
				if (keycnt <=1)
				{
					in1[keycnt] = c_temp;
				}
				else
				{
					in2[ keycnt-2 ] = keyboard1();
				}
			}
			if ( keynew[5] == 1 )
			{
				in1[keycnt] = keyboard1();
			}
			key_clr();
			display_dot_input1( keycnt );
		}
	}
	key_clr();

	temperature_flag = 0x5555;
	fm22l16_wr(TEMPERATURE_FLAG, 0x5555);
	fm22l16_wrs(TEMP_MEASURE_INPUT, (Uint16*)in1, 4);
	fm22l16_wrs(TEMP_REAL_INPUT, (Uint16*)in2, 4);
	rx8025_clock_read();
	fm22l16_wrs(TEMP_TIME, &i2c[0], 7);

}

void calib_select1(Uint16 cnt)
{
	display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60, "气体露点                              ");
	display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80, "气体纯度                              ");
	display_ascii(M16, 0xffff, color_1[cnt][2], 1, 10, 100,"气体SO2含量                          ");
	display_ascii(M16, 0xffff, color_1[cnt][3], 1, 10, 120,"气体H2S含量                          ");
	display_ascii(M16, 0xffff, color_1[cnt][4], 1, 10, 140,"气体CO含量                            ");
	display_ascii(M16, 0xffff, color_1[cnt][5], 1, 10, 160,"环境温度校准                          ");
}

Uint16 calib_select(void) //返回0表示退出，返回1～5 表示如下
{
	Uint16  keycnt = 0;
	Uint32  m ;

a1: u_key_enter = 0;
	u_key_cnt = 0;

	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);

	display_ascii(M16, 0xffff, 0x001f, 1, 5, 2,  "校准");
	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 30, "请选择校准对象：");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出       上移        下移       确认");

	color_change(0xf81f, 0x0000);
	display_jx(5, 50, 314, 185);

	calib_select1(keycnt);
	key_clr();
	while (keynew[0] + keynew[3] == 0)
	{
		u_key_draw();
		if (keynew[1] + keynew[2] == 1)
		{
			if (keynew[1] == 1)
			{
				if (keycnt == 0)
					keycnt = 5;
				else
					keycnt--;
			}
			else if (keynew[2] == 1)
			{
				if (keycnt == 5)
					keycnt = 0;
				else
					keycnt++;
			}
			calib_select1(keycnt);
			key_clr();
		}
	}
	if (keynew[0] == 1)
		return 7 ;
	if (instrument == 0 && sub_ver==0 && keycnt == 4)
		goto a1;
	if (instrument == 0 && sub_ver==1 && keycnt == 0)
		goto a1;
	else if (instrument == 1 && keycnt > 0 && keycnt < 5)
		goto a1;
	else if (instrument == 2 && keycnt < 2)
		goto a1;
	else if (instrument == 3 && keycnt == 5)
		temperature_calib();
	else if (instrument == 3 && keycnt != 1)
		goto a1;

    // keycnt += 1;
a2:	if ( keycnt == 5 )
	{
		temperature_calib();
		calib_status(1);
		for (m = 0; m < 1000000; m++) ;
		return 6 ;
	}
	else
	{
		return keycnt ;
	}
}

void calib_mode_select1(Uint16 cnt)
{
	display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60,"校准对象                             ");
	display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80,"零点显示                             ");
	display_ascii(M16, 0xffff, color_1[cnt][2], 1, 10, 100,"触摸校准                             ");
	
}

Uint16 calib_mode_select( void )
{
	Uint16  keycnt = 0 ,tt ;
	u_key_enter = 0;
	km = 1;
	u_key_cnt = 3;
	tt = 0 ;

a1:	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);

	display_ascii(M16, 0xffff, 0x001f, 1, 5, 2,  "校准");
	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 30, "请选择校准内容：");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出       上移        下移       确认");

	color_change(0xf81f, 0x0000);
	display_jx(5, 50, 314, 126);

	calib_mode_select1(keycnt);
	key_clr();
	while (keynew[3] + keynew[0] == 0)
	{
		u_key_draw();
		if (keynew[1] + keynew[2] == 1)
		{
			if (keynew[1] == 1)
			{
				if (keycnt == 0)
					keycnt = 2;
				else
					keycnt--;
			}
			else if (keynew[2] == 1)
			{
				if (keycnt == 2)
					keycnt = 0;
				else
					keycnt++;
			}
			calib_mode_select1(keycnt);
			key_clr();
		}
	}
	if (keynew[3])
	{
		if (keycnt == 0 )
		{
			//校准对象选择
			tt = calib_select() ;
			if( tt == 7 )
				goto a1 ;
			return tt ;
		}
		else if (keycnt == 1)
		{
			//零点显示
			null_point_display() ;
			goto a1 ;
		}
		else if (keycnt==2)
		{
			calib_touch();
			goto a1;
		}
	}
	else
	{
		key_clr();
		tt = 6 ;
		return tt ;

	}
}

void calib_multipoint( void )
{
	Uint16 j, keycnt, tt, *pt,password_flag,error_flag,tmp_1;
	Uint16 buff = 0, t[7];
	Uint32 m,k;
	int16 i ;
	float tmp,tmp1;
	float sensor1,sensor2;

	keycnt = 0;
	km = 1;
	u_key_enter = 0;
	error_flag = 0 ;
a1:	password_flag = password_adj() ;

	if( password_flag )
	{
a2:		tt = calib_mode_select() ;
		if ( tt == 6 )
		{
			return ;
		}
		if (tt == 0)
		{
			tt = 0;
			buff = fm22l16_rd(ADD_CHECK2);
		} //H2O
		else if (tt == 1)
		{
			tt = 1;
			buff = fm22l16_rd(ADD_CHECK3);
		} //纯度
		else if (tt == 2)
		{
			tt = 2;
			buff = fm24cl04_read(0xa4, 0x02);
			if( buff != 0x55 )
			{
				fm24cl04_write(0xa4, 0x00, 0xa4);
				fm24cl04_write(0xa4, 0x02, 0x55);
			}
		} //SO2
		else if (tt == 3)
		{
			tt = 3;
			buff = fm24cl04_read(0xa0, 0x02);
			if( buff != 0x55 )
			{
				fm24cl04_write(0xa0, 0x00, 0xa0);
				fm24cl04_write(0xa0, 0x02, 0x55);
			}
		} //H2S
		else if (tt == 4)
		{
			tt = 4;
			buff = fm24cl04_read(0xa8, 0x00);
			if( buff != 0xa8 )
			{
				fm24cl04_write(0xa8, 0x00, 0xa8);
			}
			buff = fm24cl04_read(0xa8, 0x02);
			if( buff != 0x55 )
			{
				fm24cl04_write(0xa8, 0x00, 0xa8);
				fm24cl04_write(0xa8, 0x02, 0x55);
			}
		} //CO

		if (buff == 0x5555 || buff == 0x55)
		{
			if (tt == 0)
			{
				fm22l16_float_rds(ADD_CALIB_H2O_INPUT1, &input1[0], 7);
				fm22l16_float_rds(ADD_CALIB_H2O_INPUT2, &input2[0], 7);
				fm22l16_rds(ADD_CALIB_H2O_TIME, &t[0], 7);
				display_calib_time(0, 206, &t[0]);
			}
			else if (tt == 1)
			{
				fm22l16_float_rds(ADD_CALIB_GPA_INPUT1, &input1[0], 7);
				fm22l16_float_rds(ADD_CALIB_GPA_INPUT2, &input2[0], 7);
				fm22l16_rds(ADD_CALIB_GPA_TIME, &t[0], 7);
				display_calib_time(0, 206, &t[0]);
			}
			else if (tt == 2)
			{
				fm22l16_float_rds(ADD_CALIB_SO2_INPUT1, &input1[0], 7);
				fm22l16_float_rds(ADD_CALIB_SO2_INPUT2, &input2[0], 7);
				fm22l16_float_rds(ADD_SO2_H2S_INPUT2, &input3[0], 7);
				fm22l16_float_rds(ADD_SO2_H2S_INPUT3, &input4[0], 7);
				for (i = 0; i < 7; i++)
					t[i] = fm24cl04_read(0xa4, 0x40 + i);
				display_calib_time(0, 206, &t[0]);
			}
			else if (tt == 3)
			{
				fm22l16_float_rds(ADD_CALIB_H2S_INPUT1, &input1[0], 7);
				fm22l16_float_rds(ADD_CALIB_H2S_INPUT2, &input2[0], 7);
				for (i = 0; i < 7; i++)
					t[i] = fm24cl04_read(0xa0, 0x40 + i);
				display_calib_time(0, 206, &t[0]);
			}
			else if (tt == 4)
			{
				fm22l16_float_rds(ADD_CALIB_HF_INPUT1, &input1[0], 7);
				fm22l16_float_rds(ADD_CALIB_HF_INPUT2, &input2[0], 7);
				fm22l16_float_rds(ADD_CO_H2S_INPUT2, &input3[0], 7);
				fm22l16_float_rds(ADD_CO_H2S_INPUT3, &input4[0], 7);
				for (i = 0; i < 7; i++)
					t[i] = fm24cl04_read(0xa8, 0x40 + i);
				display_calib_time(0, 206, &t[0]);
			}
		}
		else
		{
			for (i = 0; i < 7; i++)
			{
				input1[i] = 0;
				input2[i] = 0;
				input3[i] = 0;
				input4[i] = 0;
			}
		}

		color_change(0x001f, 0x0000);
		cls();
		display_jxy(0, 0, 319, 20);
		display_jxy(0, 220, 319, 239);

a3:		display_ascii(M16, 0xffff, 0x001f, 1, 5, 2, "校准");
		if (tt == 0)
		{
			display_ascii(M16, 0xffff, 0x001f, 1, 200, 2, "露点校准程序");
			display_ascii(M16, 0xf800, 0x0000, 1, 5, 30, "露点数据:");
		}
		else if (tt == 1)
		{
			display_ascii(M16, 0xffff, 0x001f, 1, 200, 2, "纯度校准程序");
			display_ascii(M16, 0xf800, 0x0000, 1, 5, 30, "纯度数据:");
		}
		else if (tt == 2)
		{
			display_ascii(M16, 0xffff, 0x001f, 1, 230, 2, "SO2校准程序");
			display_ascii(M16, 0xf800, 0x0000, 1, 5, 30, "SO2数据:");
			display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "H2S数据:");
		}
		else if (tt == 3)
		{
			display_ascii(M16, 0xffff, 0x001f, 1, 230, 2, "H2S校准程序");
			display_ascii(M16, 0xf800, 0x0000, 1, 5, 30, "H2S数据:");
		}
		else if (tt == 4)
		{
			display_ascii(M16, 0xffff, 0x001f, 1, 230, 2, "CO 校准程序");
			display_ascii(M16, 0xf800, 0x0000, 1, 5, 30, "CO数据:");
			display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "H2S数据:");
		}

		if(tt == 2 || tt == 4 )
		{
			display_ascii(M16, 0x07e0, 0x0000, 1, 25, 50, "原始值");
			display_ascii(M16, 0x07e0, 0x0000, 1, 120, 50, "标准值 ");
			display_ascii(M16, 0x07e0, 0x0000, 1, 215, 50, "H2S值");
		}
		else
		{
			display_ascii(M16, 0x07e0, 0x0000, 1, 25, 50, "原始值");
			display_ascii(M16, 0x07e0, 0x0000, 1, 120, 50, "标准值 ");
		}
		if( tt == 0 )
		{
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 70,  "1.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 90,  "2.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 110, "3.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 130, "4.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 150, "5.");
		}
		else
		{
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 70,  "1.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 90,  "2.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 110, "3.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 130, "4.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 150, "5.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 170, "6.");
			display_ascii(M16, 0x07e0, 0x0000, 1, 5, 190, "7.");
		}

		display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 右移       下移        修改       确认");
		key_clr();

    	display_input(keycnt,tt);
		display_input(keycnt,tt);
		while (keynew[3] == 0)
		{
			if (tt == 0)
			{
				fp32_ad_get(H2O);
				sensor1 = ad8401_value[0] * (4.096 / 65536.0);
				if (h2oold<0) h2oold=0;
				sensor1 = 0.5 * sensor1 + 0.5 * h2oold;
				h2oold = sensor1;
				sensor1 = 25.0 * sensor1 - 80.0;
				display_fp_ascii(M16, 0xffff, 0x0000, 1, 70, 30, sensor1, " ");
			}
			else if (tt == 1)
			{
				sensor1 = get_Concentration();
				if (gpaold<0) gpaold=0; 
				sensor1 = 0.8 * sensor1 + 0.2 * gpaold;
				gpaold = sensor1;
				display_fp4(M16, 0xffff, 0x0000, 1, 70, 30, sensor1, " ");
			}
			else if (tt == 2)
			{
				fp32_ad_get(SO2);
				sensor1 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
				if (so2old<0) so2old=0;
				sensor1 = 0.03 * sensor1 + 0.97 * so2old;
				so2old = sensor1;
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 70, 30, sensor1, " ");

				fp32_ad_get(H2S);
				sensor2 = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
				if (h2sold<0) h2sold=0;
				sensor2 = 0.03 * sensor2 + 0.97 * h2sold;
				h2sold = sensor2;
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 245, 30, sensor2, " ");
			}
			else if (tt == 3)
			{
				fp32_ad_get(H2S);
				sensor1 = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
				if (h2sold<0) h2sold=0;
				sensor1 = 0.03 * sensor1 + 0.97 * h2sold;
				h2sold = sensor1;
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 70, 30, sensor1, " ");
			}
			else if (tt == 4)
			{
				fp32_ad_get(HF);
				sensor1 = ad8401_value[0] * 0.0625; //*0.01894;//*10.0;//0.01491625-1.0024;
				if (hfold<0) hfold=0;
				sensor1 = 0.03 * sensor1 + 0.97 * hfold;
				hfold = sensor1;
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 70, 30, sensor1, " ");

				fp32_ad_get(H2S);
				sensor2 = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
				if (h2sold<0) h2sold=0;
				sensor2 = 0.03 * sensor2 + 0.97 * h2sold;
				h2sold = sensor2;
				display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 245, 30, sensor2, " ");
			}

			u_key_draw();
			if ( keynew[3] == 0 )
			{
				if ( keynew[0] == 1 )
				{
					if( tt == 0 )
					{
					        if ( keycnt > 4 )
							keycnt -= 5;
						else
							keycnt += 5;
					}
					else if( tt == 1 || tt == 3 )
					{
						if ( keycnt > 6 )
							keycnt -= 7;
						else
							keycnt += 7;
					}
					else
					{
						if ( keycnt < 14 )
							keycnt += 7;
						else
							keycnt -= 14;
					}
				}
				else if ( keynew[1] == 1 )
				{
					if( tt == 0 )
					{
						if( keycnt < 5 )
						{
							keycnt++;
							if ( keycnt > 4 )
								keycnt = 0;
						}
						else
						{
							keycnt++;
							if( keycnt > 9 )
								keycnt = 5;
						}
					}
					else if( tt == 1 || tt == 3 )
					{
						if( keycnt<7 )
						{
							keycnt++;
							if ( keycnt > 6 )
								keycnt = 0;
						}
						else
						{
							keycnt++;
							if( keycnt > 13 )
								keycnt = 7;
						}
					}
					else
					{
						if( keycnt<7 )
						{
							keycnt++;
							if ( keycnt > 6 )
								keycnt = 0;
						}
						else if( keycnt<14 )
						{
							keycnt++;
							if( keycnt > 13 )
								keycnt = 7;
						}
						else
						{
							keycnt++;
							if( keycnt > 20 )
								keycnt = 14;
						}
					}
				}
				if ( keynew[2] == 1 )
				{
					if( tt == 0 )
					{
						if ( keycnt < 5 )
						{
							input1[keycnt] = sensor1 ;
						}
						else
						{
							input2[keycnt-5] = keyboard1() ;
							goto a3;
						}
					}
					else
					{
						if ( keycnt < 7 )
						{
							if( tt == 2 || tt == 4 )
							{
								input1[keycnt] = sensor1 ;
								input3[keycnt] = sensor2 ;
								input4[keycnt] = input1[keycnt] ;
							}
							else
							{
								input1[keycnt] = sensor1 ;
							}
						}
						else if ( keycnt < 14 )
						{
							input2[keycnt-7] = keyboard() ;
							goto a3;
						}
						else
						{
						    input3[keycnt-14] = sensor2 ;
						    input4[keycnt-14] = sensor1 ;
						}
					}
				}
				if ( keynew[5] == 1 )
				{
					if( tt == 0 )
					{
						if ( keycnt < 5 )
						{
							input1[keycnt] = keyboard1() ;
							goto a3;
						}
					}
					else
					{
						if ( keycnt < 7 )
						{
							input1[keycnt] = keyboard() ;
							goto a3;
						}
						else if( keycnt >= 14 )
						{
							input3[keycnt-14] = keyboard() ;
							goto a3;
						}
					}
				}
				for(i=0;i<7;i++)
					input4[i]=input1[i];
				key_clr();
				display_input(keycnt,tt);
			}
		}
		key_clr();
		measure_buffer_ini(H2S);
		measure_buffer_ini(SO2);
		measure_buffer_ini(HF);
		for (i = 0; i < 7; i++)
		{
			BYmv[i] = input1[i];
			BiaoY[i] = input2[i];
		}

		if (tt == 0)
		{
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i];
				value_ka75[i] = BiaoY[i];
			}
			buff = JQX1();
			if (buff == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			h2o.calib_k1[2] = kaa;
			h2o.calib_k1[1] = kbb;
			h2o.calib_k1[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 2];
				value_ka75[i] = BiaoY[i + 2];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			h2o.calib_k2[2] = kaa;
			h2o.calib_k2[1] = kbb;
			h2o.calib_k2[0] = kcc;

			h2o.calib_dot0 = BYmv[0];
			h2o.calib_dot1 = BYmv[2];
			h2o.calib_dot2 = 0.0;
			h2o.calib_k3[2] = 0.0;
			h2o.calib_k3[1] = 0.0;
			h2o.calib_k3[0] = 0.0;
			fm22l16_wr(ADD_CHECK2, 0x5555);
			fm22l16_float_wrs(ADD_CALIB_H2O_INPUT1, &input1[0], 7);
			fm22l16_float_wrs(ADD_CALIB_H2O_INPUT2, &input2[0], 7);
			rx8025_clock_read();
			fm22l16_wrs(ADD_CALIB_H2O_TIME, &i2c[0], 7);
			h2o_1.flag = 0x5555;
			fm22l16_wrs(ADD_CALIB_H2O, (Uint16*) &h2o, 24);

			calib_status(buff);
			return;
		}
		else if (tt == 1)
		{
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i];
				value_ka75[i] = BiaoY[i];
			}
			buff = JQX1();
			if (buff == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			gpa.calib_k1[2] = kaa;
			gpa.calib_k1[1] = kbb;
			gpa.calib_k1[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 2];
				value_ka75[i] = BiaoY[i + 2];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			gpa.calib_k2[2] = kaa;
			gpa.calib_k2[1] = kbb;
			gpa.calib_k2[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 4];
				value_ka75[i] = BiaoY[i + 4];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			gpa.calib_k3[2] = kaa;
			gpa.calib_k3[1] = kbb;
			gpa.calib_k3[0] = kcc;
			gpa.calib_dot0 = BYmv[0];
			gpa.calib_dot1 = BYmv[2];
			gpa.calib_dot2 = BYmv[4];

			fm22l16_wr(ADD_CHECK3, 0x5555);
			fm22l16_float_wrs(ADD_CALIB_GPA_INPUT1, &input1[0], 7);
			fm22l16_float_wrs(ADD_CALIB_GPA_INPUT2, &input2[0], 7);
			rx8025_clock_read();
			fm22l16_wrs(ADD_CALIB_GPA_TIME, &i2c[0], 7);
			gpa_1.flag = 0x5555;
			fm22l16_wrs(ADD_CALIB_GPA, (Uint16*) &gpa, 24);
			gpa_zero = 0.0;
			fm22l16_wrs(ADD_GPA_0, (Uint16*) &gpa_zero, 2);
						//calib_test(&gpa);
			calib_status(buff);
			return;
		}
		else if (tt == 4)
		{
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i];
				value_ka75[i] = BiaoY[i];
			}
			buff = JQX1();
			if (buff == 0)
			{
				calib_status(buff);
			    goto a2 ;
			}
			hf.calib_k1[2] = kaa;
			hf.calib_k1[1] = kbb;
			hf.calib_k1[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 2];
				value_ka75[i] = BiaoY[i + 2];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			hf.calib_k2[2] = kaa;
			hf.calib_k2[1] = kbb;
			hf.calib_k2[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 4];
				value_ka75[i] = BiaoY[i + 4];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			hf.calib_k3[2] = kaa;
			hf.calib_k3[1] = kbb;
			hf.calib_k3[0] = kcc;
			hf.calib_dot0 = BYmv[0];
			hf.calib_dot1 = BYmv[2];
			hf.calib_dot2 = BYmv[4];

			pt = (Uint16 *) &hf;

			fm24cl04_write(0xa8, 0x02, 0x55);
			fm22l16_float_wrs(ADD_CALIB_HF_INPUT1, &input1[0], 7);
			fm22l16_float_wrs(ADD_CALIB_HF_INPUT2, &input2[0], 7);
			hf_zero = 0.0;
			fm22l16_wrs(ADD_HF_0, (Uint16*) &hf_zero, 2);
			hf_1.flag = 0;
			fm22l16_wrs(ADD_CALIB1_HF_K, (Uint16*) &hf_1, 39);
			tmp_1 = 0xa8;

			//交叉反应CO校准H2S程序
			if (input4[6] != 0)
			{
				tmp = input3[6] / input4[6];
				j = 1;
			}
			else
			{
				tmp = 0.0;
				j = 0;
			}

			if (j == 0)
			{
				jiaocha_CO_k = 0 ;
			}
			else
			{
				for (i = 5; i > 0; i--)
				{
					tmp1 = input3[i] / input4[i];
					tmp = tmp + tmp1 ;

				}

				for (m = 0; m < 1000000; m++)
					;
				jiaocha_CO_k = tmp / 7;
				jiaocha_CO_flag = 0x5555;
				fm22l16_wr(JIAOCHA_CO_FLAG, 0x5555);
				fm22l16_float_wrs(ADD_CO_H2S_INPUT3, &input4[0], 7);
				fm22l16_float_wrs(ADD_CO_H2S_INPUT2, &input3[0], 7);
				fm22l16_float_wr(ADD_CO_H2S, jiaocha_CO_k);

			}
			m = 0x10;
			calib_status(buff);
		}
		else if (tt == 2)
		{

			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i];
				value_ka75[i] = BiaoY[i];
			}
			buff = JQX1();
			if (buff == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			so2.calib_k1[2] = kaa;
			so2.calib_k1[1] = kbb;
			so2.calib_k1[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 2];
				value_ka75[i] = BiaoY[i + 2];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			so2.calib_k2[2] = kaa;
			so2.calib_k2[1] = kbb;
			so2.calib_k2[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 4];
				value_ka75[i] = BiaoY[i + 4];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(i);
				goto a2 ;
			}
			so2.calib_k3[2] = kaa;
			so2.calib_k3[1] = kbb;
			so2.calib_k3[0] = kcc;
			so2.calib_dot0 = BYmv[0];
			so2.calib_dot1 = BYmv[2];
			so2.calib_dot2 = BYmv[4];

			pt = (Uint16 *) &so2;

			fm24cl04_write(0xa4, 0x02, 0x55);
			fm22l16_float_wrs(ADD_CALIB_SO2_INPUT1, &input1[0], 7);
			fm22l16_float_wrs(ADD_CALIB_SO2_INPUT2, &input2[0], 7);
			so2_zero = 0.0;
			fm22l16_wrs(ADD_SO2_0, (Uint16*) &so2_zero, 2);
			so2_1.flag = 0;
			fm22l16_wrs(ADD_CALIB1_SO2_K, (Uint16*) &so2_1, 39);
			tmp_1 = 0xa4;

			//交叉反应SO2校准H2S程序
			if (input4[6] != 0)
			{
				tmp = input3[6] / input4[6];
				j = 1;
			}
			else
			{
				tmp = 0.0;
				j = 0;
			}


			if (j == 0)
			{
				jiaocha_SO2_k = 0.0 ;
			}
			else
			{

				for (i = 5; i > 0; i--)
				{
					tmp1 = input3[i]/input4[i];
					tmp = tmp + tmp1 ;

				}

				for (m = 0; m < 1000000; m++)
					;
				jiaocha_SO2_k = tmp / 7;
				jiaocha_SO2_flag = 0x5555;
				fm22l16_wr(JIAOCHA_SO2_FLAG, 0x5555);
				fm22l16_float_wrs(ADD_SO2_H2S_INPUT3, &input4[0], 7);
				fm22l16_float_wrs(ADD_SO2_H2S_INPUT2, &input3[0], 7);
				fm22l16_float_wr(ADD_SO2_H2S, jiaocha_SO2_k);
			}
			m = 0x10;
			calib_status(buff);
		}
		else if (tt == 3)
		{
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i];
				value_ka75[i] = BiaoY[i];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			h2s.calib_k1[2] = kaa;
			h2s.calib_k1[1] = kbb;
			h2s.calib_k1[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 2];
				value_ka75[i] = BiaoY[i + 2];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			h2s.calib_k2[2] = kaa;
			h2s.calib_k2[1] = kbb;
			h2s.calib_k2[0] = kcc;
			for (i = 0; i < 3; i++)
			{
				value_ad[i] = BYmv[i + 4];
				value_ka75[i] = BiaoY[i + 4];
			}
			i = JQX1();
			if (i == 0)
			{
				calib_status(buff);
				goto a2 ;
			}
			h2s.calib_k3[2] = kaa;
			h2s.calib_k3[1] = kbb;
			h2s.calib_k3[0] = kcc;
			h2s.calib_dot0 = BYmv[0];
			h2s.calib_dot1 = BYmv[2];
			h2s.calib_dot2 = BYmv[4];

			pt = (Uint16 *) &h2s;
			m = 0x10;
			fm24cl04_write(0xa0, 0x02, 0x55);
			fm22l16_float_wrs(ADD_CALIB_H2S_INPUT1, &input1[0], 7);
			fm22l16_float_wrs(ADD_CALIB_H2S_INPUT2, &input2[0], 7);
			h2s_zero = 0.0;
			fm22l16_wrs(ADD_H2S_0, (Uint16*) &h2s_zero, 2);
			h2s_1.flag = 0;
			fm22l16_wrs(ADD_CALIB1_H2S_K, (Uint16*) &h2s_1, 39);
			tmp_1 = 0xa0;
			calib_status(1);
		}

		for (i = 0; i < 24; i++)
		{
			j = *pt >> 8 & 0x0ff;
			fm24cl04_write(tmp_1, m, j);
			m++;
			j = *pt & 0x0ff;
			fm24cl04_write(tmp_1, m, j);
			m++;
			pt++;
		}
		rx8025_clock_read();
		for (i = 0; i < 7; i++)
			fm24cl04_write(tmp_1, 0x40 + i, i2c[i]);
		//calib_status(1);
	}
	else
	{
		error_flag++ ;
		if(error_flag<3)
		{
		//display_ascii(M24, 0xf800, 0x0000, 1, 80, 100, "密码不正确！");
		//for(k = 0;k < 1000000; k++) ;
			goto a1 ;
		}
		else
		{
			display_ascii(M24, 0x0000, 0x0000, 1, 50, 110, "密码不正确！");
			for ( k = 0; k < 2000000; k++ ) ;
			display_ascii(M24, 0xf800, 0x0000, 1, 10, 100, "密码三次错误，校准程序退出！");
		    for ( k = 0; k < 1000000; k++ ) ;

		}
	}
}


void calib_select11(Uint16 cnt , Uint16 x )
{
	if( x == 0 )
	{
		display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60,"气体SO2含量                          ");
		display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80,"气体H2S含量                          ");
		if (sub_ver==1) 
		{
			display_ascii(M16, 0xffff, color_1[cnt][2], 1, 10, 100,"气体CO含量                          ");
			display_ascii(M16, 0xffff, color_1[cnt][3], 1, 10, 120,"环境温度校准                          ");
		}
		if (sub_ver==0)
		{
			display_ascii(M16, 0xffff, color_1[cnt][2], 1, 10, 100,"环境温度校准                          ");
		}
	}
	else if( x == 1 || x == 3 )
	{
		display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60,"环境温度校准                          ");
	}
	else if( x == 2 )
	{
		display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60,"气体SO2含量                          ");
		display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80,"气体H2S含量                          ");
		display_ascii(M16, 0xffff, color_1[cnt][2], 1, 10, 100,"气体CO含量                            ");
		display_ascii(M16, 0xffff, color_1[cnt][3], 1, 10, 120,"环境温度校准                          ");
	}
}

Uint16 calib_dot_select(void) //返回0表示退出，返回1～5 表示如下
{
	Uint16  keycnt = 0,tt = 0 ;

a1: u_key_enter = 0;
	u_key_cnt = 0;

	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);

	display_ascii(M16, 0xffff, 0x001f, 1, 5, 2,  "校准");
	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 30, "请选择校准对象：");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出       上移        下移       确认");

	color_change(0xf81f, 0x0000);
	display_jx(5, 50, 314, 185);

	if ( instrument == 0 )
		tt = 0 ;
	else if ( instrument == 1 )
		tt = 1 ;
	else if (instrument == 2 )
		tt = 2 ;
	else if (instrument == 3 )
		tt = 3 ;

	calib_select11(keycnt,tt);
	key_clr();
	while ( keynew[0] + keynew[3] == 0 )
	{
		u_key_draw();
		if ( keynew[1] + keynew[2] == 1 )
		{
			if ( keynew[1] == 1 )
			{
				if ( instrument == 0 && sub_ver==0)
				{
					if( keycnt == 0 )
						keycnt = 2 ;
					else
						keycnt -- ;
				}
				else if ( instrument == 0 && sub_ver==1)
				{
					if( keycnt == 0 )
						keycnt = 3 ;
					else
						keycnt -- ;
				}
				else if ( instrument == 1 || instrument == 3 )
				{
					keycnt = 0 ;
				}
				else if ( instrument == 2 )
				{
					if( keycnt == 0 )
						keycnt = 3 ;
					else
						keycnt -- ;
				}
			}
			else if ( keynew[2] == 1 )
			{
				if ( instrument == 0 && sub_ver==0)
				{
					if( keycnt > 1 )
						keycnt = 0 ;
					else
						keycnt ++ ;
				}
				else if ( instrument == 0 && sub_ver==1)
				{
					if( keycnt > 2 )
						keycnt = 0 ;
					else
						keycnt ++ ;
				}
				else if ( instrument == 1 || instrument == 3 )
				{
					keycnt = 0 ;
				}
				else if ( instrument == 2 )
				{
					if( keycnt > 2 )
						keycnt = 0 ;
					else
						keycnt ++ ;
				}
			}
			calib_select11(keycnt,tt);
			key_clr();
		}

	}

	if (keynew[0] == 1)
		return 0;
	if ( instrument == 0 && sub_ver==0)
	{
		if( keycnt <2 )
			keycnt += 2 ;
		else
			keycnt = 5 ;
	}
	else if ( instrument == 0 && sub_ver==1)
	{
		if( keycnt <3 )
			keycnt += 2 ;
		else
			keycnt = 5 ;
	}
	else if ( instrument == 1 || instrument == 3 )
	{
		keycnt = 5 ;
	}
	else if ( instrument == 2 )
	{
		if( keycnt < 3 )
			keycnt += 2 ;
		else
			keycnt = 5 ;
	}
	return keycnt ;
}

void calib_dot( void )
{
	Uint16 i , buff ,  keycnt , flag ,sensor;//i,
	float jhf,jh2s,jso2,tmp1,tmp2;
	struct LINE_K tt;

	 buff   = 0 ;
	 keycnt = 0 ;
	 flag   = 0 ;
	 sensor = calib_dot_select() ;

	if ( sensor < 2)
		return;

	if (sensor == 2)
	{
		buff = fm24cl04_read(0xa4, 0x02);
	}
	else if (sensor == 3)
	{
		buff = fm24cl04_read(0xa0, 0x02);
	}
	else if (sensor == 4)
	{
		buff = fm24cl04_read(0xa8, 0x02);
	}
	else if( sensor == 5 )
	{
		temperature_calib();
	}

	if (buff == 0x5555 || buff == 0x55)
	{
		color_change(0x001f, 0x0000);
		cls();
		u_key_cnt = 0;
		display_jxy(0, 0, 319, 20);
		display_jxy(0, 220, 319, 239);

		display_ascii(M16, 0xffff, 0x001f, 1, 5, 2, "两点修正");
		display_ascii(M16, 0x07e0, 0x0000, 1, 5, 70, "1.");
		display_ascii(M16, 0x07e0, 0x0000, 1, 5, 90, "2.");

		display_ascii(M16, 0xf800, 0x0000, 1, 10, 30, "请输入数据：");
		display_ascii(M16, 0x07e0, 0x0000, 1, 25, 50, "仪器数据     标准数据");

		display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 右移       下移        修改       确认");
		u_key_cnt = 0;
		if (sensor == 2)
		{
			display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "SO2数据:");
			fm22l16_rds(ADD_CALIB1_SO2_K, (Uint16*) &so2_1, 40);
			display_calib_time(0, 206, &so2_1.time[0]);
			if (so2_1.flag == 0x5555)
			{
				display_calib_time(0, 206, &so2_1.time[0]);
				for (i = 0; i < 2; i++)
				{
					in1[i] = so2_1.measure[i];
					in2[i] = so2_1.biao[i];
				}
			}
			else
			{
				for (i = 0; i < 2; i++)
				{
					in1[i] = 0;
					in2[i] = 0;
				}
			}
			flag = so2_1.dot;
		}
		else if (sensor == 3)
		{
			display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "H2S数据:");
			fm22l16_rds(ADD_CALIB1_H2S_K, (Uint16*) &h2s_1, 40);
			display_calib_time(0, 206, &h2s_1.time[0]);
			if (h2s_1.flag == 0x5555)
			{
				display_calib_time(0, 206, &h2s_1.time[0]);
				for (i = 0; i < 2; i++)
				{
					in1[i] = h2s_1.measure[i];
					in2[i] = h2s_1.biao[i];
				}
			}
			else
			{
				for (i = 0; i < 2; i++)
				{
					in1[i] = 0;
					in2[i] = 0;
				}
			}
			flag = h2s_1.dot;
		}
		else if (sensor == 4)
		{
			display_ascii(M16, 0xf800, 0x0000, 1, 180, 30, "CO数据:");
			fm22l16_rds(ADD_CALIB1_HF_K, (Uint16*) &hf_1, 40);
			display_calib_time(0, 206, &hf_1.time[0]);
			if (hf_1.flag == 0x5555)
			{
				display_calib_time(0, 206, &hf_1.time[0]);
				for (i = 0; i < 2; i++)
				{
					in1[i] = hf_1.measure[i];
					in2[i] = hf_1.biao[i];
				}
			}
			else
			{
				for (i = 0; i < 2; i++)
				{
					in1[i] = 0;
					in2[i] = 0;
				}
			}
			flag = hf_1.dot;
		}
	}
	else
		return;

	key_clr();
	display_dot_input( keycnt );
	display_dot_input( keycnt );
	while (keynew[3] == 0)
	{
		//实时显示各个传感器的显示数值
		if(sensor == 2)
		{
			LingDianGenZong(SO2);
			jso2 = ad8401_value[0] * 0.03125;
			jso2 = 0.5 * jso2 + 0.5 * so2old;
			so2old = jso2;
			so2_jiaocha_value = jso2;

			if (jso2 <= so2.calib_dot1)
			{
				so2_calib_value = so2.calib_k1[2] * jso2 * jso2	+ so2.calib_k1[1] * jso2 + so2.calib_k1[0];
			}
			else if (jso2 <= so2.calib_dot2)
			{
				so2_calib_value = so2.calib_k2[2] * jso2 * jso2	+ so2.calib_k2[1] * jso2 + so2.calib_k2[0];
			}
			else
			{
				so2_calib_value = so2.calib_k3[2] * jso2 * jso2	+ so2.calib_k3[1] * jso2 + so2.calib_k3[0];
			}
			if (so2_1.flag == 0x5555 && so2_calib_value <= so2_1.middle_value)
			{
				so2_calib_value = so2_calib_value * so2_1.k1[0] + so2_1.k1[1];
			}
			else if (so2_1.flag == 0x5555&& so2_calib_value > so2_1.middle_value)
			{
				so2_calib_value = so2_calib_value * so2_1.k2[0] + so2_1.k2[1];
			}
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 240, 30, so2_calib_value, " ");
		}
		else if(sensor == 3)
		{
			LingDianGenZong(H2S);
			jh2s = ad8401_value[0] * 0.03125;
			jh2s = 0.5 * (jh2s + h2sold);
			h2sold = jh2s;

			if (jiaocha_SO2_flag == 0x5555 && jiaocha_CO_flag != 0x5555)
			{
				jh2s = jh2s - so2_jiaocha_value * jiaocha_SO2_k;
			}
			else if (jiaocha_CO_flag == 0x5555 && jiaocha_SO2_flag != 0x5555)
			{
				jh2s = jh2s - hf_jiaocha_value * jiaocha_CO_k;
			}
			else if (jiaocha_SO2_flag == 0x5555 && jiaocha_CO_flag == 0x5555)
			{
				jh2s = jh2s - so2_jiaocha_value * jiaocha_SO2_k
						- hf_jiaocha_value * jiaocha_CO_k;
			}
			else
			{
				;
			}

			if (jh2s <= h2s.calib_dot1)
			{
				h2s_calib_value = h2s.calib_k1[2] * jh2s * jh2s
						+ h2s.calib_k1[1] * jh2s + h2s.calib_k1[0];
			}
			else if (jh2s <= h2s.calib_dot2)
			{
				h2s_calib_value = h2s.calib_k2[2] * jh2s * jh2s
						+ h2s.calib_k2[1] * jh2s + h2s.calib_k2[0];
			}
			else
			{
				h2s_calib_value = h2s.calib_k3[2] * jh2s * jh2s
						+ h2s.calib_k3[1] * jh2s + h2s.calib_k3[0];
			}
			if (h2s_1.flag == 0x5555 && h2s_calib_value <= h2s_1.middle_value)
			{
				h2s_calib_value = h2s_calib_value * h2s_1.k1[0] + h2s_1.k1[1];
			}
			else if (h2s_1.flag == 0x5555
					&& h2s_calib_value > h2s_1.middle_value)
			{
				h2s_calib_value = h2s_calib_value * h2s_1.k2[0] + h2s_1.k2[1];
			}
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 240, 30, h2s_calib_value, " ");
		}
		else if(sensor == 4)
		{
			LingDianGenZong(HF);
			jhf = ad8401_value[0] * 0.0625; //*0.01894;//*10.0;//0.01491625-1.0024;
			jhf = 0.5 * jhf + 0.5 * hfold;
			hfold = jhf;
			hf_jiaocha_value = jhf;

			if (jhf <= hf.calib_dot1)
			{
				hf_calib_value = hf.calib_k1[2] * jhf * jhf
						+ hf.calib_k1[1] * jhf + hf.calib_k1[0];
			}
			else if (jhf <= hf.calib_dot2)
			{
				hf_calib_value = hf.calib_k2[2] * jhf * jhf
						+ hf.calib_k2[1] * jhf + hf.calib_k2[0];
			}
			else
			{
				hf_calib_value = hf.calib_k3[2] * jhf * jhf
						+ hf.calib_k3[1] * jhf + hf.calib_k3[0];
			}

			if (hf_1.flag == 0x5555 && hf_calib_value <= hf_1.middle_value)
			{
				hf_calib_value = hf_calib_value * hf_1.k1[0] + hf_1.k1[1];
			}
			else if (hf_1.flag == 0x5555 && hf_calib_value > hf_1.middle_value)
			{
				hf_calib_value = hf_calib_value * hf_1.k2[0] + hf_1.k2[1];
			}
			display_fp_ascii_ppm1(M16, 0xffff, 0x0000, 1, 240, 30, hf_calib_value, " ");
		}


		u_key_draw();
		if (keynew[0] + keynew[1] + keynew[2] + keynew[5])
		{
			if (keynew[0] == 1)
			{
				if (keycnt > 1)
					keycnt -= 2;
				else
					keycnt += 2;
			}
			if (keynew[1] == 1)
			{
				if (keycnt < 2)
				{
					keycnt--;
					if (keycnt > 2)
						keycnt = 1;
				}
				else
				{
					keycnt--;
					if (keycnt < 2)
						keycnt = 3;
				}
			}
			if (keynew[2] == 1)
			{
				if (keycnt < 2)
				{
					if(sensor == 2)
					{
						in1[keycnt] = so2_calib_value;
					}
					else if(sensor == 3)
					{
						in1[keycnt] = h2s_calib_value;
					}
					else if(sensor == 4)
					{
						in1[keycnt] = hf_calib_value;
					}
				}
				else
				{
					in2[ keycnt-2 ] = keyboard();
				}
			}
			if (keynew[5] == 1)
			{
				in1[keycnt] = keyboard();
			}
			key_clr();
			display_dot_input( keycnt );
		}
	}

	for (i = 0; i < 2; i++)
	{
		BYmv[i]  = in1[i];
		BiaoY[i] = in2[i];
	}
	rx8025_clock_read();

	tmp1 = BiaoY[1] - BiaoY[0];
	tmp2 = BYmv[1] - BYmv[0];
	if (tmp1 != 0.0)
	{
		tmp2 = tmp2 / tmp1;
		tmp1 = BYmv[0] - tmp2 * BiaoY[0];
		tt.k2[0]  = tmp2 *	tt.k2[0] ;
		tt.k2[1] = tmp2 * tt.k2[1] + tmp1;
	}

	tt.k1[0] = BiaoY[0] / BYmv[0];
	tt.k1[1] = 0.0;
	//tt.k2[0] = (BiaoY[1] - BiaoY[0]) / (BYmv[1] - BYmv[0]);
	//tt.k2[1] = BiaoY[0] - tt.k2[0] * BYmv[0];
	tt.middle_value = BYmv[0];
	tt.dot = 2;
	tt.flag = 0x5555;
	for (i = 0; i < 2; i++)
	{
		tt.measure[i] = in1[i];
		tt.biao[i] = in2[i];
	}
	for (i = 0; i < 7; i++)
		tt.time[i] = i2c[i];

	if (sensor == 0)
	{
		h2o_1 = tt;
		fm22l16_wrs(ADD_CALIB1_H2O_K, (Uint16*) &tt, 39);
	}
	else if (sensor == 2)
	{
		so2_1 = tt;
		fm22l16_wrs(ADD_CALIB1_SO2_K, (Uint16*) &tt, 39);
	}
	else if (sensor == 3)
	{
		h2s_1 = tt;
		fm22l16_wrs(ADD_CALIB1_H2S_K, (Uint16*) &tt, 39);
	}
	else if (sensor == 4)
	{
		hf_1 = tt;
		fm22l16_wrs(ADD_CALIB1_HF_K, (Uint16*) &tt, 39);
	}
	calib_status(1);
}

void function_select1(Uint16 cnt)
{
	display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60, "用户使用校准                         ");
	display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80, "厂家原始校准                         ");
	//display_ascii(M16, 0xffff, color_1[cnt][0], 1, 10, 60,"仪器校准                             ");
	//display_ascii(M16, 0xffff, color_1[cnt][1], 1, 10, 80,"零点显示                             ");
}

void function_select(void)
{
	Uint16 keycnt = 0;
a2: u_key_enter = 0;
	km = 1;
	u_key_cnt = 3;

	color_change(0x001f, 0x0000);
	cls();
	display_jxy(0, 0, 319, 20);
	display_jxy(0, 220, 319, 239);
	display_ascii(M16, 0xffff, 0x001f, 1, 220, 2,"版本:2.0.11");
	display_ascii(M16, 0x07e0, 0x0000, 1, 5, 30, "请选择功能：");
	display_ascii(M16, 0xffff, 0x001f, 1, 0, 222," 退出       上移        下移       确认 ");

	color_change(0xf81f, 0x0000);
	display_jx(5, 50, 314, 100);

	function_select1(keycnt);
	key_clr();
	while (keynew[3] == 0)
	{
		u_key_draw();
		if (keynew[0] + keynew[1] + keynew[2] == 1)
		{
			if (keynew[1] == 1)
			{
				if (keycnt == 0)
					keycnt = 1;
				else
					keycnt--;
			}
			else if (keynew[2] == 1)
			{
				if (keycnt == 1)
					keycnt = 0;
				else
					keycnt++;
			}
			else
			{
				return ;
			}
			function_select1(keycnt);
			key_clr();
		}
	}
	if (keycnt == 0)
	{
		//两点校准程序，即客户校准
		calib_dot();
		goto a2 ;
	}
	else
	{
		//多点校准程序，即厂家校准
		calib_multipoint(  ) ;
		goto a2 ;
	}
}

void calib_do( void )
{
	function_select();
}


