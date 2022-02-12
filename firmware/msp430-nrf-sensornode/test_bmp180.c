/**
 * @file test_bmp180.c
 * @author Robert Fromm
 * @date 12.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "term.h"
#include "bmp180.h"
#include "test_bmp180.h"

/**
 * Test BMP180 is responding on I2C address 0x77
 */
void test_bmp180_i2c_detect() {
    term_log("\e[1mtest_bmp180_i2c_detect()\e[0m");
    uint8_t data[2];
    uint8_t result = i2c_read(BMP180_I2C_ADDR, data, 1);

    term_log_begin();
    term_print("I2C read on address 0x77: ");
    term_test_result(!result);
    term_end();
}

void test_bmp180_get_cal_param() {
    term_log("\e[1mtest_bmp180_get_cal_param()\e[0m");

    bmp_180_cal_param_t cal_param;
    bmp180_get_cal_param(&cal_param);

    term_log_begin();
    term_print("AC1=");
    term_int(cal_param.AC1, 5);
    term_print("  AC2=");
    term_int(cal_param.AC2, 5);
    term_print("  AC3=");
    term_int(cal_param.AC3, 5);
    term_print("  AC4=");
    term_uint(cal_param.AC4, 5);
    term_print("  AC5=");
    term_uint(cal_param.AC5, 5);
    term_print("  AC6=");
    term_uint(cal_param.AC6, 5);
    term_print("  B1=");
    term_int(cal_param.B1, 5);
    term_print("  B2=");
    term_int(cal_param.B2, 5);
    term_print("  MB=");
    term_int(cal_param.MB, 5);
    term_print("  MC=");
    term_int(cal_param.MC, 5);
    term_print("  MD=");
    term_int(cal_param.MD, 5);
    term_end();
}

void test_bmp180_get_temperature() {
    term_log("\e[1mtest_bmp180_get_temperature()\e[0m");

    bmp_180_cal_param_t cal_param;
    bmp180_get_cal_param(&cal_param);
    int32_t ut;
    bmp180_get_ut(&ut);
    int32_t temperature = bmp180_get_temperature(&cal_param, ut);

    term_log_begin();
    term_print("UT=");
    term_int(ut, 5);
    term_print("  T=");
    term_signed_decimal(temperature, 4, 1);
    term_print("^C");
    term_end();
}

void test_bmp180_get_pressure() {
    term_log("\e[1mtest_bmp180_get_pressure()\e[0m");

    bmp_180_cal_param_t cal_param;
    bmp180_get_cal_param(&cal_param);
    int32_t ut;
    bmp180_get_ut(&ut);
    int32_t up;
    bmp180_get_up(&up);

    // Datasheet values
//    bmp_180_cal_param_t cal_param;
//    cal_param.AC1 = 408;
//    cal_param.AC2 = -72;
//    cal_param.AC3 = -14383;
//    cal_param.AC4 = 32741;
//    cal_param.AC5 = 32757;
//    cal_param.AC6 = 23153;
//    cal_param.B1 = 6190;
//    cal_param.B2 = 4;
//    cal_param.MB = -32468;
//    cal_param.MC = -8711;
//    cal_param.MD = 2868;
//    int32_t ut = 27898;
//    int32_t up = 23843;

    int32_t pressure = bmp180_get_pressure(&cal_param, ut, up);

    term_log_begin();
    term_print("UT=");
    term_int(ut, 5);
    term_print("  UP=");
    term_int(up, 5);
    term_print("  P=");
    term_int(pressure, 6);
    term_print(" Pa");
    term_end();
}
