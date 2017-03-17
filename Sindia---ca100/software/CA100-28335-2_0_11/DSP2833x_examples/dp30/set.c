
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File



Uint16 temp[12],sel[2],clean_flag;
Uint32 sub_ver=0;
//sub_ver defination
//bit        31--1    0
//value		   nc	  ca100_version		
//mean                0-ca100,1-ca100v2
const Uint16 limit[9][2]=
{
0,1,//QCA	
0,99,  //jia shi  YEAR
1,12,//gas		  MONTH
1,31,//tan tou	  day
0,6,//vapo		  week
0,23, //		  hour
0,59, //		  minute
0,1, //    清洗控制
0,1,
};

#define front 0x8410
#define back  0x0000

const Uint16 reftab[][13]=
{
front,back,back,back,back,back,back,back,back,back,back,back,back,
back,front,back,back,back,back,back,back,back,back,back,back,back,
back,back,front,back,back,back,back,back,back,back,back,back,back,
back,back,back,front,back,back,back,back,back,back,back,back,back,
back,back,back,back,front,back,back,back,back,back,back,back,back,
back,back,back,back,back,front,back,back,back,back,back,back,back,
back,back,back,back,back,back,front,back,back,back,back,back,back,
back,back,back,back,back,back,back,front,back,back,back,back,back,
back,back,back,back,back,back,back,back,front,back,back,back,back,
back,back,back,back,back,back,back,back,back,front,back,back,back,
back,back,back,back,back,back,back,back,back,back,front,back,back,
back,back,back,back,back,back,back,back,back,back,back,front,back,
back,back,back,back,back,back,back,back,back,back,back,back,front,
};

