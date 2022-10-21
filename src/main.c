#include <avr/io.h>

int main(void) {
    
    DDRB  |= (1 << PORT0) | (1 << PORT1) | (1 << PORT2) | (1 << PORT3) | (1 << PORT4);
    PORTB |= (1 << PORT0) | (1 << PORT1) | (1 << PORT2) | (1 << PORT3) | (1 << PORT4);
    
    while(1);
}
