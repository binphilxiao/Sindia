#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//七点厂家校准程序
void calib7(struct SENSOR * pt)
{
	if (pt->val <= pt->calib_vender.dot1)
	{
		pt->calib_val = pt->calib_vender.k1[2] * pt->val * pt->val + pt->calib_vender.k1[1] * pt->val + pt->calib_vender.k1[0];
	}
	else if (pt->val <= pt->calib_vender.dot2)
	{
		pt->calib_val = pt->calib_vender.k2[2] * pt->val * pt->val + pt->calib_vender.k2[1] * pt->val + pt->calib_vender.k2[0];
	}
	else
	{
		pt->calib_val = pt->calib_vender.k3[2] * pt->val * pt->val + pt->calib_vender.k3[1] * pt->val + pt->calib_vender.k3[0];
	}
}
//两点用户校准程序
void calib_usr(struct SENSOR * pt)
{
	if (pt->calib_usr_flag == 0x5555)
	{
		pt->calib_val = (pt->calib_val-pt->calib_usr.realdot) * pt->calib_usr.k + pt->calib_usr.stddot;
	}
}
//H2S 交叉校准程序
void calib_cross(struct SENSOR * pt,struct SENSOR * so2_pt,struct SENSOR * co_pt)
{
	if (pt->calib_cross_so2_flag == 0x5555)
		pt->val = pt->val - so2_pt->val * pt->calib_cro.cross_so2_k;
	else if (pt->calib_cross_co_flag  == 0x5555)
		pt->val = pt->val - co_pt->val * pt->calib_cro.cross_co_k;
	else if (pt->calib_cross_so2_flag == 0x5555&&pt->calib_cross_co_flag == 0x5555)
		pt->val = pt->val - so2_pt->val * pt->calib_cro.cross_so2_k-co.val * pt->calib_cro.cross_co_k;
}

Uint16 JQX3(struct CALIB_7 * pt, float * in1, float * in2)
{
	float buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7;
	float w, x, y, z, kaa, kbb, kcc;
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
		buffer1 = buffer1 + *(in1+i); //ci1
		buffer2 = buffer2 + *(in1+i) * *(in1+i); //ci2
		buffer3 = buffer3 + *(in1+i) * *(in1+i) * *(in1+i); //ci3
		buffer4 = buffer4
				+ *(in1+i) * *(in1+i) * *(in1+i) * *(in1+i); //ci4
		buffer5 = buffer5 + *(in2+i) * *(in1+i) * *(in1+i); //aici2
		buffer6 = buffer6 + *(in2+i) * *(in1+i); //aici1
		buffer7 = buffer7 + *(in2+i); //ai
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
		return (0);
	}
	kaa = y / z;
	w = points * buffer6 - buffer7 * buffer1;
	x = buffer1 * buffer2 - points * buffer3;
	x = kaa * x;
	y = w + x;
	z = points * buffer2 - buffer1 * buffer1;
	if (z == 0)
	{
		return (0);
	}
	kbb = y / z;
	y = buffer7 - kbb * buffer1 - kaa * buffer2;
	kcc = y / points;

	pt->k1[2] = kaa;
	pt->k1[1] = kbb;
	pt->k1[0] = kcc;

	return (1);
}

Uint16 JQX5(struct CALIB_7 * pt, float * in1, float * in2)
{
	float buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7;
	float w, x, y, z, kaa, kbb, kcc;
	Uint16 i,j;
	Uint16 points = 3;

	buffer1 = 0;
	buffer2 = 0;
	buffer3 = 0;
	buffer4 = 0;
	buffer5 = 0;
	buffer6 = 0;
	buffer7 = 0;

	for (j=0;j<3;j+=2)
	{
		for (i = 0; i < points; i++)
		{
			buffer1 = buffer1 + *(in1+i+j); //ci1
			buffer2 = buffer2 + *(in1+i+j) * *(in1+i+j); //ci2
			buffer3 = buffer3 + *(in1+i+j) * *(in1+i+j) * *(in1+i+j); //ci3
			buffer4 = buffer4
					+ *(in1+i+j) * *(in1+i+j) * *(in1+i+j) * *(in1+i+j); //ci4
			buffer5 = buffer5 + *(in2+i+j) * *(in1+i+j) * *(in1+i+j); //aici2
			buffer6 = buffer6 + *(in2+i+j) * *(in1+i+j); //aici1
			buffer7 = buffer7 + *(in2+i+j); //ai
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
			return (0);
		}
		kaa = y / z;
		w = points * buffer6 - buffer7 * buffer1;
		x = buffer1 * buffer2 - points * buffer3;
		x = kaa * x;
		y = w + x;
		z = points * buffer2 - buffer1 * buffer1;
		if (z == 0)
		{
			return (0);
		}
		kbb = y / z;
		y = buffer7 - kbb * buffer1 - kaa * buffer2;
		kcc = y / points;

		if (j==0)
		{
			pt->k1[2] = kaa;
			pt->k1[1] = kbb;
			pt->k1[0] = kcc;
			pt->dot0=*in1;
		}
		else if (j==2)
		{
			pt->k2[2] = kaa;
			pt->k2[1] = kbb;
			pt->k2[0] = kcc;
			pt->dot0=*(in1+2);
		}
	}

	return (1);
}

Uint16 JQX7(struct CALIB_7 * pt, float * in1, float * in2)
{
	float buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7;
	float w, x, y, z, kaa, kbb, kcc;
	Uint16 i,j;
	Uint16 points = 3;

	buffer1 = 0;
	buffer2 = 0;
	buffer3 = 0;
	buffer4 = 0;
	buffer5 = 0;
	buffer6 = 0;
	buffer7 = 0;

	for (j=0;j<5;j+=2)
	{
		for (i = 0; i < points; i++)
		{
			buffer1 = buffer1 + *(in1+i+j); //ci1
			buffer2 = buffer2 + *(in1+i+j) * *(in1+i+j); //ci2
			buffer3 = buffer3 + *(in1+i+j) * *(in1+i+j) * *(in1+i+j); //ci3
			buffer4 = buffer4
					+ *(in1+i+j) * *(in1+i+j) * *(in1+i+j) * *(in1+i+j); //ci4
			buffer5 = buffer5 + *(in2+i+j) * *(in1+i+j) * *(in1+i+j); //aici2
			buffer6 = buffer6 + *(in2+i+j) * *(in1+i+j); //aici1
			buffer7 = buffer7 + *(in2+i+j); //ai
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
			return (0);
		}
		kaa = y / z;
		w = points * buffer6 - buffer7 * buffer1;
		x = buffer1 * buffer2 - points * buffer3;
		x = kaa * x;
		y = w + x;
		z = points * buffer2 - buffer1 * buffer1;
		if (z == 0)
		{
			return (0);
		}
		kbb = y / z;
		y = buffer7 - kbb * buffer1 - kaa * buffer2;
		kcc = y / points;

		if (j==0)
		{
			pt->k1[2] = kaa;
			pt->k1[1] = kbb;
			pt->k1[0] = kcc;
			pt->dot0=*in1;
		}
		else if (j==2)
		{
			pt->k2[2] = kaa;
			pt->k2[1] = kbb;
			pt->k2[0] = kcc;
			pt->dot0=*(in1+2);
		}
		else if (j==4)
		{
			pt->k3[2] = kaa;
			pt->k3[1] = kbb;
			pt->k3[0] = kcc;
			pt->dot0=*(in1+4);
		}
	}

	return (1);
}
