#ifndef _CS1237_H_
#define _CS1237_H_

#include <stdint.h>

uint8_t CS1237_Init(void);
uint8_t CS1237_GetValue(uint16_t* currentValuePtr);
uint8_t CS1237_Test(void);

#endif
