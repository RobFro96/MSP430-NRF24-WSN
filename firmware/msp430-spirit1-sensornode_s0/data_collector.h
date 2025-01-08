/*
 * data_collector.h
 *
 *  Created on: 12.02.2022
 *      Author: rob
 */

#ifndef DATA_COLLECTOR_H_
#define DATA_COLLECTOR_H_

void data_collector();
void data_collector_rf_values();
void data_collector_vbat();
void data_collector_temperature_internal();
void data_collector_s0();

extern volatile uint32_t timer_overflows;
extern volatile uint32_t last_interrupt;
extern volatile uint32_t s0_delta;
extern volatile uint32_t s0_counter;

void data_collector_init_timer();

#endif /* DATA_COLLECTOR_H_ */
