/*!
    \file  bme280.h
    \brief the header file of bme280.c
*/

#ifndef __BME280_H_
#define __BME280_H_
#include "stm32f10x.h"

#define BME280_ID			0xD0

#define BME280_TEMP_XLSB	0xFC
#define BME280_TEMP_LSB		0xFB
#define BME280_TEMP_MSB		0xFA
#define BME280_PRESS_XLSB 	0xF9
#define BME280_PRESS_LSB 	0xF8
#define BME280_PRESS_MSB 	0xF7




void bme280_read_id(uint8_t *id);
double bme280_read_press(void);
void bme280_read_byte(uint8_t offset, uint8_t *value);
void bme280_write_byte(uint8_t offset, uint8_t value);

#endif /* __BME280_H_ */
