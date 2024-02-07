
#include<stdio.h>
#include"NUC100Series.h"

#define Addr_h 0x5c
#define Addr_l 0x23

void SysTick_Handler(void);
void delayMs(uint32_t ms);
void BH1750_Init(uint8_t addr);
void BH1750_Read(I2C_T *i2c,uint8_t addr,float *lux);