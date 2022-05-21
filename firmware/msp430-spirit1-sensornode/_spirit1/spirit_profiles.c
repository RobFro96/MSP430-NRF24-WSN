/**
 * MSP430 and NRF24L01 based WSN
 * @file spirit_profiles.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Defining the default profiles. Custom profiles can be added to the user code.
 * Profiles are loaded on the SPIRIT1 by the function spirit_apply_profile()
 */

#include <stdint.h>

#include "config.h"
#include "spirit.h"
#include "spirit_profiles.h"

const spirit_reg_val_pair_t spirit_profile_basic_868mhz[] = {
// GPIO0 Settings
		GPIO0_CONF_BASE, CONF_GPIO_OUT_nIRQ + CONF_GPIO_MODE_DIG_OUTL, // GPIO0 as nIRQ signal
		IRQ_MASK0_BASE, IRQ_MASK0_RX_DATA_READY + IRQ_MASK0_TX_DATA_SENT, // IRQs on GPIO0
// Synthesizer Errata Fixes
		SYNTH_CONFIG0_BASE, 0xA0, // SEL_TSPLIT
		VCO_CONFIG_BASE, 0x25, // increase VCO current
		0xA3, 0b00110101, // DEM_CONFIG -> 0
// RF Settings
		SYNTH_CONFIG1_BASE, 0b01011001 + SPIRIT1_868M_REFDIV + SPIRIT1_868M_VCO_LH_SEL, // REF DIV, VCO L/H
		IF_OFFSET_ANA_BASE, SPIRIT1_868M_IF_OFFSET_ANA, // Intermediate Frequency
		IF_OFFSET_DIG_BASE, SPIRIT1_868M_IF_OFFSET_DIG,
		SYNT3_BASE, (SPIRIT1_868M_WCP << 5) + SPIRIT1_868M_SYNT3, // Synthesiser Frequency
		SYNT2_BASE, SPIRIT1_868M_SYNT2,
		SYNT1_BASE, SPIRIT1_868M_SYNT1,
		SYNT0_BASE, (SPIRIT1_868M_SYNT0 << 3) + SPIRIT1_868M_BS, // Band Select
		AGCCTRL1_BASE, (SPIRIT1_868M_AGC_THRESHOLD_HIGH << 4) + SPIRIT1_868M_AGC_THRESHOLD_LOW, // Automatic Gain Control
		ANT_SELECT_CONF_BASE, 0b00000101 + (SPIRIT1_868M_CS_BLANKING << 4),
		AFC2_BASE, 0b01001000 + (SPIRIT1_868M_AFC_FREEZE_ON_SYNC << 7), // Automatic Frequency Controll Freeze
		PROTOCOL0_BASE, BIT3 + BIT1, // NACK + Persistant RX
		0xFF, 0xFF
};

const spirit_reg_val_pair_t spirit_profile_basic_433mhz[] = {
// GPIO0 Settings
		GPIO0_CONF_BASE, CONF_GPIO_OUT_nIRQ + CONF_GPIO_MODE_DIG_OUTL, // GPIO0 as nIRQ signal
		IRQ_MASK0_BASE, IRQ_MASK0_RX_DATA_READY + IRQ_MASK0_TX_DATA_SENT, // IRQs on GPIO0
// Synthesizer Errata Fixes
		SYNTH_CONFIG0_BASE, 0xA0, // SEL_TSPLIT
		VCO_CONFIG_BASE, 0x25, // increase VCO current
		0xA3, 0b00110101, // DEM_CONFIG -> 0
// RF Settings
		SYNTH_CONFIG1_BASE, 0b01011001 + SPIRIT1_868M_REFDIV + SPIRIT1_868M_VCO_LH_SEL, // REF DIV, VCO L/H
		IF_OFFSET_ANA_BASE, SPIRIT1_868M_IF_OFFSET_ANA, // Intermediate Frequency
		IF_OFFSET_DIG_BASE, SPIRIT1_868M_IF_OFFSET_DIG,
		SYNT3_BASE, (SPIRIT1_868M_WCP << 5) + SPIRIT1_868M_SYNT3, // Synthesiser Frequency
		SYNT2_BASE, SPIRIT1_868M_SYNT2,
		SYNT1_BASE, SPIRIT1_868M_SYNT1,
		SYNT0_BASE, (SPIRIT1_868M_SYNT0 << 3) + 3, // Band Select
		AGCCTRL1_BASE, (SPIRIT1_868M_AGC_THRESHOLD_HIGH << 4) + SPIRIT1_868M_AGC_THRESHOLD_LOW, // Automatic Gain Control
		ANT_SELECT_CONF_BASE, 0b00000101 + (SPIRIT1_868M_CS_BLANKING << 4),
		AFC2_BASE, 0b01001000 + (SPIRIT1_868M_AFC_FREEZE_ON_SYNC << 7), // Automatic Frequency Controll Freeze
		PROTOCOL0_BASE, BIT3 + BIT1, // NACK + Persistant RX
		PA_POWER8_BASE, 7, // Reduce PA-Power (10 dBm max.)
		0xFF, 0xFF
};

