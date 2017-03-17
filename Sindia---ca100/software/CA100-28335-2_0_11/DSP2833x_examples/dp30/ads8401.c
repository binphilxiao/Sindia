#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


float ad8401_value[3];

float so2_zero_spy[50],h2s_zero_spy[50],hf_zero_spy[50];
float so2_zero,h2s_zero,hf_zero;
float so2_measure_old,h2s_measure_old,hf_measure_old;
float so2_measure_min,h2s_measure_min,hf_measure_min;
float so2_spy_min,h2s_spy_min,hf_spy_min;
Uint16 so2_zero_count,h2s_zero_count,hf_zero_count;

void ad_select(Uint16 chl)
{
    switch (chl)
    {
        case 0: ANSA2(0);ANSA1(0);ANSA0(0);break;
        case 1: ANSA2(0);ANSA1(0);ANSA0(1);break;
        case 2: ANSA2(0);ANSA1(1);ANSA0(0);break;
        case 3: ANSA2(0);ANSA1(1);ANSA0(1);break;
        case 4: ANSA2(1);ANSA1(0);ANSA0(0);break;
        case 5: ANSA2(1);ANSA1(0);ANSA0(1);break;
        case 6: ANSA2(1);ANSA1(1);ANSA0(0);break;
        case 7: ANSA2(1);ANSA1(1);ANSA0(1);break;
        default:break;
    }
}



float fp_ad_get(Uint16 chl)
{
        Uint32 m;
        float n;

        ad_select(chl);
        for(m=0;m<100;m++);//for(m=0;m<100;m++);
        *((Uint16 *)0x004000)=0;
        for(m=0;m<1;m++);
        n=*((Uint16 *)0x004001)&0x0ffff;
        n=(n*4.096)/65536.0;
        return n;
}
Uint16 fp32_ad_get(Uint16 chl)
{
        Uint16 i,m;
        Uint32 m1[10],k;

		if(chl==8) 
		{
		    m=sht71_ht(&ad8401_value[0],&ad8401_value[1]);//(float *p_humidity ,float *p_temperature)
		    return m;
		}
		else if(chl==9)
		{

			return OK;//此处添加电池电流采样：可用DSP的AD或跳线到AD8401
		}
		else
		{
		    ad_select(chl);
			for(m=0;m<100;m++);
			for(i=0;i<10;i++)
            {
                *((Uint16 *)0x004000)=0;
                for(m=0;m<1;m++);
                m1[i]=*((Uint16 *)0x004001)&0x0ffff;
        
            }
			ad_select(7);
            for(i=0;i<10;i++)
            {
                for(m=0;m<9;m++)
                {
                    if (m1[m]>m1[m+1])
                    {
                        k=m1[m];m1[m]=m1[m+1];m1[m+1]=k;
                    }
                 }   
    
            }
            ad8401_value[0]=((float)(m1[3]+m1[4]+m1[5]+m1[6]+m1[7]+m1[2]))*0.1666667;
			
			return OK;
		}
     
}

float ad_get(Uint16 chl)
{
        Uint16 i,m;
        Uint32 m1[10],k;


		ad_select(chl);
		for(m=0;m<100;m++);
		for(i=0;i<10;i++)
        {
            *((Uint16 *)0x004000)=0;
            for(m=0;m<1;m++);
            m1[i]=*((Uint16 *)0x004001)&0x0ffff;
        
        }
		ad_select(7);
        for(i=0;i<10;i++)
        {
             for(m=0;m<9;m++)
             {
                  if (m1[m]>m1[m+1])
                  {
                        k=m1[m];m1[m]=m1[m+1];m1[m+1]=k;
                   }
              }   
    
         }
         ad8401_value[0]=((float)(m1[3]+m1[4]+m1[5]+m1[6]+m1[7]+m1[2]))*0.1666667;
			
		 return ad8401_value[0];
}

void battery_do(Uint32 x,Uint32 y,Uint16 k,Uint16 code,Uint16 flag)
{
    Uint16 i;

    color_change(0x001f,0x001f);
    display_jxy(x,y,x+18,y+10);

    if(code==1)return;

    color_change(0xffff,0x001f);
    display_jx(x,y,x+16,y+10);
    display_jxy(x+17,y+2,x+18,y+8);
    if(k==0)return;
	if(k==4 && flag==1)color_change(0xf489,0x001f);

	for(i=0;i<k;i++)
	{
        display_jxy(x+3+3*i,y+2,x+4+3*i,y+8);
    } 
}


