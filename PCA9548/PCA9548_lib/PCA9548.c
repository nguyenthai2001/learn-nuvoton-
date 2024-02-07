#include <stdio.h>
#include "NUC100Series.h"

void PCA9548_write(I2C_T *i2c, uint32_t addr, uint32_t channel) 
{
	PA0 = 1;
	I2C_Open(i2c,100000);
	I2C_START(i2c);
	I2C_WAIT_READY(i2c);
	
	I2C_SET_DATA(i2c,addr<<1 | 0);
	I2C_SET_CONTROL_REG(i2c,I2C_I2CON_SI_Msk);
	I2C_WAIT_READY(i2c);
	
	I2C_SET_DATA(i2c,channel);
	I2C_SET_CONTROL_REG(i2c,I2C_I2CON_SI_Msk);
	I2C_WAIT_READY(i2c);
	
	I2C_STOP(i2c);
	
}

uint8_t  PCA9548_read(I2C_T *i2c,uint32_t addr)
{
  uint8_t data;
	I2C_Open(i2c,100000);
	I2C_START(i2c);
  I2C_WAIT_READY(i2c);
	
	I2C_SET_DATA(i2c,addr<<1 | 0x01);
	I2C_SET_CONTROL_REG(i2c,I2C_I2CON_SI_Msk );
	I2C_WAIT_READY(i2c);
	
	data = I2C_GET_DATA(i2c);
	I2C_SET_CONTROL_REG(i2c,I2C_I2CON_SI_Msk);
//	I2C_WAIT_READY(i2c);
	
	I2C_STOP(i2c);
	
	return data;

}