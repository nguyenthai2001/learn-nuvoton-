#include <stdio.h>
#include "NUC100Series.h"
//#include "Init.h"

volatile uint32_t g_delayMsFlag = 0;

#define BH1750_ADDR     0x5c   // dia chi BH1750 H: 0x5c L: 0x23
#define BH1750_BIT_H    0x13   // bit doc cao 
#define BH1750_BIT_L    0x01  // bit doc thap 

uint8_t data_H, data_L;
    uint16_t lux_data;
	
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

void BH1750_Init(uint8_t addr)
{
//	
//    I2C_Open(I2C0, 100000);  // Khoi tao I2C 100Mhz
//	I2C_START(I2C0);                                // Gui tin hieu start I2C0
//    I2C_WAIT_READY(I2C0);

//    I2C_SET_DATA(I2C0, addr << 1 | 0);       // Gui dia chi BH1750 va bit 0 de bat dau doc
//    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk);    // tao thanh ghi  SI (Start)
//    I2C_WAIT_READY(I2C0);
//	
//    I2C_SET_DATA(I2C0, BH1750_BIT_H );             
//  // GUi bit cao de doc 
//    I2C_SET_CONTROL_REG(I2C0, I2C_I2CON_SI_Msk);    // chi toi thanh ghi SI (Start)
//    I2C_WAIT_READY(I2C0);
//	
//	  I2C_STOP(I2C0);
}

void BH1750_Read(I2C_T *i2c,uint8_t addr,float *lux)
{
	
    uint8_t data_H, data_L;
    uint16_t lux_data;
	  I2C_Open(i2c, 400000);  // Khoi tao I2C 100Mhz
	I2C_START(i2c);                                // Gui tin hieu start I2C0
    I2C_WAIT_READY(i2c);

    I2C_SET_DATA(i2c, addr << 1 | 0);       // Gui dia chi BH1750 va bit 0 de bat dau doc
    I2C_SET_CONTROL_REG(i2c, I2C_I2CON_SI_Msk);    // tao thanh ghi  SI (Start)
    I2C_WAIT_READY(i2c);
	
    I2C_SET_DATA(i2c, BH1750_BIT_H );             
  // GUi bit cao de doc 
    I2C_SET_CONTROL_REG(i2c, I2C_I2CON_SI_Msk);    // chi toi thanh ghi SI (Start)
    I2C_WAIT_READY(i2c);
	
	  I2C_STOP(i2c);

   	delayMs(10);
  	I2C_START(i2c);                                  // gui start 
    I2C_WAIT_READY(i2c);                             // cho phan hoi 

    I2C_SET_DATA(i2c,addr <<1 | 0x01 );      // Gui bit thap de doc 
    I2C_SET_CONTROL_REG(i2c, I2C_I2CON_SI_Msk);     // dat thanh ghi  SI (Start)
    I2C_WAIT_READY(i2c);                            // cho phan hoi 
		I2C_SET_CONTROL_REG(i2c,I2C_I2CON_SI_Msk | I2C_I2CON_AA_Msk);// Gui si va ACk 
    I2C_WAIT_READY(i2c);                            // Cjo phan hoi 
	
    data_H = I2C_GET_DATA(i2c);                     // doc byte tu dia chi BH1750
    I2C_SET_CONTROL_REG(i2c, I2C_I2CON_SI_Msk | I2C_I2CON_AA_Msk);    // Gui tin hieu  SI (Start)
    I2C_WAIT_READY(i2c);
    
    data_L = I2C_GET_DATA(i2c);                     // doc byte thap 
    I2C_SET_CONTROL_REG(i2c, I2C_I2CON_SI_Msk );  // gui tin hieu  SI (Start) và Nack
		I2C_WAIT_READY(i2c);                          // Cjo phan hoi 
		I2C_STOP(i2c);                                // Gui stop 
   lux_data = (data_H << 8) | data_L;               // Ket hop 2 byte
//I2C_STOP(I2C0);
    *lux = (float)lux_data / 1.2; 
}