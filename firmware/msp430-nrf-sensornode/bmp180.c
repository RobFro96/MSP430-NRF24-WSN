/**
 * @file bmp180.c
 * @author Robert Fromm
 * @date 12.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "isr.h"
#include "i2c.h"
#include "bmp180.h"

static int32_t bmp180_compute_B5(const bmp_180_cal_param_t *cal_param, int32_t UT);

uint8_t bmp180_get_cal_param(bmp_180_cal_param_t *cal_param) {
    const uint8_t req = 0xAA;
    uint8_t err = i2c_write(BMP180_I2C_ADDR, &req, 1);
    err |= i2c_read(BMP180_I2C_ADDR, (uint8_t *) cal_param, sizeof(bmp_180_cal_param_t));

    cal_param->AC1 = __swap_bytes(cal_param->AC1);
    cal_param->AC2 = __swap_bytes(cal_param->AC2);
    cal_param->AC3 = __swap_bytes(cal_param->AC3);
    cal_param->AC4 = __swap_bytes(cal_param->AC4);
    cal_param->AC5 = __swap_bytes(cal_param->AC5);
    cal_param->AC6 = __swap_bytes(cal_param->AC6);
    cal_param->B1 = __swap_bytes(cal_param->B1);
    cal_param->B2 = __swap_bytes(cal_param->B2);
    cal_param->MB = __swap_bytes(cal_param->MB);
    cal_param->MC = __swap_bytes(cal_param->MC);
    cal_param->MD = __swap_bytes(cal_param->MD);


    return err;
}

uint8_t bmp180_get_ut(int32_t *ut) {
    const uint8_t req1[] = { 0xF4, 0x2E };
    uint8_t err = i2c_write(BMP180_I2C_ADDR, req1, 2);
    isr_delay(148); // 4.5 ms

    const uint8_t req2 = 0xF6;
    err |= i2c_write(BMP180_I2C_ADDR, &req2, 1);

    uint8_t res[2];
    err |= i2c_read(BMP180_I2C_ADDR, res, 2);
    *ut = (res[0] << 8) + res[1];
    return err;
}

int32_t bmp180_get_temperature(const bmp_180_cal_param_t *cal_param, int32_t UT) {
    int32_t B5 = bmp180_compute_B5(cal_param, UT);
    return (B5 + 8) >> 4;
}

static int32_t bmp180_compute_B5(const bmp_180_cal_param_t *cal_param, int32_t UT) {
    int32_t X1 = ((UT - (int32_t) cal_param->AC6) * (int32_t) cal_param->AC5) >> 15;
    int32_t X2 = ((int32_t) cal_param->MC << 11) / (X1 + (int32_t) cal_param->MD);

    return X1 + X2;
}

uint8_t bmp180_get_up(int32_t *up) {
    const uint8_t req1[] = { 0xF4, 0x34 + (BMP180_OSS << 6) };
    uint8_t err = i2c_write(BMP180_I2C_ADDR, req1, 2);

    switch (BMP180_OSS) {
    case 0:
        isr_delay_ms(5); // 4.5 ms
        break;
    case 1:
        isr_delay_ms(8); // 7.5 ms
        break;
    case 2:
        isr_delay_ms(14); // 13.5 ms
        break;
    case 3:
        isr_delay_ms(26); // 25.5 ms
        break;
    default:
        break;
    }

    const uint8_t req2 = 0xF6;
    err |= i2c_write(BMP180_I2C_ADDR, &req2, 1);

    uint8_t res[3];
    err |= i2c_read(BMP180_I2C_ADDR, res, 3);
    *up = (((int32_t) res[0] << 16) + ((int32_t) res[1] << 8) + res[2]) >> (8 - BMP180_OSS);
    return err;
}

int32_t bmp180_get_pressure(const bmp_180_cal_param_t *cal_param, int32_t UT, int32_t UP) {
    int32_t B3 = 0;
    int32_t B5 = 0;
    int32_t B6 = 0;
    int32_t X1 = 0;
    int32_t X2 = 0;
    int32_t X3 = 0;
    int32_t pressure = 0;
    uint32_t B4 = 0;
    uint32_t B7 = 0;

    B5 = bmp180_compute_B5(cal_param, UT);

    // pressure calculation
    B6 = B5 - 4000;
    X1 = ((int32_t) cal_param->B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t) cal_param->AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t) cal_param->AC1 * 4 + X3) << BMP180_OSS) + 2) / 4;

    X1 = ((int32_t) cal_param->AC3 * B6) >> 13;
    X2 = ((int32_t) cal_param->B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t) cal_param->AC4 * (X3 + 32768L)) >> 15;
    B7 = (UP - B3) * (50000UL >> BMP180_OSS);

    if (B7 < 0x80000000)
        pressure = (B7 * 2) / B4;
    else
        pressure = (B7 / B4) * 2;

    X1 = (pressure >> 8) * (pressure >> 8);
    X1 = (X1 * 3038L) >> 16;
    X2 = (-7357L * pressure) >> 16;

    return pressure + ((X1 + X2 + 3791L) >> 4);
}
