#ifndef _MS1100_H_
#define _MS1100_H_

#include <stdint.h>
#include "sIIC.h"

void MS1100A0_Write(uint8_t reg);
uint16_t MS1100A0_ReadByte(void);
uint8_t ADC_MS1100_Init(void);
uint8_t ADC_MS1100_GetValue(uint16_t* ADCValuePtr, uint8_t avgTimes);

#endif
