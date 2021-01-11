/**
  ******************************************************************************
  * @file    stm32_i2c.h
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains definitions for emperature Sensor LM75 (on I2C)
  *          
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_I2C_H
#define __STM32_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C2
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C2
#define sEE_I2C_SCL_PIN                  GPIO_Pin_10                 /* PB.10 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define sEE_I2C_SDA_PIN                  GPIO_Pin_11                  /* PB.11 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define sEE_M24C64_32

#define sEE_I2C_DMA                      DMA1   
#define sEE_I2C_DMA_CHANNEL_TX           DMA1_Channel4
#define sEE_I2C_DMA_CHANNEL_RX           DMA1_Channel5 
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC4   
#define sEE_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL4 
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC5 
#define sEE_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL5    
#define sEE_I2C_DMA_CLK                  RCC_AHBPeriph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005810)
#define sEE_USE_DMA
   
#define sEE_I2C_DMA_TX_IRQn              DMA1_Channel4_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Channel5_IRQn
#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Channel4_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Channel5_IRQHandler  
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0
 
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1 

/* Time constant for the delay caclulation allowing to have a millisecond 
   incrementing counter. This value should be equal to (System Clock / 1000).
   ie. if system clock = 24MHz then sEE_TIME_CONST should be 24. */
#define sEE_TIME_CONST          24 



/** @addtogroup STM32100E_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */  
#define LM75_I2C                         I2C2
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C2
#define LM75_I2C_SCL_PIN                 GPIO_Pin_10                 /* PB.10 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_APB2Periph_GPIOB
#define LM75_I2C_SDA_PIN                 GPIO_Pin_11                 /* PB.11 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_APB2Periph_GPIOB
#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_12                 /* PB.12 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOB                       /* GPIOB */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_APB2Periph_GPIOB
#define LM75_I2C_DR                      ((uint32_t)0x40005810)

#define LM75_DMA_CLK                     RCC_AHBPeriph_DMA1
#define LM75_DMA_TX_CHANNEL              DMA1_Channel4
#define LM75_DMA_RX_CHANNEL              DMA1_Channel5
#define LM75_DMA_TX_TCFLAG               DMA1_FLAG_TC4
#define LM75_DMA_RX_TCFLAG               DMA1_FLAG_TC5   
   

void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction); 

void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void); 

  
#endif /* __STM32_I2C_H */

  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
