#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
float h2sold, h2s_calib_value, h2s_k[3];
float h2oold, h2o_calib_value, h2o_k[3];
float c_temp, c_temp_calib,c_humid, flwold;
float so2old, so2_calib_value, so2_k[3],so2_jiaocha_value;
float hfold, hf_calib_value, hf_k[3],hf_jiaocha_value;
float sf6old, sf6_calib_value, sf6_k[3];
float gpaold, gpa_calib_value;

Uint16 flow_alarm_flag;
Uint16 f_cnt, in8;
void flow_alarm_display(Uint16 type, Uint16 Switch) //type---HIGH,LOW;switch---ON,OFF
{
	Uint16 i;
	f_cnt++;
	i = f_cnt & 0x01;
	if (Switch == ON)
	{
		if (type == LOW && i == 1)
		{
			display_ascii(M12, 0xf800, 0x0000, 1, 280, 207, "流量低");
			HEATER(0);
		}
		else if (type == HIGH && i == 1)
		{
			display_ascii(M12, 0xf800, 0x0000, 1, 280, 207, "流量高");
			spk_start();
			HEATER(0);
		}
		else if (type == HIGH && i == 0)
		{
			display_ascii(M12, 0x0000, 0x0000, 1, 280, 207, "流量高");
			HEATER(0);
		}
	}
	else if (Switch == OFF)
	{
		display_ascii(M12, 0x0000, 0x0000, 1, 280, 207, "流量高");
		HEATER(1);
	}
}

const float flow_tab[][13] =
{
//00    05     10     15     20     25     30     35     40     45    50     50    55
1.224, 1.485, 1.784, 2.055, 2.277, 2.467, 2.637, 2.789, 2.930, 3.064, 3.18, 3.18, 3.293, //SF6
1.227, 1.256, 1.296, 1.343, 1.398, 1.454, 1.514, 1.573, 1.631, 1.686, 1.74, 1.74, 1.794, //N2
1.30,  1.40,  1.50,  1.61,  1.74,  1.85,  1.94,  2.03,  2.11,  2.19,  2.27, 2.33, 2.40,//CO2
};

Uint16 flow_flag, flow_flag1 = 0;
char *flow_table1[] =
{ "00 ", "05 ", "10 ", "15 ", "20 ", "25 ", "30 ", "35 ", "40 ", "45 ", "50 " };
void flow_do(void)
{
	float flw;
	Uint16 i;

	fp32_ad_get(FLOW);
	flw = ad8401_value[0] * (4.096 / 65536.0);
	flw = 0.5 * flw + 0.5 * flwold;
	if (flw < 1.30)
		flow_flag = 1;
	else
		flow_flag = 0;
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

	if (in8 < 1)
	{
		flow_alarm_flag = 0;
		flow_alarm_display(LOW, ON);
	}
	else if (in8 >= 1 && in8 <= 7)
	{
		flow_alarm_flag = 1;
		flow_alarm_display(HIGH, OFF);
	}
	else
	{
		flow_alarm_flag = 2;
		flow_alarm_display(HIGH, ON);
	}

	for (i = 0; i < 9; i++)
	{
		if (i <= in8)
			display_ascii(M12, 0x5d18, 0xffe0, 1, 298, 24 + 16 * (9 - i), flow_table1[i]);
		else
			display_ascii(M12, 0x5d18, 0x0000, 1, 298, 24 + 16 * (9 - i), flow_table1[i]);
	}
}
float so2_0, h2s_0;
void so2_do(void)
{
	float jso2;
	Uint16 i, tem;

	LingDianGenZong(SO2);
	jso2 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
	jso2 = 0.5 * jso2 + 0.5 * so2old;
	so2old = jso2;

	if (jso2 <= so2.calib_dot1)
	{
		so2_calib_value = so2.calib_k1[2] * jso2 * jso2 + so2.calib_k1[1] * jso2 + so2.calib_k1[0];
	}
	else if (jso2 <= so2.calib_dot2)
	{
		so2_calib_value = so2.calib_k2[2] * jso2 * jso2 + so2.calib_k2[1] * jso2 + so2.calib_k2[0];
	}
	else
	{
		so2_calib_value = so2.calib_k3[2] * jso2 * jso2 + so2.calib_k3[1] * jso2 + so2.calib_k3[0];
	}

	if (so2_1.flag == 0x5555 && so2_calib_value <= so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k1[0] + so2_1.k1[1];
	}
	else if (so2_1.flag == 0x5555 && so2_calib_value > so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k2[0] + so2_1.k2[1];
	}

	if (so2_calib_value < 0.0)
		so2_calib_value = 0.0;

	if (so2_calib_value <= 100.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 35, 80, so2_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 35, 80, so2_calib_value, " PPM");

	tem = (Uint16) (so2_calib_value * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 35, 116,  "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 209, 116, "100PPM");
	color_change(0x07e0, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(35 + i * 7, 108, 40 + i * 7, 115);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(35 + i * 7, 108, 40 + i * 7, 115);
			color_change(0x07e0, 0x0000);
			display_jx(35 + i * 7, 108, 40 + i * 7, 115);
		}
	}
}

