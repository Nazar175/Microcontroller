#include <pic.h>
#include <htc.h>
#include "LCD.h"

#define _XTAL_FREQ 4000000

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON &
       LVP_OFF & CP_OFF);

void main(void)
{
    TRISD = 0x00;
    TRISC = 0b11111000;

    PORTD = 0x00;
    PORTC = 0x00;

    lcd_init();

    lcd_goto(0x80);
    lcd_string("Nazar");

    lcd_goto(0xC0);
    lcd_string("Huchko");

    while (1)
    {
    }
}
