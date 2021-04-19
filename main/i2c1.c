/**
  * @file    i2c1.c
  * @brief   GPIO模拟i2c1
  */

#include "i2c1.h"
#include "systick.h"

#define I2C1_GPIO_PORT 		GPIOB
#define I2C1_RCU_PORT		RCC_APB2Periph_GPIOB
#define I2C1_SCL_PIN 		GPIO_Pin_6
#define I2C1_SDA_PIN		GPIO_Pin_7

#define I2C1_HALF_CYCLE	(5)

#define I2C1_SCL_SET() 		GPIO_SetBits			(I2C1_GPIO_PORT, I2C1_SCL_PIN)
#define I2C1_SCL_RESET() 	GPIO_ResetBits			(I2C1_GPIO_PORT, I2C1_SCL_PIN)
#define I2C1_SDA_SET() 		GPIO_SetBits			(I2C1_GPIO_PORT, I2C1_SDA_PIN)
#define I2C1_SDA_RESET()	GPIO_ResetBits			(I2C1_GPIO_PORT, I2C1_SDA_PIN)
#define I2C1_READ_SDA() 	GPIO_ReadInputDataBit	(I2C1_GPIO_PORT, I2C1_SDA_PIN)

void I2C1_Set_SDA_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);	
}

void I2C1_Set_SDA_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);	
}

/**
  *	@brief	i2c1配置函数，初始化I2C1端口
  *	@param	无
  *	@retval 无
  */
void i2c1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(I2C1_RCU_PORT, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(I2C1_GPIO_PORT, I2C1_SCL_PIN | I2C1_SDA_PIN);
}

/**
  *	@brief	i2c1起始信号
  *	@param	无
  *	@retval 无
  */
void i2c1_start(void)
{
	/* 设置数据线为输出 */
	I2C1_Set_SDA_Out();
	
	/* 拉高数据线和时钟线 */
	I2C1_SCL_SET();
	I2C1_SDA_SET();
	delay_1us(I2C1_HALF_CYCLE);
	
	/* 拉低数据线产生开始信号 */
	I2C1_SDA_RESET();
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_RESET();
}

/**
  *	@brief	i2c1停止信号
  *	@param	无
  *	@retval 无
  */
void i2c1_stop(void)
{
	I2C1_Set_SDA_Out();
	I2C1_SCL_RESET();
	
	I2C1_SDA_RESET();	/* 拉低数据线 */
	delay_1us(I2C1_HALF_CYCLE);
	
	/* 先拉高时钟线,再拉高数据线以产生停止信号 */
	I2C1_SCL_SET();
	delay_1us(I2C1_HALF_CYCLE);
	I2C1_SDA_SET();
}

/**
  *	@brief	i2c1检测从机应答函数
  *	@param	无
  *	@retval 1：没有收到从机应答
  * @retval 0: 收到从机应答
  */
uint8_t i2c1_wait_ack(void)
{
	uint8_t TimeOut = 0;

	I2C1_Set_SDA_In();
	
	I2C1_SDA_SET();	//释放SDA,等待从机拉低SDA
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_SET();
	delay_1us(I2C1_HALF_CYCLE);
	
	while(I2C1_READ_SDA())	/* 检测应答信号 */
	{
		TimeOut++;
		if(TimeOut > 250) {
			
			return 1;	//没有收到应答
		}
	}
	I2C1_SCL_RESET();
	
	return 0;
}

/**
  *	@brief	i2c1主机发送应答信号
  *	@param	无
  *	@retval 无
  */
void i2c1_ack(void)
{
	I2C1_SCL_RESET();
	I2C1_Set_SDA_Out();
	
	I2C1_SDA_RESET();	/* 拉低数据线产生应答信号 */
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_SET();
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_RESET();
}

/**
  *	@brief	i2c1主机发送非应答信号
  *	@param	无
  *	@retval 无
  */
void i2c1_nack(void)
{
	I2C1_SCL_RESET();
	I2C1_Set_SDA_Out();
	
	I2C1_SDA_SET();	/* 拉高数据线产生非应答信号 */
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_SET();
	delay_1us(I2C1_HALF_CYCLE);
	
	I2C1_SCL_RESET();	
}

/**
  *	@brief	i2c1发送一个字节数据
  *	@param	data：要发送的数据
  *	@retval 无
  */
void i2c1_transmit_byte(uint8_t data)
{
	uint8_t i;
	I2C1_Set_SDA_Out();
	I2C1_SCL_RESET();	/* 开始传输数据 */
	
	for (i = 0; i < 8; i++)
	{
		if( data >> 7) {
			I2C1_SDA_SET();
		} else {
			I2C1_SDA_RESET();
		}
		
		data <<= 1;
		delay_1us(I2C1_HALF_CYCLE);
		
		I2C1_SCL_SET();
		delay_1us(I2C1_HALF_CYCLE);
		
		I2C1_SCL_RESET();
	}
}

/**
  *	@brief	i2c1接收一个字节数据
  *	@param	ack,接收数据后是否应答。
			0，不应答；
			1，应答
  *	@retval 接收的数据
  */
uint8_t i2c1_receive_byte(uint8_t ack)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	I2C1_Set_SDA_In();	/* 将数据线设置为输入 */
	for (i = 0; i < 8; i++)
	{
		I2C1_SCL_RESET();
		delay_1us(I2C1_HALF_CYCLE);
		
		/* 接收数据 */
		I2C1_SCL_SET();
		data <<= 1;

		if(I2C1_READ_SDA()) {
			data++;
		}
		
		delay_1us(I2C1_HALF_CYCLE);
	}
	
	if(ack) {
		i2c1_ack();
	} else {
		i2c1_nack();
	}
	
	return data;
}

/**
  *	@brief	i2c1 写一个字节
  * @param	offset i2c设备片内地址
  *	@param	value 期待写入i2c设备的值
  *
  */
void i2c1_byte_write(uint8_t slave_addr, uint8_t offset, uint8_t value)
{
	i2c1_start();
	i2c1_transmit_byte(slave_addr << 1);
	i2c1_wait_ack();
	i2c1_transmit_byte(offset);
	i2c1_wait_ack();
	i2c1_transmit_byte(value);
	i2c1_wait_ack();
	i2c1_stop();
}

/**
  *	@brief	i2c1 读一个字节
  * @param	slave_addr 7位i2c地址
  * @param 	offet i2c设备片内地址
  * @param 	value 返回参数，保存读取的值
  *
  */
void i2c1_byte_read(uint8_t slave_addr, uint8_t offset, uint8_t *value)
{
	i2c1_start();
	i2c1_transmit_byte(slave_addr << 1);		//发送从机地址 + Write
	i2c1_wait_ack();
	i2c1_transmit_byte(offset);
	i2c1_wait_ack();
	i2c1_start();
	i2c1_transmit_byte((slave_addr << 1) + 1);	//发送从机地址 + Read
	i2c1_wait_ack();
	*value = i2c1_receive_byte(0);				//接收数据并发送Nack
	i2c1_stop();
}