void h2s_do(void)
{
	float jh2s;
	Uint16 i, tem;

	LingDianGenZong(H2S);
	jh2s = ad8401_value[0] * 0.03125;
	jh2s = 0.5 * jh2s + 0.5 * h2sold;
	h2sold = jh2s;

	if (jiaocha_SO2_flag == 0x5555&&jiaocha_CO_flag != 0x5555)
	{
		jh2s = jh2s - (so2_jiaocha_value * jiaocha_SO2_k) ;
	}
	else if(jiaocha_CO_flag == 0x5555&&jiaocha_SO2_flag != 0x5555)
	{
		jh2s = jh2s - (hf_jiaocha_value * jiaocha_CO_k);
	}
	else if(jiaocha_SO2_flag == 0x5555&&jiaocha_CO_flag == 0x5555)
	{
		jh2s = jh2s - (so2_jiaocha_value * jiaocha_SO2_k) - (hf_jiaocha_value * jiaocha_CO_k);
	}
	else
	{
		 ;
	}

	if (jh2s <= h2s.calib_dot1)
	{
		h2s_calib_value = h2s.calib_k1[2] * jh2s * jh2s + h2s.calib_k1[1] * jh2s + h2s.calib_k1[0];
	}
	else if (jh2s <= h2s.calib_dot2)
	{
		h2s_calib_value = h2s.calib_k2[2] * jh2s * jh2s + h2s.calib_k2[1] * jh2s + h2s.calib_k2[0];
	}
	else
	{
		h2s_calib_value = h2s.calib_k3[2] * jh2s * jh2s + h2s.calib_k3[1] * jh2s + h2s.calib_k3[0];
	}

	if (h2s_1.flag == 0x5555 && h2s_calib_value <= h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k1[0] + h2s_1.k1[1];
	}
	else if (h2s_1.flag == 0x5555 && h2s_calib_value > h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k2[0] + h2s_1.k2[1];
	}

	if (h2s_calib_value < 0.0)
		h2s_calib_value = 0.0;
	if (h2s_calib_value <= 50.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 35, 140, h2s_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 35, 140, h2s_calib_value, " PPM");

	tem = (Uint16) (h2s_calib_value * 0.6);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 35, 176, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 215, 176, "50PPM");
	color_change(0xffe0, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(35 + i * 7, 168, 40 + i * 7, 175);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(35 + i * 7, 168, 40 + i * 7, 175);
			color_change(0xffe0, 0x0000);
			display_jx(35 + i * 7, 168, 40 + i * 7, 175);
		}
	}
}

