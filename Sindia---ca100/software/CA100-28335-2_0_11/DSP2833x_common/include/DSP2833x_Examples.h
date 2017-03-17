// TI File $Revision: /main/9 $
// Checkin $Date: July 2, 2008   14:31:12 $
//###########################################################################
//
// FILE:   DSP2833x_Examples.h
//
// TITLE:  DSP2833x Device Definitions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_EXAMPLES_H
#define DSP2833x_EXAMPLES_H


#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
      Specify the PLL control register (PLLCR) and divide select (DIVSEL) value.
-----------------------------------------------------------------------------*/
//#define DSP28_DIVSEL   0   // Enable /4 for SYSCLKOUT
//#define DSP28_DIVSEL   1 // Enable /4 for SYSCKOUT
#define DSP28_DIVSEL     2 // Enable /2 for SYSCLKOUT
//#define DSP28_DIVSEL     3 // Enable /1 for SYSCLKOUT

#define DSP28_PLLCR   10
//#define DSP28_PLLCR    9
//#define DSP28_PLLCR    8
//#define DSP28_PLLCR    7
//#define DSP28_PLLCR    6
//#define DSP28_PLLCR    5
//#define DSP28_PLLCR    4
//#define DSP28_PLLCR    3
//#define DSP28_PLLCR    2
//#define DSP28_PLLCR    1
//#define DSP28_PLLCR    0  // PLL is bypassed in this mode
//----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
      Specify the clock rate of the CPU (SYSCLKOUT) in nS.

      Take into account the input clock frequency and the PLL multiplier
      selected in step 1.

      Use one of the values provided, or define your own.
      The trailing L is required tells the compiler to treat
      the number as a 64-bit value.

      Only one statement should be uncommented.

      Example 1:150 MHz devices:
                CLKIN is a 30MHz crystal.

                In step 1 the user specified PLLCR = 0xA for a
                150Mhz CPU clock (SYSCLKOUT = 150MHz).

                In this case, the CPU_RATE will be 6.667L
                Uncomment the line:  #define CPU_RATE  6.667L

      Example 2:  100 MHz devices:
                  CLKIN is a 20MHz crystal.

	              In step 1 the user specified PLLCR = 0xA for a
	              100Mhz CPU clock (SYSCLKOUT = 100MHz).

	              In this case, the CPU_RATE will be 10.000L
                  Uncomment the line:  #define CPU_RATE  10.000L
-----------------------------------------------------------------------------*/
#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    7.143L   // for a 140MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   10.000L   // for a 100MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz CPU clock speed  (SYSCLKOUT)

//----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
      Target device (in DSP2833x_Device.h) determines CPU frequency
      (for examples) - either 150 MHz (for 28335 and 28334) or 100 MHz
      (for 28332). User does not have to change anything here.
-----------------------------------------------------------------------------*/
#if DSP28_28332                   // DSP28_28332 device only
  #define CPU_FRQ_100MHZ    1     // 100 Mhz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_150MHZ    0
#else
  #define CPU_FRQ_100MHZ    0     // DSP28_28335||DSP28_28334
  #define CPU_FRQ_150MHZ    1     // 150 MHz CPU Freq (30 MHz input freq) by DEFAULT
#endif


//---------------------------------------------------------------------------
// Include Example Header Files:
//

#include "DSP2833x_GlobalPrototypes.h"         // Prototypes for global functions within the
                                              // .c files.

#include "DSP2833x_ePwm_defines.h"             // Macros used for PWM examples.
#include "DSP2833x_Dma_defines.h"              // Macros used for DMA examples.
#include "DSP2833x_I2C_defines.h"              // Macros used for I2C examples.

#define PARTNO_28335  0xEF
#define PARTNO_28334  0xEE
#define PARTNO_28332  0xED
#define PARTNO_28235  0xE8
#define PARTNO_28234  0xE7
#define PARTNO_28232  0xE6


