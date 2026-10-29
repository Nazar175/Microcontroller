#include <htc.h>

#define _XTAL_FREQ 4000000

void uart_write(char data)
{
    while (!TXIF)
    {
    }

    TXREG = data;
}

char uart_read(void)
{
    while (!RCIF)
    {
    }

    if (OERR)
    {
        CREN = 0;
        CREN = 1;
    }

    return RCREG;
}

void uart_init(void)
{
    TRISC6 = 0;
    TRISC7 = 1;

    SPBRG = 25;
    TXSTA = 0x24;
    RCSTA = 0x90;
}

void main(void)
{
    char symbol;

    uart_init();

    uart_write('\r');
    uart_write('\n');

    {
        const char name[] = "YOURNAME\r\n";
        unsigned char i = 0;

        while (name[i] != '\0')
        {
            uart_write(name[i]);
            i++;
        }
    }
    while (1)
    {
        symbol = uart_read();
        uart_write(symbol);
    }
}