void h2o_do(void)
{
	float jh2o;
	Uint16 i, tem;
	fp32_ad_get(H2O);
	jh2o = ad8401_value[0] * (4.096 / 65536.0);
	jh2o = 0.5 * jh2o + 0.5 * h2oold;
	h2oold = jh2o;
	jh2o = 25.0 * jh2o - 80.0;
	if (h2o_1.flag == 0x5555)
	{
		if (jh2o <= h2o.calib_dot1)
			h2o_calib_value = h2o.calib_k1[2] * jh2o * jh2o + h2o.calib_k1[1] * jh2o + h2o.calib_k1[0];
		else
			h2o_calib_value = h2o.calib_k2[2] * jh2o * jh2o + h2o.calib_k2[1] * jh2o + h2o.calib_k2[0];
	}
	else
		h2o_calib_value = jh2o;

	tem = (Uint16) ((h2o_calib_value + 80.0) * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 35, 56, "-80℃");
	display_ascii(M12, 0x5d18, 0x0, 1, 215, 56, "+20℃");
	color_change(0xffff, 0x0000);

	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(35 + i * 7, 48, 40 + i * 7, 55);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(35 + i * 7, 48, 40 + i * 7, 55);
			color_change(0xffff, 0x0000);
			display_jx(35 + i * 7, 48, 40 + i * 7, 55);
		}
	}
}
void so2_do_1(Uint16 number)
{
	float jso2;
	Uint16 i, tem;

	fp32_ad_get(SO2);
	LingDianGenZong(SO2);
	jso2 = ad8401_value[0] * 0.03125;
	jso2 = 0.5 * jso2 + 0.5 * so2old;
	so2old = jso2;
	so2_jiaocha_value = jso2 ;

	if (jso2 <= so2.calib_dot1)
	{
		so2_calib_value = so2.calib_k1[2] * jso2 * jso2 + so2.calib_k1[1] * jso2 + so2.calib_k1[0];
	}
	else if (jso2 <= so2.calib_dot2)
	{
		so2_calib_value = so2.calib_k2[2] * jso2 * jso2 + so2.calib_k2[1] * jso2 + so2.calib_k2[0];
	}
	else
	{
		so2_calib_value = so2.calib_k3[2] * jso2 * jso2 + so2.calib_k3[1] * jso2 + so2.calib_k3[0];
	}
	if (so2_1.flag == 0x5555 && so2_calib_value <= so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k1[0] + so2_1.k1[1];
	}
	else if (so2_1.flag == 0x5555 && so2_calib_value > so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k2[0] + so2_1.k2[1];
	}

	if (so2_calib_value < 0.0)
		so2_calib_value = 0.0;
	if (so2_calib_value <= 100.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 25 + number * 65, so2_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 25 + number * 65, so2_calib_value, " PPM");

	tem = (Uint16) (so2_calib_value * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 56 + number * 65, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 230, 56 + number * 65, "100PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		}
	}
}

void h2s_do_1(Uint16 number)
{
	float jh2s,jhf,jso2;
	Uint16 i, tem;

	fp32_ad_get(H2S);
	LingDianGenZong(H2S);
	jh2s = ad8401_value[0] * 0.03125;
	jh2s = 0.5 * (jh2s + h2sold);
	h2sold = jh2s;

	fp32_ad_get(SO2);
	jso2 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
	jso2 = 0.5 * jso2 + 0.5 * so2old;
	so2old = jso2;

	fp32_ad_get(HF);
	jhf = ad8401_value[0] * 0.0625; //*0.003342*10.0;//0.01491625-1.0024;
	jhf = 0.5 * jhf + 0.5 * hfold;
	hfold = jhf;


	if ( jiaocha_SO2_flag == 0x5555 )
		jh2s = jh2s - jso2 * jiaocha_SO2_k;
	else if ( jiaocha_CO_flag  == 0x5555 )
		jh2s = jh2s - jhf * jiaocha_CO_k;
	else if ( jiaocha_SO2_flag == 0x5555 && jiaocha_CO_flag == 0x5555 )
		jh2s = jh2s - jso2 * jiaocha_SO2_k-jhf * jiaocha_CO_k;

	if (jh2s <= h2s.calib_dot1)
	{
		h2s_calib_value = h2s.calib_k1[2] * jh2s * jh2s + h2s.calib_k1[1] * jh2s + h2s.calib_k1[0];
	}
	else if (jh2s <= h2s.calib_dot2)
	{
		h2s_calib_value = h2s.calib_k2[2] * jh2s * jh2s + h2s.calib_k2[1] * jh2s + h2s.calib_k2[0];
	}
	else
	{
		h2s_calib_value = h2s.calib_k3[2] * jh2s * jh2s + h2s.calib_k3[1] * jh2s + h2s.calib_k3[0];
	}
	if (h2s_1.flag == 0x5555 && h2s_calib_value <= h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k1[0] + h2s_1.k1[1];
	}
	else if (h2s_1.flag == 0x5555 && h2s_calib_value > h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k2[0] + h2s_1.k2[1];
	}

	if (h2s_calib_value < 0.0)
	{
		h2s_calib_value = 0.0;
	}
	if (h2s_calib_value <= 50.0)
	{
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 25 + number * 65, h2s_calib_value, " PPM");
	}
	else
	{
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 25 + number * 65, h2s_calib_value, " PPM");
	}
	//   display_fp_number(M12,0xf800,0x0000,1,66,180,h2s_zero);
	tem = (Uint16) (h2s_calib_value * 0.6);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 56 + number * 65, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 236, 56 + number * 65, "50PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		}
	}
}