// Include files not used with DSP/BIOS
#ifndef DSP28_BIOS
#include "DSP2833x_DefaultISR.h"
#endif


// DO NOT MODIFY THIS LINE.
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_EXAMPLES_H definition




/*存储地址的分配   */

#define   ADD_GAS      1 //测量气体
#define   ADD_LANGUAGE 2 //语言
#define   ADD_CHECK1   3
#define   ADD_CHECK2   4
#define   ADD_CHECK3   5
#define   ADD_CHECK4   6
#define   ADD_USB_NUM  7


#define   ADD_CALIB_H2O    10//
//#define   ADD_CALIB_SO2    20
//#define   ADD_CALIB_H2S    30
#define   ADD_CNT          40//记录计数 4字节
#define   ADD_PUR          48
#define   ADD_SO2_0        50
#define   ADD_H2S_0        60
#define   ADD_HF_0         70

#define   ADD_INSTRUMENT_CHECK   80
#define   ADD_INSTRUMENT         81

#define   ADD_CALIB_H2O_INPUT1       100//40个存储空间
#define   ADD_CALIB_H2O_INPUT2       140//40个存储空间

#define   ADD_CALIB_H2S_INPUT1       180//40个存储空间
#define   ADD_CALIB_H2S_INPUT2       220//40个存储空间

#define   ADD_CALIB_HF_INPUT1       260//40个存储空间
#define   ADD_CALIB_HF_INPUT2       300//40个存储空间

#define   ADD_CALIB_SO2_INPUT1      340//40个存储空间
#define   ADD_CALIB_SO2_INPUT2      380//40个存储空间



#define   ADD_CALIB_H2O_TIME      430//7个存储空间

#define   ADD_CALIB1_H2O_K      450//18个存储空间
#define   ADD_CALIB1_SO2_K      500//18个存储空间  用于单点和俩点校准时存储数据
#define   ADD_CALIB1_H2S_K      550//18个存储空间
#define   ADD_CALIB1_HF_K       600//18个存储空间
#define   ADD_CALIB1_GPA_K       650//18个存储空间

#define   ADD_CALIB_GPA_INPUT1      700//40个存储空间
#define   ADD_CALIB_GPA_INPUT2      750//40个存储空间
#define   ADD_CALIB_GPA_TIME        800//7个存储空间
#define   ADD_CALIB_GPA             810
#define   ADD_PASSWORD      950//4个存储空间
#define   ADD_PASSWORD_FLAG 960//1个存储空间
#define   ADD_GPA_0         965//2个存储空间

#define   JIAOCHA_SO2_FLAG     968//1个存储空间,交叉校准标志0X5555
#define   ADD_SO2_H2S_INPUT1   970//40个存储空间,上次原始数据SO2
#define   ADD_SO2_H2S_INPUT2   1010//20个存储空间,上次原始数据H2S
#define   ADD_SO2_H2S_INPUT3   1030//20个存储空间,上次原始数据H2S
#define   ADD_SO2_H2S_TIME     1050//7个存储空间,上次校准时间
#define   ADD_SO2_H2S          1060//2个存储空间,计算的校准系数

#define   JIAOCHA_CO_FLAG     1062//1个存储空间,交叉校准标志0X5555
#define   ADD_CO_H2S_INPUT1   1063//40个存储空间,上次原始数据SO2
#define   ADD_CO_H2S_INPUT2   1103//20个存储空间,上次原始数据H2S
#define   ADD_CO_H2S_INPUT3   1123//20个存储空间,上次原始数据H2S
#define   ADD_CO_H2S_TIME     1143//7个存储空间,上次校准时间
#define   ADD_CO_H2S          1152//2个存储空间,计算的校准系数

