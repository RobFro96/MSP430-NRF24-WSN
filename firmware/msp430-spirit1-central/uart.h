#ifndef UART_H_
#define UART_H_

#define UART_START_BYTE 0x55	//!> Value of start byte
#define UART_MESSAGE_EXTRA_LEN 2	//!> Total message length = EXTRA_LEN + DATA_LEN

#define UART_LEN 1 //!> Index of data length in message
#define UART_DATA 2 //!> Index of first data byte in message

extern uint8_t uart_rx_buffer[];

void uart_init();
void uart_send(uint8_t c);
#endif /* UART_H_ */