void display_battery(Uint32 x,Uint32 y,Uint16 code)
{
    Uint16 i,k;float vi,vb;


	fp32_ad_get(AN18V);vb=ad8401_value[0]*409.6/65536.0;
	fp32_ad_get(IOUT);vi=ad8401_value[0]*409.6/65536.0;

	i=code&0x01;vi=((Uint16)vi);vi=vi/100.0;vb=((Uint16)vb);vb=vb/100.0;

    if (vb>=1.64)//18.0v  *2/22 
	{
	    if(vi<0.2){k=4;}
    	else if(vi<1.0){k=3;}
	    else if(vi<2.0){k=2;}
		else if(vi<3.0){k=1;}
	    else{k=0;}

        if(k==4)battery_do(x,y,k,0,1);
		else battery_do(x,y,k,code,1);

	}
	else
	{
	    if(vb>1.45){k=4;}          //>15.9v*2/22 
   	    else if(vb>1.31){k=3;}     //>14.4v*2/22 
	    else if(vb>1.14){k=2;}     //>12.5v*2/22 
		else if(vb>1.09){k=1;}     //>12.0v*2/22 
	    else {k=0;}                //>11.5v*2/22 

        battery_do(x,y,k,0,0);
	}
	if(k==0)HEATER(0);
	else HEATER(1);
	color_change(0xffff,0x0000);
}

void measure_buffer_ini(Uint16 chl)
{
	unsigned int j;	
	if(chl==SO2)
	{
	   for(j=0;j<50;j++)
	   {	 	
	 	   so2_zero_spy[j]= ad_get(SO2);
	   }
	   so2_measure_old=so2_zero_spy[49];
	}
	else if(chl==H2S)
	{
	   for(j=0;j<50;j++)
	   {	 	
	 	   h2s_zero_spy[j]= ad_get(H2S);
	   }
	   h2s_measure_old=h2s_zero_spy[49];
	}
	else if(chl==HF)
	{
	   for(j=0;j<50;j++)
	   {	 	
	 	   hf_zero_spy[j]= ad_get(HF);
	   }
	   hf_measure_old=hf_zero_spy[49];
	}
  
}

void measure_buffer_fill(Uint16 chl)
{
	unsigned int i;

    if(chl==SO2)
    {
       for(i=0;i<49;i++)so2_zero_spy[i]=so2_zero_spy[i+1];
	   so2_zero_spy[49]=ad_get(SO2);
    }
    else if(chl==H2S)
    {
       for(i=0;i<49;i++)h2s_zero_spy[i]=h2s_zero_spy[i+1];
	   h2s_zero_spy[49]=ad_get(H2S);
    }
    else if(chl==HF	)
    {
       for(i=0;i<49;i++)hf_zero_spy[i]=hf_zero_spy[i+1];
	   hf_zero_spy[49]=ad_get(HF);
    }	
}


