#include "stm32f10x.h" 
#include "LCD.h"
#include "SPI.h"
#include "SysTick.h" 
#include "Picture.h"
#include "GUI.h"
void LCD_GPIO_Init(void)
{
	SPI_Config();
  GPIO_InitTypeDef GPIO_InitStructure;
	
	LCD_RS_SCK_APBxClock_FUN(LCD_RS_SCK,ENABLE);
	LCD_RST_SCK_APBxClock_FUN(LCD_RST_SCK,ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_RS_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_RST_PORT,&GPIO_InitStructure);
	SPI_CS_LOW();
}




/*
 *��������Lcd_WriteData
 *��������Һ����дһ��8λ����
 *���룺һ���ֽڵ�����(8λ)
 *�������
 */
  void Lcd_WriteData(u8 Data)
{
   SPI_CS_LOW();
   LCD_RS_Set;
   SPI_SendByte(Data);
   SPI_CS_HIGH(); 
}
/*
 *��������Lcd_WriteIndex
 *��������Һ����дһ��8λָ��
 *���룺һ���ֽڵ�ָ��(8λ)
 *�������
 */
void Lcd_WriteIndex(u8 Index)
{
	//SPI д����ʱ��ʼ
  SPI_CS_LOW();
	LCD_RS_Reset;
	SPI_SendByte(Index);
	SPI_CS_HIGH();
}
/*
 *��������Lcd_WriteData
 *��������Һ����дһ��16λ����
 *���룺�����ֽڵ�����(16λ)
 *�������
 */
void LCD_WriteData_16Bit(u16 Data)
{
	SPI_CS_LOW();
	LCD_RS_Set;
	SPI_SendHalfWord(Data);
	SPI_CS_HIGH(); 
}

void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
	Lcd_WriteData(Data);
}
/*
 *��������Lcd_Reset
 *��������Ļ��λ
 *���룺��
 *�������
 */
void Lcd_Reset(void)
{
	LCD_RST_Reset;
	SysTick_Delay_ms(100);
	LCD_RST_Set;
	SysTick_Delay_ms(50);
}
/*
 *��������LCD_Init
 *�������Բ�ͬ�ͺŵ�оƬ�����г�ʼ��
 *���룺dir������ͬоƬ
 *�������
 */
void LCD_Init(u8 dir)
{
	LCD_GPIO_Init();
	Lcd_Reset();
	Lcd_WriteIndex(0x11);//Sleep exit 
	if(dir==0)//Init for ST7735R
{
	Lcd_WriteIndex(0x11);//Sleep exit 
	SysTick_Delay_ms (120);
	
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); Lcd_WriteData(0x2C); Lcd_WriteData(0x2D); 
	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); Lcd_WriteData(0x2C); Lcd_WriteData(0x2D); 
	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); Lcd_WriteData(0x2C); Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); Lcd_WriteData(0x2C); Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); Lcd_WriteData(0x02); Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); Lcd_WriteData(0xC5); 
	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); Lcd_WriteData(0x00); 
	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
	Lcd_WriteData(0xC0); 
	
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); Lcd_WriteData(0x37); Lcd_WriteData(0x00); 
	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); Lcd_WriteData(0x10); 
	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);Lcd_WriteData(0x7f);
	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);Lcd_WriteData(0x9f);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	
	Lcd_WriteIndex(0x29);//Display on
}

