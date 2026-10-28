#include <htc.h>

__CONFIG(XT & WDTDIS & PWRTEN & BORDIS & LVPDIS & CPDIS);

#define _XTAL_FREQ 4000000

#define LCD_RS RC0
#define LCD_RW RC1
#define LCD_E  RC2

void lcd_pulse(void)
{
    LCD_E = 1;
    __delay_us(2);
    LCD_E = 0;
    __delay_us(50);
}

void lcd_com(unsigned char command)
{
    LCD_RS = 0;
    LCD_RW = 0;
    PORTD = command;
    lcd_pulse();

    if (command == 0x01 || command == 0x02)
        __delay_ms(2);
}

void lcd_data(unsigned char data)
{
    LCD_RS = 1;
    LCD_RW = 0;
    PORTD = data;
    lcd_pulse();
}

void lcd_init(void)
{
    __delay_ms(30);

    lcd_com(0x38);
    __delay_ms(5);

    lcd_com(0x0C);
    lcd_com(0x01);
    lcd_com(0x06);
}

void lcd_string(const char *text)
{
    while (*text != '\0')
    {
        lcd_data(*text);
        text++;
    }
}

void main(void)
{
    ADCON1 = 0x06;
    TRISD = 0x00;
    TRISC = 0x00;

    PORTD = 0x00;
    PORTC = 0x00;

    lcd_init();

    lcd_com(0x80);
    lcd_string("Nazar");

    lcd_com(0xC0);
    lcd_string("Huchko");

    while (1)
    {
    }
}
