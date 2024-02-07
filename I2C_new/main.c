#include<stdio.h>
#include"NUC100Series.h"

#define I2C_ADDR 0x27

uint8_t ferring =1;

 uint32_t g_delayMsFlag = 0;
 
 char text[] = "thai";
 
 uint8_t data[2];

uint8_t lcd_command[4];
uint8_t lcd_data[4];
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

void SysTick_Handler(void)
{
    if (g_delayMsFlag > 0) {
        g_delayMsFlag--;
    }
}

void delayMs(uint32_t ms)
{
	SysTick_Config(SystemCoreClock / 1000);
    g_delayMsFlag = ms;

    while (g_delayMsFlag);
}

void I2c_init(void)
{
	I2C_Open(I2C0,100000);
	
}

void lcd_writecommand8(uint8_t cmd)
{
	 lcd_command[0] = (cmd & 0xF0) | 0x0C;
   lcd_command[1] = lcd_command[0] & 0xFB;
	
	I2C_START(I2C0);
	I2C_WAIT_READY(I2C0);
	
	I2C_SET_DATA(I2C0,I2C_ADDR<<1);       // Gui dia chi BH1750 va bit 0 de bat dau doc
    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk);    // tao thanh ghi  SI (Start)
    I2C_WAIT_READY(I2C0);
	
	
	I2C_SetData(I2C0,lcd_command[0]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
  
	I2C_SetData(I2C0,lcd_command[1]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
	
	I2C_STOP(I2C0);
}

void lcd_writeCommand(uint8_t command)
{
	lcd_command[0]=(command & 0xF0) | 0x0C;
	lcd_command[1] = lcd_command[0] & 0xFB;
	lcd_command[2] = (command << 4) | 0x0C;
   lcd_command[3] = lcd_command[2] & 0xFB;   

	I2C_START(I2C0);
	I2C_WAIT_READY(I2C0);
	
	I2C_SET_DATA(I2C0,I2C_ADDR<<1);       // Gui dia chi BH1750 va bit 0 de bat dau doc
    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk);    // tao thanh ghi  SI (Start)
    I2C_WAIT_READY(I2C0);
	
	
	I2C_SetData(I2C0,lcd_command[0]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
  
	I2C_SetData(I2C0,lcd_command[1]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
	
	I2C_SetData(I2C0,lcd_command[2]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
	
	I2C_SetData(I2C0,lcd_command[3]);
	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(I2C0);
	
	I2C_STOP(I2C0);
}

void lcd_writeData(uint8_t data)
{
	
	lcd_data[0] = (data & 0xF0) | 0x0D;  // high 4 bit, E = 1, RW = 0, RS = 1
   lcd_data[1] = lcd_data[0] & 0xFB;  // E = 0;
   lcd_data[2] = (data	<< 4 ) | 0x0D;   // low  4 bit, E = 1, RW = 0, RS = 1
   lcd_data[3] = lcd_data[2] & 0xFB;  // E = 0;
	
	I2C_START(I2C0);
	I2C_WAIT_READY(I2C0);
	
	
	I2C_SET_DATA(I2C0,I2C_ADDR<<1);       // Gui dia chi BH1750 va bit 0 de bat dau doc
    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk);    // tao thanh ghi  SI (Start)
    I2C_WAIT_READY(I2C0);
//		I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI_Msk | I2C_I2CON_AA_Msk);// Gui si va ACk 
//    I2C_WAIT_READY(I2C0);
	
	I2C_SET_DATA(I2C0,lcd_data[0]);
//	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI );
//	I2C_WAIT_READY(I2C0);
	 I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk );    // Gui tin hieu  SI (Start)
    I2C_WAIT_READY(I2C0);
		
		I2C_SET_DATA(I2C0,lcd_data[1]);
//	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI );
//	I2C_WAIT_READY(I2C0);
	 I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk );    // Gui tin hieu  SI (Start)
    I2C_WAIT_READY(I2C0);
		
		I2C_SET_DATA(I2C0,lcd_data[2]);
//	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI );
//	I2C_WAIT_READY(I2C0);
	 I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk );    // Gui tin hieu  SI (Start)
    I2C_WAIT_READY(I2C0);
		
		I2C_SET_DATA(I2C0,lcd_data[3]);
//	I2C_SET_CONTROL_REG(I2C0,I2C_I2CON_SI );
//	I2C_WAIT_READY(I2C0);
	 I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk );    // Gui tin hieu  SI (Start)
    I2C_WAIT_READY(I2C0);
	
//	 I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk );  // gui tin hieu  SI (Start) và Nack
//		I2C_WAIT_READY(I2C0);                          // Cjo phan hoi 
		I2C_STOP(I2C0);      
}

void lcd_init(void)
{
	
	lcd_writecommand8(0x30);
	delayMs(10);
	lcd_writecommand8(0x30);
	delayMs(6);
	lcd_writecommand8(0x30);
	

	lcd_writecommand8(0x20);

	lcd_writeCommand(0x28);

	lcd_writeCommand(0x0C);

	lcd_writeCommand(0x06);

	lcd_writeCommand(0x01);
	delayMs(4);
}

void lcd_print(uint8_t cmd, char *str)
{
	lcd_writeCommand(cmd);
	while(*str!= '\0')
	{lcd_writeData(*str);str++;}
}

void LCD_putStr( char *string )
{
   while (*string != '\0')   // display string
   {
      lcd_writeData(*string);
      string++;
   }  
}

void lcd_cursor(uint8_t col,uint8_t row )
{
	int row_offset[] = {0x00,0x40,0x14,0x54};
	lcd_writeCommand(0x80 |col + row_offset[row] );
}
int main (void)
{
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
I2c_init();
	lcd_init();
	lcd_writeCommand(0x80);
	lcd_cursor(1,1);
	LCD_putStr("thaidfghjhgfg");
//	lcd_print(0x80,"thaidfghj");
	
	//lcd_print(text);
	while(1)
	{//lcd_print("thai");}
	}
}








