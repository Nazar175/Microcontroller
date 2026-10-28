#include "LCD.h"

void zapys(void)
{
    LCD_E = 1;
    delay_us(2);
    LCD_E = 0;
    delay_us(50);
}

void lcd_com(unsigned char command)
{
    LCD_RS = 0;
    LCD_DATA = command;

    zapys();

    if (command == 0x01 || command == 0x02)
    {
        delay_ms(2);
    }
}

void lcd_data(unsigned char data)
{
    LCD_RS = 1;
    LCD_DATA = data;

    zapys();
}

void lcd_goto(unsigned char address)
{
    lcd_com(address);
    delay_ms(1);
}

void lcd_init(void)
{
    LCD_RS = 0;
    LCD_E = 0;

    delay_ms(20);

    lcd_com(0x38);
    delay_ms(5);

    lcd_com(0x0C);
    delay_ms(1);

    lcd_com(0x01);
    delay_ms(2);

    lcd_com(0x06);
    delay_ms(1);
}

void lcd_string(const char *text)
{
    while (*text != '\0')
    {
        lcd_data(*text);
        text++;
    }
}