void LingDianGenZong(Uint16 chl)     //零点跟踪
{
 	  float sum,temp2,zerotemp,temp1,reameasure;
	  int i;	
	
      measure_buffer_fill(chl);	

	  if(chl==SO2)
	  {
	  	  sum=0.0;
	  	  for(i=0;i<20;i++)
		  {
	  		  sum=sum+so2_zero_spy[i];
		  }
	  	  zerotemp=sum*0.05;	
	   
	  	  sum=0.0;
		  for(i=30;i<50;i++)
		  {
			  sum=so2_zero_spy[i]+sum;
		  }
	  	  reameasure=0.05*sum;
	   
	  	  temp1=reameasure-zerotemp;
	  	  temp2=reameasure-so2_measure_old; 	   
/*	  	  if( reameasure<so2_zero) 	
	  	  {
	  	  	  so2_zero_count++;
	  	  	  if(so2_zero_count>3)
	  	  	  {
	  	  	   	   so2_zero_count=0;
	  	  	   	   so2_zero=reameasure;
	  	  	  }
	  	  }	*/   
		  if ((fabs(temp1)<so2_spy_min)&&(fabs(temp2)<so2_measure_min))
	 	  {
	  	   	    so2_zero_count=0;
	  	      //  so2_zero=so2_zero+temp2; 
	 	  }
	  	  so2_measure_old=reameasure;
	  	  if(flow_flag==0)ad8401_value[0]=so2_measure_old-so2_zero;
		  else ad8401_value[0]=so2.calib_dot0*32.0;
	  }
	  else if(chl==H2S)
	  {
	  	  sum=0.0;
	  	  for(i=0;i<20;i++)
		  sum=sum+h2s_zero_spy[i];
	  	  zerotemp=sum*0.05;	
	   
	  	  sum=0.0;
		  for(i=30;i<50;i++)
		  sum=h2s_zero_spy[i]+sum;
	  	  reameasure=0.05*sum;
	   
	  	  temp1=reameasure-zerotemp;
	  	  temp2=reameasure-h2s_measure_old; 
/*	  	  	   
	  	  if( reameasure<h2s_zero) 	
	  	  {
	  	  	  h2s_zero_count++;
	  	  	  if(h2s_zero_count>3)
	  	  	  {
	  	  	   	   h2s_zero_count=0;
	  	  	   	   h2s_zero=0.0;
	  	  	  }
	  	  }*/	   
		  if ((fabs(temp1)<h2s_spy_min)&&(fabs(temp2)<h2s_measure_min))
	 	  {
	  	   	    h2s_zero_count=0;
	  	     //   h2s_zero=h2s_zero+temp2; 
	 	  }
	  	  h2s_measure_old=reameasure;
	  	  if(flow_flag==0)ad8401_value[0]=h2s_measure_old-h2s_zero;
		  else ad8401_value[0]=h2s.calib_dot0*32.0;
	  }
	  else if(chl==HF)
	  {
	  	  sum=0.0;
	  	  for(i=0;i<20;i++)
		  sum=sum+hf_zero_spy[i];
	  	  zerotemp=sum*0.05;	
	   
	  	  sum=0.0;
		  for(i=30;i<50;i++)
		  sum=hf_zero_spy[i]+sum;
	  	  reameasure=0.05*sum;
	   
	  	  temp1=reameasure-zerotemp;
	  	  temp2=reameasure-hf_measure_old; 
/*	  	  	   
	  	  if( reameasure<h2s_zero) 	
	  	  {
	  	  	  h2s_zero_count++;
	  	  	  if(h2s_zero_count>3)
	  	  	  {
	  	  	   	   h2s_zero_count=0;
	  	  	   	   h2s_zero=0.0;
	  	  	  }
	  	  }*/	   
		  if ((fabs(temp1)<hf_spy_min)&&(fabs(temp2)<hf_measure_min))
	 	  {
	  	   	    hf_zero_count=0;
	  	     //   hf_zero=hf_zero+temp2; 
	 	  }
	  	  hf_measure_old=reameasure;
	  	  if(flow_flag==0)ad8401_value[0]=hf_measure_old-hf_zero;
		  else ad8401_value[0]=hf.calib_dot0*16.0;
	  }

}
void zero_set1(Uint16 chl)//0---纯度调零  1---SO2调零   2---H2S调零 3---CO调零
{

    if(chl==2)
	{
	    h2s_zero=h2s_measure_old-h2s.calib_dot0*32.0;
		fm22l16_wrs(ADD_H2S_0,(Uint16*)&h2s_zero,2);
	}
	else if(chl==1)
	{
	    so2_zero=so2_measure_old-so2.calib_dot0*32.0;
		fm22l16_wrs(ADD_SO2_0,(Uint16*)&so2_zero,2);
	}
	else if(chl==3)
	{
	    hf_zero=hf_measure_old-hf.calib_dot0*16.0;
		fm22l16_wrs(ADD_HF_0,(Uint16*)&hf_zero,2);
	}
	else if(chl==0)
	{
	    gpa_zero=get_Concentration();
		gpa_zero=gpa_zero-gpa.calib_dot0;
		fm22l16_wrs(ADD_GPA_0,(Uint16*)&gpa_zero,2);
	}
}