const spirit_reg_val_pair_t spirit_profile_fsk_38k4[] = {
CHSPACE_BASE, SPIRIT_FSK_38K4_CHSPACE, // Channel Spacing
		MOD1_BASE, SPIRIT_FSK_38K4_DATARATE_M, // Datarate
		MOD0_BASE, (SPIRIT_FSK_38K4_BT_SEL << 6) + (SPIRIT_FSK_38K4_MOD_TYPE << 4) // Modulation
				+ SPIRIT_FSK_38K4_DATARATE_E, // Daterate
		FDEV0_BASE, (SPIRIT_FSK_38K4_FDEV_E << 4) + SPIRIT_FSK_38K4_FDEV_M, // Frequency Deviation
		CHFLT_BASE, (SPIRIT_FSK_38K4_CHFLT_M << 4) + SPIRIT_FSK_38K4_CHFLT_E, // Input Filter
		0xFF, 0xFF
};

const spirit_reg_val_pair_t spirit_profile_fsk_250k[] = {
CHSPACE_BASE, SPIRIT_FSK_250K_CHSPACE, // Channel Spacing
		MOD1_BASE, SPIRIT_FSK_250K_DATARATE_M, // Datarate
		MOD0_BASE, (SPIRIT_FSK_250K_BT_SEL << 6) + (SPIRIT_FSK_250K_MOD_TYPE << 4) // Modulation
				+ SPIRIT_FSK_250K_DATARATE_E, // Daterate
		FDEV0_BASE, (SPIRIT_FSK_250K_FDEV_E << 4) + SPIRIT_FSK_250K_FDEV_M, // Frequency Deviation
		CHFLT_BASE, (SPIRIT_FSK_250K_CHFLT_M << 4) + SPIRIT_FSK_250K_CHFLT_E, // Input Filter
		0xFF, 0xFF
};

// Default Package
const spirit_reg_val_pair_t spirit_profile_pck[] = {
CHNUM_BASE, SPIRIT_PCK_CHNUM, // Channel number
		PCKTCTRL4_BASE, (SPIRIT_PCK_ADDR_LEN << 3) + SPIRIT_PCK_CTRL_LEN, // ADDR and CTRL Length
		PCKTCTRL2_BASE, (SPIRIT_PCK_PREAMBLE_LEN << 3) + (SPIRIT_PCK_SYNC_LEN << 1)
				+ BIT0, // Preamble, Sync, Length Byte, Variable Length
		PCKTCTRL1_BASE, (SPIRIT_PCK_CRC_MODE << 5) + (SPIRIT_PCK_WHIT_EN << 4), // CRC, Whitening
		PCKT_FLT_OPTIONS_BASE, (SPIRIT_PCK_CONTROL_FILTERING << 5)
				+ (SPIRIT_PCK_ADDRESS_FILTERING << 3)
				+ (SPIRIT_PCK_MULTICAST_FILTERING << 2) + (SPIRIT_PCK_BROADCAST_FILTERING << 1)
				+ SPIRIT_PCK_CRC_FILTERING, // CRC Check
		PROTOCOL1_BASE, BIT0, // Automatic Package Filtering
		SYNC1_BASE, SPIRIT_PCK_SYNC1,
		SYNC2_BASE, SPIRIT_PCK_SYNC2,
		SYNC3_BASE, SPIRIT_PCK_SYNC3,
		SYNC4_BASE, SPIRIT_PCK_SYNC4,
		0xFF, 0xFF
};

/**
 * Sending a profile to the SPIRIT1 module. A profile is a list of spirit_reg_val_pair_t.
 * The end of the profile list is detected if both register and value are 0xff.
 *
 * @param profile profile to be applied.
 */
void spirit_profile_apply(const spirit_reg_val_pair_t *profile) {
	while (profile->reg != 0xff && profile->val != 0xff) {
		spirit_write_register(profile->reg, profile->val);
		profile++;
	}
}
