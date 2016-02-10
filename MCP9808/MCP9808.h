#ifndef __MCP9807HEADER__
#define __MCP9807HEADER__

#include "i2c.h"

// Configuration Register
#define REG_CONF 0x01
// Upper Temperature Limit Register
#define REG_TUPPER 0x02
// Lower Temperature Limit Register
#define REG_TLOWER 0x03
// Critical Temperature Register
#define REG_TCRIT 0x04
// Temperature Register
#define REG_TEMP 0x05
// Manufacture ID Register
#define REG_MANID 0x06
// Device ID Register
#define REG_DEVID 0x07
// Resolution Register
#define REG_RES 0x08

#define REGMASK 0x0f

// Register Specific Definitions
// Config Register

#define CONFMASK 0x7ff


// Shutdown Mode bit
// 0 = Continuous conversion (power-up default)
// 1 = Shutdown (Low-Power mode)
#define CONF_SHDN (1<<8)
// 0 = Unlocked. TCRIT register can be written (power-up default)
// 1 = Locked. TCRIT register can not be written
#define CONF_CRITLOCK (1<<7)
// 0 = Unlocked; TUPPER and TLOWER registers can be written (power-up default)
// 1 = Locked; TUPPER and TLOWER registers can not be written
#define CONF_WINLOCK (1<<6)
// 0 = No effect (power-up default)
// 1 = Clear interrupt output; when read, this bit returns to ‘0’
#define CONF_INTCLEAR (1<<5)
// 0 = Alert output is not asserted by the device (power-up default)
// 1 = Alert output is asserted as a comparator/Interrupt or critical temperature output
#define CONF_ALERTSTAT (1<<4)
// Alert Output Control bit
// 0 = Disabled (power-up default)
// 1 = Enabled
#define CONF_ALERTCNT (1<<3)
// 0 = Alert output for TUPPER, TLOWER and TCRIT (power-up default)
// 1 = TA > TCRIT only (TUPPER and TLOWER temperature boundaries are disabled)
#define CONF_ALERTSEL (1<<2)
// Alert Output Polarity bit
// 0 = Active-low (power-up default; pull-up resistor required)
// 1 = Active-high
#define CONF_ALERTPOL (1<<1)
// Alert Output Mode bit
// 0 = Comparator output (power-up default)
// 1 = Interrupt output
#define CONF_ALERTMOD 1

// Capsulated Read / Writes
float ReadTemp(uint8_t deviceAddr);
uint8_t WriteConfig(uint8_t deviceAddr, uint16_t config);

// Raw Methods
void initializeI2C();
uint8_t WriteWord(uint8_t deviceAddr, uint8_t regPtr, uint16_t data);
uint16_t ReadWord( uint8_t deviceAddr, uint8_t regPtr );


#endif