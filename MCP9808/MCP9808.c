#include "MCP9808.h"

#define RETRIES 50

void initializeI2C(){
	TWIInit();
}

uint8_t WriteWord(uint8_t deviceAddr, uint8_t regPtr, uint16_t data)
{
	uint8_t datal = *((uint8_t*)&data);
	uint8_t datah = *(((uint8_t*)&data)+1); // Step a bit forward, just like using an array
	uint8_t reply;
	TWIStart();
	if (TWIGetStatus() != 0x08)
	return ERROR;
	
	// Write Device Addr
	// Device addr is built according to the scheme:
	// 0011,A2,A1,A0,RW
	// 0011 = 0x30
	// A2,A1,A0 Mask = 0x0E
	uint8_t addr = 0;
	addr = 0x30 | ( deviceAddr & 0x0E );
	// Clear RW Bit for Writing, this is actually not needed. Just in case
	addr = addr & 0xFE;
	TWIWrite(addr);
	if ((reply = TWIGetStatus()) != 0x18) // READ an ACK
	return ERROR;
	
	TWIWrite(regPtr);
	if ((reply = TWIGetStatus()) != 0x18) // READ an ACK
	return ERROR;
	
	TWIWrite(datah);
	if ((reply = TWIGetStatus()) != 0x18) // READ an ACK
	return ERROR;
	
	TWIWrite(datal);
	if ((reply = TWIGetStatus()) != 0x18) // READ an ACK
	return ERROR;

	TWIStop();
	return SUCCESS;
}

uint16_t ReadWord( uint8_t deviceAddr, uint8_t regPtr ){
	uint16_t data = 0;
	uint8_t * datal = ((uint8_t*)&data);
	uint8_t * datah = (((uint8_t*)&data)+1); // Step a bit forward, just like using an array
	uint8_t reply;
	int i = 0;
	
	// Write Device Addr
	// Device addr is built according to the scheme:
	// 0011,A2,A1,A0,RW
	// 0011 = 0x30
	// A2,A1,A0 Mask = 0x0E
	uint8_t addr = 0;
	addr = 0x90 | ( deviceAddr & 0x0E );
	// Set RW Bit for Reading
	// addr = addr | 0x0C;
	// addr = addr | 0x80;
	
	
	// Write Device Addr, WRITING
	i = 0;
	for( ; i < RETRIES; i++ ){
		TWIStart();
		if ((reply = TWIGetStatus()) != TW_START){
			TWIStop();
			return ERROR;
		}
		TWIWrite(addr & 0xFE);
		reply = TWIGetStatus();
		if (reply == TW_MT_SLA_ACK){ // READ an ACK
			break;
			}else if (reply == TW_MT_SLA_NACK || reply == TW_MT_ARB_LOST ){
			continue;
			}else{
			TWIStop();
			return ERROR;
		}
		if( i == RETRIES -1 ){
			TWIStop();
			return ERROR;
		}
	}
	
	// Write Register Addr
	TWIWrite(regPtr);
	reply = TWIGetStatus();
	if (reply != TW_MT_DATA_ACK){ // READ an ACK
		TWIStop();
		return ERROR;
	}



	TWIStart();
	TWIWrite(addr | 0x01);
	
	//*datah = TWIReadACK();
	uint8_t data1 = TWIReadACK();
	if ((reply = TWIGetStatus()) != TW_MR_DATA_ACK){ // READ an ACK
		TWIStop();
		return ERROR;
	}
	
	//*datal = TWIReadNACK();
	uint8_t data2 = TWIReadNACK();
	if ((reply = TWIGetStatus()) != TW_MR_DATA_NACK){ // READ an ACK
		TWIStop();
		return ERROR;
	}

	TWIStop();
	return data;
}

uint8_t WriteConfig(uint8_t deviceAddr, uint16_t config){
	return WriteWord(deviceAddr, REG_CONF, config);
}

float ReadTemp(uint8_t deviceAddr){
	uint16_t data = ReadWord(deviceAddr,REG_TEMP);
	if( data == ERROR ){
		return 0.0;
	}
	uint8_t datal = *((uint8_t*)&data);
	uint8_t datah = *(((uint8_t*)&data)+1); // Step a bit forward, just like using an array
	float Temperature = 0;
	if ((datah & 0x80) == 0x80){ //TA ³ TCRIT
	}
	if ((datah & 0x40) == 0x40){ //TA > TUPPER
	}
	if ((datah & 0x20) == 0x20){ //TA < TLOWER
	}
	datah = datah & 0x1F; //Clear flag bits
	if ((datah & 0x10) == 0x10){ //TA < 0°C
		datah = datah & 0x0F; //Clear SIGN
		Temperature = 256 - (datah * 16.0 + datal / 16.0);
		}else{
		//TA ³ 0°C
		Temperature = (datah * 16.0 + datal / 16.0);
		//Temperature = Ambient Temperature (°C)
	}
	
	return Temperature;
}