#define   TEMPERATURE_FLAG     1154//1个存储空间,温度湿度校准标志0X5555
#define   TEMP_MEASURE_INPUT   1155//10个存储空间,上次温度原始数据
#define   TEMP_REAL_INPUT      1195//10个存储空间,上次湿度原始数据
#define   TEMP_TIME            1235//7个存储空间,上次校准时间
#define   ADD_TEMP_RANGE       1245//2个存储空间,计算的校准系数
#define   ADD_TEMP_OFFSET      1247//2个存储空间,计算的校准系数

#define   ADD_BASE     1250//存储基地址
#define   ADD_TAB      50//单位存储区大小

#define   ALL           1
#define   SINGLE        0
///////////////////////////////////////////////////////////////////////////////
#define FLOW       0
#define SO2        1
#define H2S        2
#define HF         3
#define AN18V      4
#define IOUT       5
#define H2O        6
#define PT100      7
#define STH11      8
#define GND        9
///////////////////////////////////////////////////////////////////////////////

#define ERROR      1
#define OK         0

#define HIGH       1
#define LOW        0
#define ON         1
#define OFF        0

#define M12 0
#define M16 1
#define M24 2
#define M32 3
#define M64 4
#define M80 5

//#define LDREG(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO10=1):(GpioDataRegs.GPASET.bit.GPIO10=1)
//#define DACL(x)  (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO14=1):(GpioDataRegs.GPASET.bit.GPIO14=1)
//#define DAOUT(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO13=1):(GpioDataRegs.GPASET.bit.GPIO13=1)
//#define LDAC(x)  (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO11=1):(GpioDataRegs.GPASET.bit.GPIO11=1)
//#define CSD(x)   (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO12=1):(GpioDataRegs.GPASET.bit.GPIO12=1)

#define AD_busy GpioDataRegs.GPBDAT.bit.GPIO61
#define ANSA0(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO49=1):(GpioDataRegs.GPBSET.bit.GPIO49=1)
#define ANSA1(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO50=1):(GpioDataRegs.GPBSET.bit.GPIO50=1)
#define ANSA2(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO51=1):(GpioDataRegs.GPBSET.bit.GPIO51=1)

#define K5A0(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO5=1):(GpioDataRegs.GPASET.bit.GPIO5=1)
#define K6A1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO6=1):(GpioDataRegs.GPASET.bit.GPIO6=1)

#define SPK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO9=1):(GpioDataRegs.GPASET.bit.GPIO9=1)

#define SCK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO1=1):(GpioDataRegs.GPASET.bit.GPIO1=1)
#define DATA(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO2=1):(GpioDataRegs.GPASET.bit.GPIO2=1)
#define HUMID 0
#define TEMPR 1

#define KG1(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO48=1):(GpioDataRegs.GPBSET.bit.GPIO48=1)
#define BPL(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO53=1):(GpioDataRegs.GPBSET.bit.GPIO53=1)
//#define COMPRESSER(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO52=1):(GpioDataRegs.GPBSET.bit.GPIO52=1)
#define HEATER(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO52=1):(GpioDataRegs.GPBSET.bit.GPIO52=1)
//#define SWITCH1(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO54=1):(GpioDataRegs.GPBSET.bit.GPIO54=1)

#define UDSEL(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO10=1):(GpioDataRegs.GPASET.bit.GPIO10=1)
#define UDSCK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO11=1):(GpioDataRegs.GPASET.bit.GPIO11=1)
#define UDSO(x)  (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO12=1):(GpioDataRegs.GPASET.bit.GPIO12=1)
//#define UDSI(x)  (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO10=1):(GpioDataRegs.GPASET.bit.GPIO10=1)


#define SCL1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO3=1):(GpioDataRegs.GPASET.bit.GPIO3=1)
#define SDA1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO4=1):(GpioDataRegs.GPASET.bit.GPIO4=1)


#define D_color  0x4698
#define E_color   0x87f0
/////////////////////////////////////////////////////////////////////////////
#define CA100_V2 (sub_ver&0x1==1 && sub_ver&0xfffffffe==0)?1:0//ca100把露点替换成co的版本

//===========================================================================
// End of file.
//===========================================================================
