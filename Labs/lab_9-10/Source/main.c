#include <htc.h>

#define _XTAL_FREQ 4000000

__CONFIG(0x3F3A);

unsigned int adc_value;

void adc_init(void)
{
    TRISA = 0b00000001;

    TRISB = 0x00;
    TRISC = 0x00;

    PORTB = 0x00;
    PORTC = 0x00;

    ADCON1 = 0x8E;

    ADCON0 = 0x81;

    __delay_ms(1);
}

unsigned int adc_read(void)
{
    __delay_us(40);

    GO = 1;

    while (GO)
    {
    }

    return (((unsigned int)ADRESH << 8) | ADRESL);
}

void main(void)
{
    adc_init();

    while (1)
    {
        adc_value = adc_read();

        PORTB = adc_value & 0xFF;

        PORTC = (adc_value >> 8) & 0x03;

        __delay_ms(50);
    }
}
