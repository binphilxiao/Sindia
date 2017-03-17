#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define PWM_HALF_PERIOD		1027//1027(73khz)//3750(20khz)			// period/2 for 2 kHz symmetric PWM
#define PWM_DUTY_CYCLE		513//513//1875(50%)			// 25% duty cycle

Uint16 LoopCount;
Uint16 ErrorCount;

Uint32 temp_cnt,flow_cnt,clock_cnt,sht11_cnt;
Uint16 run_temp_flag,run_flow_flag,run_clock_flag,run_sht11_flag;

interrupt void scibRxFifoIsr(void)
{   
    Uint16 i;
	for(i=0;i<10;i++)
	{
	   key.buffer[i]=ScibRegs.SCIRXBUF.all;	 // Read data
	}
    if(key.buffer[1]==0x72)
    {	
        	key.flag=1;
		key_get();
    }
	key.buffer[1]=0;
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag;
	PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
	    
    if (run_temp_flag==0)temp_cnt++;
    if (temp_cnt>100)//60
    {
        temp_cnt=0;
        run_temp_flag=1;
    }
    if (run_flow_flag==0)flow_cnt++;
    if (flow_cnt>1000)
    {
        flow_cnt=0;
        run_flow_flag=1;
    }
    if (run_clock_flag==0)clock_cnt++;
    if (clock_cnt>500)
    {
        clock_cnt=0;run_clock_flag=1;
    }
    if (run_sht11_flag==0)sht11_cnt++;
	if (sht11_cnt>1000)
	{
	    sht11_cnt=0;run_sht11_flag=1;
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

Uint32 balance_cnt;
interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;
    EDIS;
}

interrupt void cpu_timer2_isr(void)
{  EALLOW;
   CpuTimer2.InterruptCount++;
   EDIS;
}

void Gpio_select(void)
{
	/*
	IO0:PWM1A => CPLD.24(PWM1A)
	IO1-IO12:OUTPUT
	IO13-IO14:INPUT(io 14 used as input on CPLD)
	IO15:INTA <= RX8025SA.10(/INTA)  intrupt not used, used as io

	IO16-IO19:OUTPUT
	IO20-IO23:CAN & SCIb
	IO24-IO27:OUTPUT
	IO28-IO29:SCIA
	IO30-IO31:XA17-XA18
	*/
	/*
	IO32-IO33:IICA(RX8025SA) but used io mode
	IO34-IO47:BUS
	IO48-IO53:OUTPUT
	IO54-IO58:INPUT
	IO59-IO61:NOT USED
	IO62-IO63:SCIC
	*/
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000001;  
	GpioCtrlRegs.GPAMUX2.all = 0xA500FF00;
	GpioCtrlRegs.GPADIR.all  = 0xFFFF5FFF;

	GpioCtrlRegs.GPBMUX1.all = 0xFFFFFFF0;
	GpioCtrlRegs.GPBMUX2.all = 0x50000000;
	GpioCtrlRegs.GPBDIR.all  = 0x183FFFFF;
    EDIS;
}

void scib_loopback_init()
{
	// SCIA
	// in the InitSysCtrl() function

	SciaRegs.SCICCR.all = 0x0066; // 1 stop bit,  No loopback
								  //  parity-EVEN,7 char bits,
								  // async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all = 0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA = 0;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;
	SciaRegs.SCIHBAUD = 0x07;
	SciaRegs.SCILBAUD = 0xa0; //2400
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	SciaRegs.SCIFFTX.all = 0x8000;
	SciaRegs.SCIFFRX.all = 0x0000;
	SciaRegs.SCIFFCT.all = 0x00;

	SciaRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

	// SCIB
	// in the InitSysCtrl() function

	ScibRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
								  // No parity,8 char bits,
								  // async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all = 0x0002;//Tx,Rx Interupt Enable
	ScibRegs.SCICTL2.bit.TXINTENA = 0;
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
	ScibRegs.SCIHBAUD = 0x0000;
	ScibRegs.SCILBAUD = 0x0027; //0x0027;115200
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	ScibRegs.SCIFFTX.all = 0xC02f;
	ScibRegs.SCIFFRX.all = 0x002a;
	ScibRegs.SCIFFCT.all = 0x00;

	ScibRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset //Reset
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;//Reset Tx Fifo
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;//Reset Rx Fifo

	//SCIC

	ScicRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
								  // No parity,8 char bits,
								  // async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all = 0x0003;
	ScicRegs.SCICTL2.bit.TXINTENA = 1;
	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
	ScicRegs.SCIHBAUD = 0x0001;
	ScicRegs.SCILBAUD = 0x00e8; //9600
//	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	ScicRegs.SCIFFTX.all = 0xC02a;
	ScicRegs.SCIFFRX.all = 0x002a;
	ScicRegs.SCIFFCT.all = 0x00;

	ScicRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;

}

/**********************************************************************
* Function: InitEPwm()
*
* Description: Initializes the Enhanced PWM modules on the F2833x.
**********************************************************************/
void InitEPwm(void)
{
	//---------------------------------------------------------------------
	//--- Must disable the clock to the ePWM modules if you       
	//--- want all ePMW modules synchronized.
	//---------------------------------------------------------------------
	asm(" EALLOW");						// Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	asm(" EDIS");						// Disable EALLOW protected register access
	//---------------------------------------------------------------------
	//--- Configure ePWM1 for 2 kHz symmetric PWM on EPWM1A pin    
	//---------------------------------------------------------------------
	EPwm1Regs.TBCTL.bit.CTRMODE = 0x3;		// Disable the timer
	EPwm1Regs.TBCTL.all = 0xC033;			// Configure timer control register
	// bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
	// bit 13        0:      PHSDIR, 0 = count down after sync event
	// bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
	// bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
	// bit 6         0:      SWFSYNC, 0 = no software sync produced
	// bit 5-4       11:     SYNCOSEL, 11 = sync-out disabled
	// bit 3         0:      PRDLD, 0 = reload PRD on counter=0
	// bit 2         0:      PHSEN, 0 = phase control disabled
	// bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)
	EPwm1Regs.TBCTR = 0x0000;				// Clear timer counter
	EPwm1Regs.TBPRD = PWM_HALF_PERIOD;		// Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;	// Set timer phase
	EPwm1Regs.CMPA.half.CMPA = PWM_DUTY_CYCLE;	// Set PWM duty cycle
	EPwm1Regs.CMPCTL.all = 0x0002;			// Compare control register
	// bit 15-10     0's:    reserved
	// bit 9         0:      SHDWBFULL, read-only
	// bit 8         0:      SHDWAFULL, read-only
	// bit 7         0:      reserved
	// bit 6         0:      SHDWBMODE, don't care
	// bit 5         0:      reserved
	// bit 4         0:      SHDWAMODE, 0 = shadow mode
	// bit 3-2       00:     LOADBMODE, don't care
	// bit 1-0       10:     LOADAMODE, 10 = load on zero or PRD match
	EPwm1Regs.AQCTLA.all = 0x0060;		// Action-qualifier control register A
	// bit 15-12     0000:   reserved
	// bit 11-10     00:     CBD, 00 = do nothing
	// bit 9-8       00:     CBU, 00 = do nothing
	// bit 7-6       01:     CAD, 01 = clear
	// bit 5-4       10:     CAU, 10 = set
	// bit 3-2       00:     PRD, 00 = do nothing
	// bit 1-0       00:     ZRO, 00 = do nothing
	EPwm1Regs.AQSFRC.all = 0x0000;		// Action-qualifier s/w force register
	// bit 15-8      0's:    reserved
	// bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
	// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
	// bit 4-3       00:     ACTSFB, don't care
	// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
	// bit 1-0       00:     ACTSFA, don't care
	EPwm1Regs.AQCSFRC.all = 0x0000;		// Action-qualifier continuous s/w force register
	// bit 15-4      0's:    reserved
	// bit 3-2       00:     CSFB, 00 = forcing disabled
	// bit 1-0       00:     CSFA, 00 = forcing disabled
	EPwm1Regs.DBCTL.bit.OUT_MODE = 0;	// Deadband disabled
	EPwm1Regs.PCCTL.bit.CHPEN = 0;		// PWM chopper unit disabled
	EPwm1Regs.TZCTL.bit.TZA = 0x3;		// Trip action disabled for output A
	EPwm1Regs.TBCTL.bit.CTRMODE = 0x2;	// Enable the timer in count up/down mode
	//---------------------------------------------------------------------
	//--- Enable the clocks to the ePWM module.                   
	//--- Note: this should be done after all ePWM modules are configured
	//--- to ensure synchronization between the ePWM modules.
	//---------------------------------------------------------------------
	asm(" EALLOW");							// Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;	// HSPCLK to ePWM modules enabled
	asm(" EDIS");							// Disable EALLOW protected register access
} // end InitEPwm()

void system_init(void)
{
    InitSysCtrl();
    Gpio_select();
    InitXintf();
	DINT;// Disable and clear all CPU interrupts:
	IER = 0x0000;
	IFR = 0x0000;
    InitPieVectTable();
	// Enable CPU and PIE interrupts
	// This example function is found in the DSP2833x_PieCtrl.c file.
    EnableInterrupts();
	// Step 4. Initialize all the Device Peripherals to a known state:
	// This function is found in DSP2833x_InitPeripherals.c
	// InitPeripherals(); skip this for SCI tests
	// Step 5. User specific functions, Reassign vectors (optional), Enable Interrupts:
    LoopCount = 0;
    ErrorCount = 0;
	// scib_fifo_init();	   // Initialize the SCI FIFO
    scib_loopback_init();  // Initalize SCI for digital loop back
    InitEPwm();
    EALLOW;	// This is needed to write to EALLOW protected registers
    PieVectTable.SCIRXINTB = &scibRxFifoIsr;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.XINT13 = &cpu_timer1_isr;
    PieVectTable.TINT2 = &cpu_timer2_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers 
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 150MHz CPU Freq, 1 second Period (in uSeconds)
    ConfigCpuTimer(&CpuTimer0, 150, 1000);
    ConfigCpuTimer(&CpuTimer1, 150, 10000);
    ConfigCpuTimer(&CpuTimer2, 150, 1000000);
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
    CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	// Step 5. User specific code, enable interrupts:
	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	// Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	// Enable global Interrupts and higher priority real-time debug events:
	// PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	// PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT1
	// IER |=0x100;
	// EINT;   // Enable Global interrupt INTM
	// ERTM;	  // Enable Global realtime interrupt DBGM
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	// PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
	// PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
    PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
	// PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4
    IER |= M_INT1;
    IER |= M_INT13;
    IER |= M_INT14;
    IER |= 0x100;	// Enable CPU INT
    EINT;
    //EINT;   // Enable Global interrupt INTM
    ERTM;	  // Enable Global realtime interrupt DBGM
    // Note: Autobaud lock is not required for this example
}

void driver_init(void)
{
	Uint32 i;
	rx8025_clock_init();
	
	valve(1,0);
	valve(2,1);
	valve(3,1);
	
	dump(0,0);
	
	HEATER(1);

	for(i=0;i<3000000;i++);
	cls();
}

void parameter_init(void)
{
	Uint16 i,x,y,ret;

	rx8025_clock_read(&clock);
	
	fm22l16_rds(ADD_FLAG,&plat.flag,sizeof(Uint16));
	if(plat.flag!=0x5555)
	{
		plat.flag=0x5555;
		plat.store_number=0;
		plat.gas=0;
		plat.usb_num=0;
		fm22l16_wrs(ADD_FLAG,&plat.flag,sizeof(Uint16));
		fm22l16_wrs(ADD_GAS,&plat.gas,sizeof(Uint16));
		fm22l16_wrs(ADD_USB_NUM,&plat.usb_num,sizeof(Uint16));
		fm22l16_wrs(ADD_CNT,&plat.store_number,sizeof(Uint16));
	}

	disp_background(0);
	x=100;y=80;
	plat.clean=0;
	plat.gas=fm22l16_rd(ADD_GAS);
	if (plat.gas>1) {plat.gas=0;fm22l16_wr(ADD_GAS, plat.gas);}
	plat.store_number=fm22l16_rd(ADD_CNT);
	if (plat.store_number>300) {plat.store_number=0;fm22l16_wr(ADD_CNT, plat.store_number);}
	plat.usb_num=fm22l16_rd(ADD_USB_NUM);
	plat.pur_add=fm22l16_rd(ADD_PUR);
	if (plat.pur_add>255) {plat.pur_add=0;fm22l16_wr(ADD_PUR, plat.pur_add);}
	display_ascii(M32,WHITE,BLUE,1,x,y,"设备初始化---OK");y+=40;
	//微水检测
	fm22l16_rds(ADD_CALIBVENDER_H2O,(Uint16*)&h2o.calib_vender,sizeof(struct CALIB_7));
	display_ascii(M32,WHITE,BLUE,1,x,y,"微水初始化---OK");y+=40;
	//纯度检测
	fm22l16_rds(ADD_CALIBVENDER_PUR,(Uint16*)&purity.calib_vender,sizeof(struct CALIB_7));
	//purity_det(x,y);y+=40;
	//温度检测
	fm22l16_rds(ADD_CALIBVENDER_TEMP,(Uint16*)&temprature.calib_usr,sizeof(struct CALIB_2));

	plat.sensor_number=0;
	//H2S传感器探测
	ret=fm24cl04_read(0xa0,ADD24_SENSOR_ID,&i);
	if(i==0xa0)
	{
		plat.sensor[plat.sensor_number]=i;
		plat.sensor_number++;

		i=fm22l16_rd(ADD_CALIBUSR_H2S_FLAG);
		if (i==0x5555) fm22l16_rds(ADD_CALIBUSR_H2S, (Uint16 *)&h2s.calib_usr, sizeof(struct CALIB_2));
		
		ret=fm24cl04_read(0xa0,ADD24_CALIB7_FLAG,&i);
		if(i==0x55) fm24cl04_rds(0xa0, ADD24_CALIB7_VAL, (Uint16 *)&h2s.calib_vender, sizeof(struct CALIB_7));

		h2s.ad_ch=H2S;
		h2s.at24_add=0xa0;
		h2s.temp_add=0x92;
		
		display_ascii(M32,WHITE,BLUE,1,x,y,"H2S 初始化---OK");y+=40;
		if (i==0x55) display_ascii(M32,WHITE,BLUE,1,x+320,y-40,"已校准");
	}
	//SO2传感器探测
	ret=fm24cl04_read(0xa4,ADD24_SENSOR_ID,&i);
	if(i==0xa4)
	{
		plat.sensor[plat.sensor_number]=i;
		plat.sensor_number++;

		i=fm22l16_rd(ADD_CALIBUSR_SO21_FLAG);
		if (i==0x5555) fm22l16_rds(ADD_CALIBUSR_SO21, (Uint16 *)&so2.calib_usr, sizeof(struct CALIB_2));
		
		ret=fm24cl04_read(0xa4,ADD24_CALIB7_FLAG,&i);
		if(i==0x55) fm24cl04_rds(0xa4, ADD24_CALIB7_VAL, (Uint16 *)&so2.calib_vender, sizeof(struct CALIB_7));

		so2.ad_ch=SO2;
		so2.at24_add=0xa4;
		so2.temp_add=0x96;
		
		display_ascii(M32,WHITE,BLUE,1,x,y,"SO2 初始化---OK");y+=40;
		if (i==0x55) display_ascii(M32,WHITE,BLUE,1,x+320,y-40,"已校准");
	}
	//CO传感器探测
	ret=fm24cl04_read(0xa8,ADD24_SENSOR_ID,&i);
	if(i==0xa8)
	{
		plat.sensor[plat.sensor_number]=i;
		plat.sensor_number++;

		i=fm22l16_rd(ADD_CALIBUSR_CO_FLAG);
		if (i==0x5555) fm22l16_rds(ADD_CALIBUSR_CO, (Uint16 *)&co.calib_usr, sizeof(struct CALIB_2));
		
		ret=fm24cl04_read(0xa8,ADD24_CALIB7_FLAG,&i);
		if(i==0x55) fm24cl04_rds(0xa8, ADD24_CALIB7_VAL, (Uint16 *)&co.calib_vender, sizeof(struct CALIB_7));

		co.ad_ch=CO;
		co.at24_add=0xa8;
		co.temp_add=0x9e;
		
		display_ascii(M32,WHITE,BLUE,1,x,y,"CO  初始化---OK");y+=40;
		if (i==0x55) display_ascii(M32,WHITE,BLUE,1,x+320,y-40,"已校准");
	}
}

void menu(void)
{
	//usb_check();
	ca100_main();
	while(1);
}

void main(void)
{
	Uint32 i;

	run_temp_flag=0;temp_cnt=0;
	run_flow_flag=0;flow_cnt=0;
	run_clock_flag=0;clock_cnt=0;
	run_sht11_flag=0;sht11_cnt=0;
	
	system_init();
	driver_init(); 
	
	disp_background(1);
	for (i=0;i<5000000;i++);
	parameter_init();
	for (i=0;i<5000000;i++);

	menu();
	while(1);
}

//===========================================================================
// No more.
//===========================================================================



