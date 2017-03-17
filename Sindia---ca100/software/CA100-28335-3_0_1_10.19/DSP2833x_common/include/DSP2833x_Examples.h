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

//--------------------------AT24C04存储地址的分配-----------------------------------
#define   ADD24_SENSOR_ID         0x0
#define   ADD24_CALIB7_FLAG      (ADD24_SENSOR_ID+sizeof(Uint16))	
#define   ADD24_CALIB7_VAL        (ADD24_CALIB7_FLAG+sizeof(Uint16))	
#define   ADD24_CALIB7_TIME      (ADD24_CALIB7_VAL+sizeof(struct CALIB_7)*2)	
#define   ADD24_END                     (ADD24_CALIB7_TIME+sizeof(struct CLOCK)*2)
//--------------------------系统设置存储空间-----------------------------------
#define   ADD_FLAG                        0 //首次上电标志位
#define   ADD_GAS                         1 //测量气体
#define   ADD_LANGUAGE              2 //语言
#define   ADD_ID                            3 //设备ID号
#define   ADD_USB_NUM                4//
#define   ADD_CNT                         5//测量记录计数 
#define   ADD_PASSWORD              6//密码 占用2个地址位
#define   ADD_PUR                   8
#define   ADD_PLAT_END               0x10
//--------------------------零点数据存储空间-----------------------------------
#define   ADD_H2O_ZERO          (ADD_PLAT_END+0x01)
#define   ADD_PUR_ZERO          (ADD_PLAT_END+0x02)
#define   ADD_SO21_ZERO        (ADD_PLAT_END+0x03)
#define   ADD_SO22_ZERO        (ADD_PLAT_END+0x04)
#define   ADD_H2S_ZERO          (ADD_PLAT_END+0x05)
#define   ADD_CO_ZERO            (ADD_PLAT_END+0x06)
#define   ADD_HF_ZERO            (ADD_PLAT_END+0x07)

#define   ADD_ZERO_END          (ADD_PLAT_END+0x10)
//--------------------------校准标志位存储空间-------------------------------
#define   ADD_CALIBVENDER_H2O_FLAG           (ADD_ZERO_END+0x00)//厂家校准标志位
#define   ADD_CALIBVENDER_PUR_FLAG 	    (ADD_ZERO_END+0x01)
#define   ADD_CALIBVENDER_TEMP_FLAG         (ADD_ZERO_END+0x02)

#define   ADD_CALIBUSR_H2O_FLAG                 (ADD_ZERO_END+0x03)//用户校准标志位
#define   ADD_CALIBUSR_PUR_FLAG                 (ADD_ZERO_END+0x04)
#define   ADD_CALIBUSR_H2S_FLAG                 (ADD_ZERO_END+0x05)
#define   ADD_CALIBUSR_SO21_FLAG               (ADD_ZERO_END+0x06)
#define   ADD_CALIBUSR_SO22_FLAG               (ADD_ZERO_END+0x07)
#define   ADD_CALIBUSR_CO_FLAG                   (ADD_ZERO_END+0x08)
#define   ADD_CALIBUSR_HF_FLAG                   (ADD_ZERO_END+0x09)

#define   ADD_CROSS_CO_FLAG                        (ADD_ZERO_END+0x0a)//交叉干扰校正标志位
#define   ADD_CROSS_SO2_FLAG                      (ADD_ZERO_END+0x0b)

#define   ADD_CALIB_FLAG_END                       (ADD_ZERO_END+0x10)
//--------------------------厂家校准时间存储空间-----------------------------
#define   ADD_CALIBVENDERTIME_H2O           (ADD_CALIB_FLAG_END +sizeof(struct CLOCK)*0x00)//厂家校准时间
#define   ADD_CALIBVENDERTIME_PUR 	         (ADD_CALIB_FLAG_END +sizeof(struct CLOCK)*0x01)
#define   ADD_CALIBVENDERTIME_TEMP         (ADD_CALIB_FLAG_END +sizeof(struct CLOCK)*0x02)

#define   ADD_CALIBVENDERTIME_END           (ADD_CALIB_FLAG_END +sizeof(struct CLOCK)*0x10)
//--------------------------厂家校准数据存储空间-----------------------------
#define   ADD_CALIBVENDER_H2O_INPUT1       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x00)
#define   ADD_CALIBVENDER_H2O_INPUT2       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x01)

