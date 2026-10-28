#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CP_OFF);

void uart_send_char(unsigned char data)
{
    while (!TXIF)
    {
    }

    TXREG = data;
}

unsigned char uart_read_char(void)
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

void uart_send_text(const char *text)
{
    while (*text != '\0')
    {
        uart_send_char(*text);
        text++;
    }
}

void main(void)
{
    unsigned char received_char;

    TRISC = 0b10000000;

    SPBRG = 25;

    TXSTA = 0b00100100;

    RCSTA = 0b10010000;

    uart_send_text("My name is YOUR_NAME\r\n");
    uart_send_text("UART is ready.\r\n");
    uart_send_text("Type text: ");

    while (1)
    {
        received_char = uart_read_char();

        uart_send_char(received_char);
    }
}
