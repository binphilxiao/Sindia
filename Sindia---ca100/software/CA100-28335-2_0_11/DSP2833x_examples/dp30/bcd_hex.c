#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File




/********** HEXתBCD******/
/***bcd_data(<0x255,>0)***/
Uint16 BCD2HEX(Uint16 bcd_data)
{
    Uint16 temp;
    temp=(((bcd_data&0x0f00)>>8)*100)+(((bcd_data&0x00f0)>>4)*10)+(bcd_data&0x000f);
  //  temp=(_iror_(bcd_data&0x0f00,8)*100)+(_iror_(bcd_data&0x00f0,4)*10)+(bcd_data&0x000f); //the function of c51 is slower
    return temp;
}

/********** HEXתBCD******/
/***hex_data(<0xff,>0)****/

Uint16 HEX2BCD(Uint16 hex_data)
{
    Uint16 bcd_data;
    Uint16 temp;
    temp=hex_data%100;
    bcd_data=((Uint16)hex_data)/100<<8;
    bcd_data=bcd_data|temp/10<<4;
    bcd_data=bcd_data|temp%10;
    return bcd_data;
}
/********** HEXתBCD******/
/***bcd_data(<0x65535,>0)***/
Uint16 BCD4HEX(Uint32 bcd_data)
{
    Uint16 temp;
    temp=  (((bcd_data&0x000f0000)>>16)*10000)
          +(((bcd_data&0x0000f000)>>12)*1000)
          +(((bcd_data&0x00000f00)>>8)*100)
          +(((bcd_data&0x000000f0)>>4)*10)
          +  (bcd_data&0x0000000f);
    return temp;
}
/********** HEXתBCD******/
/***hex_data(<0xff,>0)****/

Uint32 HEX4BCD(Uint16 hex_data)
{
    Uint32 bcd_data;
    Uint16 temp;
    temp=hex_data%10000;
    bcd_data=((unsigned long)hex_data)/10000<<16;
    bcd_data=bcd_data|((temp/1000)<<12);
    temp=hex_data%1000;
    bcd_data=bcd_data|((temp/100)<<8);
    temp=hex_data%100;
    bcd_data=bcd_data|((temp/10)<<4);
    bcd_data=bcd_data|temp%10;
    return bcd_data;
}
