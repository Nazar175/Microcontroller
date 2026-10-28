#ifndef LCD_H
#define LCD_H

#include <pic.h>
#include <htc.h>

#define LCD_DATA PORTD

#define LCD_RS RC0
#define LCD_E  RC1

void zapys(void);
void lcd_com(unsigned char command);
void lcd_data(unsigned char data);
void lcd_init(void);
void lcd_string(const char *text);
void lcd_goto(unsigned char address);

#endif
