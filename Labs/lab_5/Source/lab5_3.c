#include <pic.h>

__CONFIG(0x3FF1);
#define _XTAL_FREQ 4000000UL

static void wait_about_5ms(void)
{
    TMR0 = 100;
    T0IF = 0;
    while (T0IF == 0) {
        ;
    }
}

void main(void)
{
    OPTION_REG = 0x04;
    INTCON = 0x00;
    TMR0 = 100;
    T0IF = 0;

    while (1) {
        unsigned char i;
        for (i = 0; i < 10; i++) {
            wait_about_5ms();
        }
    }
}
