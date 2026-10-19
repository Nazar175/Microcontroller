#include <htc.h>

void main(void)
{
    TRISA = 0x03;

    TRISB = 0x00;

    PORTB = 0x00;

    while (1)
    {
        unsigned char buttons = PORTA & 0x03;
        PORTB = buttons;
    }
}
