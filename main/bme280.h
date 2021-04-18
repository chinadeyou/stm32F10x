/*!
    \file  bme280.h
    \brief the header file of bme280.c
*/

#ifndef __BME280_H_
#define __BME280_H_
#include "stm32f10x.h"

#define BME280_ID			0xD0
#define BME280_PRESS_MSB 	0xF7
#define BME280_PRESS_LSB 	0xF8
#define BME280_PRESS_XLSB 	0xF9


void bme280_read_id(uint8_t *id);

#endif /* __BME280_H_ */
