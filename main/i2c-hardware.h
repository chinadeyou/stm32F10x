/*!
    \file  i2c-hardware.h
    \brief the header file of i2c-hardware.c
*/

#ifndef __I2C_HARDWARE_H
#define __I2C_HARDWARE_H

#include "stm32f10x.h"

#define I2C_OK 		0
#define I2C_FAIL 	1   
#define I2C_LONG_TIMEOUT 4096

#define I2C1_OWN_ADDR7 	0x82
#define I2C1_STD_SPEED 	100000
#define I2C1_FAST_SPEED 400000

#define SLAVE_DEVICE_ADDR 0xA0


#endif /* __I2C_HARDWARE_H */
