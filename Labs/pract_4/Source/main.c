
#include <pic.h>

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);

#define _XTAL_FREQ 4000000

volatile unsigned char number = 0;
volatile unsigned char digit = 0;
volatile unsigned char tick_count = 0;

const unsigned char segment_code[10] =
{
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void interrupt timer_interrupt(void)
{
    unsigned char value;

    if (T0IE && T0IF)
    {
        TMR0 = 156;

        T0IF = 0;

        RA0 = 0;
        RA2 = 0;

        if (digit == 0)
        {
            value = number / 10;
            PORTB = segment_code[value];
            RA0 = 1;
            digit = 1;
        }
        else
        {
            value = number % 10;
            PORTB = segment_code[value];
            RA2 = 1;
            digit = 0;
        }

        tick_count++;

        if (tick_count >= 78)
        {
            tick_count = 0;

            RA1 = !RA1;

            number++;

            if (number >= 100)
            {
                number = 0;
            }
        }
    }
}

void main(void)
{
    TRISA = 0b11111000;
    TRISB = 0b00000000;

    PORTA = 0;
    PORTB = 0;

    OPTION_REG = 0b11000101;

    TMR0 = 156;

    T0IF = 0;

    T0IE = 1;

    GIE = 1;

    while (1)
    {

    }
}
