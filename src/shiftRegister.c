#include<avr/io.h>
#include<stdint-gcc.h>
#include<util/delay.h>  // sets up the use of _delay_ms  and _delay_us

volatile uint8_t *ControlPort;

uint8_t DataPin;
uint8_t EnablePin;
uint8_t LatchCLKPin;
uint8_t ShiftCLKPin;
uint8_t ResetPin;


void ShiftRegister_Setup (volatile uint8_t *SetPort, uint8_t SetDataPin, uint8_t SetEnablePin, uint8_t SetLatchCLKPin, uint8_t SetShiftCLKPin, uint8_t SetResetPin) {
    ControlPort = SetPort;
    DataPin = SetDataPin;
    EnablePin = SetEnablePin;
    LatchCLKPin = SetLatchCLKPin;
    ShiftCLKPin = SetShiftCLKPin;
    ResetPin = SetResetPin;
}

void ClockPulse(void) {
    (*ControlPort) |= _BV(ShiftCLKPin);
    _delay_ms(1);
    (*ControlPort) &= ~_BV(ShiftCLKPin);
    _delay_ms(1);
}

void LatchPulse(void) {
    (*ControlPort) |= _BV(LatchCLKPin);
    _delay_ms(1);
    (*ControlPort) &= ~_BV(LatchCLKPin);
    _delay_ms(1);
}

void SetBit(uint8_t Value) {
    if(Value)
        (*ControlPort) |= _BV(DataPin);
    else
        (*ControlPort) &= ~_BV(DataPin);
}

void ShiftRegister_SendByte(uint8_t Data) {
    for (int i = 0; i < 8; i++) {
        SetBit(Data & 0b10000000);
        ClockPulse();
        Data <<= 1;
    }
    LatchPulse();
}

void ShiftRegister_EnableOutput(void) {
    (*ControlPort) &= ~_BV(EnablePin);
}

void ShiftRegister_DisableOutput(void) {
    (*ControlPort) |= _BV(EnablePin);
}
