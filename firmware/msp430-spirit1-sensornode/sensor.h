#ifndef SENSOR_H_
#define SENSOR_H_

#define OUT_REGS_ADDR_INDEX 0
#define OUT_REGS_RETRIES_INDEX 1
#define OUT_REGS_VBAT_INDEX 2

typedef struct {
    uint16_t testing;
} in_regs_t;

extern int16_t out_regs[];
extern in_regs_t in_regs;

void sensor_mainloop();
void sensor_collect_data();



#endif /* SENSOR_H_ */
