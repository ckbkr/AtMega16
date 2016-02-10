#include "STLM75.h"

void initializeI2C(){
	TWIInit();
}

uint16_t ReadTemp(uint8_t var){
	uint8_t addr = 0x90;
	uint8_t regPtr = 0x00;
	addr = addr & 0xFE;
	
	TWIStart();
	TWIWrite(addr);
	TWIWrite(regPtr);

	TWIStart();
	TWIWrite(addr | 0x01);
	uint8_t data1 = TWIReadACK();
	uint8_t data2 = TWIReadNACK();
	TWIStop();
	
	uint8_t val = (data1 & 0x7F )<<1;
	uint8_t add = (data2 & 0x80)>>7;
	uint8_t sign = (data1 & 0x80)>>7;
	uint16_t res = val | add;
	res = res | ((uint16_t)sign) << 8;
	return res;
}