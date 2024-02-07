#include <stdio.h>
#include "NUC100Series.h"
#define step_pin PC12
#define dir_pin PC15
#define stepsPerRevolution 200
int i;
uint8_t direction ;
uint32_t speed ;
uint32_t steps ;
void SYS_init(void)
{
	CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC,CLK_CLKDIV_HCLK(1));
	CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
	CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
	CLK_SetCoreClock(50000000);
	
}
void delay_ms(uint32_t ms) {
  uint32_t i ;
	for( i =0 ; i<ms ;i++);
	CLK_SysTickDelay(10);
	
}
//void stepMotor ( uint8_t dir,uint32_t speed , uint32_t steps)
//{
//	int i ;
//	if(dir  == 0 )
//	{
//		dir_pin = 0;
//	}
//	else 
//	{
//		dir_pin = 1;
//	}
//	
//	for (i =0 ; i< steps ; i++)
//	{
//		step_pin = 0;
//		delay_ms(speed);
//		step_pin = 1;
//		delay_ms(speed);
//	}
//}

//int main (void )
//{
//	SYS_UnlockReg();
//  SYS_init();
//	SYS_LockReg();
//	GPIO_SetMode(PC,12,GPIO_PMD_OUTPUT);
//	GPIO_SetMode(PC,15,GPIO_PMD_OUTPUT);
//	speed = 500;
//	steps = ; //goc/0.9
//	direction =0;
//	while(1)
//	{
//			//direction =!direction;
//		stepMotor(direction , speed,steps);
//	//	delay_ms(1000);
//		//direction =1;
//		//delay(100000);
//	}
//}

void Stepper_SetDirection(int dir)
{
    if (dir == 0) {
        // Set direction pin LOW for clockwise rotation
        dir_pin = 0;
    } else {
        // Set direction pin HIGH for counterclockwise rotation
        dir_pin =1;
    }
}

void Stepper_Step(int steps, int delay)
{
    // Step the motor 'steps' number of times with a delay between each step
    for ( i = 0; i < steps; i++) {
        // Set pulse pin HIGH to trigger the step
        step_pin = 0;
        
        // Delay for the specified amount of time
        CLK_SysTickDelay(delay);
        
        // Set pulse pin LOW to complete the step
       step_pin= 1;
        
        // Delay for the specified amount of time
        CLK_SysTickDelay(delay);
    }
}

int main(void)
{
	SYS_UnlockReg();
	SYS_init();
	SYS_LockReg();
	GPIO_SetMode(PC,12,GPIO_PMD_OUTPUT);
  GPIO_SetMode(PC,15,GPIO_PMD_OUTPUT);
	while (1) {
        // Rotate clockwise for 200 steps with a delay of 1ms between each step
        Stepper_SetDirection(0);
        Stepper_Step(400, 800);
        
        // Pause for 1 second
        CLK_SysTickDelay(1000);
        
        // Rotate counterclockwise for 200 steps with a delay of 1ms between each step
        Stepper_SetDirection(1);
        Stepper_Step(200, 1000);
        
//        // Pause for 1 second
//        CLK_SysTickDelay(1000000);
    }
}
