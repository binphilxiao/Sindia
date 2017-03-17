#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

struct CLOCK clock;
struct CLOCK measure_time;
struct PLATFORM plat;

struct SENSOR h2o;
struct SENSOR so2;
struct SENSOR h2s;
struct SENSOR co;
struct SENSOR purity;
struct SENSOR temprature;
float humidity;
struct FLOW flw;
float ppm,ppm20;

Uint16 const color[7][7] =
{ YELLOW, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
  WHITE, YELLOW, WHITE, WHITE, WHITE, WHITE, WHITE,
  WHITE, WHITE, YELLOW, WHITE, WHITE, WHITE, WHITE,
  WHITE, WHITE, WHITE, YELLOW, WHITE, WHITE, WHITE,
  WHITE, WHITE, WHITE, WHITE, YELLOW, WHITE, WHITE,
  WHITE, WHITE, WHITE, WHITE, WHITE, YELLOW, WHITE,
  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, YELLOW,
};
char *week_string[]={"ÐÇÆÚÈÕ","ÐÇÆÚÒ»","ÐÇÆÚ¶þ","ÐÇÆÚÈý","ÐÇÆÚËÄ","ÐÇÆÚÎå","ÐÇÆÚÁù"};
char *flow_lable[] =
{ "00 ", "05 ", "10 ", "15 ", "20 ", "25 ", "30 ", "35 ", "40 ", "45 ", "50 " };

void battery_do(Uint16 blink)
{
	Uint16 k;
	float vi,vb;

	fp32_ad_get(VOUT,&vb);vb=vb*409.6/65536.0;
	fp32_ad_get(IOUT,&vi);vi=vi*409.6/65536.0;
	vi=((Uint16)vi);vi=vi/100.0;vb=((Uint16)vb);vb=vb/100.0;

	if (vb>=1.64)//18.0v  *2/22 ³äµç¹ý³ÌÖÐÓÃµçÁ÷ÅÐ¶Ï
	{
		if(vi<0.2){k=4;}
		else if(vi<1.0){k=3;}
		else if(vi<2.0){k=2;}
		else if(vi<3.0){k=1;}
		else{k=0;}
	}
	else//·Åµç¹ý³ÌÖÐÓÃµçÑ¹ÅÐ¶Ï
	{
		if(vb>1.45){k=4;}          //>15.9v*2/22 
		else if(vb>1.31){k=3;}     //>14.4v*2/22 
		else if(vb>1.14){k=2;}     //>12.5v*2/22 
		else if(vb>1.09){k=1;}     //>12.0v*2/22 
		else {k=0;}                //>11.5v*2/22 
	}

	if (k==0)         display_picture(63, 58 , 8, 102, 31, 450, 9);
	else if (k==1) display_picture(63, 58 , 8, 102, 31, 450, 9);
	else if (k==2) display_picture(63, 122, 8, 166, 31, 450, 9);
	else if (k==3) display_picture(63, 186, 8, 229, 31, 450, 9);
	else if (k==4) display_picture(63, 249, 8, 291, 31, 450, 9);
	
	
	if(k==0)HEATER(0);
	else HEATER(1);
}


/*
str: ÌáÊ¾×Ö·û´®
dly:ÑÓ³ÙºÁÃëÊý
*/
void promote(char * str, Uint16 dly)
{
	Uint32 i,j;
	display_ascii(M24, 0xf800, BLACK, 1, 100, 150, str);
	for (i = 0; i < dly; i++)
		for (j=0;j<1846;j++)
			;
}

void display_page(struct MEASURE_DATA * data)
{
	Uint16 i;
	char s[17];
     
	i=data->number[0]+data->number[1]*10+data->number[2]*100+data->number[3]*1000;//±àºÅ
	display_number(M32,WHITE,BLUE_GREEN,1,90,40+45*1,i);

	s[0]=0x30+2;//ÈÕÆÚ
	s[1]=0x30+0;
	i=data->clk.year/10;
	if (i>9) i=9;
	s[2]=i+0x30;
	i=data->clk.year%10;
	s[3]=i+0x30;
	s[4]='.';
	i=data->clk.month/10;
	s[5]=i+0x30;
	i=data->clk.month%10;
	s[6]=i+0x30;
	s[7]='.';
	i=data->clk.date/10;
	s[8]=i+0x30;
	i=data->clk.date%10;
	s[9]=i+0x30;
	s[10]='\0';
	display_ascii(M32,0xffff,0x0000,1,90,40+45*2,&s[2]);


	i=data->clk.hour/10;//Ê±¼ä
	s[0]=i+0x30;
	i=data->clk.hour%10;
	s[1]=i+0x30;
	s[2]=':';
	i=data->clk.minute/10;
	s[3]=i+0x30;
	i=data->clk.minute%10;
	s[4]=i+0x30;
	s[5]='\0';  
	display_ascii(M32,0xffff,0x0000,1,90,40+45*3,&s[0]);

	display_fp_ascii(M32,0xffff,0x0000,1,90,40+45*4,data->c_temp,6,"¡æ");//ÎÂ¶ÈºÍÊª¶È
	display_fp_ascii(M32,0xffff,0x0000,1,90,40+45*5,data->c_humid,6,"%");//x<999.999

	display_ascii(M32,0xffff,0x0000,1,90,40+45*6,flow_lable[data->flw]);
	display_ascii(M32,0xffff,0x0000,1,90+48,40+45*6,"L/h");

	display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*1,data->h2o_calib_value,6," ¡æ");
	if(data->h2oppm<6033.0)display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*2,data->h2oppm,6,"uL/L");
	else display_ascii(M32,0xffff,0x0000,1,320,40+45*2,">6033uL/L");
	if(data->h2oppm20<6033.0)display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*3,data->h2oppm20,6,",20¡æ");
	else display_ascii(M32,0xffff,0x0000,1,320,40+45*3,"*********");
	display_gpa(M32,0xffff,0x0000,1,320,40+45*4,data->pur_calib_value," %");

	display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*5,data->h2s_calib_value,6,"PPM");
	display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*6,data->so2_calib_value,6,"PPM");
	display_fp_ascii(M32,0xffff,0x0000,1,320,40+45*7,data->hf_calib_value,6,"PPM");


} 

void measure_buffer_init(Uint16 chl)
{
	unsigned int j;	
	if(chl==SO2)
	{
		for(j=0;j<50;j++)
		{	 	
			fp32_ad_get(SO2,so2.zero_spy+j);
		}
		so2.measure_old=so2.zero_spy[49];
	}
	else if(chl==H2S)
	{
		for(j=0;j<50;j++)
		{	 	
			fp32_ad_get(H2S,h2s.zero_spy+j);
		}
		h2s.measure_old=h2s.zero_spy[49];
	}
	else if(chl==CO)
	{
		for(j=0;j<50;j++)
		{	 	
			fp32_ad_get(CO,co.zero_spy+j);
		}
		co.measure_old=co.zero_spy[49];
	}
}

void measure_buffer_fill(Uint16 chl)
{
	unsigned int i;

	if(chl==SO2)
	{
		for(i=0;i<49;i++)so2.zero_spy[i]=so2.zero_spy[i+1];
		fp32_ad_get(SO2,so2.zero_spy+49);
	}
	else if(chl==H2S)
	{
		for(i=0;i<49;i++)h2s.zero_spy[i]=h2s.zero_spy[i+1];
		fp32_ad_get(H2S,h2s.zero_spy+49);
	}
	else if(chl==CO	)
	{
		for(i=0;i<49;i++)co.zero_spy[i]=co.zero_spy[i+1];
		fp32_ad_get(CO,co.zero_spy+49);
	}	
}


