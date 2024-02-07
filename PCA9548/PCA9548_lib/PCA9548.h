#include <stdio.h>
#include "NUC100Series.h"

/**************************************
A2 A1 A0
L L L 112 (decimal), 70 (hexadecimal)
L L H 113 (decimal), 71 (hexadecimal)
L H L 114 (decimal), 72 (hexadecimal)
L H H 115 (decimal), 73 (hexadecimal)
H L L 116 (decimal), 74 (hexadecimal)
H L H 117 (decimal), 75 (hexadecimal)
H H L 118 (decimal), 76 (hexadecimal)
H H H 119 (decimal), 77 (hexadecimal)
***************************************/
#define PCA9548_ADDR_1 0x70
#define PCA9548_ADDR_2 0x77
#define PCA9548_ADDR_3 0x71

#define Channel_0 0x01
#define Channel_1 0x02
#define Channel_2 0x04
#define Channel_3 0x08
#define Channel_4 0x10
#define Channel_5 0x20
#define Channel_6 0x40
#define Channel_7 0x80
#define No_channel 0x00
#define All_channel 0xff
#define Chance_channel 0xff

void PCA9548_write(I2C_T *i2c, uint32_t addr, uint32_t channel);
uint16_t  PCA9548_read(I2C_T *i2c,uint32_t addr);