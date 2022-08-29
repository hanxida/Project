/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   MPU6050����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief   д���ݵ�MPU6050�Ĵ���
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
  * @brief   ��MPU6050�Ĵ�����ȡ����
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
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ����ʣ�1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //��ͨ�˲��������ã���ֹƵ����1K��������5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //���ü��ٶȴ�����������2Gģʽ�����Լ�
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}
/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
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
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
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
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


#else

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)

#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)

#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)

#define	GYRO_CONFIG		0x1B	
//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
/*
0bxx<<3:
00 ~ +-250��
01 ~ +-500��
10 ~ +-1000��
11 ~ +-2000��
*/


#define	ACCEL_CONFIG	0x1C	
//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
/*
0bxx<<3:
00 ~ +-2g
01 ~ +-4g
10 ~ +-18g
11 ~ +-16g
*/

#define	ACCEL_XOUT_H	0x3B//���ٶȼƲ���ֵ�Ĵ�����ַ
#define	ACCEL_XOUT_L	0x3C 

#define	ACCEL_YOUT_H	0x3D //2 3
#define	ACCEL_YOUT_L	0x3E

#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
/********************/
#define	TEMP_OUT_H		0x41//�¶Ȳ���ֵ�Ĵ�����ַ
#define	TEMP_OUT_L		0x42
/*********************/
#define	GYRO_XOUT_H		0x43// 8 9 �����ǲ���ֵ�Ĵ�����ַ
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

1��I2Cָ�룬����I2C1 ���� I2C2��
2��������ַuint16_t DevAddress
3��Ҫд����ڴ��ַ uint16_t MemAddress
4���ڴ��ַ���ͣ���һ����ַ��8bit ,����16bit���� , uint16_t MemAddSize
5��Ҫд�������ָ��uint8_t *pData
6������ ��С
7������ʱ��û��ɶ���Ϊ��ʱ
*/
/*д���ʼ��MPU6050*/
void MPU6050_Init(void)
{
  unsigned char data;
	unsigned char temp = 0;
reset_MPU6050:
  data = 0x80;
	Sensors_I2C_WriteRegister(MPU6050_ADDR,PWR_MGMT_1,1,&data);	//��λMPU6050
	mdelay(50);
  data = 0x01;
	Sensors_I2C_WriteRegister(MPU6050_ADDR,PWR_MGMT_1,1,&data);	//����MPU6050��ʹ��x��PLLΪʱ��
	Sensors_I2C_ReadRegister(MPU6050_ADDR, MPU6050_WHO_AM_I, 1 , &temp);	//��ID
	temp &= 0x7e;
	printf("%d\r\n",temp);
   
	
  if(temp != 0x68)
		goto reset_MPU6050;
		
	data = 0x00;
	Sensors_I2C_WriteRegister(MPU6050_ADDR, SMPLRT_DIV, 1, &data);	//�����ʷ�Ƶ������Ƶ
	Sensors_I2C_WriteRegister(MPU6050_ADDR, CONFIG, 1,&data);	//��ͨ�˲���������256~260Hz
	data = (0x00<<3);
	Sensors_I2C_WriteRegister(MPU6050_ADDR,GYRO_CONFIG,1,&data);	//��������,+-250��
	Sensors_I2C_WriteRegister(MPU6050_ADDR,ACCEL_CONFIG,1,&data);	//���ٶȼ�����,+-2g
/*
	IIC_Write(MPU6050_ADDR,0x6B,0x00);
	IIC_Write(MPU6050_ADDR,0x6A,0x00);
	IIC_Write(MPU6050_ADDR,0x37,0x02);
*/
}


void MPU6050_Get(void)
{
	Sensors_I2C_ReadRegister(MPU6050_ADDR,ACCEL_XOUT_H, 14, IIC_buf);

	ax = (IIC_buf[0]<<8) + IIC_buf[1] - ax_cl;//X����ٶ�
	ay = (IIC_buf[2]<<8) + IIC_buf[3] - ay_cl;//Y����ٶ�
	az = (IIC_buf[4]<<8) + IIC_buf[5] - az_cl;//Z����ٶ�

	gx = (IIC_buf[8]<<8) + IIC_buf[9] - gx_cl;//X����ٶ�	
	gy = (IIC_buf[10]<<8) + IIC_buf[11] - gy_cl;//Y����ٶ�	
	gz = (IIC_buf[12]<<8) + IIC_buf[13] - gz_cl;//Z����ٶ�	
  
//  printf("ax:%1.3f g,\tgx:%f deg/s\r\n",(float)ax/(0xffff/4),(float)gx/(0xffff/500));
//	printf("ay:%1.3f g,\tgy:%f deg/s\r\n",(float)ay/(0xffff/4),(float)gy/(0xffff/500));
//	printf("az:%1.3f g,\tgz:%f deg/s\r\n",(float)az/(0xffff/4),(float)gz/(0xffff/500));
//	printf("----------------------------------\r\n");	 
}


/*
MPU6050У׼����
��IMUˮƽ���ã�z������ʱ������У׼
˼·�Ǽ���N�����ڵ�ƽ��ֵ���õ�У׼����
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
	az_cl = temp[2]/CL_cnt - (0xffff>>2); //ƽ��ʱz�����������ٶ�g����ȥgֵ
	gx_cl = temp[3]/CL_cnt;
	gy_cl = temp[4]/CL_cnt;
	gz_cl = temp[5]/CL_cnt;
}
#endif