void LingDianGenZong(Uint16 chl)     //Áãµã¸ú×Ù
{
	float sum,temp2,zerotemp,temp1,reameasure;
	int i;	
		
	measure_buffer_fill(chl);	

	if(chl==SO2)
	{
		sum=0.0;
		for(i=0;i<20;i++)
		{
			sum=sum+so2.zero_spy[i];
		}
		zerotemp=sum*0.05;	
		sum=0.0;
		for(i=30;i<50;i++)
		{
			sum=so2.zero_spy[i]+sum;
		}
		reameasure=0.05*sum; 
		temp1=reameasure-zerotemp;
		temp2=reameasure-so2.measure_old; 	   
		if ((fabs(temp1)<so2.spy_min)&&(fabs(temp2)<so2.measure_min))
		{
			so2.zero_count=0;
			//  so2_zero=so2_zero+temp2; 
		}
		so2.measure_old=reameasure;
		if(flw.flag==0)so2.val=so2.measure_old-so2.zero;
		else so2.val=so2.calib_vender.dot0*32.0;
	}
	else if(chl==H2S)
	{
		sum=0.0;
		for(i=0;i<20;i++)
		sum=sum+h2s.zero_spy[i];
		zerotemp=sum*0.05;	
		sum=0.0;
		for(i=30;i<50;i++)
		sum=h2s.zero_spy[i]+sum;
		reameasure=0.05*sum;
		temp1=reameasure-zerotemp;
		temp2=reameasure-h2s.measure_old; 
		if ((fabs(temp1)<h2s.spy_min)&&(fabs(temp2)<h2s.measure_min))
		{
			h2s.zero_count=0;
			//   h2s_zero=h2s_zero+temp2; 
		}
		h2s.measure_old=reameasure;
		
		if(flw.flag==0)h2s.val=h2s.measure_old-h2s.zero;
		else h2s.val=h2s.calib_vender.dot0*32.0;
	}
	else if(chl==CO)
	{
		sum=0.0;
		for(i=0;i<20;i++)
		sum=sum+co.zero_spy[i];
		zerotemp=sum*0.05;	
		sum=0.0;
		for(i=30;i<50;i++)
		sum=co.zero_spy[i]+sum;
		reameasure=0.05*sum;
		temp1=reameasure-zerotemp;
		temp2=reameasure-co.measure_old;   
		if ((fabs(temp1)<co.spy_min)&&(fabs(temp2)<co.measure_min))
		{
			co.zero_count=0;
			//   hf_zero=hf_zero+temp2; 
		}
		co.measure_old=reameasure;
		if(flw.flag==0)co.val=co.measure_old-co.zero;
		else co.val=co.calib_vender.dot0*16.0;
	}
}
void zero_set1(Uint16 chl)//0---´¿¶Èµ÷Áã  1---SO2µ÷Áã   2---H2Sµ÷Áã 3---COµ÷Áã 4---hfµ÷Áã
{

	if(chl==2)
	{
		h2s.zero=h2s.measure_old-h2s.calib_vender.dot0*32.0;
		fm22l16_wrs(ADD_H2S_ZERO,(Uint16*)&h2s.zero,sizeof(float)/2);
	}
	else if(chl==1)
	{
		so2.zero=so2.measure_old-so2.calib_vender.dot0*32.0;
		fm22l16_wrs(ADD_SO21_ZERO,(Uint16*)&so2.zero,sizeof(float)/2);
	}
	else if(chl==3)
	{
		co.zero=co.measure_old-co.calib_vender.dot0*16.0;
		fm22l16_wrs(ADD_CO_ZERO,(Uint16*)&co.zero,sizeof(float)/2);
	}
	else if(chl==0)
	{
		//get_Concentration(&purity.zero);
		purity.zero=purity.zero-purity.calib_vender.dot0;
		fm22l16_wrs(ADD_PUR_ZERO,(Uint16*)&purity.zero,sizeof(float)/2);
	}
}

Uint16 zero_set_ca100(void)
{
	//if(flw.flag==1)return ERROR;

	cls();
	disp_background(8);
	key_clr();
	
	while(key.keynew[7]==0 && key.keynew[1]==0)
	{
		disp_ca100(1);
		if(key.keynew[3]==1)
			{zero_set1(4);key_clr();}
		else if (key.keynew[4]==1)
			{zero_set1(3);key_clr();}
		else if (key.keynew[5]==1)
			{zero_set1(2);key_clr();}
		else if (key.keynew[6]==1)
			{zero_set1(1);key_clr();}
	}
	return OK;
}

void null_point_display()
{
	cls();
	disp_background(13);

	display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 60, "Ð£×¼ÆøÌåÔ­Ê¼Êý¾Ý");

	display_ascii(M24, WHITE, BLUE_GREEN, 1, 50,   120, "Â¶µãÎÂ¶È:");
	display_ascii(M24, WHITE, BLUE_GREEN, 1, 50,   180, "ÆøÌå´¿¶È:");
	display_ascii(M24, WHITE, BLUE_GREEN, 1, 300,  120, "SO2ÆøÌå:");
	display_ascii(M24, WHITE, BLUE_GREEN, 1, 300,  180, "H2SÆøÌå:");
	display_ascii(M24, WHITE, BLUE_GREEN, 1, 300,  240, "COÆøÌå:");

	key_clr();
	while ( key.keynew[7] == 0)
	{
		convert_h2o(&h2o);
		convert_so2(&so2);
		convert_h2s(&h2s);
		convert_co(&co);
 		convert_purity(&purity);
		display_fp_ascii(M24, WHITE, BLUE_GREEN, 1, 170, 120, h2o.val,6, "¡æ");
		display_fp_ascii(M24, WHITE, BLUE_GREEN, 1, 170, 180, purity.val,6, "%");
		display_fp_ascii(M24, WHITE, BLUE_GREEN, 1, 400, 120, so2.val,6, "");
		display_fp_ascii(M24, WHITE, BLUE_GREEN, 1, 400, 180, h2s.val,6, "");
		display_fp_ascii(M24, WHITE, BLUE_GREEN, 1, 400, 240, co.val,6, "");
        }
	key_clr();
}

Uint16 password_adj(void) //0---½øÈë£×¼ 1---ÃÜÂëÉèÖÃÍê³É
{
	Uint16 error_flag;
	Uint32 password1,password,password_old,i;

	fm22l16_rds(ADD_PASSWORD,(Uint16 *)&password_old,sizeof(Uint32));

a0:	cls();
	disp_background(10);

	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÊäÈëÃÜÂë£º              ");

	key_clr();
	error_flag = 0 ;
	while ( key.keynew[7] == 0)
	{
a1:		if (key.keynew[1] == 1)
		{
a2:			key_clr();
			display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÊäÈë¾ÉµÄÃÜÂë£¡          ");
			for (i = 0; i < 2000000; i++) ;
			display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÊäÈë¾ÉµÄÃÜÂë£¡          ");
			keyboard_int(630-288,470-320, &password) ;

			if( password == password_old || password == 588588 )
			{
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂëÕýÈ·£¡                      ");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂëÕýÈ·£¡                      ");
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÊäÈëÐÂµÄÃÜÂë£¡     ");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÊäÈëÐÂµÄÃÜÂë£¡     ");
				keyboard_int(630-288,470-320, &password) ;

				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÔÙÊäÈëÒ»´ÎÃÜÂë£¡");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÇëÔÙÊäÈëÒ»´ÎÃÜÂë£¡");
				keyboard_int(630-288,470-320, &password1) ;

				if( password1 == password )
				{
					password_old = password ;
					fm22l16_wrs(ADD_PASSWORD,(Uint16 *)&password,sizeof(Uint32));
					display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÉèÖÃÃÜÂëÍê³É£¡                           ");
					for (i = 0; i < 2000000; i++) ;
					goto a0 ;
				}
				else
				{
					display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "Á½´ÎÃÜÂë²»Í¬ÇëÖØÐÂÊäÈë£¡");
					for (i = 0; i < 2000000; i++) ;
					goto a2 ;
				}
			}
			else
			{
				error_flag++ ;
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂë²»ÕýÈ·£¡                                         ");
				for (i = 0; i < 2000000; i++) ;
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂë²»ÕýÈ·£¡                                         ");
				if( error_flag<3 )
				{
					goto a2 ;
				}
				else
				{
					display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂëÈý´Î²»ÕýÈ·£¬ÉèÖÃÍË³ö£¡");
					for (i = 0; i < 2000000; i++) ;
					display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂëÈý´Î²»ÕýÈ·£¬ÉèÖÃÍË³ö£¡");
					goto a1 ;
				}
			}
		}
		else if (key.keynew[7] == 1)
			return 0;
		else if(key.keynew[2] == 1)
		{
			keyboard_int(630-288,470-320, &password) ;
			key_clr();
			if(password == password_old || password == 588588 )
			{
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂëÕýÈ·£¡                                   ");
				for (i = 0; i < 2000000; i++) ;
				return 1 ;
			}
			else
			{
				display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂë²»ÕýÈ·£¡                              ");
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

	display_ascii(M12, 0x07e0, BLACK, 1, x, y, "ÉÏ´ÎÐ£×¼Ê±¼ä:");
	s[0] = 0x30 + 2; //ÈÕÆÚ
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
	i = pt[2] >> 4 & 0x0f; //Ê±¼ä
	s[11] = i + 0x30;
	i = pt[2] & 0x0f;
	s[12] = i + 0x30;
	s[13] = ':';
	i = pt[1] >> 4 & 0x0f;
	s[14] = i + 0x30;
	i = pt[1] & 0x0f;
	s[15] = i + 0x30;
	s[16] = '\0';
	display_ascii(M12, 0x07e0, BLACK, 1, x + 90, y, &s[0]);
}

void disp_table7x3(Uint16 foc,Uint16 sensor,float * input1,float * input2, float * input3)
{
	Uint16 i; //j, t1 = 0;

	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 80,  "1.");
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 120,  "2.");
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 160, "3.");
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 200, "4.");
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 240, "5.");
	if(sensor!=0)
	{
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 280, "6.");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 320, "7.");
	}

	if( sensor == 0 )
	{
	    for ( i = 0; i < 5; i++ )
	    {
	        if (foc < 5)
	        {
	        	display_fp_ascii(M32, color[foc][i], BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
	        	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 300, 80+40*i, *(input2+i),4, " ");
	        }
	        else
	        {
	        	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
	        	display_fp_ascii(M32, color[foc-5][i], BLUE_GREEN, 1, 300, 80+40*i, *(input2+i),4, " ");
	        }
	    }
	}
	else if( sensor == 1 || sensor == 3 )
	{
	    for (i = 0; i < 7; i++)
	    {
	        if (foc < 7)
	        {
	        	display_fp_ascii(M32, color[foc][i],BLUE_GREEN , 1, 100, 80+40*i, *(input1+i),4, " ");
	        	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 300, 80+40*i, *(input2+i),4, " ");
	        }
	        else
	        {
	        	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
	        	display_fp_ascii(M32, color[foc-7][i], BLUE_GREEN, 1, 300, 80+40*i, *(input2+i),4, " ");
	        }
	    }
	}
	else if(sensor == 2 || sensor == 4 )
	{
		for (i = 0; i < 7; i++)
		{
		    if (foc < 7)
		    {
		    	display_fp_ascii(M32, color[foc][i], BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
		        display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 220, 80+40*i, *(input2+i),4, " ");
		        display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 340, 80+40*i, *(input3+i),4, " ");
		    }
		    else if( foc < 14 )
		    {
		        display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
		        display_fp_ascii(M32, color[foc-7][i], BLUE_GREEN, 1, 220, 80+40*i, *(input2+i),4, " ");
		        display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 340, 80+40*i, *(input3+i),4, " ");
		    }
		    else
		    {
		    	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 80+40*i, *(input1+i),4, " ");
		    	display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 220, 80+40*i, *(input2+i),4, " ");
		    	display_fp_ascii(M32, color[foc-14][i],BLUE_GREEN , 1, 340, 80+40*i, *(input3+i),4, " ");
		    }
		 }
	}
}

