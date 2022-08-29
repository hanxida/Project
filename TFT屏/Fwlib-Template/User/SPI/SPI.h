#ifndef __SPI_H
#define __SPI_H

#define      	SPIx                        SPI2
#define      	SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define      	SPI_CLK                     RCC_APB1Periph_SPI2

//CS(NSS)引脚 片选选普通GPIO即可
#define      	SPI_CS_APBxClock_FUN      	RCC_APB2PeriphClockCmd
#define      	SPI_CS_CLK                  RCC_APB2Periph_GPIOB   
#define      	SPI_CS_PORT                 GPIOB
#define      	SPI_CS_PIN                  GPIO_Pin_12
//SCK引脚
#define       SPI_SCK_APBxClock_FUN      	RCC_APB2PeriphClockCmd
#define       SPI_SCK_CLK                 RCC_APB2Periph_GPIOB   
#define       SPI_SCK_PORT                GPIOB   
#define       SPI_SCK_PIN                 GPIO_Pin_13
////MISO引脚
//#define       SPI_MISO_APBxClock_FUN     	RCC_APB2PeriphClockCmd
//#define       SPI_MISO_CLK                RCC_APB2Periph_GPIOB   
//#define       SPI_MISO_PORT               GPIOB 
//#define       SPI_MISO_PIN                GPIO_Pin_14
//MOSI引脚
#define       SPI_MOSI_APBxClock_FUN    	RCC_APB2PeriphClockCmd
#define       SPI_MOSI_CLK                RCC_APB2Periph_GPIOB 
#define       SPI_MOSI_PORT               GPIOB 
#define       SPI_MOSI_PIN                GPIO_Pin_15

#define  			SPI_CS_LOW()     						GPIO_ResetBits(SPI_CS_PORT,  SPI_CS_PIN )
#define  			SPI_CS_HIGH()    						GPIO_SetBits(SPI_CS_PORT,  SPI_CS_PIN )

void SPI_Config(void);
void SPI_SendByte(u8 byte);
void SPI_SendHalfWord(u16 HalfWord);
#endif
