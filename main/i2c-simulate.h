/*!
    \file  i2c-simulate.h
    \brief the header file of i2c-simulate.c
*/

#ifndef __I2C_SIMULATE_H_
#define __I2C_SIMULATE_H_
#include "stm32f10x.h"

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_transmit_byte(uint8_t data);
uint8_t i2c_receive_byte(uint8_t ack);
uint8_t i2c_wait_ack(void);

void i2c_byte_write(uint8_t slave_addr, uint8_t offset, uint8_t value);
void i2c_byte_read(uint8_t slave_addr, uint8_t offset, uint8_t *value);

#endif /* __I2C_SIMULATE_H_ */