void disp_table2x2(Uint16 cnt, float * line1, float * line2)
{
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 160, "1.");
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 220, "2.");
	
	display_fp_ascii(M32, color[cnt][0], BLUE_GREEN, 1, 150, 160, *(line1+0),6, " ");
	display_fp_ascii(M32, color[cnt][1], BLUE_GREEN, 1, 150, 220, *(line1+1),6, " ");
	display_fp_ascii(M32, color[cnt][2], BLUE_GREEN, 1, 300, 160, *(line2+0),6, " ");
	display_fp_ascii(M32, color[cnt][3], BLUE_GREEN, 1, 300, 220, *(line2+1),6, " ");
}

void calib_temperature(void)
{
	Uint16  buff =0,i,keycnt = 0;
	float input1[2],input2[2];

       buff = fm22l16_rd( ADD_CALIBVENDER_TEMP_FLAG );
	if (buff == 0x5555)
	{
		fm22l16_rds(ADD_CALIBVENDER_TEMP_INPUT1, (Uint16*)input1, sizeof(float)*2);
		fm22l16_rds(ADD_CALIBVENDER_TEMP_INPUT2 , (Uint16*)input2, sizeof(float)*2);
	}
	else
	{
		for (i = 0; i < 2; i++)
		{
			input1[i] = 0;
			input2[i] = 0;
		}
	}

a1:	cls();
	disp_background(14);
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "ÎÂ¶È:");
	key_clr();

       while (key.keynew[7] == 0 && key.keynew[2] == 0)
	{

		sht71_ht(&humidity,&temprature.val);
		display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, temprature.val,4, " ");
		disp_table2x2(keycnt, input1, input2);
		if (key.keynew[4] + key.keynew[5])
		{
			if (keycnt >=2)
				keycnt -= 2;
			else
				keycnt += 2;
			key_clr();
		}
		if (key.keynew[0] + key.keynew[1])
		{
			if (keycnt <=1)
			{
				if (keycnt==1) keycnt=0;
				else keycnt=1;
			}
			else
			{
				if (keycnt==3) keycnt=2;
				else keycnt=3;
			}
			key_clr();
		}
		if (key.keynew[3])
		{
			if (keycnt <=1)
			{
				input1[keycnt] = temprature.val;
			}
			else
			{
				keyboard_flt(630-288,470-320,input2+keycnt-2);
				goto a1;
			}
			key_clr();
		}
		if (key.keynew[6])
		{
			if (keycnt <=1)
			{
				keyboard_flt(630-288,470-320,input1+keycnt);
				goto a1;
			}
		}
	}
	if (key.keynew[7])
		return;
	else if (key.keynew[2])
	{
		fm22l16_wrs(ADD_CALIBVENDER_TEMP_INPUT1, (Uint16*)input1, sizeof(float)*2);
		fm22l16_wrs(ADD_CALIBVENDER_TEMP_INPUT2, (Uint16*)input2, sizeof(float)*2);
		rx8025_clock_read(&temprature.calib_usr.clk);
		if (input1[1]!=input1[0])
		{
			fm22l16_wr(ADD_CALIBVENDER_TEMP_FLAG, 0x5555);
			temprature.calib_usr.k=(input2[1]-input2[0])/(input1[1]-input1[0]);
			temprature.calib_usr.realdot=input1[0];
			temprature.calib_usr.stddot=input2[0];
			fm22l16_wrs(ADD_CALIBVENDER_TEMP, (Uint16*)&temprature.calib_usr, sizeof(struct CALIB_2));
			promote("Ð£×¼³É¹¦", 2000);
		}
		else
			promote("Ð£×¼Ê§°Ü", 2000);
	}
}


Uint16 calib_vender_select(void) //·µ»Ø0±íÊ¾ÍË³ö£¬·µ»Ø1¡«5 ±íÊ¾ÈçÏÂ
{
	Uint16  keycnt = 0;

	cls();
	disp_background(13);
	
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 60, "ÇëÑ¡ÔñÐ£×¼¶ÔÏó£º");

	key_clr();
	while (key.keynew[7] + key.keynew[2] == 0)
	{
		display_ascii(M32, color[keycnt][0], BLUE_GREEN, 1, 100, 100, "ÆøÌåÂ¶µã                              ");
		display_ascii(M32, color[keycnt][1], BLUE_GREEN, 1, 100, 140, "ÆøÌå´¿¶È                              ");
		display_ascii(M32, color[keycnt][2], BLUE_GREEN, 1, 100, 180,"ÆøÌåSO2º¬Á¿                          ");
		display_ascii(M32, color[keycnt][3], BLUE_GREEN, 1, 100, 220,"ÆøÌåH2Sº¬Á¿                          ");
		display_ascii(M32, color[keycnt][4], BLUE_GREEN, 1, 100, 260,"ÆøÌåCOº¬Á¿                            ");
		display_ascii(M32, color[keycnt][5], BLUE_GREEN, 1, 100, 300,"»·¾³ÎÂ¶ÈÐ£×¼                          ");
		
		if (key.keynew[0] + key.keynew[1] == 1)
		{
			if (key.keynew[0] == 1)
			{
				if (keycnt == 0)
					keycnt = 5;
				else
					keycnt--;
			}
			else if (key.keynew[1] == 1)
			{
				if (keycnt == 5)
					keycnt = 0;
				else
					keycnt++;
			}
			key_clr();
		}
	}
	if (key.keynew[2] == 1)
		return keycnt;
	return 6;
}

