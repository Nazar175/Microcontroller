#include <pic16f84a.h>

__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & CP_OFF);

unsigned char counter = 0;

void main(void)
{
    TRISA = 0x00;
    TRISB = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;

    while(1)
    {
        PORTB = counter;
        counter++;

        if(counter == 255)
        {
            counter = 0;
        }
    }
}