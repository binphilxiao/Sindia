#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define PWM_HALF_PERIOD		1027//1027(73khz)//3750(20khz)			// period/2 for 2 kHz symmetric PWM
#define PWM_DUTY_CYCLE		513//513//1875(50%)			// 25% duty cycle

//char  *soft_version={"版本:2.02"};
// Prototype statements for functions found within this file.
extern void scib_loopback_init(void);
extern void scib_fifo_init(void);
extern void scib_xmit(Uint16 a);
//interrupt void scib_rx_isr(void);
//interrupt void scib_tx_isr(void);
interrupt void scibRxFifoIsr(void);
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);

void InitEPwm(void);
void Gpio_select(void);
extern void color_change(Uint16 front,Uint16 back);

Uint16 LoopCount;
Uint16 ErrorCount;

/*const Uint16 color[5][2]=
{
0x0000,0xf800,
0xffff,0x001f,
0x00ff,0x0fff,
0x1fff,0x0111,
0x2fff,0x0010,
};
void print_test()
{

    scic_xmit(0x0D);
	scic_xmit(0x0D);
	scic_xmit(0x0D);
    scic_xmit(0x1B);
    scic_xmit(0x40);
	scic_xmit(0x1c);
    scic_xmit(0x26);
	print_string_c("Good morning every one!");
	print_string_c("Good morning every one!");
	print_string_c("Good morning every one!");
	print_string_c("中华人民共和国！");
	scic_xmit(0x0D);
	scic_xmit(0x0D);
	scic_xmit(0x0D);
	scic_xmit(0x1c);
    scic_xmit(0x2E);
//	scic_xmit(0x1B);
//	scic_xmit(0x6B);
//	scic_xmit(0x1);
}
void print_test1()
{
    scic_xmit(0x1B);
    scic_xmit(0x40);
    scic_xmit(0x0A);
    scic_xmit(0x0A);
	scic_xmit(0x1D);
	scic_xmit(0x6B);
	scic_xmit(0x3);
	scic_xmit(1);
	scic_xmit(2);
	scic_xmit(3);
	scic_xmit(4);
	scic_xmit(5);
	scic_xmit(6);
	scic_xmit(7);
	scic_xmit(8);

	scic_xmit(0x0);

	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0A);
	scic_xmit(0x0D);

	scic_xmit(0x1B);
	scic_xmit(0x6B);
	scic_xmit(0x1);
}*/

void main(void)
{
   //  Uint16 f,dd[100],h=0,m,n;float x;
    Uint32 i,j,h;
	InitSysCtrl();

    Gpio_select();
//	init_value();
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

 //   scib_fifo_init();	   // Initialize the SCI FIFO
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
 //   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
 //   PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT1
 //   IER |=0x100;
 //	EINT;   // Enable Global interrupt INTM
 //	ERTM;	  // Enable Global realtime interrupt DBGM

   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
//   PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1
//   PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
   PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
//   PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4
   IER |= M_INT1;
   IER |= M_INT13;
   IER |= M_INT14;
   IER |= 0x100;	// Enable CPU INT
   EINT;

  //EINT;   // Enable Global interrupt INTM
   ERTM;	  // Enable Global realtime interrupt DBGM

    // Note: Autobaud lock is not required for this example
    
   //dw_int( ) ;
   //dw_int1( ) ;
   rx8025_clock_inti();
   parameter_initi();

    for(i=0;i<3000000;i++);
    color_change(0xffe0,0x205e);
    cls();
    
    KG1(0);
    BPL(0);
    HEATER(1);
    instrument_select();
    if(instrument==0 ||instrument==3)
    {
        gpaold=get_Concentration();
    }
    sensor_select();
    menu();
    while(1)
    {
    }
}

// Step 7. Insert all local Interrupt Service Routines (ISRs) and functions here:
interrupt void scibRxFifoIsr(void)
{   
    Uint16 i;
//	ScibRegs.SCICTL1.bit.RXENA=0;
	for(i=0;i<10;i++)
	{
	   databuffer[i]=ScibRegs.SCIRXBUF.all;	 // Read data
	}
    if(databuffer[1]==0x72)
    {	
        keyflag=1;
		key_get();
		screen_cnt=0;
		run_screen_flag=0;
    }
	databuffer[1]=0;
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag;
	PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    if (run_ad_flag==0){run_ad_flag=1;}
	    
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

    key_cnt++;
	if(key_cnt>=1)
	{
		key_cnt=0;
		u_key_get();
	}

	if(key_alarm_flag==1)
    {
        key_alarm_cnt++;
        if(key_alarm_cnt>=6)
        {
            key_alarm_cnt=0;
            key_alarm_flag=0;
            SPK(0);
        }
    }
    EDIS;
}

interrupt void cpu_timer2_isr(void)
{  EALLOW;
   CpuTimer2.InterruptCount++;

   if(ha100_dot_flag==0)
   {
       ha100_dot_cnt++;
	   if(ha100_dot_cnt>=3)
	   {
	       ha100_dot_flag=1;
		   ha100_data_cnt++;
		   if(ha100_data_cnt>240)
		   {
		       ha100_redo_flag=1;
		   }
	   }
   }
   if(run_screen_flag==0)
   {
       //screen_cnt++;
	   if(screen_cnt>1000)run_screen_flag=1;
   }
   else screen_cnt=0;
   EDIS;
}

void Gpio_select(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000001;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0xA500FF00;  // All GPIO,set rxda and txda
    GpioCtrlRegs.GPADIR.all  = 0xFFFF5FFF;   // All outputs

    GpioCtrlRegs.GPBMUX1.all = 0xFFFFFFF0;  // All GPIO
	GpioCtrlRegs.GPBMUX2.all = 0x50000000;  // All GPIO
    GpioCtrlRegs.GPBDIR.all  = 0x183FFFFF;   // All outputs
    EDIS;
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


//===========================================================================
// No more.
//===========================================================================



