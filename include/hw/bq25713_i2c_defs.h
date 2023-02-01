// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BQ25713_I2C_DEFS_H_
#define	BQ25713_I2C_DEFS_H_


/* 
 * this file contains the i2c definitions for the bq25713 charger 
 * ic. I've only included the registers that I plan on using.
 * You can use the 'bit' struct to bit the individual bits, and
 * then byte to read all the bits as a whole uint8_t.
 */

namespace hermes {
namespace hw {

#define LOWBYTE(Data) (uint8_t)(0x00ff & Data)
#define HIGHBYTE(Data) (uint8_t)((0xff00 & Data)>>8)

constexpr uint8_t CHARGE_OPTION_0_HIGH = 0x01;
constexpr uint8_t CHARGE_OPTION_0_LOW = 0x00;
constexpr uint8_t CHARGE_OPTION_1_HIGH = 0x31;
constexpr uint8_t CHARGE_OPTION_1_LOW = 0x30;
constexpr uint8_t CHARGE_OPTION_2_HIGH = 0x33;
constexpr uint8_t CHARGE_OPTION_2_LOW = 0x32;
constexpr uint8_t CHARGE_OPTION_3_HIGH = 0x35;
constexpr uint8_t CHARGE_OPTION_3_LOW = 0x34;
constexpr uint8_t CHARGE_CURRENT_HIGH = 0x03;
constexpr uint8_t CHARGE_CURRENT_LOW = 0x02;
constexpr uint8_t CHARGER_STATUS_HIGH = 0x21;
constexpr uint8_t CHARGER_STATUS_LOW = 0x20;

union ChargeOption0_high
{
	struct
	{
		uint8_t LOW_PTM_RIPPLE : 1;
		uint8_t PWM_FREQ : 1;
		uint8_t EN_OOA : 1;
		uint8_t OTG_ON_CHRGOK : 1;
		uint8_t IDPM_AUTO_DISABLE : 1;
		uint8_t WDTMR_ADJ : 2; // 00 disable, 01 5s, 10 88s, 11 175s
		uint8_t EN_LWPWR : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption0_low
{
	struct
	{
		uint8_t CHRG_INHIBIT : 1;
		uint8_t EN_IDPM : 1;
		uint8_t EN_LDO : 1;
		uint8_t IBAT_GAIN : 1;
		uint8_t IADPT_GAIN : 1;
		uint8_t EN_LEARN : 1;
		uint8_t SYS_SHORT_DISABLE : 1;
		uint8_t RSVD : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption1_high
{
	struct
	{
		uint8_t PTM_PINSEL : 1;
		uint8_t PSYS_RATIO : 1;
		uint8_t RSNS_RSR : 1;
		uint8_t RSNS_RAC : 1;
		uint8_t EN_PSYS : 1;
		uint8_t EN_PROCHOT_LPWR : 2;
		uint8_t EN_IBAT : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption1_low
{
	struct
	{
		uint8_t AUTO_WAKEUP_EN : 1;
		uint8_t EN_SHIP_DCHG : 1;
		uint8_t EN_PTM : 1;
		uint8_t FORCE_LATCHOFF : 1;
		uint8_t CMP_DEG : 2;
		uint8_t CMP_POL : 1;
		uint8_t CMP_REF : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption2_high
{
	struct
	{
		uint8_t PKPWR_TMAX : 2;
		uint8_t PKPWR_RELAX_STAT : 1;
		uint8_t PKPWR_OVLD_STAT : 1;
		uint8_t EN_PKPWR_VSYS : 1;
		uint8_t EN_PKPWR_IDPM : 1;
		uint8_t PKPWR_TOVLD_DEG : 2;
	}bit;
	uint8_t byte;
};

union ChargeOption2_low
{
	struct
	{
		uint8_t BATOC_VTH : 1;
		uint8_t EN_BATOC : 1;
		uint8_t ACOC_VTH : 1;
		uint8_t EN_ACOC : 1;
		uint8_t ACX_OCP : 1;
		uint8_t Q2_OCP : 1;
		uint8_t EN_ICHG_IDCHG : 1;
		uint8_t EN_EXTILIM : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption3_high
{
	struct
	{
		uint8_t RSVD : 3;
		uint8_t EN_ICO_MODE : 1;
		uint8_t EN_OTG : 1;
		uint8_t REbit_VINDPM : 1;
		uint8_t REbit_REG : 1;
		uint8_t EN_HIZ : 1;
	}bit;
	uint8_t byte;
};

union ChargeOption3_low
{
	struct
	{
		uint8_t RSVD : 1;
		uint8_t EN_CON_VAP : 1;
		uint8_t OTG_VAP_MODE : 1;
		uint8_t IL_AVG : 2;
		uint8_t OTG_RANGE_LOW : 1;
		uint8_t BATFETOFF_HIZ : 1;
		uint8_t PSYS_OTG_IDCHG : 1;
	}bit;
	uint8_t byte;
};

union ChargeCurrent_high
{
	struct
	{
		uint8_t B2 : 1;
		uint8_t B3 : 1;
		uint8_t B4 : 1;
		uint8_t B5 : 1;
		uint8_t B6 : 1;
		uint8_t RSVD : 3;
	}bit;
	uint8_t byte;
};

union ChargeCurrent_low
{
	struct
	{
		uint8_t RSVD : 6;
		uint8_t B0_ : 1;
		uint8_t B1_ : 1;
	}bit;
	uint8_t byte;
};

union ChargerStatus_high
{
	struct
	{
		uint8_t IN_OTG : 1;
		uint8_t IN_PCHRG : 1;
		uint8_t IN_FCHRG : 1;
		uint8_t IN_IINDPM : 1;
		uint8_t IN_VINDPM : 1;
		uint8_t IN_VAP : 1;
		uint8_t ICO_DONE : 1;
		uint8_t AC_STAT : 1;
	}bit;
	uint8_t byte;
};

union ChargerStatus_low
{
	struct
	{
		uint8_t FAULT_OTG_UVP : 1;
		uint8_t FAULT_OTG_OVP : 1;
		uint8_t FAULT_LATCHOFF : 1;
		uint8_t FAULT_SYS_SHORT : 1;
		uint8_t SYSOVP_STAT : 1;
		uint8_t FAULT_ACOC : 1;
		uint8_t FAULT_BATOC : 1;
		uint8_t FAULT_ACOV : 1;
	}bit;
	uint8_t byte;
};

}
}

#endif // BQ25713_I2C_DEFS_H_h