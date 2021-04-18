/**	@file	bme280.c
  *	@brief	the driver of bme280 
  *
  */
  
#include "bme280.h"  
#include "i2c1.h"

#define BME_ADDR	0x76

void bme280_read_byte(uint8_t offset, uint8_t *value)
{
	i2c1_byte_read(BME_ADDR, offset, value);
}

void bme280_read_id(uint8_t *id)
{
	printf("read\r\n");
	i2c1_byte_read(BME_ADDR, BME280_ID, id);
}

void bme280_read_pressure()
{
	uint8_t press_msb = 0;
	uint8_t press_lsb = 0;
	uint8_t press_xlsb = 0;
	
	bme280_read_byte(BME280_PRESS_MSB, &press_msb);
	bme280_read_byte(BME280_PRESS_LSB, &press_lsb);
	bme280_read_byte(BME280_PRESS_XLSB, &press_xlsb);
}
