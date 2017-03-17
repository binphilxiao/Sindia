// TI File $Revision: /main/11 $
// Checkin $Date: May 12, 2008   14:30:08 $
//###########################################################################
//
// FILE:   DSP2833x_GlobalPrototypes.h
//
// TITLE:  Global prototypes for DSP2833x Examples
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_GLOBALPROTOTYPES_H
#define DSP2833x_GLOBALPROTOTYPES_H


#ifdef __cplusplus
extern "C" {
#endif

/*---- shared global function prototypes -----------------------------------*/
extern void InitAdc(void);

extern void DMAInitialize(void);
// DMA Channel 1
extern void DMACH1AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH1TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH1WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH1(void);
// DMA Channel 2
extern void DMACH2AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH2BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH2TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH2WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH2ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH2(void);
// DMA Channel 3
extern void DMACH3AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH3BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH3TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH3WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH3ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH3(void);
// DMA Channel 4
extern void DMACH4AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH4BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH4TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH4WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH4ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH4(void);
// DMA Channel 5
extern void DMACH5AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH5BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH5TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH5WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH5ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH5(void);
// DMA Channel 6
extern void DMACH6AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH6BurstConfig(Uint16 bsize,Uint16 srcbstep, int16 desbstep);
extern void DMACH6TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH6WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH6ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH6(void);
extern void InitPeripherals(void);
#if DSP28_ECANA
extern void InitECan(void);
extern void InitECana(void);
extern void InitECanGpio(void);
extern void InitECanaGpio(void);
#endif // endif DSP28_ECANA
#if DSP28_ECANB
extern void InitECanb(void);
extern void InitECanbGpio(void);
#endif // endif DSP28_ECANB
extern void InitECap(void);
extern void InitECapGpio(void);
extern void InitECap1Gpio(void);
extern void InitECap2Gpio(void);
#if DSP28_ECAP3
extern void InitECap3Gpio(void);
#endif // endif DSP28_ECAP3
#if DSP28_ECAP4
extern void InitECap4Gpio(void);
#endif // endif DSP28_ECAP4
#if DSP28_ECAP5
extern void InitECap5Gpio(void);
#endif // endif DSP28_ECAP5
#if DSP28_ECAP6
extern void InitECap6Gpio(void);
#endif // endif DSP28_ECAP6
extern void InitEPwm(void);
extern void InitEPwmGpio(void);
extern void InitEPwm1Gpio(void);
extern void InitEPwm2Gpio(void);
extern void InitEPwm3Gpio(void);
#if DSP28_EPWM4
extern void InitEPwm4Gpio(void);
#endif // endif DSP28_EPWM4
#if DSP28_EPWM5
extern void InitEPwm5Gpio(void);
#endif // endif DSP28_EPWM5
#if DSP28_EPWM6
extern void InitEPwm6Gpio(void);
#endif // endif DSP28_EPWM6
#if DSP28_EQEP1
extern void InitEQep(void);
extern void InitEQepGpio(void);
extern void InitEQep1Gpio(void);
#endif // if DSP28_EQEP1
#if DSP28_EQEP2
extern void InitEQep2Gpio(void);
#endif // endif DSP28_EQEP2
extern void InitGpio(void);
extern void InitI2CGpio(void);
extern void InitMcbsp(void);
extern void InitMcbspa(void);
extern void delay_loop(void);
extern void InitMcbspaGpio(void);
extern void InitMcbspa8bit(void);
extern void InitMcbspa12bit(void);
extern void InitMcbspa16bit(void);
extern void InitMcbspa20bit(void);
extern void InitMcbspa24bit(void);
extern void InitMcbspa32bit(void);
#if DSP28_MCBSPB
extern void InitMcbspb(void);
extern void InitMcbspbGpio(void);
extern void InitMcbspb8bit(void);
extern void InitMcbspb12bit(void);
extern void InitMcbspb16bit(void);
extern void InitMcbspb20bit(void);
extern void InitMcbspb24bit(void);
extern void InitMcbspb32bit(void);
#endif // endif DSP28_MCBSPB
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitSci(void);
extern void InitSciGpio(void);
extern void InitSciaGpio(void);
#if DSP28_SCIB
extern void InitScibGpio(void);
#endif // endif DSP28_SCIB
#if DSP28_SCIC
extern void InitScicGpio(void);
#endif
extern void InitSpi(void);
extern void InitSpiGpio(void);
extern void InitSpiaGpio(void);
extern void InitSysCtrl(void);
extern void InitTzGpio(void);
extern void InitXIntrupt(void);
extern void XintfInit(void);
extern void InitXintf16Gpio();
extern void InitXintf32Gpio();
extern void InitPll(Uint16 pllcr, Uint16 clkindiv);
extern void InitPeripheralClocks(void);
extern void EnableInterrupts(void);
extern void DSP28x_usDelay(Uint32 Count);
extern void ADC_cal (void);
#define KickDog ServiceDog     // For compatiblity with previous versions
extern void ServiceDog(void);
extern void DisableDog(void);
extern Uint16 CsmUnlock(void);
// DSP28_DBGIER.asm
extern void SetDBGIER(Uint16 dbgier);
//                 CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
extern void InitFlash(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
//---------------------------------------------------------------------------
// External symbols created by the linker cmd file
// DSP28 examples will use these to relocate code from one LOAD location
// in either Flash or XINTF to a different RUN location in internal
// RAM
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 XintffuncsLoadStart;
extern Uint16 XintffuncsLoadEnd;
extern Uint16 XintffuncsRunStart;
#ifdef __cplusplus
}
#endif /* extern "C" */
#endif   // - end of DSP2833x_GLOBALPROTOTYPES_H

/****************************************************/
//===========================================================================
struct PLATFORM
{
	Uint16 ver;
	Uint16 sub_ver;	
	Uint16 flag;
	Uint16 store_number;
	Uint16 gas;
	Uint16 usb_num;
	Uint16 sensor[3];
	Uint16 sensor_number;
	Uint16 clean;
	Uint16 pur_add;
};
struct CLOCK
{
	Uint16 year;
	Uint16 month;
	Uint16 week;
	Uint16 date;
	Uint16 hour;
	Uint16 minute;
	Uint16 second;
};
struct KEY
{
	Uint16 keynew[8];
	Uint16 x;
	Uint16 y;
	Uint16 buffer[16];
	Uint16 flag;
};

struct CALIB_2
{
	struct CLOCK clk;
	float k;
	float realdot;
	float stddot;
};
struct CALIB_7
{
	struct CLOCK clk;
	float dot0;
	float dot1;
	float dot2;
	float k1[3];
	float k2[3];
	float k3[3];
};
struct CALIB_CROSS
{
	float cross_so2_k;
	float cross_co_k;
};
struct SENSOR
{
	float val;
	float calib_val;

	Uint16 at24_add;//iic flash ´æ´¢µØÖ·
	Uint16 temp_add;//ÎÂ¶È´«¸ÐÆ÷
	Uint16 ad_ch;//SO21,SO22,H2S,CO

	float zero_spy[50];
	float zero;
	float measure_old;
	float measure_min;
	float spy_min;
	Uint16 zero_count;

	Uint16 calib_vender_flag;
	Uint16 calib_usr_flag;
	Uint16 calib_cross_so2_flag;
	Uint16 calib_cross_co_flag;
	
	struct CALIB_7 calib_vender;
	struct CALIB_2 calib_usr;
	struct CALIB_CROSS calib_cro;
};
struct FLOW
{
	float val;
	Uint16 cnt;
	Uint16 bar;
	Uint16 alarm;
	Uint16 flag;
};
struct MEASURE_DATA
{
	float  h2o_calib_value;
	float  so2_calib_value;
	float  h2s_calib_value;
	float  hf_calib_value;
	float  pur_calib_value;
	Uint16 flw;
	float  h2oppm;
	float  h2oppm20;
	float  c_humid;
	float  c_temp;
	struct CLOCK clk;
	struct CLOCK clock;
	Uint16 number[4];
	Uint16 sensor[3];
	Uint16 sensor_number;
	Uint16 instrument;
	Uint16 temp[13];
};
//-----------------------------------------------------------------------------------
extern struct CLOCK clock;
extern struct PLATFORM plat;
extern struct KEY key;

extern struct CLOCK measure_time;

extern struct SENSOR h2o;
extern struct SENSOR so2;
extern struct SENSOR h2s;
extern struct SENSOR co;
extern struct SENSOR purity;
extern struct SENSOR temprature;
extern float  ppm,ppm20;
extern struct FLOW flw;
extern float  humidity;

extern float in1[3],in2[3];

extern Uint16 const color[7][7];
extern char *flow_lable[];
extern Uint16 run_sht11_flag,run_clock_flag,run_flow_flag,run_temp_flag,run_ad_flag;
extern Uint32 temp_cnt,flow_cnt,clock_cnt,sht11_cnt;
//-----------------------------------------------------------------------------------
//display_touch.c
extern void print_string(char *dat);
extern void color_change(Uint16 front,Uint16 back);
extern void scib_xmit(Uint16 a);
extern void scib_loopback_init();
extern void scic_xmit(Uint16 a);
extern void print_string_c(char *dat);
extern void scia_send_byte(Uint16 a);
extern Uint16 scia_receive_byte(void);
extern Uint16 scia_status(void);
extern void cls(void);
extern void display_ascii(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,char *strings);
extern Uint16 BCD2HEX(Uint16 bcd_data);
extern Uint16 HEX2BCD(Uint16 hex_data);
extern Uint16 BCD4HEX(Uint32 bcd_data);
extern Uint32 HEX4BCD(Uint16 hex_data);
extern void sflt_to_str(float in, char * out);
extern void display_number(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 x1);
extern void display_dot(Uint16 x,Uint16 y);
extern void display_picture(Uint16 number,Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 x,Uint16 y);
extern void display_clock(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y);
extern void dw_int(void);
extern void dw_int1(void);
extern void display_num_ascii(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 num,char *strings);
extern void display_measure_time(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y);
extern void display_fp_ascii(Uint16 dots, Uint16 front, Uint16 back, Uint16 switch1,Uint16 x, Uint16 y, float x1, Uint16 bit, char *strings) ;
extern void display_num_ascii1(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 num,char *strings);
extern void display_battery(Uint32 x,Uint32 y,Uint16 code);
//fm22l16.c
extern Uint16 fm22l16_rd(Uint32 address);
extern void fm22l16_wr(Uint32 address,Uint16 data);
extern void fm22l16_wrs(Uint32 address,Uint16 * source,Uint16 number);
extern void fm22l16_rds(Uint32 address,Uint16 * destination,Uint16 number);
//rx8025.c
extern void rx8025_clock_init(void);
extern void rx8025_time_set(struct CLOCK * c);
extern void rx8025_date_set(struct CLOCK * c);
extern void rx8025_clock_read(struct CLOCK * c);
//ADS8401.C
extern void fp32_ad_get(Uint16 chl, float * out);
//key.c
extern void key_clr(void);
extern void key_get(void);
//fm24cl04.c
extern Uint16 fm24cl04_write(Uint16 device,Uint16 add,Uint16 wbyte);
extern Uint16 fm24cl04_wrs(Uint16 device,Uint16 add, Uint16 * in, Uint16 len);
extern Uint16 fm24cl04_read(Uint16 device,Uint16 add, Uint16 * dat);
extern Uint16 fm24cl04_rds(Uint16 device,Uint16 add, Uint16 * out, Uint16 len);
//calib.c
extern void calib_do( void );
//sht11.c
extern Uint16 sht71_ht(float *p_humidity ,float *p_temperature);
extern void get_Concentration(float * out);
extern Uint16 get_zero(void);
extern Uint16 set_zero(void);
//menu.c
extern void menu(void);
extern void parameter_init(void);
extern void LingDianGenZong(Uint16 chl);     //Áãµã¸ú×Ù
extern void measure_buffer_init(Uint16 chl);
extern void measure_buffer_fill(Uint16 chl);
////////////////////////////////////////////////////////  
void disp_ca100(Uint16 page);
extern Uint16 zero_set_ca100(void);
extern void display_gpa(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);//x<999.999
