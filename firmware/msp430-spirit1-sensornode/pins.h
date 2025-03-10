/**
 * THE WAKEUP RECEIVER PROJECT
 * @file pins.h
 * @author Robert Fromm
 * @date 08.12.2020
 *
 * This file is generated by the MSP430 Pinmapper.
 * Copy the following JSON code into Pinmapper program to edit project.
 * {"definitions":[{"name":"LED","pin":"P1.0","functions":[{"dir":"1","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"},{"dir":"-","out":"1","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"h","type":"set"},{"dir":"-","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"l","type":"set"}]},{"name":"SPIA_MISO","pin":"P1.1","functions":[{"dir":"-","out":"0","sel":"1","sel2":"1","in":"-","ren":"1","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"}]},{"name":"SPIA_MOSI","pin":"P1.2","functions":[{"dir":"-","out":"-","sel":"1","sel2":"1","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"}]},{"name":"1_3","pin":"P1.3","functions":[{"dir":"0","out":"0","sel":"-","sel2":"-","in":"-","ren":"1","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"},{"dir":"-","out":"-","sel":"-","sel2":"-","in":"?","ren":"-","ie":"-","ies":"-","ifg":"-","name":"val","type":"get"},{"dir":"-","out":"1","sel":"-","sel2":"-","in":"-","ren":"1","ie":"-","ies":"-","ifg":"-","name":"pullup","type":"set"},{"dir":"-","out":"0","sel":"-","sel2":"-","in":"-","ren":"1","ie":"-","ies":"-","ifg":"-","name":"pulldown","type":"set"},{"dir":"-","out":"-","sel":"-","sel2":"-","in":"-","ren":"0","ie":"-","ies":"-","ifg":"-","name":"pullno","type":"set"}]},{"name":"SPIA_SCK","pin":"P1.4","functions":[{"dir":"-","out":"-","sel":"1","sel2":"1","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"},{"dir":"1","out":"0","sel":"0","sel2":"0","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"mode_gpio","type":"set"},{"dir":"-","out":"1","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"h","type":"set"},{"dir":"-","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"l","type":"set"}]},{"name":"SPIRIT_IRQ","pin":"P2.0","functions":[{"dir":"0","out":"1","sel":"-","sel2":"-","in":"-","ren":"1","ie":"1","ies":"1","ifg":"0","name":"setup","type":"set"},{"dir":"-","out":"-","sel":"-","sel2":"-","in":"?","ren":"-","ie":"-","ies":"-","ifg":"-","name":"val","type":"get"},{"dir":"-","out":"-","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"?","name":"ifg","type":"get"},{"dir":"-","out":"-","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"0","name":"ifg_clr","type":"set"}]},{"name":"SPIRIT_CS","pin":"P2.1","functions":[{"dir":"1","out":"1","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"},{"dir":"-","out":"1","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"h","type":"set"},{"dir":"-","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"l","type":"set"}]},{"name":"I2C_SDA","pin":"P1.7","functions":[{"dir":"-","out":"-","sel":"1","sel2":"1","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"}]},{"name":"I2C_SCL","pin":"P1.6","functions":[{"dir":"-","out":"-","sel":"1","sel2":"1","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"}]},{"name":"1_5","pin":"P1.5","functions":[{"dir":"1","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"setup","type":"set"},{"dir":"-","out":"1","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"h","type":"set"},{"dir":"-","out":"0","sel":"-","sel2":"-","in":"-","ren":"-","ie":"-","ies":"-","ifg":"-","name":"l","type":"set"}]}],"device":"MSP430G2553","xinXout":true,"filename":"pins.h","prefix":"p_","dcoCalibration":true,"delayFunctions":true,"doxygenHeader":"THE WAKEUP RECEIVER PROJECT\n@file pins.h\n@author Robert Fromm\n@date 08.12.2020\n\nThis file is generated by the MSP430 Pinmapper.\nCopy the following JSON code into Pinmapper program to edit project."}
 */

#ifndef PINS_H_
#define PINS_H_

#include <msp430.h>
#if defined P_CLOCK_FREQ_1_MHZ
#define P_CLOCK_FREQ_MHZ 1
#elif defined P_CLOCK_FREQ_8_MHZ
#define P_CLOCK_FREQ_MHZ 8
#elif defined P_CLOCK_FREQ_12_MHZ
#define P_CLOCK_FREQ_MHZ 12
#elif defined P_CLOCK_FREQ_16_MHZ
#define P_CLOCK_FREQ_MHZ 16
#else
#include "config.h"
#endif

// PORT 1
#define P_LED BIT0
#define P_SPIA_MISO BIT1
#define P_SPIA_MOSI BIT2
#define P_1_3 BIT3
#define P_SPIA_SCK BIT4
#define P_1_5 BIT5
#define P_I2C_SCL BIT6
#define P_I2C_SDA BIT7
#define P_PORT1_UNUSED (0)

// PORT 2
#define P_SPIRIT_IRQ BIT0
#define P_SPIRIT_CS BIT1
#define P_XIN BIT6
#define P_XOUT BIT7
#define P_PORT2_UNUSED (BIT2 + BIT3 + BIT4 + BIT5)

