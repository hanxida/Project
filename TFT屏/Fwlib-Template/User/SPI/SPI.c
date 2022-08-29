#include "stm32f10x.h" 
#include "SPI.h"
#include "LCD.h"

/*
 *函数名:SPI_Config
 *描述：初始化SPI2，配置SPI工作模式
 *输入：无
 *输出：无
 */
void SPI_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	/* 使能SPI时钟 */
	SPI_APBxClock_FUN ( SPI_CLK, ENABLE );
	/* 使能SPI引脚相关的时钟 */
 	SPI_CS_APBxClock_FUN (SPI_CS_CLK|SPI_SCK_CLK|SPI_MOSI_CLK, ENABLE );
  /* 配置SPI的 CS引脚，普通IO即可 */
  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN);

	
  /* 配置SPI的 SCK引脚*/
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

//  /* 配置SPI的 MISO引脚*/
//  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);	
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPIx,&SPI_InitStructure);
	
	SPI_Cmd(SPIx,ENABLE);
}

/*
 *函数名：SPI_SendByte
 *描述：使用SPI发送一个字节的数据
 *输入：byte：要发送的数据
 *输出：无
 */
void SPI_SendByte(u8 byte)
{
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPIx , byte);


//  /* 等待接收缓冲区非空，RXNE事件 */
//  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);

//  /* 读取数据寄存器，获取接收缓冲区数据 */
//  return SPI_I2S_ReceiveData(SPIx);
}
 /**
  *函数名：SPI_FLASH_SendHalfWord
	*描述：使用SPI发送两个字节的数据
  *输入：要发送的两字节数据(16位)
  *输出：无
  */
void SPI_SendHalfWord(u16 HalfWord)
{

  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);
	
  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPIx , HalfWord);
}
