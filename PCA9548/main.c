#include <stdio.h>
#include "NUC100Series.h"
#include "PCA9548.h"

//#define PCA9548_ADDR 0x70
//#define Channel_0 0x01
//#define Channel_1 0x02
//#define Channel_2 0x04
//#define Channel_3 0x08
//#define Channel_4 0x10
//#define Channel_5 0x20
//#define Channel_6 0x40
//#define Channel_7 0x80
//#define No_channel 0x00
//#define All_channel 0xff
//#define Chance_channel 0x07

	char a[16];
	char b[16];
	
	uint8_t data;
int i ;
		
	float lux;
	
	uint32_t data_pca;
	
	uint8_t data_1, data_2;
	
	uint8_t channelStatus;

void SYS_Init(void)
{
	 CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(50000000);
	
}

void I2C0_init(void)
{
	CLK_EnableModuleClock(I2C0_MODULE);
//		* Set GPA multi-function pins for I2C0 SDA and SCL */
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA8_Msk | SYS_GPA_MFP_PA9_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA8_I2C0_SDA | SYS_GPA_MFP_PA9_I2C0_SCL;
}

void I2C1_init(void)
{
	CLK_EnableModuleClock(I2C1_MODULE);
//		* Set GPA multi-function pins for I2C0 SDA and SCL */
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA10_Msk | SYS_GPA_MFP_PA11_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA10_I2C1_SDA | SYS_GPA_MFP_PA11_I2C1_SCL;
}

void UART_init(void)
{
	 CLK_EnableModuleClock(UART0_MODULE);  
	 CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
	 SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
   SYS->GPB_MFP |= SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD;
}

void CheckChannelStatus(uint8_t data)
{
    for (i= 0; i < 8; i++) {
        if (data & (1 << i)) {
            printf("Channel %d is open\n", i);
        } else {
            printf("Channel %d is closed\n", i);
        }
    }
}

int main (void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	
	GPIO_SetMode(PA,BIT0 | BIT1,GPIO_PMD_OUTPUT);
	UART_init();
	UART_Open(UART0,9600);
	I2C0_init();
	I2C1_init();
	PA0 = 0;
	while(1)
	{
		PCA9548_write(I2C0,0x70,0x07);
		data = PCA9548_read(I2C0,0x70);
		UART_WRITE(UART0,data);
		CLK_SysTickDelay(2000000);
//		lcd_init();
//		i=i++;
//		sprintf(a,"%d",i);
//		lcd_cursor(0,0);
//	  LCD_putStr(a);
//		delayMs(500);

//		PA0 =0;
//	PCA9548_write(I2C1,0x70,Channel_0);
//		BH1750_Read(I2C1,0x23,&lux);		
		
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	