#include <pic.h>

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);

void main(void)
{
    TRISA = 0b11111;
    TRISB = 0b00000000;

    PORTB = 0x00;

    while (1)
    {
        if (RA1 == 0)
        {
            PORTB = 0xAA;
        }
        else
        {
            PORTB = 0x99;
        }
    }
}
