#include <stdio.h>
#include "NUC100Series.h"
#include "LCD_16x2.h"
#include "I2C_LCD.h"

#define PLL_CLOCK   72000000

#define HIGH 1
#define LOW 0

#define dht_port PC 
#define dht_pin PC0

uint32_t i;
uint32_t prev_time;

uint8_t checksum ;
uint8_t humidity_int , humidity_dec,temperature_int ,temperature_dec ;

uint16_t temp,humi ;

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

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Enable TIMER0 module clock */
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));

    /* Select TIMER0 module clock source */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, NULL);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP = (SYS->GPB_MFP & ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk)) | \
                   (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
									 
		CLK_EnableModuleClock(I2C0_MODULE);
	
	    /* Set GPA multi-function pins for I2C0 SDA and SCL */
    SYS->GPA_MFP = SYS_GPA_MFP_PA8_I2C0_SDA | SYS_GPA_MFP_PA9_I2C0_SCL;
	
}

void TMR0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init TIMER                                                                                              */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset TIMER0 */
    SYS_ResetModule(TMR0_RST);

    /* Set periodic mode */
    TIMER0->TCSR = TIMER_PERIODIC_MODE;

    /* Set prescaler value to make unit count time is 1 us */
    /* Timer clock input is HCLK = 72 MHz, prescaler value is HCLK/(10^6) - 1 = 71 */
    TIMER_SET_PRESCALE_VALUE(TIMER0, SystemCoreClock / 1000000 - 1);

    /* Set 24-bit comapre register value to maximum value 0xFFFFFF */
    TIMER_SET_CMP_VALUE(TIMER0, 0xFFFFFF);

    /* Enable TDR_EN to make TDR register be updated continuously to monitor 24-bit up counter value */
    TIMER0->TCSR |= TIMER_TCSR_TDR_EN_Msk;

    /* Start TIMER0 */
    TIMER_Start(TIMER0);
}

//void DHT_init(void)
//{
//	GPIO_SetMode(dht_port,BIT0,GPIO_PMD_QUASI);
//	dht_pin =HIGH ;
//	dht_pin =LOW;
//	CLK_SysTickDelay(18000);
//}

uint32_t Dht_start(void)
{
	uint32_t poll_time;
	
	GPIO_SetMode(dht_port,BIT0,GPIO_PMD_QUASI);
	dht_pin =HIGH ;
	dht_pin =LOW;
	CLK_SysTickDelay(20000);
	dht_pin = 1;
	// lay gia tri timer 
	poll_time = TIMER_GetCounter(TIMER0);
	
	// cho phan hoi tu DHT 11
	while(1)
  {
		if(dht_pin == LOW){break;}// tieeps tucj cho 
		if((TIMER_GetCounter(TIMER0) - poll_time) > 100){return FALSE;} // cho 1ms neu khong phan hoi thi that bai 
	}
	// cho den khi dht11 keo len 
	while(1)
  {
	  if(dht_pin == HIGH){break;}
	}
	return TRUE;
}

uint32_t DHT_Read(void)
{
	TMR0_Init();
	
	if(Dht_start() == TRUE)
  {
		// cho den khi co data duoc gui 
		while(dht_pin == HIGH);
		// doc 8 bit dau tien 
		humidity_int = 0;
		humidity_dec = 0;
		temperature_dec = 0;
		temperature_int =0;
		
		// doc byte dau tien 
		for(i =0;i<8;i++)
		{
			humidity_int = humidity_int <<1;
			// cho den khi data pin len cao 
			while(dht_pin == LOW);
			
			prev_time = TIMER_GetCounter(TIMER0);
			
			while(dht_pin == HIGH);
			
			if((TIMER_GetCounter(TIMER0) - prev_time ) > 40)
			{
				humidity_int |= 0x01;
			}
		}
		
		// dodc byte thu 2
		
		for(i=0;i<8;i++)
		{
			humidity_dec = humidity_dec << 1;
			while(dht_pin == LOW);
			prev_time = TIMER_GetCounter(TIMER0);
			
			while(dht_pin == HIGH);
			
			if((TIMER_GetCounter(TIMER0) - prev_time ) > 40)
			{
				humidity_dec |= 0x01;
			}
		}
		
		// doc byte 3
		
		for(i=0;i<8;i++)
		{
			temperature_int = temperature_int <<1;
			while(dht_pin == LOW);
			prev_time = TIMER_GetCounter(TIMER0);
			while(dht_pin == HIGH);
			if((TIMER_GetCounter(TIMER0) - prev_time ) > 40 )
			{
				temperature_int |= 0x01;
			}
		}
		
		//  doc byte thu 4
		
		for(i=0;i<8;i++)		
    {
       temperature_dec = temperature_dec <<1;
       while(dht_pin == LOW);
       prev_time = TIMER_GetCounter(TIMER0);
       while(dht_pin == HIGH);
       if((TIMER_GetCounter(TIMER0) - prev_time ) > 40)
			 {
				 temperature_dec |=0x01;
			 }				 
		}
		
		// doc bit thu 5 
		
		checksum =0;
		for(i=0;i<8;i++)
		{
			checksum = checksum <<1;
			while(dht_pin == LOW);
			prev_time = TIMER_GetCounter(TIMER0);
			while(dht_pin == HIGH);
			if((TIMER_GetCounter(TIMER0) - prev_time ) > 40)
			{
				checksum |= 0x01;
			}
		}
		
		// tong byte 
		temp = temperature_int|temperature_dec;
    humi = humidity_int|humidity_dec;		
		// stop timer 
		TIMER_Stop(TIMER0);
		if((uint8_t)(humidity_int+humidity_dec+temperature_int+temperature_dec) == checksum)
		{
			return TRUE;
		}
		else 
		{
			lcd_cursor(0,0);
			LCD_putStr("Checksum error");
			return FALSE;
		}
		return temp;
		return humi;
		
	}
	
	else
	{
		// stop timer 
		TIMER_Stop(TIMER0);
		return FALSE;
		lcd_cursor(0,0);
		LCD_putStr("Khong ket noi dht ");
	}
}

int main (void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	
	LCD_Init();
	LCD_Clear_Display();
	I2C_Open(I2C0,100000);
	lcd_init();
	//LCD_String(0,0,"thaiaiaa");
	//DHT_init();

	while(1)
  {
//		CLK_SysTickDelay(1000000);
		if(DHT_Read() == TRUE)
		{
			sprintf (a,"%d.%2d",temp);
			sprintf (b,"%d.%2d",humi);
			lcd_cursor(0,0);
			LCD_putStr(a);
			lcd_cursor(0,1);
			LCD_putStr(b);
			CLK_SysTickDelay(1000000);
		}
	}
}





















