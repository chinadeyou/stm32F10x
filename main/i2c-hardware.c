/**
  * @file    i2c-hardware.c
  * @date    2016/12/16
  * @brief   stm32 I2C 控制器
  */
  
#include "i2c-hardware.h"

#define I2C1_SCL_PORT 	GPIOB
#define I2C1_SDA_PORT 	GPIOB
#define I2C1_SCL_RCC    RCC_APB2Periph_GPIOB
#define I2C1_SDA_RCC    RCC_APB2Periph_GPIOB
#define I2C1_SCL_PIN 	GPIO_Pin_6
#define I2C1_SDA_PIN 	GPIO_Pin_7


void I2C1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	/* 始能SCL、SDA GPIO 时钟 */
	RCC_APB2PeriphClockCmd(I2C1_SCL_RCC | I2C1_SDA_RCC, ENABLE);
	
	/* 始能I2C外设时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	/* SCL GPIO初始化 */
	GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);	 
	
	/* SDA GPIO初始化 */
	GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
	GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);	 

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_OWN_ADDR7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C1_STD_SPEED;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be read is higher than 1, this function just 
  *       configures the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used. The byte
  *       is read in polling mode.
  * 
  * @retval I2C_OK (0) if operation is correctly performed, else return I2C_FAIL (1)
  */
void I2C1_Buffer_Read(uint8_t* p_buffer, uint16_t read_addr, uint16_t number_of_byte)
{

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	/* Test on EV5 and clear it : 等待事件5，表示开始信号正常发起 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send I2C device address for read */
	I2C_Send7bitAddress(I2C1, SLAVE_DEVICE_ADDR, I2C_Direction_Transmitter);
	
	/* Test on EV6 and clear it : 等待从机应答, 事件6,判断主机是否处于主机发送模式 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		
#ifdef ONE_BYTE_ADDR	/* 8位片内地址 */
  
	/* Send the EEPROM's internal address toread from : only one byte Address */
	I2C_SendData(sEE_I2C, ReadAddr);  
  
#elif defined TWO_BYTE_ADDR 	/* 16位片内地址 */
	/* Send the EEPROM's internal address to write to : MSB of the address first */
	I2C_SendData(I2C1, (uint8_t)((read_addr & 0xFF00) >> 8));

	/* Test on EV8 and clear it : 事件8,数据已发送 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
	/* Send the EEPROM's internal address to read from : LSB of the address */
	I2C_SendData(I2C1, (uint8_t)(read_addr & 0x00FF));
  
#endif /* ONE_BYTE_ADDR */ 
	
	/* Test on EV8 and clear it : 事件8,数据已发送 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C1, ENABLE);
	
	/* Test on EV5 and clear it : 等待事件5，表示开始信号正常发起 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C1, SLAVE_DEVICE_ADDR, I2C_Direction_Receiver);
	
	/* Test on EV6 and clear it : 等待从机应答, 事件6,判断主机是否处于主机接收模式 */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	
	/* One Byte Master Reception procedure (POLLING) ---------------------------*/
	while(number_of_byte)
	{
		if (number_of_byte < 2)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C1, DISABLE);   

			/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
			(void)I2C1->SR2;
		
			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C1, ENABLE);
			
			/* Wait to make sure that STOP control bit has been cleared */
			while(I2C1->CR1 & I2C_CR1_STOP);
		}
//		/* Wait for the byte to be received */
//		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
		/* Test on EV7 and clear it : 接收数据 */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
		/* Read the byte received from the EEPROM */
		*p_buffer = I2C_ReceiveData(I2C1);
	
		/* Decrement the read bytes counter */
		number_of_byte--;
		p_buffer++;
	}
			
	/* Re-Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);  
	
}



