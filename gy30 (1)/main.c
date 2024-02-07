#include <stdio.h>
#include "NUC100Series.h"
#include "LCD_16X2.h"
#include "GY_30.h"
#include "I2C_LCD.h"

 char a[16];
char b[16];
char c[16];

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
		 SYS_ResetModule(UART1_RST);
//		  SYS_ResetModule(UART0_RST);
		
		 SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA10_Msk | SYS_GPA_MFP_PA11_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA10_I2C1_SDA | SYS_GPA_MFP_PA11_I2C1_SCL;
}




void UART_Init(void)
{
	/*step 1 : EBABLE module clock */
	CLK_EnableModuleClock(UART0_MODULE);
  CLK_SetModuleClock(UART0_MODULE,CLK_CLKSEL1_UART_S_HXT,CLK_CLKDIV_UART(1));
	/*step 2 : Xac dinh GPIO  */
	SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
  SYS->GPB_MFP |= SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD;
	/**Step 3: config uart and baudrate */
	SYS_ResetModule(UART0_RST);

}

int main()
{
    float lux;
	
    SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	UART_Init();
	UART_Open(UART0,115200);

//	BH1750_Init(0x23);
	
    while (1){ 
			BH1750_Read(I2C0,0x23,&lux);
			delayMs(1000);
      sprintf(c,"\n%f",lux);
      printf(c);
  
			}
}















