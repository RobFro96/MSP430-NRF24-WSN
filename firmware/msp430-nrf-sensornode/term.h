#ifndef TERM_H_
#define TERM_H_

void term_init();
void term_putchar(char c);
void term_print(char *str);
void term_log_begin();
void term_end();
void term_log(char *str);
void term_debug_begin();
void term_debug(char *str);
void term_uint(uint32_t number, uint8_t digit_count);
void term_decimal(uint32_t number, uint8_t digit_count, uint8_t decimals);
void term_int(int32_t number, uint8_t digit_count);
void term_signed_decimal(int32_t number, uint8_t digit_count, uint8_t decimals);
void term_binary(uint16_t number, uint8_t digit_count);
void term_hex(uint32_t number, uint8_t digit_count);
void term_test_result(uint8_t result);
void term_wait_and_clear();

#endif /* TERM_H_ */
