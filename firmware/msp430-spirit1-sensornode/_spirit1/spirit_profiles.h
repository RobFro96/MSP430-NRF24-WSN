#ifndef SPIRIT_PROFILES_H_
#define SPIRIT_PROFILES_H_

// Basic 868 MHz Settings
#define SPIRIT1_868M_REFDIV (1 << 7)
#define SPIRIT1_868M_VCO_LH_SEL ((0 << 2) + (1 << 1))
#define SPIRIT1_868M_IF_OFFSET_ANA 0x36
#define SPIRIT1_868M_IF_OFFSET_DIG 0xAC
#define SPIRIT1_868M_WCP 0
#define SPIRIT1_868M_SYNT 27304919  // 868e6/50e6 * 6 * 2^18
#define SPIRIT1_868M_SYNT3 0x0D // int(SYNT / 2**21) & 0b00011111
#define SPIRIT1_868M_SYNT2 0x05 // int(SYNT / 2**13) & 0xff
#define SPIRIT1_868M_SYNT1 0x1E // int(SYNT / 2**5) & 0xff
#define SPIRIT1_868M_SYNT0 0x17 // int(SYNT) & 0b00011111
#define SPIRIT1_868M_BS 1
#define SPIRIT1_868M_AFC_FREEZE_ON_SYNC 1
#define SPIRIT1_868M_AGC_THRESHOLD_HIGH 0x6
#define SPIRIT1_868M_AGC_THRESHOLD_LOW 0x2
#define SPIRIT1_868M_CS_BLANKING 1

// FSK Settings
#define SPIRIT_FSK_38K4_CHSPACE 66
#define SPIRIT_FSK_38K4_DATARATE_M 0x93
#define SPIRIT_FSK_38K4_BT_SEL 1
#define SPIRIT_FSK_38K4_MOD_TYPE 1
#define SPIRIT_FSK_38K4_DATARATE_E  0xA
#define SPIRIT_FSK_38K4_FDEV_E 4
#define SPIRIT_FSK_38K4_FDEV_M 5
#define SPIRIT_FSK_38K4_CHFLT_M 1
#define SPIRIT_FSK_38K4_CHFLT_E 3

#define SPIRIT_FSK_250K_CHSPACE 66
#define SPIRIT_FSK_250K_DATARATE_M 72
#define SPIRIT_FSK_250K_BT_SEL 1
#define SPIRIT_FSK_250K_MOD_TYPE 1
#define SPIRIT_FSK_250K_DATARATE_E  13
#define SPIRIT_FSK_250K_FDEV_E 7
#define SPIRIT_FSK_250K_FDEV_M 2
#define SPIRIT_FSK_250K_CHFLT_M 7
#define SPIRIT_FSK_250K_CHFLT_E 0

typedef struct {
    uint8_t reg;
    uint8_t val;
} spirit_reg_val_pair_t;

extern const spirit_reg_val_pair_t spirit_profile_basic_868mhz[];
extern const spirit_reg_val_pair_t spirit_profile_basic_433mhz[];
extern const spirit_reg_val_pair_t spirit_profile_fsk_38k4[];
extern const spirit_reg_val_pair_t spirit_profile_fsk_250k[];
extern const spirit_reg_val_pair_t spirit_profile_pck[];

void spirit_profile_apply(const spirit_reg_val_pair_t *profile);

#endif /* SPIRIT_PROFILES_H_ */
