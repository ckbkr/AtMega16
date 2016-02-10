// Source: http://www.embedds.com/programming-avr-i2c-interface/
#ifndef __I2CHEADER__
#define __I2CHEADER__

#include <util/twi.h>

#ifndef ERROR
#define ERROR 1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t u8data);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);
uint8_t TWIGetStatus(void);

#endif