void hf_do_1(Uint16 number)
{
	float jhf;
	Uint16 i, tem;

	fp32_ad_get(HF);
	LingDianGenZong(HF);
	jhf = ad8401_value[0] * 0.0625;
	jhf = 0.5 * jhf + 0.5 * hfold;
	hfold = jhf;
	hf_jiaocha_value = jhf ;

	if (jhf <= hf.calib_dot1)
	{
		hf_calib_value = hf.calib_k1[2] * jhf * jhf + hf.calib_k1[1] * jhf + hf.calib_k1[0];
	}
	else if (jhf <= hf.calib_dot2)
	{
		hf_calib_value = hf.calib_k2[2] * jhf * jhf + hf.calib_k2[1] * jhf + hf.calib_k2[0];
	}
	else
	{
		hf_calib_value = hf.calib_k3[2] * jhf * jhf + hf.calib_k3[1] * jhf + hf.calib_k3[0];
	}
	if (hf_1.flag == 0x5555 && hf_calib_value <= hf_1.middle_value)
	{
		hf_calib_value = hf_calib_value * hf_1.k1[0] + hf_1.k1[1];
	}
	else if (hf_1.flag == 0x5555 && hf_calib_value > hf_1.middle_value)
	{
		hf_calib_value = hf_calib_value * hf_1.k2[0] + hf_1.k2[1];
	}

	if (hf_calib_value < 0.0)
		hf_calib_value = 0.0;
	if (hf_calib_value <= 500.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 25 + number * 65, hf_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 25 + number * 65, hf_calib_value, " PPM");

	tem = (Uint16) (hf_calib_value * 0.06);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 56 + number * 65,  "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 230, 56 + number * 65, "500PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 49 + number * 65, 61 + i * 7, 54 + number * 65);
		}
	}
}

void so2_do_0(Uint16 number)
{
	float jso2;
	Uint16 i, tem;

	LingDianGenZong(SO2);
	jso2 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
	jso2 = 0.5 * jso2 + 0.5 * so2old;
	so2old = jso2;

	if (jso2 <= so2.calib_dot1)
	{
		so2_calib_value = so2.calib_k1[2] * jso2 * jso2 + so2.calib_k1[1] * jso2 + so2.calib_k1[0];
	}
	else if (jso2 <= so2.calib_dot2)
	{
		so2_calib_value = so2.calib_k2[2] * jso2 * jso2 + so2.calib_k2[1] * jso2 + so2.calib_k2[0];
	}
	else
	{
		so2_calib_value = so2.calib_k3[2] * jso2 * jso2 + so2.calib_k3[1] * jso2 + so2.calib_k3[0];
	}

	if (so2_1.flag == 0x5555 && so2_calib_value <= so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k1[0] + so2_1.k1[1];
	}
	else if (so2_1.flag == 0x5555 && so2_calib_value > so2_1.middle_value)
	{
		so2_calib_value = so2_calib_value * so2_1.k2[0] + so2_1.k2[1];
	}

	if (so2_calib_value < 0.0)
		so2_calib_value = 0.0;
	if (so2_calib_value <= 100.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 21 + number * 46,
				so2_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 21 + number * 46,
				so2_calib_value, " PPM");
	tem = (Uint16) (so2_calib_value * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 53 + number * 46, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 230, 53 + number * 46, "100PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		}
	}
}

