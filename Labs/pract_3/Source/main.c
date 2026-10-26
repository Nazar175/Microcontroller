#include <pic.h>

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);

#define TASK 1

void wait_overflow_no_prescaler(void)
{
    TMR0 = 0;
    T0IF = 0;

    while (T0IF == 0)
    {

    }
}

void wait_50ms(void)
{
    OPTION_REG = 0b00000111;
    TMR0 = 61;
    T0IF = 0;

    while (T0IF == 0)
    {

    }

    T0IF = 0;
}

void main(void)
{
    TRISB0 = 0;
    RB0 = 0;

#if TASK == 1

    OPTION_REG = 0b00001000;
    TMR0 = 56;
    T0IF = 0;

    while (T0IF == 0)
    {

    }

    RB0 = 1;

    while (1)
    {

    }

#elif TASK == 2

    OPTION_REG = 0b00001000;

    while (1)
    {
        unsigned int i;

        RB0 = 1;

        for (i = 0; i < 78; i++)
        {
            wait_overflow_no_prescaler();
        }

        RB0 = 0;

        for (i = 0; i < 781; i++)
        {
            wait_overflow_no_prescaler();
        }
    }

#elif TASK == 3

    while (1)
    {
        wait_50ms();
        RB0 = !RB0;
    }

#elif TASK == 4

    while (1)
    {
        unsigned char i;

        for (i = 0; i < 10; i++)
        {
            wait_50ms();
        }

        RB0 = !RB0;
    }

#endif
}
