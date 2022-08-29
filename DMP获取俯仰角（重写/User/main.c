#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./mpu6050/mpu6050.h"
#include "./i2c/bsp_i2c.h"
#include "DMP_test.h"
#define MPU_DMP 1
#define Kalman   0
#if Kalman 
#include "Kalman.h"
#endif

#if MPU_DMP
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpkey.h"
#include "dmpmap.h"

extern signed short  ax;
extern signed short ay;
extern signed short az;

extern signed short gx;
extern signed short gy;
extern signed short gz;

extern signed short ax_cl;
extern signed short ay_cl;
extern signed short az_cl;

extern signed short gx_cl;
extern signed short gy_cl;
extern signed short gz_cl;

#endif
/*
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */  
int main(void)
{
	float pitch,roll,yaw;         //欧拉角
  //初始化systick
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* 串口1通信初始化 */
	USART_Config();
	//I2C初始化
  
	I2C_Bus_Init();
 

  MPU6050_Init();
 
	MPU6050_calibrate();
  
	printf("\r\n 这是一个MPU6050_DMP测试例程 \r\n");
  DMP_test();
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			printf("pitch:%f\t,roll:%f\t,yaw:%f\r\n",pitch,roll,yaw);	
		}
	}
	
}



/*********************************************END OF FILE**********************/
