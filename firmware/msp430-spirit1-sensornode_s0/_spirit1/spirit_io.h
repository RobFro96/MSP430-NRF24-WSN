#ifndef SPIRIT1_SPIRIT_IO_H_
#define SPIRIT1_SPIRIT_IO_H_

#define SPIRIT_SPI_WRITE     0x00  //! Write mask for header byte
#define SPIRIT_SPI_READ      0x01  //! Read mask for header byte*/
#define SPIRIT_SPI_ADDRESS   0x00  //! Address mask for header byte*/
#define SPIRIT_SPI_COMMAND   0x80  //! Command mask for header byte*/

#include "pins.h"
#if SPIRIT_SPI == 'A'
#include "spia.h"
#elif SPIRIT_SPI == 'B'
#include "spib.h"
#endif

/**
 * Inline function for pulling the CS pin of the SPIRIT1 to GND
 */
inline void spirit_cs_select() {
	p_spirit_cs_l();
	p_delay_us(2);
}

/**
 * Inline function for releasing the CS pin of the SPIRIT1 to Vcc
 * Flag polling to ensure the SPI interaction is completed before releasing the pin
 */
inline void spirit_cs_disable() {
#if SPIRIT_SPI == 'A'
	spia_wait_cs_disable();
#elif SPIRIT_SPI == 'B'
	spib_wait_cs_disable();
#endif

	p_spirit_cs_h();
}

/**
 * Inline function for the transmit function of the used SPI module. This inline function makes it
 * easier to change SPI module for the whole SPIRIT1 implementation.
 *
 * @param data_out byte to be sent.
 * @return received byte
 */
inline char spirit_spi_transmit(char data_out) {
#if SPIRIT_SPI == 'A'
	return spia_transmit(data_out);
#elif SPIRIT_SPI == 'B'
	return spib_transmit(data_out);
#endif
}

/**
 * Inline function for the send function of the used SPI module. This inline function makes it
 * easier to change SPI module for the whole SPIRIT1 implementation.
 *
 * @param data_out byte to be sent.
 */
inline void spirit_spi_send(char data_out) {
#if SPIRIT_SPI == 'A'
	spia_send(data_out);
#elif SPIRIT_SPI == 'B'
	spib_send(data_out);
#endif
}

void spirit_write_register_burst(uint8_t addr, uint8_t len, const uint8_t *data);
void spirit_read_register_burst(uint8_t addr, uint8_t len, uint8_t *data);
void spirit_write_register(uint8_t addr, uint8_t val);
uint16_t spirit_command(uint8_t command);
uint16_t spirit_get_status();
uint8_t spirit_check_irq0(uint8_t mask);
uint8_t spirit_check_irq1(uint8_t mask);
uint8_t spirit_check_irq2(uint8_t mask);
uint8_t spirit_check_irq3(uint8_t mask);

#endif /* SPIRIT1_SPIRIT_IO_H_ */