char *week_string[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六","星期日","星期日","星期日"};
void refresh(Uint16 num)
{
    Uint16 i,j,tmp[12];char x[10];
    
    for (i=0;i<9;i++) tmp[i]=HEX4BCD(temp[i]);
    
    j=tmp[0];
    if (j==0) display_ascii(M16,0xffff,reftab[num][0],1,53,30,"SF6");
    else if (j==1) display_ascii(M16,0xffff,reftab[num][0],1,53,30,"N2 ");
    else if (j==2) display_ascii(M16,0xffff,reftab[num][0],1,53,30,"CO2");

    
    j=tmp[1];
	i=j>>4&0x0f;x[0]=(i+0x30);i=j&0x0f;x[1]=i+0x30;x[2]='\0';
	display_ascii(M16,0xffff,reftab[num][1],1,53,56,&x[0]);

    display_ascii(M16,0xffff,0x0000,0,53+16,56,".");
  
    j=tmp[2];
    i=j>>4&0x0f;x[0]=(i+0x30);i=j&0x0f;x[1]=i+0x30;x[2]='\0';
	display_ascii(M16,0xffff,reftab[num][2],1,53+24,56,&x[0]);

    display_ascii(M16,0xffff,0x0000,0,53+24+16,56,".");
    
    j=tmp[3];
	i=j>>4&0x0f;x[0]=(i+0x30);i=j&0x0f;x[1]=i+0x30;x[2]='\0';
	display_ascii(M16,0xffff,reftab[num][3],1,53+24+16+8,56,&x[0]);

	j=tmp[4];
	display_ascii(M16,0xffff,reftab[num][4],1,53+24+16+8+16,56,week_string[j]);

    j=tmp[5];
	i=j>>4&0x0f;x[0]=(i+0x30);i=j&0x0f;x[1]=i+0x30;x[2]='\0';
	display_ascii(M16,0xffff,reftab[num][5],1,53,56+26,&x[0]);

    display_ascii(M16,0xffff,0xC618,0,53+16,56+26,":");
    
    j=tmp[6];
	i=j>>4&0x0f;x[0]=(i+0x30);i=j&0x0f;x[1]=i+0x30;x[2]='\0';
	display_ascii(M16,0xffff,reftab[num][6],1,53+16+8,56+26,&x[0]);

	j=tmp[7];
    if (j==0)      display_ascii(M16,0xffff,reftab[num][7],1,53,56+52,"关");
    else if (j==1) display_ascii(M16,0xffff,reftab[num][7],1,53,56+52,"开");

    j=tmp[8];
    if (j==0)      display_ascii(M16,0xffff,reftab[num][8],1,53,56+78,"关");
    else if (j==1) display_ascii(M16,0xffff,reftab[num][8],1,53,56+78,"开");

}
void gas_clean(Uint16 flag)
{
    Uint32 i;
    if(flag==0)
	{
	    BPL(0);for(i=0;i<3000000;i++);KG1(0);
	}
	else
	{
	    KG1(1);for(i=0;i<3000000;i++);BPL(1);
	}
}

Uint16 keyboard_2(void)
{

	Uint16  output_number,keyboard = 0,key_xx,key_yy,flag_key,x;
	Uint32 j;
	Uint16 input[2]={0};
	key_clr() ;

	x = 0 ;
	color_change(0x0000, 0x0000);
	display_jx_clear(100, 30, 220, 200) ;
	color_change(0xf800, 0x0000);

	display_jx(100, 30, 220, 60);
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
			flag_key = 1 ;
			if (key_y > 60 && key_y < 95)
			{
				if(key_x > 100 && key_x < 140)
					keyboard = 7;//7
				else if(key_x > 140 && key_x < 180)
					keyboard = 8;//8
				else if(key_x > 180 && key_x < 220)
					keyboard = 9;//9
			}
			else if (key_y > 95&&key_y < 130)
			{
				if ( key_x > 100 && key_x < 140 )
					keyboard = 4;//4
				else if (key_x > 140 && key_x < 180)
					keyboard = 5;//5
				else if (key_x > 180 && key_x < 220)
					keyboard = 6;//6
			}
			else if ( key_y > 130 && key_y < 165 )
			{
				if (key_x > 100&&key_x < 140)
					keyboard = 1;//1
				else if (key_x > 140 && key_x < 180)
					keyboard = 2;//2
				else if (key_x > 180 && key_x < 220)
					keyboard = 3;//3
			}
			else if ( key_y < 200 && key_y > 165 )
			{
				if (key_x > 100 && key_x < 140)
					keyboard = 0;//0
				else if (key_x > 140 && key_x < 180)
					keyboard = 10;//取消
				else if (key_x > 180 && key_x < 220)
					keyboard = 11;//确定
			}
			key_yy = key_y ;
			key_xx = key_x ;
		}

		if (flag_key)
		{
			flag_key = 0 ;
			if( keyboard == 10)
			{
				input[0] = 0 ;
				input[1] = 0 ;
			}
			else if(keyboard == 11)
			{
				;
			}
			else
			{
				for(j=0;j<500000;j++) ;
				input[0] = input[1];
				input[1] = keyboard;
			}
			output_number = input[0]*10+input[1] ;
		}
		display_number(M24, 0xffff, 0x0000, 1, 180, 35, output_number);
	}

	key_clr() ;
	color_change(0x0000, 0x0000);
	display_jx_clear(100, 60, 220, 200) ;
	color_change(0xf800, 0x0000);

	return output_number ;
}
void set(void)
{
    Uint16 i,keycnt,flag;
    Uint16 tmp[7];
   
    keycnt=0;
    km=1;
    flag=0;
    u_key_enter=0;
	
a1: color_change(0x001f, 0x0000);
    cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    
    display_ascii(M16,0xffff,0x001f,1,5,2,"设置");
    display_ascii(M16,0xffff,0x001f,1,0,222," 退出  ");
    key_clr();
    
   // color_change(0xf800,0x0000);
	display_ascii(M24,0xffff,0x0000,1,5,30,"气体:");
	display_jx(80, 30, 110, 54);
	display_jx(130, 30, 160, 54);
	if( gas )
	{
		display_ascii(M16,0xffff,0x0000,1,85,34,"SF6");
		display_ascii(M16,0xffff,0xf800,1,135,34,"N2");
	}
	else
	{
		display_ascii(M16,0xffff,0xf800,1,85,34,"SF6");
		display_ascii(M16,0xffff,0x0000,1,135,34,"N2");
	}

	display_ascii(M24,0xffff,0x0000,1,5,60,"日期:");
	display_jx(80, 60, 120, 84);
	display_ascii(M16,0xffff,0x0000,1,82,64,"20");
	display_jx(125, 60, 155, 84);
	display_jx(160, 60, 190, 84);
	display_jx(195, 60, 255, 84);

	display_ascii(M24,0xffff,0x0000,1,5,90,"时间:");
	display_jx(80, 90, 110, 114);
	display_jx(115, 90, 145, 114);
	display_jx(150, 90, 180, 114);

    display_ascii(M24,0xffff,0x0000,1,5,120,"清洗:");
    display_jx(80, 120, 120, 144);
    display_ascii(M16,0xffff,0x0000,1,92,124,"开");
    display_jx(140, 120, 180, 144);
    display_ascii(M16,0xffff,0xf800,1,152,124,"关");

    display_ascii(M24,0xffff,0x0000,1,5,150,"校准:");
    display_jx(80, 150, 117, 174);
    display_ascii(M16,0xffff,0x0000,1,85,154,"进入");

    while( keynew[0]==0 )
    {
	  u_key_draw();

	  if (keynew[1]+keynew[2]+keynew[3]+keynew[4]+keynew[5])
	  	key_clr();

	  rx8025_clock_read();
	  for (i=0;i<7;i++)
	  {
		  tmp[i]=BCD4HEX(i2c[i]);
	  }

	  display_number(M16,0xffff,0x0000,1,98,64,tmp[6]);
	  display_number(M16,0xffff,0x0000,1,130,64,tmp[5]);
	  display_number(M16,0xffff,0x0000,1,165,62,tmp[4]);
	  display_ascii(M16,0xffff,0x0000,1,200,64,week_string[tmp[3]]);
	  display_number(M16,0xffff,0x0000,1,85,94,tmp[2]);
	  display_number(M16,0xffff,0x0000,1,120,94,tmp[1]);
	  display_number(M16,0xffff,0x0000,1,155,94,tmp[0]);

      if (key_y > 30 && key_y < 54)
      {
    	  if ( key_x > 80 && key_x < 110 )
          {
        	  gas=0;
        	  fm22l16_wr(ADD_GAS,0);
        	  parameter.gas=gas;
        	  display_ascii(M16,0xffff,0xf800,1,85,34,"SF6");
        	  display_ascii(M16,0xffff,0x0000,1,135,34,"N2");
          }
          else if ( key_x > 130 && key_x < 160 )
          {
        	  gas=1;
        	  fm22l16_wr(ADD_GAS,1);
        	  parameter.gas=gas;
        	  display_ascii(M16,0xffff,0x0000,1,85,34,"SF6");
        	  display_ascii(M16,0xffff,0xf800,1,135,34,"N2");
          }
      }
      else if (key_y > 60 && key_y < 84)
      {
    	  if ( key_x > 80 && key_x < 120 )
    	  {
    		  tmp[6] = keyboard_2() ;
    		  for (i=0;i<7;i++)
    		  {
    			  tmp[i]=HEX4BCD(tmp[i]);
    		  }
    		  rx8025_date_set(tmp[3],tmp[4],tmp[5],tmp[6]);
    		  goto a1 ;
    	  }
    	  else if( key_x > 125 && key_x < 155 )
    	  {
    		  tmp[5] = keyboard_2() ;
    		  for (i=0;i<7;i++)
    		  {
    		      tmp[i]=HEX4BCD(tmp[i]);
    		  }
    		  rx8025_date_set(tmp[3],tmp[4],tmp[5],tmp[6]);
    		  goto a1 ;
    	  }
    	  else if( key_x > 160 && key_x < 190 )
    	  {
    	      tmp[4] = keyboard_2() ;
    	      for (i=0;i<7;i++)
    	      {
    	          tmp[i]=HEX4BCD(tmp[i]);
    	      }
    	      rx8025_date_set(tmp[3],tmp[4],tmp[5],tmp[6]);
    	      goto a1 ;
    	  }
    	  else if( key_x > 195 && key_x < 255 )
    	  {
    	      tmp[3] = keyboard_2() ;
    	      for (i=0;i<7;i++)
    	      {
    	         tmp[i]=HEX4BCD(tmp[i]);
    	      }
    	      rx8025_date_set(tmp[3],tmp[4],tmp[5],tmp[6]);
    	      goto a1 ;
    	  }
      }
      else if( key_y > 90 && key_y < 114 )
      {
    	  if ( key_x > 80 && key_x < 110 )
    	  {
    	      tmp[2] = keyboard_2() ;
    	      for (i=0;i<7;i++)
    	      {
    	          tmp[i]=HEX4BCD(tmp[i]);
    	      }
    	      rx8025_time_set(0,tmp[1],tmp[2]);
    	      goto a1 ;
    	  }
    	  else if( key_x > 115 && key_x < 145 )
    	  {
    	      tmp[1] = keyboard_2() ;
    	      for (i=0;i<7;i++)
    	      {
    	          tmp[i]=HEX4BCD(tmp[i]);
    	      }
    	      rx8025_time_set(0,tmp[1],tmp[2]);
    	      goto a1 ;
    	  }
      }
      else if( key_y > 120 && key_y < 144 )
      {
    	  if ( key_x > 80 && key_x < 120 )
    	  {
    		  display_ascii(M16,0xffff,0xf800,1,92,124,"开");
    	      display_ascii(M16,0xffff,0x0000,1,152,124,"关");
    	      gas_clean( 1 );
    	   }
    	   else if ( key_x > 140 && key_x < 180 )
    	   {
    		   display_ascii(M16,0xffff,0x0000,1,92,124,"开");
    		   display_ascii(M16,0xffff,0xf800,1,152,124,"关");
    		   gas_clean( 0 );
    	    }
      }
      else if( key_y > 150 && key_y < 174 )
      {
    	  if ( key_x > 80 && key_x < 117 )
    	  {
    		  display_ascii(M16,0xffff,0xf800,1,85,154,"进入");
    		  for( i = 0; i < 65535 ; i++ ) ;
    		  for( i = 0; i < 65535 ; i++ ) ;
    		  for( i = 0; i < 65535 ; i++ ) ;
    		  for( i = 0; i < 65535 ; i++ ) ;
    		  calib_do();
    		  display_ascii(M16,0xffff,0x0000,1,85,154,"进入");
    		  goto a1;
    	  }
      }
    }

    if( keynew[0]==0 )
    {
    	return ;
    }
    key_clr();
}

