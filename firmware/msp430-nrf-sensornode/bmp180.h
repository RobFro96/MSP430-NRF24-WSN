/*
 * bmp180.h
 *
 *  Created on: 12.02.2022
 *      Author: rob
 */

#ifndef BMP180_H_
#define BMP180_H_

#define BMP180_I2C_ADDR (0x77)

typedef struct {
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;
} bmp_180_cal_param_t;

uint8_t bmp180_get_cal_param(bmp_180_cal_param_t *cal_param);
uint8_t bmp180_get_ut(int32_t *ut);
int32_t bmp180_get_temperature(const bmp_180_cal_param_t *cal_param, int32_t UT);
uint8_t bmp180_get_up(int32_t *up);
int32_t bmp180_get_pressure(const bmp_180_cal_param_t *cal_param, int32_t UT, int32_t UP);

#endif /* BMP180_H_ */
