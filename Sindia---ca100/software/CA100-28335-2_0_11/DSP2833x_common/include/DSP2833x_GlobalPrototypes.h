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
//dp30

extern char *flow_table1[];

extern Uint16 const colortab[4][4];
extern void display_1(Uint16 x);

//display_touch.c
extern void instruction_do(void);

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
extern void display_fp_number(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1);
extern void display_number(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 x1);
extern void display_jx(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2);
extern void display_dot(Uint16 x,Uint16 y);
extern void display_line(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2);
extern void display_picture(Uint16 number,Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 x,Uint16 y);
extern void display_picture_all(Uint16 number);
extern void display_clock(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y);
extern void display_jxy(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2);
extern void display_jx_clear(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2);
extern void picture_store(Uint16 number);
extern void dw_int(void);
extern void dw_int1(void);
extern void display_num_ascii(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 num,char *strings);
extern void display_measure_time(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y);
extern void display_fp4(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);
extern void display_fp_ascii(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);
extern void display_num_ascii1(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,Uint16 num,char *strings);
extern void display_battery(Uint32 x,Uint32 y,Uint16 code);
extern const Uint16 lib_mode[6][3];
extern void display_fp_ascii_ppm(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);//x<999.999
extern void display_fp_ascii_ppm1(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);//x<999.999





//fm22l16.c
extern Uint16 fm22l16_rd(Uint32 address);
extern void fm22l16_wr(Uint32 address,Uint16 data);
extern void fm22l16_wrs(Uint32 address,Uint16 *source,Uint16 number);
extern void fm22l16_rds(Uint32 address,Uint16 *destination,Uint16 number);
extern void fm22l16_float_wr(Uint32 address,float data);
extern float fm22l16_float_rd(Uint32 address);
extern void fm22l16_float_wrs(Uint32 address,float *source,Uint16 number);
extern void fm22l16_float_rds(Uint32 address,float *destination,Uint16 number);

struct FM_PARAMETER
{
Uint16 light;
Uint16 speed;
Uint16 cooler;
Uint16 gas;
Uint16 inspeed;
Uint16 calib_flag;
Uint16 calib;
Uint16 qca;
Uint16 heater;
Uint16 language;
};
extern struct FM_PARAMETER parameter;


//ppm.c

extern Uint16 ppm_do(float x,Uint16 switch1);
extern float ppm_backup,ppm_backup20;
extern Uint16 ha100_ppm_do(float x);



//rx8025.c
extern void rx8025_clock_inti(void);
void rx8025_time_set(Uint16 second,Uint16 minute,Uint16 hour);
void rx8025_date_set(Uint16 week,Uint16 date,Uint16 month,Uint16 year);
extern void rx8025_clock_read(void);
extern Uint16 i2c[20];
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

extern struct CLOCK clock;


//ADS8401.C
extern float fp_ad_get(Uint16 chl);
extern Uint16 fp32_ad_get(Uint16 chl);
extern float ad8401_value[3];
extern float so2_0,h2s_0;

//DAC7632.C
extern void  MSpiIni(void);
extern void dac7632(Uint32 chanl,Uint16 dat);

//key.c
extern void key_clr(void);
extern void key_get(void);
extern Uint16 keynew[6],databuffer[16],keyflag,key_x,key_y;
extern void u_key_get(void);
extern void u_key_draw(void);
extern Uint16 kp1,kp2,kp3,kk1,kk2,kk3,kf,km;
extern Uint16 u_key_cnt,u_key_enter;

extern Uint16 key_alarm_cnt,key_alarm_flag;
extern Uint32 key_cnt;


//set.c
extern void set(void);
extern void instrument_select(void);
extern void sensor_select(void);
extern Uint16 instrument;//0---CA100,1---HA100,2---DA100
extern Uint16 sensor[3],sensor_number;

