#ifndef UART_H_
#define UART_H_

#define UART_START_BYTE 0x55	//!> Value of start byte
#define UART_MESSAGE_EXTRA_LEN 2	//!> Total message length = EXTRA_LEN + DATA_LEN

#define UART_LEN 1 //!> Index of data length in message
#define UART_DATA 2 //!> Index of first data byte in message

extern uint8_t uart_rx_buffer[];

void uart_init();
void uart_send(uint8_t c);
void uart_print(char *str);
void uart_send_hex16(int16_t number);
void uart_convert_buffer_to_int16(int16_t *out);

#endif /* UART_H_ */
