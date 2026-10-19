#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

#define DUTY_CYCLE 10

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);

void main(void)
{
    TRISB4 = 0;
    RB4 = 0;

    while (1)
    {
#if DUTY_CYCLE == 10
        RB4 = 1;
        delay_us(20);

        RB4 = 0;
        delay_us(180);
#else
        RB4 = 1;
        delay_us(180);

        RB4 = 0;
        delay_us(20);
#endif
    }
}