#define   ADD_CALIBVENDER_PUR_INPUT1       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x02)
#define   ADD_CALIBVENDER_PUR_INPUT2       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x03)

#define   ADD_CALIBVENDER_TEMP_INPUT1     (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x04)
#define   ADD_CALIBVENDER_TEMP_INPUT2     (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x05)

#define   ADD_CALIBVENDER_H2S_INPUT1       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x06)
#define   ADD_CALIBVENDER_H2S_INPUT2       (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x07)

#define   ADD_CALIBVENDER_SO21_INPUT1      (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x08)
#define   ADD_CALIBVENDER_SO21_INPUT2      (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x09)

#define   ADD_CALIBVENDER_SO22_INPUT1      (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0a)
#define   ADD_CALIBVENDER_SO22_INPUT2      (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0b)

#define   ADD_CALIBVENDER_CO_INPUT1          (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0c)
#define   ADD_CALIBVENDER_CO_INPUT2          (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0d)

#define   ADD_CALIBVENDER_HF_INPUT1          (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0e)
#define   ADD_CALIBVENDER_HF_INPUT2          (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x0f)

#define   ADD_CALIBVENDER_INPUT_END          (ADD_CALIBVENDERTIME_END+sizeof(float)*7*0x20)
//--------------------------厂家校准计算数据存储空间-----------------------------
#define   ADD_CALIBVENDER_H2O                     (ADD_CALIBVENDER_INPUT_END+sizeof(struct CALIB_7)*0x00)
#define   ADD_CALIBVENDER_PUR                     (ADD_CALIBVENDER_INPUT_END+sizeof(struct CALIB_7)*0x01)
#define   ADD_CALIBVENDER_TEMP                   (ADD_CALIBVENDER_INPUT_END+sizeof(struct CALIB_7)*0x02)

#define   ADD_CALIBVENDER_DATA_END          (ADD_CALIBVENDER_INPUT_END+sizeof(struct CALIB_7)*0x08)
//--------------------------用户校准数据存储空间-----------------------------
#define   ADD_CALIBUSR_H2O_INPUT1        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x00)
#define   ADD_CALIBUSR_H2O_INPUT2        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x01)

#define   ADD_CALIBUSR_PUR_INPUT1        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x02)
#define   ADD_CALIBUSR_PUR_INPUT2        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x03)

#define   ADD_CALIBUSR_H2S_INPUT1        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x04)
#define   ADD_CALIBUSR_H2S_INPUT2        (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x05)

#define   ADD_CALIBUSR_SO21_INPUT1      (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x06)
#define   ADD_CALIBUSR_SO21_INPUT2      (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x07)

#define   ADD_CALIBUSR_SO22_INPUT1      (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x08)
#define   ADD_CALIBUSR_SO22_INPUT2      (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x09)

#define   ADD_CALIBUSR_CO_INPUT1          (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x0a)
#define   ADD_CALIBUSR_CO_INPUT2          (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x0b)

#define   ADD_CALIBUSR_HF_INPUT1          (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x0c)
#define   ADD_CALIBUSR_HF_INPUT2          (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x0d)

#define   ADD_CALIBUSR_INPUT_END          (ADD_CALIBVENDER_DATA_END+sizeof(float)*3*0x20)
//--------------------------用户校准计算值存储空间-----------------------------
#define   ADD_CALIBUSR_H2O                   (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x00)
#define   ADD_CALIBUSR_PUR                   (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x01)
#define   ADD_CALIBUSR_TEMP                 (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x02)
#define   ADD_CALIBUSR_H2S                   (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x03)
#define   ADD_CALIBUSR_SO21                 (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x04)
#define   ADD_CALIBUSR_SO22                 (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x05)
#define   ADD_CALIBUSR_CO                     (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x06)
#define   ADD_CALIBUSR_HF                     (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x07)

#define   ADD_CALIBUSR_DATA_END        (ADD_CALIBUSR_INPUT_END+sizeof(struct CALIB_2)*0x10)
//--------------------------交叉干扰数据存储空间--------------------------------
#define   ADD_CROSS_SO2_H2S_INPUT1   (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x00)
#define   ADD_CROSS_SO2_H2S_INPUT2   (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x01)
#define   ADD_CROSS_SO2_H2S_INPUT3   (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x02)

