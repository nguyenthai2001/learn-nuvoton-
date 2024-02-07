#include <NUC100Series.h>
#include "LCD_16x2.h"
#define FOUR_BIT_INTERFACE

void LCD_Trigger(void)
{
	LCD_PIN_EN  = 1;
	CLK_SysTickDelay(10);
	LCD_PIN_EN = 0;
	CLK_SysTickDelay(50);
	
}

void LCD_Clear_Display(void)
{
	LCD_Cmd(0x01);
	CLK_SysTickDelay(16000);
}

void LCD_Export_8bit(unsigned char wdata)
{
  LCD_PIN_D0 = (wdata >> 0 )& 0x01;
	LCD_PIN_D1 = (wdata >> 1 )& 0x01;
	LCD_PIN_D2 = (wdata >> 2 )& 0x01;
	LCD_PIN_D3 = (wdata >> 3 )& 0x01;
	LCD_PIN_D4 = (wdata >> 4 )& 0x01;
	LCD_PIN_D5 = (wdata >> 5 )& 0x01;
	LCD_PIN_D6 = (wdata >> 6 )& 0x01;
	LCD_PIN_D7 = (wdata >> 7 )& 0x01;
}

void LCD_Export_4bit(unsigned char wdata)
{
	LCD_PIN_D4 = (wdata >> 0 )& 0x01;
	LCD_PIN_D5 = (wdata >> 1 )& 0x01;
	LCD_PIN_D6 = (wdata >> 2 )& 0x01;
	LCD_PIN_D7 = (wdata >> 3 )& 0x01;
}

void LCD_Cmd(unsigned char _cmd)
{
	LCD_PIN_EN = 0 ;
	LCD_PIN_RS = 0;
	LCD_PIN_RW = 0;
	
	#ifdef EIGHT_BIT_INTERFACE
	   LCD_Export_8bit(_cmd);
	   LCD_Trigger();
	#else 
	   LCD_Export_4bit (_cmd >>4);
	   LCD_Trigger();
	   LCD_Export_4bit (_cmd);
	   LCD_Trigger();
	#endif
}

void LCD_Data (unsigned char _data)
{
	LCD_PIN_RS = 1;
	LCD_PIN_EN = 0;
	LCD_PIN_RW = 0;
	
	#ifdef EIGHT_BIT_INTERFACE
	    LCD_Export_8bit(_data);
	    LCD_Trigger();
	#else 
	    LCD_Export_4bit(_data>>4);
	    LCD_Trigger();
	    LCD_Export_4bit(_data);
	    LCD_Trigger();
	#endif
	    
}

void LCD_Init (void)
{
	CLK_SysTickDelay(20000);
	LCD_PIN_RS = 0;
	LCD_PIN_RW = 0;
	LCD_PIN_EN = 0;
	
	#ifdef EIGHT_BIT_INTERFACE
	   LCD_Export_8bit(0x30);
	   LCD_Trigger();
	   DrvSYS_Delay(5000);
	   
	   LCD_Export_8bit(0x30);
	   LCD_Trigger();
	   DrvSYS_Delay(5000);
	   
	   LCD_Export_8bit(0x30);
	   LCD_Trigger();
	   DrvSYS_Delay(5000);
	   
	 #else 
	   LCD_Export_4bit(0x03);
	   LCD_Trigger();
		 CLK_SysTickDelay(5000);
		 
		 LCD_Export_4bit(0x03);
		 LCD_Trigger();
		 CLK_SysTickDelay(100);
		 
		 LCD_Export_4bit(0x03);
		 LCD_Trigger();
		 CLK_SysTickDelay(50);
		 
		 LCD_Export_4bit(0x02);
		 LCD_Trigger();
		 CLK_SysTickDelay(10);
		 LCD_Cmd(FOUR_BIT & TWO_LINE);
	#endif
	   LCD_Cmd(DON & CURSOR_OFF & BLINK_OFF);
		 LCD_Cmd (ENTIRE_SHIFT_OFF);
		 LCD_Cmd (CLEAR_LCD);
		 CLK_SysTickDelay(16000);
		 
} 

void LCD_Local(unsigned char line , unsigned char row)
{
	if(line == 1)
		LCD_Cmd(0x80+row);
	else 
		LCD_Cmd(0xC0+row);
	  CLK_SysTickDelay(50);
}

void LCD_Display_Data (unsigned char line , unsigned char row , unsigned char data )
{
	LCD_Local(line,row);
	LCD_Data(data);
}

void LCD_String (unsigned char line , unsigned char row , unsigned  char *_char )
{
	LCD_Local(row,line );
	while ((*_char ) != '\0')
	{
		LCD_Data(*_char );
		_char ++;
	}
}
