#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

const float flow_tab[][13] =
{
//00    05     10     15     20     25     30     35     40     45    50     50    55
1.224, 1.485, 1.784, 2.055, 2.277, 2.467, 2.637, 2.789, 2.930, 3.064, 3.18, 3.18, 3.293, //SF6
1.227, 1.256, 1.296, 1.343, 1.398, 1.454, 1.514, 1.573, 1.631, 1.686, 1.74, 1.74, 1.794, //N2
1.30,  1.40,  1.50,  1.61,  1.74,  1.85,  1.94,  2.03,  2.11,  2.19,  2.27, 2.33, 2.40,//CO2
};
void flow_alarm(void)
{
	if (flw.bar < 1)
	{
		flw.alarm = 0;
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 470, 40, "流量低");
	}
	else if (flw.bar >= 6)
	{
		flw.alarm = 1;
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 470, 40, "流量高");
	}
}
void get_flow(struct FLOW * pt)
{
	float tmp;

	fp32_ad_get(CH_FLOW,&tmp);
	tmp*= (4.096 / 65536.0);
	pt->val = tmp/FLW_MEASURE + pt->val*(FLW_MEASURE-1)/FLW_MEASURE;

	if (pt->val>10) pt->val=10;
	else if (pt->val<0) pt->val=0;
	
	if (pt->val < 1.30)
		pt->flag = 1;
	else
		pt->flag = 0;

	if (pt->val <= flow_tab[plat.gas][1])
		pt->bar = 0;
	else if (pt->val <= flow_tab[plat.gas][2])
		pt->bar = 1;
	else if (pt->val <= flow_tab[plat.gas][3])
		pt->bar = 2;
	else if (pt->val <= flow_tab[plat.gas][4])
		pt->bar = 3;
	else if (pt->val <= flow_tab[plat.gas][5])
		pt->bar = 4;
	else if (pt->val <= flow_tab[plat.gas][6])
		pt->bar = 5;
	else if (pt->val <= flow_tab[plat.gas][7])
		pt->bar = 6;
	else if (pt->val <= flow_tab[plat.gas][8])
		pt->bar = 7;
}
//温湿度采集值
void convert_sh11(struct SENSOR * temp_pt, float * humidity_pt)
{
	Uint16 ret;

	//ret=sht71_ht(humidity_pt,&temp_pt->val);
	if (ret!=0){temp_pt->val=100.0;*humidity_pt=100.0;}
}
//H2S采集值归一化、平均取值
void convert_h2s(struct SENSOR * pt)
{
	float tmp;
	//LingDianGenZong(H2S);
	fp32_ad_get(H2S,&tmp);
	tmp*=0.03125;
	pt->val=tmp/H2S_MEASURE+pt->val*(H2S_MEASURE-1)/H2S_MEASURE;
}
//SO2采集值归一化、平均取值
void convert_so2(struct SENSOR * pt)
{
	float tmp;
	//LingDianGenZong(SO2);
	fp32_ad_get(SO2,&tmp);
	tmp*=0.03125;
	pt->val=tmp/SO2_MEASURE+pt->val*(SO2_MEASURE-1)/SO2_MEASURE;
}
//CO采集值归一化、平均取值
void convert_co(struct SENSOR * pt)
{
	float tmp;
	//LingDianGenZong(CO);
	fp32_ad_get(CO,&tmp);
	tmp*=0.0625;
	pt->val=tmp/CO_MEASURE+pt->val*(CO_MEASURE-1)/CO_MEASURE;
}
//微水采集值归一化、平均取值
void convert_h2o(struct SENSOR * pt)
{
	float tmp;
	fp32_ad_get(H2O,&tmp);
	tmp*=(4.096/65536.0);
	tmp=25.0 * tmp -80.0;

	pt->val=tmp/H2O_MEASURE+pt->val*(H2O_MEASURE-1)/H2O_MEASURE;

	if (pt->val<-100) pt->val=-100;
	else if (pt->val>100) pt->val=100;
}
//纯度采集值归一化、平均取值
void convert_purity(struct SENSOR * pt)
{
	float tmp;
	//get_Concentration(&tmp);
	pt->val=tmp/PUR_MEASURE+pt->val*(PUR_MEASURE-1)/PUR_MEASURE;

	if (pt->val < 0.0)
		pt->val = 0.0;
	else if (pt->val > 99.99)
		pt->val = 99.99;
}
//SO2取最终值
void get_so2(struct SENSOR * pt)
{
	convert_so2(pt);
	calib7(pt);
	calib_usr(pt);
	if (so2.calib_val>=100) so2.calib_val=100;
	else if (so2.calib_val<=0) so2.calib_val=0;
}
//H2S取最终值
void get_h2s(struct SENSOR * pt,struct SENSOR * so2_pt,struct SENSOR * co_pt)
{
	convert_so2(so2_pt);
	convert_h2s(pt);
	convert_co(co_pt);
	calib_cross(pt,so2_pt,co_pt);
	calib7(pt);
	calib_usr(pt);
	if (h2s.calib_val>=100) h2s.calib_val=100;
	else if (co.calib_val<=0) h2s.calib_val=0;
}
//CO取最终值
void get_co(struct SENSOR * pt)
{
	convert_so2(pt);
	calib7(pt);
	calib_usr(pt);
	if (co.calib_val>=500) co.calib_val=500;
	else if (co.calib_val<=0) co.calib_val=0;
}
//微水取最终值
void get_h2o(struct SENSOR * pt)
{
	convert_h2o(pt);
	calib7(pt);
}
//纯度取最终值
void get_purity(struct SENSOR * pt)
{
	convert_purity(pt);
	calib7(pt);
}
//温湿度传感器最终值
void get_sht11(struct SENSOR * temp_pt, float * humidity_pt)
{	
	convert_sh11(temp_pt,humidity_pt);
	temp_pt->calib_val=temp_pt->calib_usr.k*(temp_pt->val-temp_pt->calib_usr.realdot)+temp_pt->calib_usr.stddot;
	if (temp_pt->calib_val>=99) temp_pt->calib_val=99;
	else if (temp_pt->calib_val<=-99) temp_pt->calib_val=-99;
	if (*humidity_pt>=100) *humidity_pt=100;
	else if (*humidity_pt<=0) *humidity_pt=0;
}