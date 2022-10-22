#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__


void ShiftRegister_Setup (volatile uint8_t * SetPort, uint8_t SetDataPin, uint8_t SetEnablePin, uint8_t SetLatchCLKPin, uint8_t SetShiftCLKPin, uint8_t SetResetPin);

void ShiftRegister_EnableOutput(void);
void ShiftRegister_DisableOutput(void);

void ShiftRegister_SendByte(uint8_t Data);



#endif