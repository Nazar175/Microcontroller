#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CP_OFF);

unsigned int adc_value;

void main(void)
{
    TRISA = 0x01;
    TRISB = 0x00;
    TRISC = 0xFC;

    PORTB = 0x00;
    PORTC = 0x00;

    ADCON1 = 0x8E;

    ADCON0 = 0x81;

    while (1)
    {
        delay_us(40);

        ADCON0 |= 0x04;

        while (ADCON0 & 0x04)
        {
        }

        adc_value = ((unsigned int)ADRESH << 8) | ADRESL;

        PORTB = (unsigned char)(adc_value & 0xFF);

        PORTC = (unsigned char)((adc_value >> 8) & 0x03);
    }
}
