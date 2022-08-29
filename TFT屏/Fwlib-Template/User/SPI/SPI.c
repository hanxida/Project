#include "stm32f10x.h" 
#include "SPI.h"
#include "LCD.h"

/*
 *������:SPI_Config
 *��������ʼ��SPI2������SPI����ģʽ
 *���룺��
 *�������
 */
void SPI_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	/* ʹ��SPIʱ�� */
	SPI_APBxClock_FUN ( SPI_CLK, ENABLE );
	/* ʹ��SPI������ص�ʱ�� */
 	SPI_CS_APBxClock_FUN (SPI_CS_CLK|SPI_SCK_CLK|SPI_MOSI_CLK, ENABLE );
  /* ����SPI�� CS���ţ���ͨIO���� */
  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN);

	
  /* ����SPI�� SCK����*/
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

//  /* ����SPI�� MISO����*/
//  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

  /* ����SPI�� MOSI����*/
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
 *��������SPI_SendByte
 *������ʹ��SPI����һ���ֽڵ�����
 *���룺byte��Ҫ���͵�����
 *�������
 */
void SPI_SendByte(u8 byte)
{
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(SPIx , byte);


//  /* �ȴ����ջ������ǿգ�RXNE�¼� */
//  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET);

//  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
//  return SPI_I2S_ReceiveData(SPIx);
}
 /**
  *��������SPI_FLASH_SendHalfWord
	*������ʹ��SPI���������ֽڵ�����
  *���룺Ҫ���͵����ֽ�����(16λ)
  *�������
  */
void SPI_SendHalfWord(u16 HalfWord)
{

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET);
	
  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(SPIx , HalfWord);
}
