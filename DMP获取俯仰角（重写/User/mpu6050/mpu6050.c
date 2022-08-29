/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   MPU6050驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "./mpu6050/mpu6050.h"
#include "./usart/bsp_usart.h"
#include "./i2c/bsp_i2c.h"
#include "./systick/bsp_Systick.h"
#ifndef MPU_DMP
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpkey.h"
#include "dmpmap.h"
const unsigned char Data0[1]={0x80};

const unsigned char Data1[1]={0x01};
#endif

#if Kalman
/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}


/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //配置加速度传感器工作在2G模式，不自检
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}
/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


#else

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)

#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)

#define	GYRO_CONFIG		0x1B	
//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
/*
0bxx<<3:
00 ~ +-250°
01 ~ +-500°
10 ~ +-1000°
11 ~ +-2000°
*/


#define	ACCEL_CONFIG	0x1C	
//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
/*
0bxx<<3:
00 ~ +-2g
01 ~ +-4g
10 ~ +-18g
11 ~ +-16g
*/

#define	ACCEL_XOUT_H	0x3B//加速度计测量值寄存器地址
#define	ACCEL_XOUT_L	0x3C 

#define	ACCEL_YOUT_H	0x3D //2 3
#define	ACCEL_YOUT_L	0x3E

#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
/********************/
#define	TEMP_OUT_H		0x41//温度测量值寄存器地址
#define	TEMP_OUT_L		0x42
/*********************/
#define	GYRO_XOUT_H		0x43// 8 9 陀螺仪测量值寄存器地址
#define	GYRO_XOUT_L		0x44
	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46

#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define MPU6050_ADDR  	0x68

#define MPU6050_WHO_AM_I         0x75

unsigned char IIC_buf[14];
static signed short ax;
static signed short ay;
static signed short az;

static signed short gx;
static signed short gy;
static signed short gz;

static signed short ax_cl = 0;
static signed short ay_cl = 0;
static signed short az_cl = 0;
  
static signed short gx_cl = 0;
static signed short gy_cl = 0;
static signed short gz_cl = 0;




/*
void IIC_Write(unsigned char dev_addr, unsigned char mem_addr, 
								unsigned char data)
{
	HAL_I2C_Mem_Write(&hi2c1, dev_addr, mem_addr,
						I2C_MEMADD_SIZE_8BIT, &data, 1, 2);
}

void IIC_Read(unsigned char dev_addr, unsigned char mem_addr, 
	unsigned char *buf, unsigned char len)
{
	HAL_I2C_Mem_Read(&hi2c1, dev_addr, mem_addr, 
					I2C_MEMADD_SIZE_8BIT, buf, len, 2);
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)

1、I2C指针，即用I2C1 还是 I2C2…
2、器件地址uint16_t DevAddress
3、要写入的内存地址 uint16_t MemAddress
4、内存地址类型，是一个地址存8bit ,还是16bit数据 , uint16_t MemAddSize
5、要写入的数组指针uint8_t *pData
6、数据 大小
7、多少时间没完成定义为超时
*/
/*写入初始化MPU6050*/
void MPU6050_Init(void)
{
  unsigned char data;
	unsigned char temp = 0;
reset_MPU6050:
  data = 0x80;
	Sensors_I2C_WriteRegister(MPU6050_ADDR,PWR_MGMT_1,1,&data);	//复位MPU6050
	mdelay(50);
  data = 0x01;
	Sensors_I2C_WriteRegister(MPU6050_ADDR,PWR_MGMT_1,1,&data);	//唤醒MPU6050，使用x轴PLL为时钟
	Sensors_I2C_ReadRegister(MPU6050_ADDR, MPU6050_WHO_AM_I, 1 , &temp);	//读ID
	temp &= 0x7e;
	printf("%d\r\n",temp);
   
	
  if(temp != 0x68)
		goto reset_MPU6050;
		
	data = 0x00;
	Sensors_I2C_WriteRegister(MPU6050_ADDR, SMPLRT_DIV, 1, &data);	//采样率分频，不分频
	Sensors_I2C_WriteRegister(MPU6050_ADDR, CONFIG, 1,&data);	//低通滤波器，带宽256~260Hz
	data = (0x00<<3);
	Sensors_I2C_WriteRegister(MPU6050_ADDR,GYRO_CONFIG,1,&data);	//陀螺量程,+-250°
	Sensors_I2C_WriteRegister(MPU6050_ADDR,ACCEL_CONFIG,1,&data);	//加速度计量程,+-2g
/*
	IIC_Write(MPU6050_ADDR,0x6B,0x00);
	IIC_Write(MPU6050_ADDR,0x6A,0x00);
	IIC_Write(MPU6050_ADDR,0x37,0x02);
*/
}


void MPU6050_Get(void)
{
	Sensors_I2C_ReadRegister(MPU6050_ADDR,ACCEL_XOUT_H, 14, IIC_buf);

	ax = (IIC_buf[0]<<8) + IIC_buf[1] - ax_cl;//X轴加速度
	ay = (IIC_buf[2]<<8) + IIC_buf[3] - ay_cl;//Y轴加速度
	az = (IIC_buf[4]<<8) + IIC_buf[5] - az_cl;//Z轴加速度

	gx = (IIC_buf[8]<<8) + IIC_buf[9] - gx_cl;//X轴角速度	
	gy = (IIC_buf[10]<<8) + IIC_buf[11] - gy_cl;//Y轴角速度	
	gz = (IIC_buf[12]<<8) + IIC_buf[13] - gz_cl;//Z轴角速度	
  
//  printf("ax:%1.3f g,\tgx:%f deg/s\r\n",(float)ax/(0xffff/4),(float)gx/(0xffff/500));
//	printf("ay:%1.3f g,\tgy:%f deg/s\r\n",(float)ay/(0xffff/4),(float)gy/(0xffff/500));
//	printf("az:%1.3f g,\tgz:%f deg/s\r\n",(float)az/(0xffff/4),(float)gz/(0xffff/500));
//	printf("----------------------------------\r\n");	 
}


/*
MPU6050校准函数
将IMU水平放置，z轴向上时，启动校准
思路是计算N个周期的平均值，得到校准参数
*/
#define	CL_cnt	128
void MPU6050_calibrate(void)
{
	unsigned short i;	
	signed int temp[6] = {0};
	for(i=0; i<CL_cnt; i++)
	{
		mdelay(10);
		MPU6050_Get();
		temp[0] += ax;
		temp[1] += ay;
		temp[2] += az;
		temp[3] += gx;
		temp[4] += gy;
		temp[5] += gz;
	}	
	ax_cl = temp[0]/CL_cnt;
	ay_cl = temp[1]/CL_cnt;
	az_cl = temp[2]/CL_cnt - (0xffff>>2); //平放时z轴有重力加速度g，减去g值
	gx_cl = temp[3]/CL_cnt;
	gy_cl = temp[4]/CL_cnt;
	gz_cl = temp[5]/CL_cnt;
}
#endif