//fm24cl04.c
extern Uint16 fm24cl04_read(Uint16 device,Uint16 add);
extern void fm24cl04_write(Uint16 device,Uint16 add,Uint16 wbyte);
extern void fm24cl04_test(void);

//calib.c
extern void calib_do( void );

//sht11.c
extern Uint16 sht71_ht(float *p_humidity ,float *p_temperature);

extern void purity_test(void);
extern void press_test(void);
extern float get_Concentration(void);
extern Uint16 get_zero(void);
extern Uint16 set_zero(void);

//menu.c
extern void menu(void);
extern void parameter_initi(void);
extern Uint16 instruction_cnt,instruction_amount;
extern Uint16 run_status,refresh_flag,program_status,run_ad_flag,run_light_flag;
extern Uint32 screen_cnt,balance_cnt,ad_cnt,alarm_cnt,light_cnt,temp_cnt,flow_cnt,clock_cnt,dot_cnt,sht11_cnt,linetab,measure_second_cnt;
extern Uint16 run_screen_flag,run_second_flag,run_half_flag,mirror_flag,run_temp_flag,run_flow_flag,run_clock_flag,run_dot_flag,run_sht11_flag;
extern char *y_table[];
struct MEASURE_TIME
{
Uint32 hour;
Uint32 minute;
Uint32 second;
};
extern struct MEASURE_TIME measure_time;
struct MEASURE_DATA
{
float  h2o_calib_value;
float  so2_calib_value;
float  h2s_calib_value;
Uint16 in8;
float  h2oppm;
float  h2oppm20;
float  c_humid;
float  c_temp;
Uint16 clock[7];
Uint16 number[4];
Uint16 temp[2];
};
struct MEASURE_DATA_0
{
float  h2o_calib_value;
float  so2_calib_value;
float  h2s_calib_value;
float  hf_calib_value;
float  gpa_calib_value;
Uint16 in8;
float  h2oppm;
float  h2oppm20;
float  c_humid;
float  c_temp;
Uint16 clock[7];
Uint16 number[4];
Uint16 sensor[3];
Uint16 sensor_number;
Uint16 instrument;
Uint16 temp[13];
};
extern struct MEASURE_DATA_0 hold_data;
extern struct MEASURE_DATA_0 store_data;


extern Uint16 hold_flag,gas;
extern float hold_value[3];
extern Uint16 calib_flag[3],store_number;

extern float jiaocha_SO2_k,jiaocha_CO_k;
extern Uint16 jiaocha_SO2_flag,jiaocha_CO_flag;

extern float temperature_range,temperature_offset;
extern Uint16 temperature_flag;

//control.c
extern void light_do(void);
extern void flow_do(void);
extern void dew_temp_do(void);
extern void display_light(void);
extern void measure(void);
extern void display_dots(void);
extern void half_do(void);
extern void init_value(void);
extern void search(void);


extern const float flow_tab[3][13];
extern Uint16 press_alarm_flag,flow_alarm_flag;
extern Uint16 flow_flag,flow_flag1,sf6n2;
extern float ram[100][8],light_k;
extern Uint32 ramcnt;
extern Uint32 ramcntbak,halfcnt;

extern Uint16 language;

extern float flwold;
struct CALIB
{
Uint16 flag;
float  k;
float  b;
};

extern struct CALIB calib;

extern Uint16 temp_count;

extern Uint16 runflag,doflag,zflag,lineflag,lightflag;
extern Uint16 in8;
extern Uint32 linecnt;
extern Uint16 jiashicnt;
extern Uint16 measure_flag;
extern Uint16 qca_flag;
extern Uint16 alarm_flow,low_flag,clean_flag;

extern float in1[3], in2[3];

struct TESTDATA
{
    Uint16 gas;
    Uint16 flow;
    Uint16 time[7];
	Uint16 time1[3];
	Uint16 balanceflag;
	Uint16 printflag;
	Uint16 ttt[8];
    float dew;
    float ppm;
    float ppm20;
	float c_temp;
	float c_humid;
	float data[241];
};
extern struct TESTDATA testdata;
extern void print_data(struct TESTDATA *data);

