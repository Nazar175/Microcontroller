#include <pic.h>

__CONFIG(0x3FF1);
#define _XTAL_FREQ 4000000UL

void main(void)
{
    OPTION_REG = 0xC8;
    INTCON = 0x00;
    TMR0 = 54;
    T0IF = 0;

    while (T0IF == 0) {
        ;
    }

    while (1) {
        ;
    }
}
