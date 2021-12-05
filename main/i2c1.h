/*!
    \file  i2c1.h
    \brief the header file of i2c1.c
*/

#ifndef __I2C1_H__
#define __I2C1_H__
#include "stm32f10x.h"

void i2c1_init(void);
void i2c1_start(void);
void i2c1_stop(void);
void i2c1_transmit_byte(uint8_t data);
uint8_t i2c1_receive_byte(uint8_t ack);
uint8_t i2c1_wait_ack(void);

void i2c1_byte_write(uint8_t slave_addr, uint8_t offset, uint8_t value);
void i2c1_byte_read(uint8_t slave_addr, uint8_t offset, uint8_t *value);

#endif /* __I2C1_H__ */