extern Uint16 data[20];
extern float qca_value;

extern float sum_e,basep,tmax,u_o,u_old,light_base_70;
extern float tempnew,tempnew1,tempold,lightnew,lightold;



extern float h2sold,h2s_calib_value,h2s_k[3]; 
extern float h2oold,h2o_calib_value,h2o_k[3];
extern float c_temp,c_temp_calib,c_humid;
extern float so2old,so2_calib_value,so2_k[3],so2_jiaocha_value;
extern float hfold,hf_calib_value,hf_k[3],hf_jiaocha_value;
extern float gpaold,gpa_calib_value;


extern char *t_report_cn;
extern char *t_report_cn1;
extern char *t_parameter_cn;
extern char *t_number_cn;
extern char *t_data_cn;
extern char *t_time_cn;
extern char *t_temp_cn;
extern char *t_humid_cn;
extern char *t_flow_cn;

extern char *t_result_cn;
extern char *t_dew_cn;
extern char *t_h2o_cn;
extern char *t_so2_cn;
extern char *t_h2s_cn;
extern char *t_hf_cn;
extern char *t_gpa_cn;

extern char *t_dot_cn;


struct LINE_K
{
    float middle_value;
	float k1[2];
	float k2[2];
	Uint16 flag;
	Uint16 dot;
	Uint16 time[7];
	float biao[2];
	float measure[2];
};
extern struct LINE_K h2o_1;
extern struct LINE_K so2_1;
extern struct LINE_K h2s_1;
extern struct LINE_K hf_1;
extern struct LINE_K gpa_1;
struct CALIB_K
{
    float calib_dot0;
    float calib_dot1;
    float calib_dot2;
	float calib_k1[3];
	float calib_k2[3];
	float calib_k3[3];
	
};
extern struct CALIB_K h2o;
extern struct CALIB_K so2;
extern struct CALIB_K h2s;
extern struct CALIB_K hf;
extern struct CALIB_K gpa;
extern struct CALIB_K tempu;


extern float so2_zero_spy[50],h2s_zero_spy[50],hf_zero_spy[50];
extern float so2_zero,h2s_zero,hf_zero,gpa_zero;
extern float so2_measure_old,h2s_measure_old,hf_measure_old;
extern float so2_measure_min,h2s_measure_min,hf_measure_min;
extern float so2_spy_min,h2s_spy_min,hf_spy_min;
extern Uint16 so2_zero_count,h2s_zero_count,hf_zero_count;

extern char  *soft_version;

extern void LingDianGenZong(Uint16 chl);     //零点跟踪
extern void measure_buffer_ini(Uint16 chl);
extern void measure_buffer_fill(Uint16 chl);

////////////////////////////////////////////////////////  
extern struct TESTDATA ha100_store_data,ha100_hold_data;
extern float humid_data[240];//120个实时数据缓存区
extern Uint16 ha100_dot_flag,ha100_dot_cnt,ha100_data_cnt,ha100_redo_flag;
extern void ha100_line(void);
extern void ha100_redo(void);
extern void ha100_search(void);
extern void ha100_data_get(struct TESTDATA *data);
extern void ha100_store(void);
extern void ha100_do(Uint16 flag);

extern void gpa_do_0(Uint16 number);
extern void gpa100_do(void);

extern void zero_set_ca100(void);
extern void zero_set_da100(void);
extern void zero_set_gpa100(void);

extern Uint16 const color_1[7][7];
extern Uint16 module_address;
extern float get_Concentration1(void);//返回为HEX的浓度值 含有负数
extern void display_gpa(Uint16 dots,Uint16 front,Uint16 back,Uint16 switch1,Uint16 x,Uint16 y,float x1,char *strings);//x<999.999

///////////////////////////////////////////////////////
extern Uint32 sub_ver;

//===========================================================================
// End of file.
//===========================================================================