#define I6  GpioDataRegs.GPBDAT.bit.GPIO56;
Uint16 instrument;//0---CA100,1---HA100,2---DA100,3---GPA100
void instrument_select(void)
{
    Uint16 temp,i;

    temp=fm22l16_rd(ADD_INSTRUMENT_CHECK);
	i=I6;
	i=i&0x01;
	if(temp!=0x5555 || i==0)
	{
        km=1;   
        u_key_enter=0;

        color_change(0x001f,0x0000);cls();
        display_jxy(0,0,319,20);
	    display_jxy(0,220,319,239);
    
//        display_ascii(M16,0xffff,0x001f,1,5,2,"仪器型号选择");
	    display_ascii(M16,0x07e0,0x0000,1,5,30,"请选择仪器型号：");
        display_ascii(M16,0xffff,0x0000,1,25,70,"F1------CA100-V2");
	    display_ascii(M16,0xffff,0x0000,1,25,100,"F2------HA100");
	    display_ascii(M16,0xffff,0x0000,1,25,130,"F3------DA100");
		display_ascii(M16,0xffff,0x0000,1,25,160,"F4------GPA100");
        display_ascii(M16,0xF800,0x0000,1,5,200,"请谨慎进行型号选择！！！");
        display_ascii(M16,0xffff,0x001f,1,0,222,"  F1         F2          F3         F4  ");
        key_clr();

        while((keynew[0]+keynew[1]+keynew[2]+keynew[3])==0)
	    {
	        u_key_draw();
	    }

	    if(keynew[0])temp=0;
	    else if(keynew[1])temp=1;
	    else if(keynew[2])temp=2;
     	else if(keynew[3])temp=3;
	    else return;

	    fm22l16_wr(ADD_INSTRUMENT_CHECK,0x5555);
		fm22l16_wr(ADD_INSTRUMENT,temp);
		instrument=temp;
		if(instrument==0||instrument==3)purity_test();
    }
	else
	{
	    instrument=fm22l16_rd(ADD_INSTRUMENT);
		if(instrument>3)
		{
             
             color_change(0x001f,0x0000);cls();
             display_jxy(0,0,319,20);
	         display_jxy(0,220,319,239);
	         display_ascii(M16,0x07e0,0x0000,1,5,30,"请重新选择仪器型号！！！");
		     while(1);
		}
		else
		{
		    if(instrument==0||instrument==3)module_address=fm22l16_rd(ADD_PUR);
		}
	}
}

