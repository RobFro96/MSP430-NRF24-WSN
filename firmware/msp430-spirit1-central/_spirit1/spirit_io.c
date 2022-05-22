/**
 * MSP430 and NRF24L01 based WSN
 * @file spirit_io.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Functions to read and write commands and registers over SPI.
 */

#include <msp430.h>
#include <stdint.h>

#include "spirit.h"
#include "spirit_io.h"

/**
 * Writing multiple registers of the SPIRIT1 module. The 16 status bits are ignored.
 *
 * @param addr address of the first register
 * @param len number of sequencial register writes
 * @param data pointer to register values
 */
void spirit_write_register_burst(uint8_t addr, uint8_t len, const uint8_t *data) {
	spirit_cs_select();

	spirit_spi_send(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_WRITE);
	spirit_spi_send(addr);

	while (len > 0) {
		spirit_spi_send(*data);
		data++;
		len--;
	}

	spirit_cs_disable();
}

/**
 * Reading multiple registers of the SPIRIT1 module. The 16 status bits are ignored.
 *
 * @param addr address of the first register
 * @param len number of sequencial register reads
 * @param data pointer to register values
 */
void spirit_read_register_burst(uint8_t addr, uint8_t len, uint8_t *data) {
	spirit_cs_select();

	spirit_spi_send(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_READ);
	spirit_spi_send(addr);

	while (len > 0) {
		*data = spirit_spi_transmit(0);
		data++;
		len--;
	}

	spirit_cs_disable();
}

/**
 * Writing a single register.
 *
 * @param addr address of the register
 * @param val value of the register
 */
void spirit_write_register(uint8_t addr, uint8_t val) {
	spirit_cs_select();

	spirit_spi_send(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_WRITE);
	spirit_spi_send(addr);
	spirit_spi_send(val);

	spirit_cs_disable();
}

/**
 * Sending a command to the SPIRIT1 module
 *
 * @param command command to be sent.
 * @return status bits
 */
uint16_t spirit_command(uint8_t command) {
	spirit_cs_select();

	uint16_t status = spirit_spi_transmit(SPIRIT_SPI_COMMAND);
	status = status << 8;
	status += spirit_spi_transmit(command);

	spirit_cs_disable();

	return status;
}

/**
 * Getting the status bits of the SPIRIT1 module.
 * Status bits are obtained by starting a read request on register 0. But no data is read.
 *
 * @return status bits
 */
uint16_t spirit_get_status() {
	spirit_cs_select();

	uint16_t status = spirit_spi_transmit(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_READ);
	status = status << 8;
	status += spirit_spi_transmit(0);

	spirit_cs_disable();
	return status;
}

/**
 * Check the IRQ_STATUS0 register for pending interrupts. Returns 0, if none of the selected
 * interrupts is pending.
 *
 * @param mask selected interrupts of IRQ_STATUS0 register
 * @return 0 if no interrupt is pending
 */
uint8_t spirit_check_irq0(uint8_t mask) {
	uint8_t irq_status0;
	spirit_read_register_burst(IRQ_STATUS0_BASE, 1, &irq_status0);

	return irq_status0 & mask;
}

/**
 * Check the IRQ_STATUS1 register for pending interrupts. Returns 0, if none of the selected
 * interrupts is pending.
 *
 * @param mask selected interrupts of IRQ_STATUS1 register
 * @return 0 if no interrupt is pending
 */
uint8_t spirit_check_irq1(uint8_t mask) {
	uint8_t irq_status1;
	spirit_read_register_burst(IRQ_STATUS1_BASE, 1, &irq_status1);

	return irq_status1 & mask;
}

/**
 * Check the IRQ_STATUS2 register for pending interrupts. Returns 0, if none of the selected
 * interrupts is pending.
 *
 * @param mask selected interrupts of IRQ_STATUS2 register
 * @return 0 if no interrupt is pending
 */
uint8_t spirit_check_irq2(uint8_t mask) {
	uint8_t irq_status2;
	spirit_read_register_burst(IRQ_STATUS2_BASE, 1, &irq_status2);

	return irq_status2 & mask;
}

/**
 * Check the IRQ_STATUS3 register for pending interrupts. Returns 0, if none of the selected
 * interrupts is pending.
 *
 * @param mask selected interrupts of IRQ_STATUS3 register
 * @return 0 if no interrupt is pending
 */
uint8_t spirit_check_irq3(uint8_t mask) {
	uint8_t irq_status3;
	spirit_read_register_burst(IRQ_STATUS3_BASE, 1, &irq_status3);

	return irq_status3 & mask;
}