// PORT 3
#define P_PORT3_UNUSED (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7)

// LED @ P1.0
#define p_led_setup() ({P1OUT &=~ P_LED; P1DIR |= P_LED;})
#define p_led_h() ({P1OUT |= P_LED;})
#define p_led_l() ({P1OUT &=~ P_LED;})

// SPIA_MISO @ P1.1
#define p_spia_miso_setup() ({P1SEL |= P_SPIA_MISO; P1SEL2 |= P_SPIA_MISO; P1OUT &=~ P_SPIA_MISO; P1REN |= P_SPIA_MISO;})

// SPIA_MOSI @ P1.2
#define p_spia_mosi_setup() ({P1SEL |= P_SPIA_MOSI; P1SEL2 |= P_SPIA_MOSI;})

// 1_3 @ P1.3
#define p_1_3_setup() ({P1OUT &=~ P_1_3; P1DIR &=~ P_1_3; P1REN |= P_1_3;})
#define p_1_3_val() ({P1IN & P_1_3;})
#define p_1_3_pullup() ({P1OUT |= P_1_3; P1REN |= P_1_3;})
#define p_1_3_pulldown() ({P1OUT &=~ P_1_3; P1REN |= P_1_3;})
#define p_1_3_pullno() ({P1REN &=~ P_1_3;})

// SPIA_SCK @ P1.4
#define p_spia_sck_setup() ({P1SEL |= P_SPIA_SCK; P1SEL2 |= P_SPIA_SCK;})
#define p_spia_sck_mode_gpio() ({P1SEL &=~ P_SPIA_SCK; P1SEL2 &=~ P_SPIA_SCK; P1OUT &=~ P_SPIA_SCK; P1DIR |= P_SPIA_SCK;})
#define p_spia_sck_h() ({P1OUT |= P_SPIA_SCK;})
#define p_spia_sck_l() ({P1OUT &=~ P_SPIA_SCK;})

// SPIRIT_IRQ @ P2.0
#define p_spirit_irq_setup() ({P2OUT |= P_SPIRIT_IRQ; P2DIR &=~ P_SPIRIT_IRQ; P2REN |= P_SPIRIT_IRQ; P2IES |= P_SPIRIT_IRQ; P2IFG &=~ P_SPIRIT_IRQ; P2IE |= P_SPIRIT_IRQ;})
#define p_spirit_irq_val() ({P2IN & P_SPIRIT_IRQ;})
#define p_spirit_irq_ifg() ({P2IFG & P_SPIRIT_IRQ;})
#define p_spirit_irq_ifg_clr() ({P2IFG &=~ P_SPIRIT_IRQ;})

// SPIRIT_CS @ P2.1
#define p_spirit_cs_setup() ({P2OUT |= P_SPIRIT_CS; P2DIR |= P_SPIRIT_CS;})
#define p_spirit_cs_h() ({P2OUT |= P_SPIRIT_CS;})
#define p_spirit_cs_l() ({P2OUT &=~ P_SPIRIT_CS;})

// I2C_SDA @ P1.7
#define p_i2c_sda_setup() ({P1SEL |= P_I2C_SDA; P1SEL2 |= P_I2C_SDA;})

// I2C_SCL @ P1.6
#define p_i2c_scl_setup() ({P1SEL |= P_I2C_SCL; P1SEL2 |= P_I2C_SCL;})

// 1_5 @ P1.5
#define p_1_5_setup() ({P1OUT &=~ P_1_5; P1DIR |= P_1_5;})
#define p_1_5_h() ({P1OUT |= P_1_5;})
#define p_1_5_l() ({P1OUT &=~ P_1_5;})

// SETUP
inline void p_setup() {
    //Clock Calibration
#if P_CLOCK_FREQ_MHZ == 1
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#elif P_CLOCK_FREQ_MHZ == 8
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
#elif P_CLOCK_FREQ_MHZ == 12
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#elif P_CLOCK_FREQ_MHZ == 16
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#endif

    // Pin Setup
    p_led_setup();
    p_spia_miso_setup();
    p_spia_mosi_setup();
    p_1_3_setup();
    p_spia_sck_setup();
    p_spirit_irq_setup();
    p_spirit_cs_setup();
    p_i2c_sda_setup();
    p_i2c_scl_setup();
    p_1_5_setup();

    // Unused Pins
    P1OUT &= ~ P_PORT1_UNUSED;
    P1REN |= P_PORT1_UNUSED;
    P2OUT &= ~ P_PORT2_UNUSED;
    P2REN |= P_PORT2_UNUSED;
    P3OUT &= ~ P_PORT3_UNUSED;
    P3REN |= P_PORT3_UNUSED;
}

#define p_delay_ms(t) (__delay_cycles(P_CLOCK_FREQ_MHZ * 1000L * (t)))
#define p_delay_us(t) (__delay_cycles(P_CLOCK_FREQ_MHZ * (t)))

#endif
