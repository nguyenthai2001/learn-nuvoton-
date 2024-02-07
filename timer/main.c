#include <stdio.h>
#include "NUC100Series.h"
#include "I2C_Lcd.h"

#define TRIG PA0
#define ECHO PA1

uint32_t rising_time =0;
uint32_t falling_time =0;
uint32_t pulse_time =0;
uint32_t distance = 0;

uint8_t detflag = 0;

uint16_t timer0counter = 0;

int hz ;

int status;
char a[16];
char b[16];
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(50000000);
		
		CLK_EnableModuleClock(I2C0_MODULE);
		 /* Set GPA multi-function pins for I2C0 SDA and SCL */
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA8_Msk | SYS_GPA_MFP_PA9_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA8_I2C0_SDA | SYS_GPA_MFP_PA9_I2C0_SCL;
		
}

void Timer_Init(void)
{
	/*Step 1 : Enable and select timer clock 
 	select timer clock 
  clk_clk_sel1_tmr0_s_hxt ------ clock tach anh 
	clk_clksel1_tmr0_s_lxt  ------ clock 32.768KHZ
	clk_clksel1_tmr0_s_hclk ------ clock HCLK
  clk_clksel1_tmr0_s_ext_trg ----- clock external trigger
  clk_clksel1_tmr0_s_lirc  ------ 10KHZ RC clock 
  clk_clksel1_tmr0_s_hirc  ------ 22.1184MHZ RC clock 
	CLK_SetModuleClock(TMR0_MODULE,);*/
	SYS_ResetModule(TMR0_RST);
	CLK_SetModuleClock(TMR0_MODULE,CLK_CLKSEL1_TMR0_S_HXT,0); // su dung clock 12MHz 
	CLK_EnableModuleClock(TMR0_MODULE);// cho phep nguon xung timer
	                              
	/* Step 2: select operation mode
	CHON CHE DO LAM VIEC  : TIMER_PERIODIC_MODE 
	                        TIMER_ONESHOT_MODE ,
                         	TIMER_TOGGLE_MODE ,
                         	TIMER_CONTINUOUS_MODE
	TINH THOI GIAN DINH THOI : TIMER0 CLOCK * TIMER USER ex : 6S TIME outp16 -> 12mhz * 6 = 72.10^6 Hz 
	- > PRESCALE = 72 -> HEX  = 0X46
	-> CMP_VALUE  = 1000000 - > HEX = 0XF42040*/
	TIMER_Open(TIMER0,TIMER_PERIODIC_MODE,1); 
	TIMER_SET_PRESCALE_VALUE(TIMER0,0x0C);
	TIMER_SET_CMP_VALUE(TIMER0,0xF4240);
	
	/* Step 3: Enable interrupt */
	TIMER_EnableInt(TIMER0);// enable interrup timer 
	TIMER_ClearIntFlag(TIMER0);// clear interrup timer
	NVIC_EnableIRQ(TMR0_IRQn);
	
	/*Step 4 : start timer */
	TIMER_Start(TIMER0);
}

void TMR0_IRQHandler(void)
{
	if(TIMER_GetIntFlag(TIMER0) == 1)
	{
	   PC12 = ~ PC12;
	   timer0counter ++;
    	sprintf(a,"%d",timer0counter);
	   lcd_cursor(0,1);
	   LCD_putStr(a);
	   TIMER_ClearIntFlag(TIMER0);
	}
}

int main (void)
{
	
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	
	I2C_Open(I2C0,100000);
	GPIO_SetMode(PC,BIT12,GPIO_PMD_OUTPUT);
	Timer_Init();
	lcd_init();
	
	hz = TIMER_GetModuleClock(TIMER0);
	sprintf(b,"%d",hz);
	lcd_cursor(0,0);
	LCD_putStr(b);
	while(1)
	{}
}
























