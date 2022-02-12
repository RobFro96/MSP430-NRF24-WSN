#ifndef SENSOR_H_
#define SENSOR_H_

#define SENSOR_BIT_ADC_VBAT_2_5V (0x8000)

typedef struct {
    uint8_t addr; // 00: device�s RF address A[7:0]
    uint8_t retries; // 01: count of current RF retries R[7:0]
    uint16_t vbat; // 02:03: 12-bit ADC reading of battery measurement N[11:0], reference R=0: 1.5 V, R=1: 2.5 V
    uint16_t temperature_internal; // 04:05: 12-bit ADC reading of internal temperature N[11:0]
    uint16_t si7021_humidity; // 06:07: Si7021 relative humidity raw reading N
    uint16_t si7021_temperature; // 08:09: Si7021 temperature raw reading N
} out_regs_t;

extern out_regs_t out_regs;

void sensor_mainloop();
void sensor_collect_data();



#endif /* SENSOR_H_ */