#ifndef ADC_H_
#define ADC_H_

typedef enum {
    ADC_REF1_5V = 0, ADC_REF2_5V = REF2_5V
} adc_reference_t;

uint16_t adc_measure_vbat();
uint16_t adc_measure_temperature();

#endif /* ADC_H_ */
