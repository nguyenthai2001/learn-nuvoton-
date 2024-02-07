#include <stdio.h>
#include "NUC100Series.h"
#include "LCD_16X2.h"

int analog ;
int threshold = 100;
int state =0;
char a[16];
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

}

void ADC_Init(void)
{
	CLK_EnableModuleClock(ADC_MODULE);
 CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_CLKDIV_ADC(255));
	GPIO_DISABLE_DIGITAL_PATH(PA,BIT5);
	
	SYS->GPA_MFP &= ~SYS_GPA_MFP_PA5_Msk;
	SYS->GPA_MFP |= SYS_GPA_MFP_PA5_ADC5;
	SYS->ALT_MFP1 =0;
	
	ADC_Open(ADC,ADC_ADCR_DIFFEN_SINGLE_END,ADC_ADCR_ADMD_SINGLE_CYCLE,BIT5);
}

int main(void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	
	GPIO_SetMode(PC,BIT12,GPIO_PMD_OUTPUT);
	
	analog =0;
	ADC_Init();
	LCD_Init();
	LCD_String(1,0,"Analog sensor : ");
	while(1)
	{
		ADC_POWER_ON(ADC);
		ADC_CLR_INT_FLAG(ADC,ADC_ADF_INT);
		ADC_START_CONV(ADC);
		while(!ADC_GET_INT_FLAG(ADC,ADC_ADF_INT));
		analog =ADC_GET_CONVERSION_DATA(ADC,5);
		sprintf(a,"%d",analog);
		LCD_String(2,0,a);
//		if(analog>= threshold)
//		{
//			if(analog ==0)
//			{
//				PC12 =0;
//				state =1;
//			}
//			else{PC12 =1; analog =0;}
//		}
	}
}
