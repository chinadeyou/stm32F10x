/**	@file	bme280.c
  *	@brief	the driver of bme280 
  *
  */
  
#include "bme280.h"  
#include "i2c1.h"

#define BME_ADDR	0x76

#define REG_DIG_T1 	0x
#define REG_DIG_T2  0x
#define REG_DIG_T3  0x
#define REG_DIG_P1  0x
#define REG_DIG_P2  0x
#define REG_DIG_P3  0x
#define REG_DIG_P4  0x
#define REG_DIG_P5  0x
#define REG_DIG_P6  0x
#define REG_DIG_P7  0x
#define REG_DIG_P8  0x
#define REG_DIG_P9  0x


uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;



void bme280_read_byte(uint8_t offset, uint8_t *value)
{
	i2c1_byte_read(BME_ADDR, offset, value);
}

void bme280_read_id(uint8_t *id)
{
	i2c1_byte_read(BME_ADDR, BME280_ID, id);
}

int16_t bme_read_trimming_parameter(uint8_t lsb_reg)
{
	uint8_t lsb;
	uint8_t msb;
	i2c1_byte_read(BME_ADDR, lsb_reg, &lsb);
	i2c1_byte_read(BME_ADDR, lsb_reg + 1, &msb);
	
	return (msb << 8) | (lsb);
	
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



//long bmp280_GetValue(void)
//{
//    long adc_T;
//    long adc_P;
//    long var1, var2, t_fine, T, p;

//    adc_T = bmp280_MultipleReadThree(BMP280_TEMP_ADDR);
//    adc_P = bmp280_MultipleReadThree(BMP280_PRESS_ADDR);

//    if(adc_P == 0)
//    {
//        return 0;
//    }

//    //Temperature
//    var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
//    var2 = ((((double)adc_T)/131072.0-((double)dig_T1)/8192.0)*(((double)adc_T)
//                /131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);

//    t_fine = (unsigned long)(var1+var2);

//    T = (var1+var2)/5120.0;

//    var1 = ((double)t_fine/2.0)-64000.0;
//    var2 = var1*var1*((double)dig_P6)/32768.0;
//    var2 = var2 +var1*((double)dig_P5)*2.0;
//    var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
//    var1 = (((double)dig_P3)*var1*var1/524288.0+((double)dig_P2)*var1)/524288.0;
//    var1 = (1.0+var1/32768.0)*((double)dig_P1);
//    p = 1048576.0-(double)adc_P;
//    p = (p-(var2/4096.0))*6250.0/var1;
//    var1 = ((double)dig_P9)*p*p/2147483648.0;
//    var2 = p*((double)dig_P8)/32768.0;
//    p = p+(var1+var2+((double)dig_P7))/16.0;

//    return p;
//}

// Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC.
// t_fine carries fine temperature as global value
int32_t t_fine;
double bmp280_compensate_T_double(int32_t adc_T)
{
	double var1;
	double var2;
	double T;

	var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
	t_fine = (int32_t)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	
	return T;
}


// Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
double bmp280_compensate_P_double(int32_t adc_P)
{
	double var1, var2, p;
	
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);

	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);

	if (var1 == 0.0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
	return p;
}