void calib_vender_main( void )
{
	Uint16 ret,buff,keycnt, tt,password_flag,error_flag;
	Uint32 i ;
	float tmp,tmp1;
	float sensor1,sensor2;
	float input1[7],input2[7],input3[7];

	keycnt = 0;
	error_flag = 0 ;
a1:	password_flag = password_adj() ;

	if (password_flag==0)
	{
		error_flag++ ;
		if(error_flag<3)
		{
			goto a1 ;
		}
		else
		{
			display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110, "ÃÜÂë²»ÕýÈ·£¡                                            ");
			for ( i = 0; i < 2000000; i++ ) ;
			display_ascii(M32, WHITE, BLUE_GREEN, 1, 50, 110,"ÃÜÂëÈý´Î´íÎó£¬Ð£×¼³ÌÐòÍË³ö£¡");
		    	for ( i = 0; i < 1000000; i++ ) ;
			return ;
		}
	}

a2:	tt = calib_vender_select() ;
	if ( tt >= 6 )
	{
		return ;
	}
	else if ( tt == 5 )
	{
		calib_temperature();
		goto a2;
	}

	for (i = 0; i < 7; i++)
	{
		input1[i] = 0;
		input2[i] = 0;
		input3[i] = 0;
	}
		
	if (tt == 0)//H2O
	{
		ret = fm22l16_rd(ADD_CALIBVENDER_H2O_FLAG);
		if (ret==0x5555)
		{
				fm22l16_rds(ADD_CALIBVENDER_H2O_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
				fm22l16_rds(ADD_CALIBVENDER_H2O_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
		}
	} 
	else if (tt == 1)//´¿¶È
	{
		ret = fm22l16_rd(ADD_CALIBVENDER_PUR_FLAG);
		if (ret==0x5555)
		{
				fm22l16_rds(ADD_CALIBVENDER_PUR_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
				fm22l16_rds(ADD_CALIBVENDER_PUR_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
		}
	} 
	else if (tt == 2)//SO2
	{
		ret = fm24cl04_read(0xa4, ADD24_CALIB7_FLAG, &buff);
		if( buff == 0x55 )
		{
			fm22l16_rds(ADD_CALIBVENDER_SO21_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_rds(ADD_CALIBVENDER_SO21_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			fm22l16_rds(ADD_CROSS_SO2_H2S_INPUT3, (Uint16 *)&input3[0], sizeof(float)*7);
		}
	} 
	else if (tt == 3)//H2S
	{
		ret = fm24cl04_read(0xa0, ADD24_CALIB7_FLAG, &buff);
		if( buff == 0x55 )
		{
			fm22l16_rds(ADD_CALIBVENDER_H2S_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_rds(ADD_CALIBVENDER_H2S_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
		}
	} 
	else if (tt == 4)//CO
	{
		ret = fm24cl04_read(0xa8, ADD24_CALIB7_FLAG, &buff);
		if( buff == 0x55 )
		{
			fm22l16_rds(ADD_CALIBVENDER_CO_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_rds(ADD_CALIBVENDER_CO_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			fm22l16_rds(ADD_CROSS_CO_H2S_INPUT3, (Uint16 *)&input3[0], sizeof(float)*7);
		}
	} 

a3:	disp_background(14);
	
	if (tt == 0)
	{
		display_ascii(M24, WHITE, BLUE, 1, 300, 7, "Â¶µãÐ£×¼³ÌÐò");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "Â¶µã:");
	}
	else if (tt == 1)
	{
		display_ascii(M24, WHITE, BLUE, 1, 300, 7, "´¿¶ÈÐ£×¼³ÌÐò");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "´¿¶È:");
	}
	else if (tt == 2)
	{
		display_ascii(M24, WHITE, BLUE, 1, 300, 7, "SO2Ð£×¼³ÌÐò");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "SO2:");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 330, 360, "H2S:");
	}
	else if (tt == 3)
	{
		display_ascii(M24, WHITE, BLUE, 1, 300, 7, "H2SÐ£×¼³ÌÐò");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "H2S:");
	}
	else if (tt == 4)
	{
		display_ascii(M24, WHITE, BLUE, 1, 300, 7, "CO Ð£×¼³ÌÐò");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 150, 360, "CO:");
		display_ascii(M32, WHITE, BLUE_GREEN, 1, 330, 360, "H2S:");
	}
	if (tt==2 || tt==4)
	{
		display_ascii(M24, WHITE, BLUE_GREEN, 1, 50, 50, "²É¼¯Êý¾Ý:");
		display_ascii(M24, WHITE, BLUE_GREEN, 1, 220, 50, "±ê×¼Êý¾Ý:");
		display_ascii(M24, WHITE, BLUE_GREEN, 1, 340, 50, "H2S:");
	}
	else
	{
		display_ascii(M24, WHITE, BLUE_GREEN, 1, 100, 50, "²É¼¯Êý¾Ý:");
		display_ascii(M24, WHITE, BLUE_GREEN, 1, 300, 50, "±ê×¼Êý¾Ý:");
	}
	key_clr();
		
	while (key.keynew[7] == 0 && key.keynew[2] == 0)
	{
		disp_table7x3(keycnt,tt,input1,input2,input3);
		if (tt == 0)
		{
			convert_h2o(&h2o);
			sensor1=h2o.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, h2o.val,4, " ");
		}
		else if (tt == 1)
		{
			convert_purity(&purity);
			sensor1=purity.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, purity.val,4, " ");
		}
		else if (tt == 2)
		{
			convert_so2(&so2);
			sensor1=so2.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, so2.val,4, " ");
			convert_h2s(&h2s);
			sensor2=h2s.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 420, 360, h2s.val,4, " ");
		}
		else if (tt == 3)
		{
			convert_h2s(&h2s);
			sensor1=h2s.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, h2s.val,4, " ");
		}
		else if (tt == 4)
		{
			convert_co(&co);
			sensor1=co.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, co.val,4, " ");
			convert_h2s(&h2s);
			sensor2=h2s.val;
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 420, 360, h2s.val,4, " ");
		}

		if ( key.keynew[4] == 1 )
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
			key_clr();
		}
		else if ( key.keynew[5] == 1 )
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
				if ( keycnt >7 )
					keycnt -= 7;
				else
					keycnt += 14;
			}
			key_clr();
		}
		else if ( key.keynew[0] == 1 )
		{
			if( tt == 0 )
			{
				if( keycnt < 5 )
				{
 					if ( keycnt ==0 ) keycnt = 4;
					else keycnt--;
				}
				else
				{
					if( keycnt ==5 ) keycnt = 9;
					else keycnt--;
				}
			}
			else if( tt == 1 || tt == 3 )
			{
				if( keycnt<7 )
				{
					if ( keycnt ==0) keycnt = 6;
					else keycnt--;
				}
				else
				{
					if( keycnt ==7 ) keycnt = 13;
					else keycnt--;
				}
			}
			else
			{
				if( keycnt<7 )
				{
					if ( keycnt ==0 ) keycnt = 6;
					else keycnt--;
				}
				else if( keycnt<14 )
				{
					if( keycnt ==7 ) keycnt = 13;
					else keycnt--;
				}
				else
				{
					if( keycnt ==14 ) keycnt = 20;
					else keycnt--;
				}
			}
			key_clr();
		}
		else if ( key.keynew[1] == 1 )
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
				key_clr();
		}
		else if ( key.keynew[3] == 1 )
		{
			if( tt == 0 )
			{
				if ( keycnt < 5 )
				{
					input1[keycnt] = sensor1 ;
				}
				else
				{
					keyboard_flt(630-288,470-320,input2+keycnt-5) ;
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
					}
					else
					{
						input1[keycnt] = sensor1 ;
					}
				}
				else if ( keycnt < 14 )
				{
					keyboard_flt(630-288,470-320,input2+keycnt-7) ;
					goto a3;
				}
				else
				{
					input3[keycnt-14] = sensor2 ;
				}
			}
			key_clr();
		}
		else if ( key.keynew[6] == 1 )
		{
			if( tt == 0 )
			{
				if ( keycnt < 5 )
				{
					keyboard_flt(630-288,470-320,input1+keycnt) ;
					goto a3;
				}
			}
			else
			{
				if ( keycnt < 7 )
				{
					keyboard_flt(630-288,470-320,input1+keycnt) ;
					goto a3;
				}
				else if( keycnt >= 14 )
				{
					keyboard_flt(630-288,470-320,input3+keycnt-14) ;
					goto a3;
				}
			}
			key_clr();
		}
	}
	if (key.keynew[2] == 1 )
	{
		if (tt == 0)//Î¢Ë®
		{
			buff = JQX5(&h2o.calib_vender,input1,input2);
			if (buff == 0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
			h2o.calib_usr_flag = 0x5555;
			fm22l16_wr(ADD_CALIBVENDER_H2O_FLAG, h2o.calib_usr_flag);
			fm22l16_wrs(ADD_CALIBVENDER_H2O_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CALIBVENDER_H2O_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			rx8025_clock_read(&h2o.calib_vender.clk);
			fm22l16_wrs(ADD_CALIBVENDER_H2O, (Uint16*) &h2o.calib_vender, sizeof(struct CALIB_7));

			promote("Ð£×¼³É¹¦",2000);
			goto a2;
		}
		else if (tt == 1)//´¿¶È
		{
			buff = JQX7(&purity.calib_vender,input1,input2);
			if (buff == 0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
			purity.calib_usr_flag = 0x5555;
			fm22l16_wr(ADD_CALIBVENDER_PUR_FLAG, purity.calib_usr_flag);
			fm22l16_wrs(ADD_CALIBVENDER_PUR_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CALIBVENDER_PUR_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			rx8025_clock_read(&purity.calib_vender.clk);
			fm22l16_wrs(ADD_CALIBVENDER_PUR, (Uint16*) &purity.calib_vender, sizeof(struct CALIB_7));
			purity.zero = 0.0;
			fm22l16_wrs(ADD_PUR_ZERO, (Uint16*) &purity.zero, sizeof(float));
				
			promote("Ð£×¼³É¹¦",2000);
			goto a2;
		}
		else if (tt == 4)//CO
		{
			ret = JQX7(&co.calib_vender,input1,input2);
			if (ret == 0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
			co.calib_vender_flag=0x55;
			fm24cl04_write(0xa8, ADD24_CALIB7_FLAG, co.calib_vender_flag);
			rx8025_clock_read(&co.calib_vender.clk);
			fm24cl04_wrs(0xa8, ADD24_CALIB7_VAL, (Uint16 *)&co.calib_vender, sizeof(struct CALIB_7));
			fm22l16_wrs(ADD_CALIBVENDER_CO_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CALIBVENDER_CO_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			co.zero = 0.0;
			fm22l16_wrs(ADD_CO_ZERO, (Uint16*) &co.zero, sizeof(float));

			//½»²æ·´Ó¦COÐ£×¼H2S³ÌÐò
			ret=1;
			for(i=0;i<7;i++)
				if (input1[i]==0) ret=0;
			if (ret==0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
					
			for (i = 5; i > 0; i--)
			{
				tmp1 = input3[i] / input1[i];
				tmp = tmp + tmp1 ;
			}

			h2s.calib_cro.cross_co_k = tmp / 7;
			h2s.calib_cross_co_flag = 0x5555;
			fm22l16_wr(ADD_CROSS_CO_FLAG, 0x5555);
			fm22l16_wrs(ADD_CROSS_CO_H2S_INPUT3, (Uint16 *)&input3[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CROSS_CO_H2S, (Uint16 *)&h2s.calib_cro.cross_co_k,sizeof(float));

			promote("Ð£×¼³É¹¦",2000);
			goto a2;
		}
		else if (tt == 2)//SO2
		{
			buff = JQX7(&so2.calib_vender,input1,input2);
			if (buff == 0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
			so2.calib_vender_flag=0x55;
			fm24cl04_write(0xa4, ADD24_CALIB7_FLAG, so2.calib_vender_flag);
			rx8025_clock_read(&so2.calib_vender.clk);
			fm24cl04_wrs(0xa4, ADD24_CALIB7_VAL, (Uint16 *)&so2.calib_vender, sizeof(struct CALIB_7));
			fm22l16_wrs(ADD_CALIBVENDER_SO21_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CALIBVENDER_SO21_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			so2.zero = 0.0;
			fm22l16_wrs(ADD_SO21_ZERO, (Uint16*) &so2.zero, sizeof(float));

			//½»²æ·´Ó¦SO2Ð£×¼H2S³ÌÐò
			ret=1;
			for(i=0;i<7;i++)
				if (input1[i]==0) ret=0;
			if (ret==0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
				
			for (i = 5; i > 0; i--)
			{
				tmp1 = input3[i]/input1[i];
				tmp = tmp + tmp1 ;
			}

			h2s.calib_cro.cross_so2_k = tmp / 7;
			h2s.calib_cross_so2_flag = 0x5555;
			fm22l16_wr(ADD_CROSS_SO2_FLAG, 0x5555);
			fm22l16_wrs(ADD_CROSS_SO2_H2S_INPUT3, (Uint16 *)&input3[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CROSS_SO2_H2S, (Uint16 *)&h2s.calib_cro.cross_so2_k,sizeof(float));

			promote("Ð£×¼³É¹¦",2000);
			goto a2;
		}
		else if (tt == 3)//H2S
		{
			i = JQX7(&h2s.calib_vender,input1,input2);
			if (i == 0)
			{
				promote("Ð£×¼Ê§°Ü",2000);
				goto a2 ;
			}
			fm24cl04_write(h2s.at24_add, ADD24_CALIB7_FLAG, 0x55);
			fm24cl04_wrs(h2s.at24_add, ADD24_CALIB7_VAL, (Uint16 *)&h2s.calib_vender, sizeof(struct CALIB_7));
			rx8025_clock_read(&clock);
			fm24cl04_wrs(h2s.at24_add, ADD24_CALIB7_TIME, (Uint16*)&clock, sizeof(struct CLOCK));
			fm22l16_wrs(ADD_CALIBVENDER_H2S_INPUT1, (Uint16 *)&input1[0], sizeof(float)*7);
			fm22l16_wrs(ADD_CALIBVENDER_H2S_INPUT2, (Uint16 *)&input2[0], sizeof(float)*7);
			h2s.zero = 0.0;
			fm22l16_wrs(ADD_H2S_ZERO, (Uint16*) &h2s.zero, sizeof(float));

			promote("Ð£×¼³É¹¦",2000);
			goto a2;
		}
	}
	else if (key.keynew[7] == 1)
		goto a2;
}

Uint16 calib_user_select(void) //·µ»Ø0±íÊ¾ÍË³ö£¬·µ»Ø1¡«4 ±íÊ¾ÈçÏÂ
{
	Uint16  keycnt = 1;

	cls();
	key_clr();
	disp_background(13);
	display_ascii(M32, WHITE, BLUE_GREEN, 1, 100, 100, "ÇëÑ¡ÔñÐ£×¼¶ÔÏó£º");

	while ( key.keynew[2] + key.keynew[7] == 0 )
	{
		display_ascii(M32, color[keycnt-1][0], BLUE_GREEN, 1, 100, 160,"ÆøÌåH2Sº¬Á¿                          ");
		display_ascii(M32, color[keycnt-1][1], BLUE_GREEN, 1, 100, 220,"ÆøÌåSO2º¬Á¿                          ");
		display_ascii(M32, color[keycnt-1][2], BLUE_GREEN, 1, 100, 280,"ÆøÌåCOº¬Á¿                          ");
		if ( key.keynew[0] + key.keynew[1])
		{
			if ( key.keynew[0])
			{

				if( keycnt <= 1 )
					keycnt = 3 ;
				else
					keycnt -- ;
			}
			else if ( key.keynew[1] )
			{
				if( keycnt >=3 )
					keycnt = 1 ;
				else
					keycnt ++ ;
			}
			key_clr();
		}
	}

	if (key.keynew[7] == 1)
		return 6;
	if(key.keynew[2] == 1)
		return keycnt ;
	return 6;
}

void calib_user_main( void )
{
	Uint16 buff ,keycnt,sensor;
	struct SENSOR * pt;
	float input1[2],input2[2];

	buff   = 0 ;
	keycnt = 0 ;
	
a0:	sensor = calib_user_select() ;

	if ( sensor >= 6 )
	{
		return ;
	}

	input1[0]=0;input1[1]=0;
	input2[0]=0;input2[1]=0;

a1:	cls();
	key_clr();
	disp_background(14);

	if ( sensor < 1)
		return;

	if (sensor == 1)
	{
		buff=fm22l16_rd(ADD_CALIBUSR_H2S_FLAG);
		display_ascii(M32, WHITE, BLUE, 1, 140,360, "H2S:");
		display_ascii(M24, WHITE, BLUE, 1, 250,5, "H2SÓÃ»§Ð£×¼");
		if (buff==0x5555)
		{
			fm22l16_rds(ADD_CALIBUSR_H2S_INPUT1, (Uint16 *)&input1, sizeof(float)*2);
			fm22l16_rds(ADD_CALIBUSR_H2S_INPUT2, (Uint16 *)&input2, sizeof(float)*2);
		}
		pt=&h2s;
	}
	else if (sensor == 2)
	{
		buff=fm22l16_rd(ADD_CALIBUSR_SO21_FLAG);
		display_ascii(M32, WHITE, BLUE, 1, 140, 360, "SO2:");
		display_ascii(M24, WHITE, BLUE, 1, 250,5, "SO2ÓÃ»§Ð£×¼");
		if (buff==0x5555)
		{
			fm22l16_rds(ADD_CALIBUSR_SO21_INPUT1, (Uint16 *)&input1, sizeof(float)*2);
			fm22l16_rds(ADD_CALIBUSR_SO21_INPUT2, (Uint16 *)&input2, sizeof(float)*2);
		}
		pt=&so2;
	}
	else if (sensor == 3)
	{
		buff=fm22l16_rd(ADD_CALIBUSR_CO_FLAG);
		display_ascii(M32, WHITE, BLUE, 1, 140,360, "CO:");
		display_ascii(M24, WHITE, BLUE, 1, 250,5, "COÓÃ»§Ð£×¼");
		if (buff==0x5555)
		{
			fm22l16_rds(ADD_CALIBUSR_CO_INPUT1, (Uint16 *)&input1, sizeof(float)*2);
			fm22l16_rds(ADD_CALIBUSR_CO_INPUT2, (Uint16 *)&input2, sizeof(float)*2);
		}
		pt=&co;
	}
	else
		return;

	while (key.keynew[7] == 0 &&  key.keynew[2] == 0)
	{

		disp_table2x2(keycnt, input1,input2 );
		if(sensor == 1)
		{
			convert_h2s(&h2s);
			convert_so2(&so2);
			convert_co(&co);
			calib_cross(&h2s,&so2,&co);
			calib7(&h2s);
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, h2s.calib_val,4, " ");
		}
		else if(sensor == 2)
		{
			convert_so2(&so2);
			calib7(&so2);
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, so2.calib_val,4, " ");
		}
		else if(sensor == 3)
		{
			convert_co(&co);
			calib7(&co);
			display_fp_ascii(M32, WHITE, BLUE_GREEN, 1, 230, 360, co.calib_val,4, " ");
		}

		if (key.keynew[4] + key.keynew[5])
		{
			if (keycnt==0) keycnt=2;
			else if (keycnt==1) keycnt=3;
			else if (keycnt==2) keycnt=0;
			else if (keycnt==3) keycnt=1;
			key_clr();
		}
		if (key.keynew[0] + key.keynew[1])
		{
			if (keycnt==0) keycnt=1;
			else if (keycnt==1) keycnt=0;
			else if (keycnt==2) keycnt=3;
			else if (keycnt==3) keycnt=2;
			key_clr();
		}
		if (key.keynew[3] == 1)
		{
			if (keycnt < 2)
			{

				input1[keycnt]= pt->calib_val;
			}
			else
			{
				keyboard_flt(630-288,470-320,input2+keycnt-2);
				goto a1;
			}
			key_clr();
		}
		if (key.keynew[6] == 1)
		{
			if (keycnt < 2)
			{

				keyboard_flt(630-288,470-320,input1+keycnt);
				goto a1;
			}
			else
			{
				keyboard_flt(630-288,470-320,input2+keycnt-2);
				goto a1;
			}
		}
	}

	if (key.keynew[2] == 1)
	{
		rx8025_clock_read(&clock);

		pt->calib_usr_flag = 0x5555;

		pt->calib_usr.clk.year= clock.year;
		pt->calib_usr.clk.month= clock.month;
		pt->calib_usr.clk.week= clock.week;
		pt->calib_usr.clk.date= clock.date;
		pt->calib_usr.clk.hour= clock.hour;
		pt->calib_usr.clk.minute= clock.minute;
		pt->calib_usr.clk.second= clock.second;

		if (input1[1]==input1[0])
		{
			promote("Ð£×¼Ê§°Ü",2000);
			goto a0;
		}

		pt->calib_usr.k=(input2[1]-input2[0])/(input1[1]-input1[0]);
		pt->calib_usr.stddot=input2[0];
		pt->calib_usr.realdot=input1[0];

		if (sensor == 1)
		{
			fm22l16_wr(ADD_CALIBUSR_H2S_FLAG, 0x5555);
			fm22l16_wrs(ADD_CALIBUSR_H2S, (Uint16*) &pt->calib_usr, sizeof(struct CALIB_2));
			fm22l16_wrs(ADD_CALIBUSR_H2S_INPUT1, (Uint16*)input1, sizeof(float)*2);
			fm22l16_wrs(ADD_CALIBUSR_H2S_INPUT2, (Uint16*)input2, sizeof(float)*2);
		}
		else if (sensor == 2)
		{
			fm22l16_wr(ADD_CALIBUSR_SO21_FLAG, 0x5555);
			fm22l16_wrs(ADD_CALIBUSR_SO21, (Uint16*) &pt->calib_usr, sizeof(struct CALIB_2));
			fm22l16_wrs(ADD_CALIBUSR_SO21_INPUT1, (Uint16*)input1, sizeof(float)*2);
			fm22l16_wrs(ADD_CALIBUSR_SO21_INPUT2, (Uint16*)input2, sizeof(float)*2);
		}
		else if (sensor == 3)
		{
			fm22l16_wr(ADD_CALIBUSR_CO_FLAG, 0x5555);
			fm22l16_wrs(ADD_CALIBUSR_CO, (Uint16*) &pt->calib_usr, sizeof(struct CALIB_2));
			fm22l16_wrs(ADD_CALIBUSR_CO_INPUT1, (Uint16*)input1, sizeof(float)*2);
			fm22l16_wrs(ADD_CALIBUSR_CO_INPUT2, (Uint16*)input2, sizeof(float)*2);
		}
		promote("Ð£×¼³É¹¦",2000);
		goto a0;
	}
	else if (key.keynew[7] == 1)
	{
		goto a0;
	}
}
/************************************************************************************
É¾³ý·½Ê½£º
1.  É¾³ýµ±Ç°Êý¾Ý£¬ºóÃæµÄÊý¾ÝÇ°ÒÆÒ»»£¬×ÜÊýõÒ»//mode=0;
2.  È«²¿É¾³ý//mode=1;
3.  xÎª´Ó0µ½299µÄÊý
4.  ·µ»ØÖµÎªµ±Ç°½øÈëÊ±µÄ²éÑ¯Ò³Êý
************************************************************************************/
Uint16 delete(Uint16 x)
{
	Uint16 temp,number;
	Uint32 k1,k2,i,j;
  
	number=fm22l16_rd(ADD_CNT);
	if(number<=1){fm22l16_wr(ADD_CNT,0);return 0;}
	if(x>number-1)return 0;

	i=0;

	for(j=x;j<(number-1);j++)
	{
		k2=ADD_RECORD_BASE+sizeof(struct MEASURE_DATA)*j;
		k1=ADD_RECORD_BASE+sizeof(struct MEASURE_DATA)*(j+1);
		for(i=0;i<sizeof(struct MEASURE_DATA);i++)
		{
			temp=fm22l16_rd(k1+i);
			fm22l16_wr(k2+i,temp);
		}
	}
	number--;
	fm22l16_wr(ADD_CNT,number);
	
	if(x==number)return (x-1);
	else return x;
}

void search_main(void)
{
	Uint16 keycnt;
	Uint32 number,h,i;
	struct MEASURE_DATA data;

search1:
	number=fm22l16_rd(ADD_CNT);
	if(number==0)
	{
		display_ascii(M32,0x0fe0,BLACK,1,100,180,"Ã»ÓÐ²éÑ¯Êý¾Ý!");
		for(i=0;i<1000000;i++);
		return;
	}
	keycnt=number;
	
	cls();
	disp_background(6);

	display_ascii(M32,YELLOW,BLACK,1,10,40+45*0,"²âÁ¿²ÎÊý");
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*1,"±àºÅ:");
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*2,"ÈÕÆÚ:");  
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*3,"Ê±¼ä:");  
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*4,"ÎÂ¶È:");  
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*5,"Êª¶È:");  
	display_ascii(M32,GREEN,BLACK,1,10, 40+45*6,"Á÷Á¿:");  
	display_ascii(M32,YELLOW,BLACK,1,230,40+45*0,"²âÁ¿½á¹û");
	display_ascii(M32,GREEN,BLACK,1,230,40+45*1,"Â¶µã:");
	display_ascii(M32,GREEN,BLACK,1,230,40+45*2,"Î¢Ë®:");  
	display_ascii(M32,GREEN,BLACK,1,230,40+45*3,"Î¢Ë®:");  
	display_ascii(M32,GREEN,BLACK,1,230,40+45*4,"´¿¶È:");  
	display_ascii(M32,0x07e0,0x0000,1,230,40+45*5,"H2S :");  
	display_ascii(M32,0x07e0,0x0000,1,230,40+45*6,"SO2 :");  
	display_ascii(M32,0x07e0,0x0000,1,230,40+45*7,"CO  :");  

	for(h=0;h<200000;h++);
	key_clr();

	while(key.keynew[7]==0)
	{
		number=fm22l16_rd(ADD_CNT);
		if(number==0)return;

		display_num_ascii1(M24,0xffff,0x001f,1,200,7,keycnt,"/");
		display_number(M24,0xffff,0x001f,1,270,7,number);
		display_ascii(M24,0xffff,0x001f,1,340,7,"Ò³");
		fm22l16_rds(ADD_RECORD_BASE+(keycnt-1)*sizeof(struct MEASURE_DATA),(Uint16 *)&data,sizeof(struct MEASURE_DATA));
		display_page(&data);
		
		if (key.keynew[5]==1)
		{
			keycnt++;
			if (keycnt>(number)) keycnt=1;
			key_clr();
		}
		else if (key.keynew[6]==1)
		{
			if (keycnt==1) keycnt=number;
			else keycnt--;
			key_clr();
		}
		else if (key.keynew[0]==1)//delete
		{
			display_ascii(M32,0x0fe0,BLACK,1,100,180,"ÕýÔÚÉ¾³ýµ±Ç°Êý¾Ý£¡");
			for(i=0;i<1000000;i++);
			i=delete(keycnt-1);
			goto search1;
		}
		else if (key.keynew[1]==1)//print
		{
			display_ascii(M32,0x0fe0,BLACK,1,10,180,"ÕýÔÚ´òÓ¡µ±Ç°Êý¾Ý£¡");
			for(i=0;i<1000000;i++);
			print_page(&data);
			goto search1;
		}
		else if (key.keynew[2]==1)//usb out
		{
			display_ascii(M32,0x0fe0,BLACK,1,10,180,"ÕýÔÚ´«ÊäÊý¾Ý£¡");
			for(i=0;i<1000000;i++);
			usb_page(data);
			goto search1;
		}
		else if (key.keynew[3]==1)//delete all
		{
			display_ascii(M32,0x0fe0,BLACK,1,100,180,"ÕýÔÚÉ¾³ýÈ«²¿Êý¾Ý£¡");
			for(i=0;i<1000000;i++);
			number=0;
			fm22l16_wr(ADD_CNT,number);
			goto search1;
		}
		else if (key.keynew[4]==1)//usb out all
		{
			display_ascii(M32,0x0fe0,BLACK,1,10,180,"ÕýÔÚ´«ÊäÈ«²¿Êý¾Ý£¡");
			for(i=0;i<1000000;i++);
			for(i=0;i<number;i++)
			{
				fm22l16_rds(ADD_RECORD_BASE+(i)*sizeof(struct MEASURE_DATA),(Uint16 *)&data,sizeof(struct MEASURE_DATA));
				usb_page(data);
			}
			goto search1;
		}
	}
}

void set_main(void)
{
a1: 	cls();
	disp_background(5);
	key_clr();

	display_ascii(M32,WHITE,BLUE_GREEN,1,50,100,"ÆøÌå:");
	if (plat.gas==0)
		display_ascii(M32,WHITE,BLUE_GREEN,1,150,100,"SF6");
	else if (plat.gas==1)
		display_ascii(M32,WHITE,BLUE_GREEN,1,150,100,"N2 ");
	display_ascii(M32,WHITE,BLUE_GREEN,1,50,160,"ÈÕÆÚ:");
	display_ascii(M32,WHITE,BLUE_GREEN,1,50,220,"Ê±¼ä:");
	display_ascii(M32,WHITE,BLUE_GREEN,1,50,280,"°æ±¾: 3.0.1");

	while( key.keynew[7]==0 )
	{
		rx8025_clock_read(&clock);
		display_number(M32,WHITE,BLUE_GREEN,1,150,160,clock.year+2000);
		display_ascii(M32,WHITE,BLUE_GREEN,1,215,160,"Äê");
		display_number(M32,WHITE,BLUE_GREEN,1,265,160,clock.month);
		display_ascii(M32,WHITE,BLUE_GREEN,1,295,160,"ÔÂ");
		display_number(M32,WHITE,BLUE_GREEN,1,345,160,clock.date);
		display_ascii(M32,WHITE,BLUE_GREEN,1,375,160,"ÈÕ");
		display_ascii(M32,WHITE,BLUE_GREEN,1,420,160,week_string[clock.week]);
		display_number(M32,WHITE,BLUE_GREEN,1,150,220,clock.hour);
		display_ascii(M32,WHITE,BLUE_GREEN,1,180,220,"Ê±");
		display_number(M32,WHITE,BLUE_GREEN,1,230,220,clock.minute);
		display_ascii(M32,WHITE,BLUE_GREEN,1,260,220,"·Ö");

		if (key.y >=86 && key.y < 144)
		{
			if ( key.x > 130 && key.x < 230 )
			{
				if (plat.gas==0)
				{
					plat.gas=1;
					display_ascii(M32,WHITE,BLUE_GREEN,1,150,100,"N2 ");
					key_clr();
					fm22l16_wr(ADD_GAS,plat.gas);
				}
				else if (plat.gas==1)
				{
					plat.gas=0;
					display_ascii(M32,WHITE,BLUE_GREEN,1,150,100,"SF6");
					key_clr();
					fm22l16_wr(ADD_GAS,plat.gas);
				}
			}
		}
		else if (key.y > 146 && key.y < 204)//ÈÕÆÚ
		{
			if ( key.x > 150 && key.x < 230 )//Äê
			{
				keyboard_int(630-288,470-320,&clock.year) ;
				clock.year%=100;
				rx8025_date_set(&clock);
				goto a1 ;
			}
			else if( key.x > 260 && key.x < 330 )//ÔÂ
			{
				keyboard_int(630-288,470-320,&clock.month) ;
				clock.month%=13;
				if (clock.month==0) clock.month++;
				rx8025_date_set(&clock);
				goto a1 ;
			}
			else if( key.x > 350 && key.x < 400 )//ÈÕ
			{
				keyboard_int(630-288,470-320, &clock.date) ;
				clock.date%=32;
				if (clock.date==0) clock.date++;
				rx8025_date_set(&clock);
				goto a1 ;
				}
			else if( key.x > 420 && key.x < 500 )
			{
				keyboard_int(630-288,470-320,&clock.week) ;
				clock.week%=7;
				rx8025_date_set(&clock);
				goto a1 ;
			}
		}
		else if( key.y > 206 && key.y < 264 )//Ê±¼ä
		{
			if ( key.x > 150 && key.x < 210 )//Ð¡Ê±
			{
				keyboard_int(630-288,470-320, &clock.hour) ;
				clock.hour%=24;
				rx8025_time_set(&clock);
				goto a1 ;
			}
			else if( key.x > 230 && key.x < 290 )//·ÖÖÓ
			{
				keyboard_int(630-288,470-320, &clock.minute) ;
				clock.minute%=60;
				rx8025_time_set(&clock);
				goto a1 ;
			}
		}
		if (key.keynew[3]==1)
		{
			calib_vender_main();
			goto a1;
		}
		if(key.keynew[4]==1)
		{
			calib_user_main();
			goto a1;
		}
		if(key.keynew[5]==1)
		{
			null_point_display();
			goto a1;
		}
		if(key.keynew[6]==1)
		{
			calib_touch();
			goto a1;
		}
	}

	if( key.keynew[0]==0 )
	{
		return ;
	}
	key_clr();
}
/************************************************************************************
´æ´¢·½Ê½£º
1.  ´ÓµØÖ·0¿ªÊ¼´æ´¢Êý¾Ý£¬ÈçNUMBER£½300£¬ÄÇÃ´Êý´æ´¢ÔÚµÚ299¸öÎ»ÖÃ
2.  ´æÂú300¸öÊýºó£¬½«×îÇ°ÃæµÄÊý¾Ý¸²¸Ç£¬ºóÃæµÄÊý¾ÝÒÀ´ÎÇ°ÒÆÒ»Î»£¬×îºóµÄÊý¾Ý´æ´¢×îÐÂµÄ


************************************************************************************/
void store_main(void)
{
	Uint32 i,j;
	struct MEASURE_DATA data,tmp;
 
	cls();
	key_clr();
	disp_background(15);

	display_ascii(M32,WHITE,BLUE_GREEN,1,60,80,"ÇëÊäÈëÊÔÑé±àºÅ£º");
	data.flw=flw.bar;
	data.h2o_calib_value=h2o.calib_val;
	data.h2s_calib_value=h2s.calib_val;
	data.so2_calib_value=so2.calib_val;
	data.hf_calib_value =co.calib_val;
	data.pur_calib_value=purity.calib_val;
	data.c_humid=humidity;
	data.c_temp=temprature.calib_val;
	rx8025_clock_read(&data.clk);
	data.clk.year=clock.year;
	data.clk.month=clock.month;
	data.clk.date=clock.date;
	data.clk.hour=clock.hour;
	data.clk.minute=clock.minute;
	data.clk.second=clock.second;
	data.clk.week=clock.week;
	data.h2oppm=ppm;
	data.h2oppm20=ppm20;
	data.number[0]=0;
	data.number[1]=0;
	data.number[2]=0;
	data.number[3]=0;
	
	i=0;
	while(key.keynew[2]==0 && key.keynew[7]==0)
	{
		display_number(M64,color[i][0],BLUE_GREEN,1,100+80*0,150,data.number[0]);
		display_number(M64,color[i][1],BLUE_GREEN,1,100+80*1,150,data.number[1]);
		display_number(M64,color[i][2],BLUE_GREEN,1,100+80*2,150,data.number[2]);
		display_number(M64,color[i][3],BLUE_GREEN,1,100+80*3,150,data.number[3]);
		if(key.keynew[1]==1)
		{
			key_clr();
			if(i>=3) i=0;
			else i++;
		}
		if(key.keynew[0]==1)
		{
			key_clr();
			if(i==0 || i>3) i=3;
			else i--;
		}
		else if(key.keynew[5]==1)
		{
			key_clr();
			if(data.number[i]>=9)data.number[i]=0;
			else data.number[i]++;
		}
		else if(key.keynew[6]==1)
		{
			key_clr();
			if(data.number[i]==0 || data.number[i]>9) data.number[i]=9;
			else data.number[i]--;

		}
	}
	if(key.keynew[7]==1)
	{
		key_clr();
		display_ascii(M32,0xf800,0x0000,1,130,180,"Êý¾ÝÃ»ÓÐ±£´æ£¡");
		for(j=0;j<1000000;j++);
		return;
	}
	if(key.keynew[2]==1)
	{
		key_clr();
		display_ascii(M32,0x0fe0,0x0000,1,130,180,"ÕýÔÚ±£´æÊý¾Ý£¡");
		for(j=0;j<1000000;j++);
	}
	
	plat.store_number=fm22l16_rd(ADD_CNT);
	if(plat.store_number<RECORD_SIZE)plat.store_number++;
	fm22l16_wr(ADD_CNT,plat.store_number);

	if(plat.store_number<RECORD_SIZE)
	{
		fm22l16_wrs(ADD_RECORD_BASE+(plat.store_number-1)*sizeof(struct MEASURE_DATA), (Uint16 *)&data,  sizeof(struct MEASURE_DATA));
	}
	else
	{
		for(j=1;j<RECORD_SIZE;j++)
		{
			fm22l16_rds(ADD_RECORD_BASE+j*sizeof(struct MEASURE_DATA), (Uint16 *)&tmp, sizeof(struct MEASURE_DATA));
			fm22l16_wrs(ADD_RECORD_BASE+(j-1)*sizeof(struct MEASURE_DATA), (Uint16 *)&tmp, sizeof(struct MEASURE_DATA));
		}
		fm22l16_wrs(ADD_RECORD_BASE+299*sizeof(struct MEASURE_DATA), (Uint16 *)&data, sizeof(struct MEASURE_DATA));
	}
}


void disp_ca100(Uint16 page)
{
	char str[10];
	Uint16 i;

	if(plat.gas==0)
		display_ascii(M24,WHITE,BLUE,1,10,7,"SF6");
	else 
		display_ascii(M24,WHITE,BLUE,1,10,7,"N2 ");
	
	if (page==0)
	{	
		sflt_to_str(h2o.calib_val, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*0,str);
		sflt_to_str(ppm, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*1,str);
		sflt_to_str(ppm20, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*2,str);
		sflt_to_str(temprature.calib_val, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*3,str);
		sflt_to_str(humidity, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*4,str);
	}
	else if (page==1)
	{
		for(i=0;i<plat.sensor_number;i++)
		{
			if(plat.sensor[i]==0xa0)
			{
				sflt_to_str(h2s.calib_val, str);
				display_ascii(M64,WHITE,BLUE,1,200,40+72*1,str);
			}
			else if(plat.sensor[i]==0xa4)
			{
				sflt_to_str(so2.calib_val, str);
				display_ascii(M64,WHITE,BLUE,1,200,40+72*0,str);
			}
			else if(plat.sensor[i]==0xa8)
			{
				sflt_to_str(co.calib_val, str);
				display_ascii(M64,WHITE,BLUE,1,200,40+72*2,str);
	              }
		}
		sflt_to_str(co.calib_val, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*3,str);
		sflt_to_str(temprature.calib_val, str);
		display_ascii(M24,WHITE,BLUE,1,165,40+72*4,str);
		sflt_to_str(humidity, str);
		display_ascii(M24,WHITE,BLUE,1,400,40+72*4,str);
	}
	else if (page==2)
	{
		sflt_to_str(100-purity.calib_val,str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*0,str);
		sflt_to_str(purity.calib_val, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*1,str);
		sflt_to_str(temprature.calib_val, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*3,str);
		sflt_to_str(humidity, str);
		display_ascii(M64,WHITE,BLUE,1,200,40+72*4,str);
	}
}


void ca100_main(void)
{
	Uint16 page=0;
	Uint16 g,h,t=0,i,ret;

start:cls();//½çÃæ³õÊ¼»¯
a1:	key_clr();
	disp_background(2+page);

	while(1)
	{	
		//²É¼¯ÈÎÎñ
		if (run_temp_flag==1)
		{
			get_h2o(&h2o);
			to_ppm(h2o.calib_val*10.0,&ppm);
			to_ppm20(h2o.calib_val*10.0,temprature.calib_val,&ppm20);
			get_purity(&purity);  
			for(i=0;i<plat.sensor_number;i++)
			{
				if(plat.sensor[i]==0xa0)
				{
					get_h2s(&h2s,&so2,&co);
				}
				else if(plat.sensor[i]==0xa4)
				{
					get_so2(&so2);
				}
				else if(plat.sensor[i]==0xa8)
				{
					get_co(&co);
	            }
			}
	              run_temp_flag=0;	
		}
		if (run_flow_flag==1)
		{
			get_flow(&flw);
			run_flow_flag=0;
			for(i=0;i<8;i++)
			{
				if (i<flw.bar) display_picture(63, 58, 82, 90, 107, 480, 318-i*38);
				else display_picture(2, 479, 284, 479+32, 284+25, 480, 318-i*38);
			}
		}
		if (run_clock_flag==1)
		{
			display_clock(M24,WHITE,BLUE,1,100,7);
	           	run_clock_flag=0;
	         	t++;
			if(t>=1)
			{
				t=0;h++;g=h&0x01;
				battery_do(g);
			}
		}
		if (run_sht11_flag==1)
		{
			get_sht11(&temprature,&humidity);
			run_sht11_flag=0;
		}
		//¼üÅÌÏìÓ¦
		if (key.keynew[0]) 
		{	
			cls();
			page=0;
			disp_background(2);
			key_clr();
		}
		else if (key.keynew[1]) 
		{	
			cls();
			page=1;
			disp_background(3);
			key_clr();
		}
		else if (key.keynew[2]) 
		{	
			cls();
			page=2;
			disp_background(4);
			key_clr();
		}
		else if (key.keynew[3])
		{
			ret=zero_set_ca100();
			key_clr();
			if (ret==OK) goto start;
			else goto a1;
		}
		else if (key.keynew[4])
		{
			if (plat.clean==0) plat.clean=1;
			else if (plat.clean==1) plat.clean=2;
			else if (plat.clean>=2) plat.clean=0;
			gas_clean(plat.clean);
			key_clr();
		}
		else if (key.keynew[5])
		{
			HEATER(1); 
			search_main();
			key_clr();
			goto start;
		}
		else if (key.keynew[6])
		{
			HEATER(1);
			store_main();
			key_clr();
			goto start;
		}
		else if (key.keynew[7])
		{
			HEATER(1);
			set_main();
			key_clr();
			goto start;
		}
		disp_ca100(page);
	}
}
