#ifndef TERM_H_
#define TERM_H_

void term_init();
void term_putchar(char c);
void term_print(char *str);
void term_log_begin();
void term_end();
void term_log(char *str);
void term_uint(uint16_t number, uint8_t digit_count);
void term_int(int16_t number, uint8_t digit_count);
void term_binary(uint16_t number, uint8_t digit_count);
void term_hex(uint16_t number, uint8_t digit_count);
void term_test_result(uint8_t result);
void term_wait_and_clear();

#endif /* TERM_H_ */