Uint16 sensor[3],sensor_number;

void calib_get(Uint16 address)
{
    Uint16 *pt,h,k,j,x;

    if(address==0xa0)pt=(Uint16 *)&h2s_k;
	else if(address==0xa4)pt=(Uint16 *)&so2_k;
	else if(address==0xa8)pt=(Uint16 *)&hf_k;
	else return;

    j=0x10;
    for(x=0;x<6;x++)
    {
        k=fm24cl04_read(address,j);
        j++;
        h=fm24cl04_read(address,j);
        h=(k<<8 &0xff00)|h;
        *pt=h;
        j++;
        pt++;
    }

}

void calib_get1(Uint16 address)
{
    Uint16 *pt,h,k,j,x;

    if(address==0xa0)pt=(Uint16 *)&h2s;
	else if(address==0xa4)pt=(Uint16 *)&so2;
	else if(address==0xa8)pt=(Uint16 *)&hf;
	else return;

    j=0x10;
    for(x=0;x<24;x++)
    {
        k=fm24cl04_read(address,j);
        j++;
        h=fm24cl04_read(address,j);
        h=(k<<8 &0xff00)|h;
        *pt=h;
        j++;
        pt++;
    }

}
void sensor_select(void)
{
    Uint16 temp,scnt=0;Uint32 i;

    if(instrument==1||instrument==3)return;

	
    color_change(0x001f,0x0000);cls();
    display_jxy(0,0,319,20);
	display_jxy(0,220,319,239);
    
    display_ascii(M16,0x07e0,0x0000,1,5,30,"系统自检中......");

    sensor_number=0;
	temp=fm24cl04_read(0xa0,0);
	if(temp==0xa0)
	{
	    sensor[sensor_number]=temp;
		sensor_number++;
		temp=fm24cl04_read(0xa0,2);
		if(temp==0x55)
		{
            calib_get1(0xa0);
			fm22l16_rds(ADD_H2S_0,(Uint16*)&h2s_zero,2);
          //  h2s_zero=0.0;//h2s.calib_dot0;
            display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"H2S检测:............正常  ");
        }
		else 
		{
		    display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"H2S检测:............请校准");
		    scnt++;
		}
	}

    temp=fm24cl04_read(0xa4,0);
	if(temp==0xa4)
	{
	    sensor[sensor_number]=temp;
		sensor_number++;
		calib_get1(0xa4);
		temp=fm24cl04_read(0xa4,2);
		if(temp==0x55)
		{
            calib_get1(0xa4);
			fm22l16_rds(ADD_SO2_0,(Uint16*)&so2_zero,2);
		//	so2_zero=0.0;//so2.calib_dot0;
            display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"SO2检测:............正常  ");
        }
		else 
		{
		    display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"SO2检测:............请校准");
		    scnt++;
		}
	}
	temp=fm24cl04_read(0xa8,0);
	if(temp==0xa8)
	{
		sub_ver|=0x01;
	    sensor[sensor_number]=temp;
		sensor_number++;
		temp=fm24cl04_read(0xa8,2);
		if(temp==0x55)
		{
            calib_get1(0xa8);
			fm22l16_rds(ADD_HF_0,(Uint16*)&hf_zero,2);
            display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"CO 检测:............正常  ");
        }
		else 
		{
		    display_ascii(M16,0xffff,0x0000,1,5,70+sensor_number*20,"CO 检测:............请校准");
		    scnt++;
		}
	}
	if (sensor_number==0 || scnt!=0)
	{
	    if (sensor_number==0)
	    {
	    	display_ascii(M16,0xF800,0x0000,1,5,180,"请重新上电检测！");
	    	while(1);
	    }
        if (scnt!=0)
        {
        	display_ascii(M16,0xF800,0x0000,1,5,200,"请进行仪器校准！");
        	for(i=0;i<1000000;i++);
        	calib_do();
        }

    }
    for(i=0;i<1000000;i++);
}


/*************************************************************************/
//end