void h2s_do_0(Uint16 number)
{
	float jh2s,jso2,jhf;
	Uint16 i, tem;

	LingDianGenZong(H2S);
	jh2s = ad8401_value[0] * 0.03125; //*0.003342*10.0;//0.01491625-1.0024;
	jh2s = 0.5 * jh2s + 0.5 * h2sold;
	h2sold = jh2s;


	fp32_ad_get(SO2);
	jso2 = ad8401_value[0] * 0.03125; //*0.011364*10.0;//0.01491625-1.0024;
	jso2 = 0.5 * jso2 + 0.5 * so2old;
	so2old = jso2;

	fp32_ad_get(HF);
	jhf = ad8401_value[0] * 0.0625; //*0.003342*10.0;//0.01491625-1.0024;
	jhf = 0.5 * jhf + 0.5 * hfold;
	hfold = jhf;


	if (jiaocha_SO2_flag == 0x5555)
		jh2s = jh2s - jso2 * jiaocha_SO2_k;
	else if (jiaocha_CO_flag  == 0x5555)
		jh2s = jh2s - jhf * jiaocha_CO_k;
	else if (jiaocha_SO2_flag == 0x5555 && jiaocha_CO_flag == 0x5555)
		jh2s = jh2s - jso2 * jiaocha_SO2_k-jhf * jiaocha_CO_k;


	if (jh2s <= h2s.calib_dot1)
	{
		h2s_calib_value = h2s.calib_k1[2] * jh2s * jh2s + h2s.calib_k1[1] * jh2s + h2s.calib_k1[0];
	}
	else if (jh2s <= h2s.calib_dot2)
	{
		h2s_calib_value = h2s.calib_k2[2] * jh2s * jh2s + h2s.calib_k2[1] * jh2s + h2s.calib_k2[0];
	}
	else
	{
		h2s_calib_value = h2s.calib_k3[2] * jh2s * jh2s + h2s.calib_k3[1] * jh2s + h2s.calib_k3[0];
	}
	if (h2s_1.flag == 0x5555 && h2s_calib_value <= h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k1[0] + h2s_1.k1[1];
	}
	else if (h2s_1.flag == 0x5555 && h2s_calib_value > h2s_1.middle_value)
	{
		h2s_calib_value = h2s_calib_value * h2s_1.k2[0] + h2s_1.k2[1];
	}


	if (h2s_calib_value < 0.0)
		h2s_calib_value = 0.0;
	if (h2s_calib_value <= 50.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 21 + number * 46, h2s_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 21 + number * 46, h2s_calib_value, " PPM");
	tem = (Uint16) (h2s_calib_value * 0.6);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 53 + number * 46, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 236, 53 + number * 46, "50PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		}
	}
}

void hf_do_0(Uint16 number)
{
	float jhf;
	Uint16 i, tem;

	LingDianGenZong(HF);
	jhf = ad8401_value[0] * 0.0625; //*0.01894;//*10.0;//0.01491625-1.0024;
	jhf = 0.5 * jhf + 0.5 * hfold;
	hfold = jhf;

	if (jhf <= hf.calib_dot1)
	{
		hf_calib_value = hf.calib_k1[2] * jhf * jhf + hf.calib_k1[1] * jhf + hf.calib_k1[0];
	}
	else if (jhf <= hf.calib_dot2)
	{
		hf_calib_value = hf.calib_k2[2] * jhf * jhf + hf.calib_k2[1] * jhf + hf.calib_k2[0];
	}
	else
	{
		hf_calib_value = hf.calib_k3[2] * jhf * jhf + hf.calib_k3[1] * jhf + hf.calib_k3[0];
	}
	if (hf_calib_value < 0.0)
		hf_calib_value = 0.0;

	if (hf_1.flag == 0x5555 && hf_calib_value <= hf_1.middle_value)
	{
		hf_calib_value = hf_calib_value * hf_1.k1[0] + hf_1.k1[1];
	}
	else if (hf_1.flag == 0x5555 && hf_calib_value > hf_1.middle_value)
	{
		hf_calib_value = hf_calib_value * hf_1.k2[0] + hf_1.k2[1];
	}

	if (hf_calib_value <= 500.0)
		display_fp_ascii_ppm1(M24, 0xffff, 0x0000, 1, 56, 21 + number * 46, hf_calib_value, " PPM");
	else
		display_fp_ascii_ppm1(M24, 0xf800, 0x0000, 1, 56, 21 + number * 46, hf_calib_value, " PPM");

	tem = (Uint16) (hf_calib_value * 0.06);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 53 + number * 46, "0.1PPM");
	display_ascii(M12, 0x5d18, 0x0, 1, 230, 53 + number * 46, "500PPM");
	color_change(D_color, 0x0000);
	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		}
	}
}

void h2o_do_0(Uint16 number)
{
	float jh2o;
	Uint16 i, tem;
	fp32_ad_get(H2O);
	jh2o = ad8401_value[0] * (4.096 / 65536.0);
	jh2o = 0.5 * jh2o + 0.5 * h2oold;
	h2oold = jh2o;
	jh2o = 25.0 * jh2o - 80.0;
	if (h2o_1.flag == 0x5555)
	{
		if (jh2o <= h2o.calib_dot1)
			h2o_calib_value = h2o.calib_k1[2] * jh2o * jh2o + h2o.calib_k1[1] * jh2o + h2o.calib_k1[0];
		else
			h2o_calib_value = h2o.calib_k2[2] * jh2o * jh2o + h2o.calib_k2[1] * jh2o + h2o.calib_k2[0];
	}
	else
		h2o_calib_value = jh2o;

	tem = (Uint16) ((h2o_calib_value + 80.0) * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 53 + number * 46, "-80℃");
	display_ascii(M12, 0x5d18, 0x0, 1, 236, 53 + number * 46, "+20℃");
	color_change(D_color, 0x0000);

	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 46 + number * 46, 61 + i * 7, 51 + number * 46);
		}
	}
}