void zero_set_ca100_1(Uint16 cnt)
{
    display_ascii(M16,0xffff,color_1[cnt][0],1,10,60,"纯度测量调零                         ");
	display_ascii(M16,0xffff,color_1[cnt][1],1,10,80,"SO2测量调零                          ");
	display_ascii(M16,0xffff,color_1[cnt][2],1,10,100,"H2S测量调零                          ");
	if (sub_ver==1)display_ascii(M16,0xffff,color_1[cnt][3],1,10,120,"CO测量调零                         ");
}

void zero_set_ca100(void)
{
    Uint32 i;

	if(flow_flag==1)return;

    color_change(0x001f,0x0000);cls();u_key_cnt=0;
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);

	display_ascii(M16,0xffff,0x001f,1,5,2,"调零");
    display_ascii(M16,0x07e0,0x0000,1,5,30,"请选择对象调零：");
    display_ascii(M16,0xffff,0x001f,1,0,222," 退出       移位                   确认");
	display_ascii(M16,0xF800,0x0000,1,5,200,"应保证固定流量的零气，一般为15L/H");
    color_change(0xf81f,0x0000);
	display_jx(5,50,314,146);
	i=0;
	zero_set_ca100_1(i);
	key_clr();km=1;
	while(keynew[3]==0 && keynew[0]==0)
    {
	    u_key_draw();
        if(keynew[1]==1)
		{
		    key_clr();
		    i++;
		    if(i>=4 && sub_ver==1)i=0;
			if(i>=3 && sub_ver==0)i=0;
		    zero_set_ca100_1(i);
		}

    }
	if(keynew[0]==1)
	{
	    key_clr();
	}
    else if (keynew[3]==0)
    {
        zero_set1(i);
    }
    
}

void zero_set_da100_1(Uint16 cnt)
{
	display_ascii(M16,0xffff,color_1[cnt][0],1,10,60,"SO2测量调零                          ");
	display_ascii(M16,0xffff,color_1[cnt][1],1,10,80,"H2S测量调零                          ");
	display_ascii(M16,0xffff,color_1[cnt][2],1,10,100,"CO测量调零                           ");
}
void zero_set_da100(void)
{
    Uint32 i;

	if(flow_flag==1)return;

    color_change(0x001f,0x0000);
    cls();
    u_key_cnt=0;
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);

	display_ascii(M16,0xffff,0x001f,1,5,2,"调零");
    display_ascii(M16,0x07e0,0x0000,1,5,30,"请选择对象调零：");
    display_ascii(M16,0xffff,0x001f,1,0,222," 退出       移位                   确认");
	display_ascii(M16,0xF800,0x0000,1,5,200,"应保证固定流量的零气，一般为15L/H");
    color_change(0xf81f,0x0000);
	display_jx(5,50,314,126);
	i=0;
	zero_set_da100_1(i);
	key_clr();
	km=1;
	while(keynew[3]==0 && keynew[0]==0)
    {
	    u_key_draw();
        if(keynew[1]==1)
		{
		    key_clr();
		    i++;
		    if(i>2)i=0;
		    zero_set_da100_1(i);
		}

    }
	if(keynew[0]==1)
	{
	    key_clr();
	}
    else 
    {
        zero_set1(i+1);
    }
    
}

void zero_set_gpa100(void)
{
    Uint32 i;

	if(flow_flag==1)return;

    color_change(0x001f,0x0000);cls();u_key_cnt=0;
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);

	display_ascii(M16,0xffff,0x001f,1,5,2,"调零");

    display_ascii(M16,0xffff,0x001f,1,0,222," 退出                              确认");
	display_ascii(M16,0xF800,0x0000,1,5,200,"应保证固定流量的零气，一般为15L/H");
    color_change(0xf81f,0x0000);
	display_jx(5,50,314,126);
	i=0;
	display_ascii(M16,0xffff,color_1[0][2],1,10,60,"纯度测量调零                         ");
	key_clr();km=1;
	while(keynew[3]==0 && keynew[0]==0)
    {
	    u_key_draw();
    }
	if(keynew[0]==1)
	{
	    key_clr();
	}
    else 
    {
        zero_set1(0);
    }
    
}
