#ifndef ISR_H_
#define ISR_H_

extern volatile uint16_t isr_flags;

typedef enum {
    ISR_SPIRIT_IRQ = BIT0, ISR_TIMER_DELAY = BIT1
} isr_flags_t;

uint16_t isr_flag_isset_with_clear(uint16_t flags);
void isr_timeout(uint16_t aclk_cycles);
void isr_disable_timer();
void isr_delay(uint16_t aclk_cycles);
void isr_delay_ms(uint16_t ms);

#endif /* ISR_H_ */
