#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

Uint16 fm24cl04_write(Uint16 device,Uint16 add,Uint16 wbyte)
{
	Uint16 i;
	
	start1();
	outbyte1(device);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	outbyte1(add);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	outbyte1(wbyte);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	stop1();	
	return 1;
}

Uint16 fm24cl04_wrs(Uint16 device,Uint16 add, Uint16 * in, Uint16 len)
{
	Uint16 i,j;
	
	start1();
	outbyte1(device);
	j=get_ack1();
	if (j) 
	{
		stop1();
		return 0;
	}
		
	outbyte1(add);
	j=get_ack1();
	if (j) 
	{
		stop1();
		return 0;
	}
	
	for(i=0;i<len;i++)
	{
		outbyte1(*(in+i));
		j=get_ack1();
		if (j) 
		{
			stop1();
			return 0;	
		}
	}
	stop1();	
	return 1;
}

Uint16 fm24cl04_read(Uint16 device,Uint16 add, Uint16 *dat)
{
	Uint16 ret;
	
	start1();
	
	outbyte1(device);
	ret=get_ack1();
	if (ret) 
	{
		stop1();
		return 0;
	}
	
	outbyte1(add);
	ret=get_ack1();
	if (ret) 
	{
		stop1();
		return 0;
	}
	
	start1();
	outbyte1(device+1);
	ret=get_ack1();
	if (ret) 
	{
		stop1();
		return 0;
	}
	
	*dat=inbyte1();
	put_ack1(1);
	
	stop1();
	return(1);
}

Uint16 fm24cl04_rds(Uint16 device,Uint16 add, Uint16 * out, Uint16 len)
{
	Uint16 i;
	
	start1();
	outbyte1(device);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	outbyte1(add);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	start1();
	outbyte1(device+1);
	i=get_ack1();
	if (i) 
	{
		stop1();
		return 0;
	}
	
	for(i=0;i<len;i++)
	{
		*(out+i)=inbyte1();
		if (i==len-1) put_ack1(1);
		else put_ack1(0);
	}
	stop1();
	return 1;
}
