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
	i2c1_byte_read(BME_ADDR, BME280_ID, id);
}

void bme280_read_pressure()
{
	int press = 0;
	uint8_t press_msb = 0;
	uint8_t press_lsb = 0;
	uint8_t press_xlsb = 0;
	
	bme280_read_byte(BME280_PRESS_MSB, &press_msb);
	bme280_read_byte(BME280_PRESS_LSB, &press_lsb);
	bme280_read_byte(BME280_PRESS_XLSB, &press_xlsb);
	
	press = ( (uint32_t)press_msb << 8) | ( (uint32_t)press_lsb << 8) | (uint32_t)press_xlsb;
	press = ((uint32_t)press >> 4);
	printf("press = %#x\r\n", press);
}



long bmp280_GetValue(void)
{
    long adc_T;
    long adc_P;
    long var1, var2, t_fine, T, p;

    adc_T = bmp280_MultipleReadThree(BMP280_TEMP_ADDR);
    adc_P = bmp280_MultipleReadThree(BMP280_PRESS_ADDR);

    if(adc_P == 0)
    {
        return 0;
    }

    //Temperature
    var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
    var2 = ((((double)adc_T)/131072.0-((double)dig_T1)/8192.0)*(((double)adc_T)
                /131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);

    t_fine = (unsigned long)(var1+var2);

    T = (var1+var2)/5120.0;

    var1 = ((double)t_fine/2.0)-64000.0;
    var2 = var1*var1*((double)dig_P6)/32768.0;
    var2 = var2 +var1*((double)dig_P5)*2.0;
    var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
    var1 = (((double)dig_P3)*var1*var1/524288.0+((double)dig_P2)*var1)/524288.0;
    var1 = (1.0+var1/32768.0)*((double)dig_P1);
    p = 1048576.0-(double)adc_P;
    p = (p-(var2/4096.0))*6250.0/var1;
    var1 = ((double)dig_P9)*p*p/2147483648.0;
    var2 = p*((double)dig_P8)/32768.0;
    p = p+(var1+var2+((double)dig_P7))/16.0;

    return p;
}

