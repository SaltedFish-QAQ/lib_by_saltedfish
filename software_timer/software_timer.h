#ifndef _SOFTWART_TIMER_H_
#define _SOFTWART_TIMER_H_

#include <stdint.h>
#include <stddef.h>

#define CONTORLTIMER    0x00
#define MODBUSTIMER     0x01

typedef struct software_timer_e
{
    uint8_t timerID;
    uint8_t timerUseFlag;
    uint16_t timerPreviousMS;
    uint16_t timerCurrentMS;
}software_timer_t;

uint8_t SoftwareTimer_Init(void);
uint8_t SoftwareTimer_Register(uint8_t ID);
uint8_t SoftwareTimer_GetCurrentMs(uint8_t ID, uint16_t* ms);
uint8_t SoftwareTimer_UpdatePreviousMS(uint8_t ID);
uint8_t SoftwareTimer_ClrCurrentMS(uint8_t ID);

#endif
