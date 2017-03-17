#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include <math.h>    
//#include <stdio.h>   


typedef union 
{
	Uint16 i;
    float f;
} value;

//----------------------------------------------------------------------------------
// modul-var
//----------------------------------------------------------------------------------
enum {TEMP,HUMI};

//#define	DATA   	P1_1
//#define	SCK   	P1_0

#define noACK 0
#define ACK   1
                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

void delay1(unsigned int x)
{
	 Uint32 j,i;
	 for (i=0;i<x;i++)
	 {
         for(j=0;j<10;j++);
     }    
}

//----------------------------------------------------------------------------------
char s_write_byte(Uint16 value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge 
{ 
  unsigned char i,error=0;  
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { if (i & value) DATA(1);          //masking value with i , write to SENSI-BUS
    else DATA(0);                        
    SCK(1);                          //clk for SENSI-BUS
    delay1(5);	
    SCK(0);
  }
  DATA(1);                           //release DATA-line
  
  EALLOW;
  GpioCtrlRegs.GPADIR.bit.GPIO2=0;
  EDIS;
  
  delay1(1);
  
  SCK(1);                            //clk #9 for ack 
  delay1(1);
  error=GpioDataRegs.GPADAT.bit.GPIO2;//check ack (DATA will be pulled down by SHT11)
  SCK(0); 
  
  EALLOW;
  GpioCtrlRegs.GPADIR.bit.GPIO2=1;
  EDIS; 
  DATA(1); 
  delay1(1);             
  return error;                     //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
Uint16 s_read_byte(Uint16 ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1" 
{ 
  unsigned char k=0,i;
  Uint16 val=0;

  DATA(1);  //release DATA-line
  EALLOW;
  GpioCtrlRegs.GPADIR.bit.GPIO2=0;
  EDIS;  
                            
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { 
    
    SCK(1);                          //clk for SENSI-BUS
    delay1(1);
    k=GpioDataRegs.GPADAT.bit.GPIO2;
    if (k) val=(val | i);        //read bit  
    SCK(0);  
    delay1(1);					 
  }
  
  EALLOW;
  GpioCtrlRegs.GPADIR.bit.GPIO2=1;
  EDIS;
  if(ack==1)DATA(0);                        //in case of "ack==1" pull down DATA-Line
  else DATA(1);
  delay1(1);
  SCK(1);                            //clk #9 for ack
  delay1(5);
  SCK(0);						    
  DATA(1); 
                            //release DATA-line
  return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start 
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{  
   DATA(1); SCK(0);                   //Initial state
   delay1(1);
   SCK(1);
   delay1(1);
   DATA(0);
   delay1(1);
   SCK(0);  
   delay1(5);
   SCK(1);
   delay1(1);
   DATA(1);		   
   delay1(1);
   SCK(0);		   
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{  
  unsigned char i; 
  DATA(1); SCK(0);                    //Initial state
  for(i=0;i<9;i++)                  //9 SCK cycles
  { SCK(1);
    delay1(1);
    SCK(0);
  }
  s_transstart();                   //transmission start
}

//----------------------------------------------------------------------------------
char s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset 
{ 
  unsigned char error=0;  
  s_connectionreset();              //reset communication
  error+=s_write_byte(RESET);       //send RESET-command to sensor
  return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{ 
  unsigned char error=0;
  s_transstart();                   //transmission start
  error=s_write_byte(STATUS_REG_R); //send command to sensor
  *p_value=s_read_byte(ACK);        //read status register (8-bit)
  *p_checksum=s_read_byte(noACK);   //read checksum (8-bit)  
  return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{ 
  unsigned char error=0;
  s_transstart();                   //transmission start
  error+=s_write_byte(STATUS_REG_W);//send command to sensor
  error+=s_write_byte(*p_value);    //send value of status register
  return error;                     //error>=1 in case of no response form the sensor
}

//Uint16 kq;
 							   
//----------------------------------------------------------------------------------
Uint16 s_measure(Uint16 *p_value, Uint16 *p_checksum, Uint16 mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{ 
  unsigned error=0,k;
  Uint32 i;
  Uint16 x,y;

  s_transstart();                   //transmission start
  switch(mode){                     //send command to sensor
    case TEMP	: error+=s_write_byte(MEASURE_TEMP); break;
    case HUMI	: error+=s_write_byte(MEASURE_HUMI); break;
    default     : break;	 
  }
  
  EALLOW;
  GpioCtrlRegs.GPADIR.bit.GPIO2=0;
  EDIS;  
  
  for (i=0;i<10000000;i++)
  {
     k=GpioDataRegs.GPADAT.bit.GPIO2;
     if(k==0) break; //wait until sensor has finished the measurement
  }
  if(k) error+=1;                // or timeout (~2 sec.) is reached
  
//  EALLOW;
//  GpioCtrlRegs.GPADIR.bit.GPIO2=1;
//  EDIS;  

 
  x=s_read_byte(ACK);    //read the first byte (MSB)
  y=s_read_byte(ACK);    //read the second byte (LSB)
  *(p_value)=x<<8 |y;
  *p_checksum =s_read_byte(noACK);  //read checksum
//  kq=(*(p_value)<<8) |(*(p_value+1));
  return error;
}


//----------------------------------------------------------------------------------------
void calc_sth11(float *p_humidity ,float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [°C] and humidity [%RH] 
// input :  humi [Ticks] (12 bit) 
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [°C]
{ 
  const float C1=-4.0;              // for 12 Bit
  const float C2=+0.0405;           // for 12 Bit
  const float C3=-0.0000028;        // for 12 Bit
  const float T1=+0.01;             // for 14 Bit @ 5V
  const float T2=+0.00008;           // for 14 Bit @ 5V	

  float rh=*p_humidity;             // rh:      Humidity [Ticks] 12 Bit 
  float t=*p_temperature;           // t:       Temperature [Ticks] 14 Bit
  float rh_lin;                     // rh_lin:  Humidity linear
  float rh_true;                    // rh_true: Temperature compensated humidity
  float t_C;                        // t_C   :  Temperature [°C]

  t_C=t*0.01 - 40;                  //calc. temperature from ticks to [°C]
  rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
  if(rh_true>100)rh_true=100;       //cut if the value is outside of
  if(rh_true<0.1)rh_true=0.1;       //the physical possible range

  *p_temperature=t_C;               //return temperature [°C]
  *p_humidity=rh_true;              //return humidity[%RH]
}

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [°C]
// output:  dew point [°C]
{ float logEx,dew_point;
  logEx=0.66077+7.5*t/(237.3+t)+(log10(h)-2);
  dew_point = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
  return dew_point;
}

Uint16 sht71_ht(float *p_humidity ,float *p_temperature)
{
    value humi_val,temp_val;
    Uint16 error,checksum;
    
    error=0;
    error+=s_measure((Uint16 *) &humi_val.i,&checksum,HUMI);  //measure humidity
    error+=s_measure((Uint16 *) &temp_val.i,&checksum,TEMP);  //measure temperature
    if(error!=0) s_connectionreset();                 //in case of an error: connection reset
    else
    { humi_val.f=(float)humi_val.i;                   //converts integer to float
      temp_val.f=(float)temp_val.i;                   //converts integer to float
      calc_sth11(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
    }
    *p_temperature=temp_val.f;               //return temperature [°C]
    *p_humidity=humi_val.f;              //return humidity[%RH]
    return error;
}

/*
//----------------------------------------------------------------------------------
void main2(void)
//----------------------------------------------------------------------------------
// sample program that shows how to use SHT11 functions
// 1. connection reset 
// 2. measure humidity [ticks](12 bit) and temperature [ticks](14 bit)
// 3. calculate humidity [%RH] and temperature [°C]
// 4. calculate dew point [°C]
// 5. print temperature, humidity, dew point  

{ value humi_val,temp_val;
  float dew_point;
  Uint16 error,checksum,humi,temp;
  Uint32 i,q=0;

  display_picture_all(3);
  s_connectionreset();
  while(1)
  {
    SPK(0);
    error=0;
    error+=s_measure((Uint16 *) &humi_val.i,&checksum,HUMI);  //measure humidity
    error+=s_measure((Uint16 *) &temp_val.i,&checksum,TEMP);  //measure temperature
    if(error!=0) s_connectionreset();                 //in case of an error: connection reset
    else
    { humi_val.f=(float)humi_val.i;                   //converts integer to float
      temp_val.f=(float)temp_val.i;                   //converts integer to float
      calc_sth11(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
      display_fp_number(M32,0xfffF,0x020F,1,20,200,humi_val.f);
      display_fp_number(M32,0xfffF,0x020F,1,20,250,temp_val.f);
	  SPK(1);
    //  printf("temp:%5.1fC humi:%5.1f%% dew point:%5.1fC\n",temp_val.f,humi_val.f,dew_point);
    }
    //----------wait approx. 0.8s to avoid heating up SHTxx------------------------------      
    for (i=0;i<80000;i++);     //(be sure that the compiler doesn't eliminate this line!)
    //-----------------------------------------------------------------------------------  
                   
  }
} 
*/