float gpa_zero;
void gpa100_do(void)
{
	float jgpa;
	Uint16 i, tem;

	jgpa = get_Concentration();
	jgpa = 0.8 * jgpa + 0.2 * gpaold;
	jgpa = jgpa - gpa_zero;
	gpaold = jgpa;

	if (jgpa <= gpa.calib_dot1)
	{
		gpa_calib_value = gpa.calib_k1[2] * jgpa * jgpa + gpa.calib_k1[1] * jgpa + gpa.calib_k1[0];
	}
	else if (jgpa <= gpa.calib_dot2)
	{
		gpa_calib_value = gpa.calib_k2[2] * jgpa * jgpa + gpa.calib_k2[1] * jgpa + gpa.calib_k2[0];
	}
	else
	{
		gpa_calib_value = gpa.calib_k3[2] * jgpa * jgpa + gpa.calib_k3[1] * jgpa + gpa.calib_k3[0];
	}
	if (gpa_calib_value < 0.0)
		gpa_calib_value = 0.0;
	else if (gpa_calib_value > 99.99)
		gpa_calib_value = 99.99;

	display_gpa(M64, 0xffff, 0x0000, 1, 15, 50, gpa_calib_value, " %");

}
void gpa_do_0(Uint16 number)
{
	float jgpa;
	Uint16 i, tem;

	jgpa = get_Concentration();
	jgpa = 0.8 * jgpa + 0.2 * gpaold;
	jgpa = jgpa - gpa_zero;
	gpaold = jgpa;

	if (jgpa <= gpa.calib_dot1)
	{
		gpa_calib_value = gpa.calib_k1[2] * jgpa * jgpa + gpa.calib_k1[1] * jgpa
				+ gpa.calib_k1[0];
	}
	else if (jgpa <= gpa.calib_dot2)
	{
		gpa_calib_value = gpa.calib_k2[2] * jgpa * jgpa + gpa.calib_k2[1] * jgpa
				+ gpa.calib_k2[0];
	}
	else
	{
		gpa_calib_value = gpa.calib_k3[2] * jgpa * jgpa + gpa.calib_k3[1] * jgpa
				+ gpa.calib_k3[0];
	}
	if (gpa_calib_value < 0.0)
		gpa_calib_value = 0.0;
	else if (gpa_calib_value > 99.99)
		gpa_calib_value = 99.99;

	display_gpa(M24, 0xffff, 0x0000, 1, 52, 21 + number * 46, gpa_calib_value,
			" %");

	tem = (Uint16) (gpa_calib_value * 0.3);
	if (tem > 30)
		tem = 30;
	display_ascii(M12, 0x5d18, 0x0, 1, 56, 53 + number * 46, "0.0%");
	display_ascii(M12, 0x5d18, 0x0, 1, 230, 53 + number * 46, "100.0%");
	color_change(D_color, 0x0000);

	for (i = 0; i < 30; i++)
	{
		if (i <= tem)
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7,
					51 + number * 46);
		else
		{
			color_change(0x0000, 0x0000);
			display_jxy(56 + i * 7, 46 + number * 46, 61 + i * 7,
					51 + number * 46);
			color_change(D_color, 0x0000);
			display_jx(56 + i * 7, 46 + number * 46, 61 + i * 7,
					51 + number * 46);
		}
	}
}

Uint16 const ppxy[] =
{ 0x1b, 0x27, 0x07, 0x0a, 0x28, 0x46, 0x64, 0x82, 0xa0, 0xbe, 0x0d, 0xff };
Uint16 const ppxy1[] =
{ 0x1b, 0x27, 0x07, 0x00, 0x0a, 0x00, 0x37, 0x00, 0x64, 0x00, 0x91, 0x00, 0xbe,
		0x00, 0xeb, 0x00, 0x18, 0x01, 0x0d, 0xff };
Uint16 const f[] =
{ 0x1c, 0x4c, 40, 0x20, 0x0a, 0x00 };
Uint16 const w[] =
{ 0x1c, 0x4d, 60, 4, 0x00 };

