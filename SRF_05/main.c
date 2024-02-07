#include <stdio.h>
#include "NUC100Series.h"
#include "I2C_LCD.h"

uint32_t rising_time =0;
uint32_t falling_time =0;
uint32_t pulse_time =0;
float distance = 0;

uint8_t flag ;
float cm ;

int hz ;
char b[16];
char a[16];

void SYS_init(void)
{
	
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
		
		/*Set GPB*/
		SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB10_Msk | SYS_GPB_MFP_PB2_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= SYS_GPB_MFP_PB10_TM2 | SYS_GPB_MFP_PB2_TM2_EXT | SYS_GPB_MFP_PB1_GPIO;
		
		SYS->ALT_MFP  &= ~SYS_ALT_MFP_PB2_Msk;
    SYS->ALT_MFP  |= SYS_ALT_MFP_PB2_TM2_EXT;
    
    SYS->ALT_MFP2 = SYS_ALT_MFP2_PB2_TM2_EXT;
		
}

void Timer_init(void)
{
	/*Step1 : Enable timer and select timer clock */
	SYS_ResetModule(TMR2_RST);
	CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2_S_HXT,0); 
	CLK_EnableModuleClock(TMR2_MODULE);
	/*Step 2 : Select timer mode */
	TIMER_Open(TIMER2,TIMER_PERIODIC_MODE,1000000);
	/*Step 3: tinhs toan thoi gian dinh thoi */
  TIMER_SET_PRESCALE_VALUE(TIMER2,11);
	TIMER_SET_CMP_VALUE(TIMER2,0xFFFFFF);
	/*step 4 : chon che do capture voi suwon len suwon xuong */
//	TIMER_EnableEventCounter(TIMER2,TIMER_COUNTER_FALLING_EDGE);
	TIMER_EnableCapture(TIMER2,TIMER_CAPTURE_FREE_COUNTING_MODE,TIMER_CAPTURE_FALLING_AND_RISING_EDGE);
	/*Strp 5: Che phep ngat */
	TIMER_EnableCaptureInt(TIMER2);
//	TIMER_ClearCaptureIntFlag(TIMER2);
  NVIC_EnableIRQ(TMR2_IRQn);
	/*Step 6: start timer */
	TIMER_Start(TIMER2);
	
}

void TMR2_IRQHandler(void)
{
//	TIMER_ClearCaptureIntFlag(TIMER2);
  if(TIMER_GetCaptureIntFlag(TIMER2) == 1) // neu co ngat 
	{
		if(PB2 == 1) // lay thoi gian echo len cao 
		{
		  rising_time = TIMER_GetCaptureData(TIMER2);
		}
	  else // lay thoi gian echo xuong thap 
	  {
	    falling_time = TIMER_GetCaptureData(TIMER2);
		  flag = 1;
	  }
   	TIMER_ClearCaptureIntFlag(TIMER2); // xoa ngat 
  }
}

void HC_SR04_init(void)
{
	PB1 = 0;             // Trigger len cao 
	CLK_SysTickDelay(10);
  PB1 = 1;             //  trigger xuong thap 
	CLK_SysTickDelay(10);
	PB1 = 0;
}

int main (void)
{
	SYS_UnlockReg();
	SYS_init();
	SYS_LockReg();
	
	I2C_Open(I2C0,100000);
	lcd_init();
	
	Timer_init();
	
	hz = TIMER_GetModuleClock(TIMER2); /*Lay tan so timer clock */
	lcd_cursor(0,0);
	sprintf(b,"%d",hz);
	LCD_putStr(b);
	
	GPIO_SetMode(PB,BIT1,GPIO_PMD_OUTPUT); // trigger dau ra 
	GPIO_SetMode(PB,BIT2,GPIO_PMD_INPUT);  // echo dau vao 
	
	while(1)
	{
		HC_SR04_init();  
//		 CLK_SysTickDelay(100000);
		if(flag) // neu echo xuong 
		{
			flag =0;  
			if(falling_time > rising_time)  
			{
        distance =  falling_time - rising_time ;  // khaong cach bang time down  - time up  *0.017 
		   	distance=	distance *0.017;
		  	cm = (float)distance;
	    	lcd_cursor(0,1);
		    sprintf(a,"%.2f",cm);
		    LCD_putStr(a);
			}
		}
		CLK_SysTickDelay(1000000);
	}
}



















