#include <pic.h>
CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);
#define _XTAL_FREQ 4000000
#define BIRTH_DAY 3
const unsigned char SEGMENTS[10] =
{
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};
volatile unsigned char scan_digit = 0;
void interrupt timer0_interrupt(void)
{
    unsigned char value;
    if (T0IE && T0IF)
    {
        TMR0 = 156;
        T0IF = 0;
        RA2 = 0;
        RA3 = 0;
        if (scan_digit == 0)
        {
            value = BIRTH_DAY / 10;
            PORTB = SEGMENTS[value];
            RA2 = 1;
            scan_digit = 1;
        }
        else
        {
            value = BIRTH_DAY % 10;
            PORTB = SEGMENTS[value];
            RA3 = 1;
            scan_digit = 0;
        }
    }
}
void main(void)
{
    TRISA = 0b11110011;
    TRISB = 0b00000000;
    PORTA = 0;
    PORTB = 0;
    OPTION_REG = 0b10000101;
    TMR0 = 156;
    T0IF = 0;
    T0IE = 1;
    GIE = 1;
    while (1)
    {
    }
}