#include <avr/io.h>
#include<util/delay.h>
#include "shiftRegister.h"


uint8_t ConvertToDice(uint8_t);
void Flasher(void);
void ShowDice(void);

void InitSystem(void) {
    //Enable Counter Clock
    TCCR0B |= 1 << CS02;
    //Set All PortB pins to output
    DDRB = _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3);
    PORTB |= _BV(PB4); //Set PB4 Input & Pull Up

    ShiftRegister_Setup(&PORTB, PB0, PB1, PB2, PB3, 8);
}


int main(void) {
    InitSystem();
    
    uint32_t Counter = 0;
    while(1) {
        if(~PINB & _BV(PINB4)) {
            Counter = 100000;
            ShiftRegister_EnableOutput();
        }
        
        if(Counter == 0) {
            Flasher();
            ShowDice();
            _delay_ms(20);
        } else {
            Counter--;
        }
    }
}

void Flasher(void) {
    static uint8_t Counter = 0;
    Counter++;
    if(Counter == 1)
        ShiftRegister_EnableOutput();
    else if(Counter == 5)
        ShiftRegister_DisableOutput();
    else if(Counter == 10)
        Counter = 0;
}

uint8_t ConvertToDice(uint8_t Number) {
    uint8_t Result = 0;
    switch (Number)
    {
    case 1:
        Result = 0b0001;
        break;
    case 2:
        Result = 0b0010;
        break;
    case 3:
        Result = 0b0011;
        break;
    case 4:
        Result = 0b1010;
        break;
    case 5:
        Result = 0b1011;
        break;
    case 6:
        Result = 0b1110;
        break;
    }
    return Result;
}

void ShowDice(void)  {
    uint8_t FirstDice = ConvertToDice((TCNT0 % 6) + 1);
    uint8_t SecondDice = ConvertToDice(((TCNT0 / 10) % 6) + 1);
    ShiftRegister_SendByte((SecondDice << 4) | FirstDice);
}