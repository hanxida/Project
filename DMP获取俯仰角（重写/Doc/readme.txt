

【*】 引脚分配
陀螺仪（MPU6050）：
MPU6050芯片的I2C接口与STM32的I2C1相连,且已接上拉电阻。
		VCC <--->3.3或5V
		GND	<--->GND
		SCL	<--->PB8
		SDA	<--->PB9		