#define   ADD_CROSS_CO_H2S_INPUT1     (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x03)
#define   ADD_CROSS_CO_H2S_INPUT2     (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x04)
#define   ADD_CROSS_CO_H2S_INPUT3     (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x05)

#define   ADD_CROSS_INPUT_END             (ADD_CALIBUSR_DATA_END+sizeof(float)*7*0x08)
//--------------------------交叉干扰计算值存储空间-----------------------------
#define   ADD_CROSS_SO2_H2S                 (ADD_CROSS_INPUT_END+0x00)
#define   ADD_CROSS_CO_H2S                   (ADD_CROSS_INPUT_END+0x01)

#define   ADD_CROSS_DATA_END              (ADD_CROSS_INPUT_END+0x08)
//-----------------------------------------------------------------------------------------
#define   ADD_RECORD_BASE                     (ADD_CROSS_DATA_END+0x00)//存储基地址
#define   ADD_RECORD_END                       0x40000//存储末地址
#define   RECORD_SIZE                               ((ADD_RECORD_END-ADD_RECORD_BASE)/sizeof(struct MEASURE_DATA))//可存储的条目数
//-----------------------------------------------------------------------------------------
#define HF 1
#define CO 2
#define H2S 3
#define H2O 4
#define SO2 5
#define SO2_DEN 6
#define VOUT 7
#define IOUT 0x0f
#define CH_FLOW 0x17

#define H2O_MEASURE 2
#define PUR_MEASURE 2
#define SO2_MEASURE 50
#define H2S_MEASURE 50
#define CO_MEASURE 50
#define FLW_MEASURE 2
///////////////////////////////////////////////////////////////////////////////
#define ERROR      1
#define OK            0

#define HIGH       1
#define LOW        0
#define ON           1
#define OFF         0

#define M12 0
#define M16 1
#define M24 2
#define M32 3
#define M64 4
#define M80 5
//----------------------------------------------------------------------------
#define BLUE                 0x001f
#define BLUE_GREEN     0x0bf2
#define WHITE               0xffff
#define BLACK                0x0000
#define GREEN                0x07e0
#define PURPLE               0xf81f
#define YELLOW               0xFFE0

#define AD_busy GpioDataRegs.GPBDAT.bit.GPIO61
#define ANSA0(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO17=1):(GpioDataRegs.GPASET.bit.GPIO17=1)
#define ANSA1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO16=1):(GpioDataRegs.GPASET.bit.GPIO16=1)
#define ANSA2(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO19=1):(GpioDataRegs.GPASET.bit.GPIO19=1)
#define ANSA3(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO18=1):(GpioDataRegs.GPASET.bit.GPIO18=1)
#define ANSA4(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO53=1):(GpioDataRegs.GPBSET.bit.GPIO53=1)

#define K5A0(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO5=1):(GpioDataRegs.GPASET.bit.GPIO5=1)
#define K6A1(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO6=1):(GpioDataRegs.GPASET.bit.GPIO6=1)

#define SPK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO9=1):(GpioDataRegs.GPASET.bit.GPIO9=1)

#define SCK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO1=1):(GpioDataRegs.GPASET.bit.GPIO1=1)
#define DATA(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO2=1):(GpioDataRegs.GPASET.bit.GPIO2=1)
#define HUMID 0
#define TEMPR 1

#define KG1(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO48=1):(GpioDataRegs.GPBSET.bit.GPIO48=1)
#define KG2(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO49=1):(GpioDataRegs.GPBSET.bit.GPIO49=1)
#define KG3(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO50=1):(GpioDataRegs.GPBSET.bit.GPIO50=1)
#define BPL(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO5=1):(GpioDataRegs.GPASET.bit.GPIO5=1)
#define HEATER(x) (x==0)?(GpioDataRegs.GPBCLEAR.bit.GPIO52=1):(GpioDataRegs.GPBSET.bit.GPIO52=1)

#define UDSEL(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO10=1):(GpioDataRegs.GPASET.bit.GPIO10=1)
#define UDSCK(x) (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO11=1):(GpioDataRegs.GPASET.bit.GPIO11=1)
#define UDSO(x)  (x==0)?(GpioDataRegs.GPACLEAR.bit.GPIO12=1):(GpioDataRegs.GPASET.bit.GPIO12=1)
//===========================================================================
// End of file.
//===========================================================================
