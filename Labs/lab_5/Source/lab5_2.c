#include <pic.h>

__CONFIG(0x3FF1);
#define _XTAL_FREQ 4000000UL

static void wait_about_20ms(void)
{
    TMR0 = 100;
    T0IF = 0;
    while (T0IF == 0) {
        ;
    }
}

void main(void)
{
    TRISB = 0b11111110;
    PORTB = 0x00;
    OPTION_REG = 0x06;
    INTCON = 0x00;
    T0IF = 0;

    while (1) {
        RB0 = 1;
        wait_about_20ms();

        RB0 = 0;
        for (unsigned char i = 0; i < 10; i++) {
            wait_about_20ms();
        }
    }
}