else if(dir==1) //Init for ST7735S
{
	Lcd_WriteIndex(0x11);
	SysTick_Delay_ms (120); //Delay 120ms

	Lcd_WriteIndex(0xb1);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3c);
	Lcd_WriteData(0x3c);
	Lcd_WriteIndex(0xb2);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3c);
	Lcd_WriteData(0x3c);
	Lcd_WriteIndex(0xb3);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3c);
	Lcd_WriteData(0x3c);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3c);
	Lcd_WriteData(0x3c);

	Lcd_WriteIndex(0xb4);
	Lcd_WriteData(0x03);

	Lcd_WriteIndex(0xc0);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x06);
	Lcd_WriteData(0x04);
	Lcd_WriteIndex(0xc1);
	Lcd_WriteData(0xc0);
	Lcd_WriteIndex(0xc2);
	Lcd_WriteData(0x0d);
	Lcd_WriteData(0x00);
	Lcd_WriteIndex(0xc3);
	Lcd_WriteData(0x8d);
	Lcd_WriteData(0xea);
	Lcd_WriteIndex(0xc5);//set vcom  
	Lcd_WriteData(0x03);//VCOM=  

	Lcd_WriteIndex(0xc4);
	Lcd_WriteData(0x8d);
	Lcd_WriteData(0xee);
	//Lcd_WriteIndex(0x21);

	Lcd_WriteIndex(0x36);
	Lcd_WriteData(0xc0);

	Lcd_WriteIndex(0xe0);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x1f);
	Lcd_WriteData(0x06);
	Lcd_WriteData(0x0b);
	Lcd_WriteData(0x35);
	Lcd_WriteData(0x35);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x31);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x34);
	Lcd_WriteData(0x3e);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x03);

	Lcd_WriteIndex(0xe1);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x1e);
	Lcd_WriteData(0x06);
	Lcd_WriteData(0x0b);
	Lcd_WriteData(0x35);
	Lcd_WriteData(0x34);
	Lcd_WriteData(0x2f);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x32);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x35);
	Lcd_WriteData(0x3e);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x04);

	Lcd_WriteIndex(0x21);
	Lcd_WriteIndex(0x29);
	Lcd_WriteIndex(0x2c);
}
else if(dir==2)//Init for ILI9163
{

		Lcd_WriteIndex(0x11);       //start OSC
    SysTick_Delay_ms (100);
    
    Lcd_WriteIndex(0x3a);       //start OSC
    Lcd_WriteData(0x05);
   
    Lcd_WriteIndex(0x26);       //start OSC
    Lcd_WriteData(0x04);
	
    Lcd_WriteIndex(0xf2);              //Driver Output Control(1)
    Lcd_WriteData(0x01);
    
    Lcd_WriteIndex(0xe0);              //Driver Output Control(1)
    Lcd_WriteData(0x3f);
    Lcd_WriteData(0x25);
    Lcd_WriteData(0x1c);
    Lcd_WriteData(0x1e);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x12);
    Lcd_WriteData(0x2a);
    Lcd_WriteData(0x90);
    Lcd_WriteData(0x24);
    Lcd_WriteData(0x11);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
     
    Lcd_WriteIndex(0xe1);              //Driver Output Control(1)
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x05);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0xa7);
    Lcd_WriteData(0x3d);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x25);
    Lcd_WriteData(0x2a);
    Lcd_WriteData(0x2b);
    Lcd_WriteData(0x2b);  
    Lcd_WriteData(0x3a);  
    
    Lcd_WriteIndex(0xb1);              //LCD Driveing control
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x08);
    
    Lcd_WriteIndex(0xb4);              //LCD Driveing control
    Lcd_WriteData(0x07);
   
   
    Lcd_WriteIndex(0xc0);              //LCD Driveing control
    Lcd_WriteData(0x0a);
    Lcd_WriteData(0x02);
      
    Lcd_WriteIndex(0xc1);              //LCD Driveing control
    Lcd_WriteData(0x02);

    Lcd_WriteIndex(0xc5);              //LCD Driveing control
    Lcd_WriteData(0x4f);
    Lcd_WriteData(0x5a);

    Lcd_WriteIndex(0xc7);              //LCD Driveing control
    Lcd_WriteData(0x40);
    
    Lcd_WriteIndex(0x2a);              //LCD Driveing control
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x7f);
   
    Lcd_WriteIndex(0x2b);              //LCD Driveing control
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x7f);
    Lcd_WriteIndex(0x36);              //LCD Driveing control
    Lcd_WriteData(0xc8);//����0xA8 ����0xC8
    Lcd_WriteIndex(0xb7);              //LCD Driveing control
    Lcd_WriteData(0x00);
//    Lcd_WriteIndex(0xb8);              //LCD Driveing control
//    Lcd_WriteData(0x01);
	   
    Lcd_WriteIndex(0x29);   
    Lcd_WriteIndex(0x2c);  
	}
}
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�,Y_IncMode��ʾ������y������x
����ֵ����
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end);

	Lcd_WriteIndex(0x2c);

}
/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	Lcd_SetRegion(x,y,x,y);
}
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);

}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
unsigned int Lcd_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   for(i=0;i<128;i++)
    for(m=0;m<160;m++)
    {	
	  	Lcd_WriteData(Color>>8);
			Lcd_WriteData(Color);
    }   
}
/*
 *��������Font_Test
 *�������ַ�
 *���룺��
 *�������
 */
void Font_Test(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"������ʾ����");

	SysTick_Delay_ms(1000);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,30,YELLOW,GRAY0,"ȫ�����Ӽ���");
	Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"רעҺ������");
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "ȫ�̼���֧��");
	Gui_DrawFont_GBK16(0,100,BLUE,GRAY0,"Tel:15989313508");
	Gui_DrawFont_GBK16(0,130,RED,GRAY0, "www.qdtech.net");	
	SysTick_Delay_ms(1800);	
}
/*
 *��������Show_Pic
 *��������ʾͼƬ
 *���룺��
 *�������
 */
//16λ ��ֱɨ��  �ҵ���  ��λ��ǰ
void Show_Pic(void)
{
	int i,j,k;
	unsigned char picH,picL;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"ͼƬ��ʾ����");
	SysTick_Delay_ms(1000);
	Lcd_Clear(GRAY0);
	k=0;
	for(i=0;i<X_MAX_PIXEL;i++)
	for(j=0;j<Y_MAX_PIXEL;j++)
	{
		picH=gImage_123[k++];
		picL=gImage_123[k++];
		Lcd_WriteData(picH);
		Lcd_WriteData(picL);
	}	
} 
/*
 *��������LCD_Color_Fill
 *������������
 *���룺x1,y1,x2,y2,color ����б�Խ����꣬��ɫ
 *�������
 */
void LCD_Color_Fill(u16 x1, u16 y1,u16 x2, u16 y2,u16 Color)
{
		 Lcd_Clear(GRAY0);
		for(int j=y1;j<y2;j++)
		{
			Gui_DrawLine(x1,j,x2,j,Color);
		}
}
