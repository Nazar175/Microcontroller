#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CP_OFF);

#define LCD_CS  PORTBbits.RB0
#define LCD_RST PORTBbits.RB1

#define BLACK  0x000
#define RED    0xF00
#define GREEN  0x0F0
#define BLUE   0x00F
#define WHITE  0xFFF
#define YELLOW 0xFF0

void lcd_trans(unsigned char data)
{
    unsigned char temp;

    PIR1bits.SSPIF = 0;
    SSPBUF = data;

    while (!PIR1bits.SSPIF)
    {
    }

    temp = SSPBUF;
}

void lcd_trans_com(unsigned char command)
{
    LCD_CS = 0;

    lcd_trans(command >> 1);
    lcd_trans(command << 7);

    LCD_CS = 1;
}

void lcd_trans_data(unsigned char data)
{
    LCD_CS = 0;

    lcd_trans(0x80 | (data >> 1));
    lcd_trans(data << 7);

    LCD_CS = 1;
}

void Init_MK(void)
{
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC4 = 1;

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    LCD_CS = 1;
    LCD_RST = 1;

    SSPSTAT = 0x40;
    SSPCON = 0x20;
}

void LCD_INIT(void)
{
    LCD_RST = 0;
    delay_ms(1);
    LCD_RST = 1;
    delay_ms(10);

    lcd_trans_com(0x01);
    delay_ms(10);

    lcd_trans_com(0x11);
    delay_ms(10);

    lcd_trans_com(0x03);

    lcd_trans_com(0x3A);
    lcd_trans_data(0x03);

    lcd_trans_com(0x36);
    lcd_trans_data(0xC8);

    lcd_trans_com(0x25);
    lcd_trans_data(0x30);

    lcd_trans_com(0x13);

    lcd_trans_com(0x29);
    delay_ms(10);
}

void LCD_WINDOW(unsigned char x0, unsigned char y0,
                unsigned char x1, unsigned char y1)
{
    lcd_trans_com(0x2A);
    lcd_trans_data(x0);
    lcd_trans_data(x1);

    lcd_trans_com(0x2B);
    lcd_trans_data(y0);
    lcd_trans_data(y1);
}

void BOX_DRW(unsigned char x0, unsigned char y0,
             unsigned char x1, unsigned char y1,
             unsigned int color)
{
    unsigned int pixels;
    unsigned int pairs;
    unsigned int i;

    unsigned char high_byte;
    unsigned char middle_byte;
    unsigned char low_byte;

    if (x0 > x1 || y0 > y1)
        return;

    if (x1 > 131 || y1 > 131)
        return;

    LCD_WINDOW(x0, y0, x1, y1);

    lcd_trans_com(0x2C);

    pixels = ((unsigned int)(x1 - x0 + 1))
           * ((unsigned int)(y1 - y0 + 1));

    pairs = (pixels + 1) / 2;

    high_byte = color >> 4;
    middle_byte = (color << 4) | (color >> 8);
    low_byte = color;

    for (i = 0; i < pairs; i++)
    {
        lcd_trans_data(high_byte);
        lcd_trans_data(middle_byte);
        lcd_trans_data(low_byte);
    }
}

void CURSOR_XY(unsigned char x, unsigned char y)
{
    LCD_WINDOW(x, y, x, y);
}

void COLOR_XY(unsigned char x, unsigned char y,
              unsigned int color)
{
    CURSOR_XY(x, y);

    lcd_trans_com(0x2C);

    lcd_trans_data(color >> 4);
    lcd_trans_data(color << 4);
}
void LINE_XY(int x0, int y0, int x1, int y1,
             unsigned int color)
{
    int dx, dy;
    int sx, sy;
    int err, e2;

    dx = x1 - x0;
    if (dx < 0)
        dx = -dx;

    dy = y1 - y0;
    if (dy < 0)
        dy = -dy;

    if (x0 < x1)
        sx = 1;
    else
        sx = -1;

    if (y0 < y1)
        sy = 1;
    else
        sy = -1;

    err = dx - dy;

    while (1)
    {
        if (x0 >= 0 && x0 < 132 &&
            y0 >= 0 && y0 < 132)
        {
            COLOR_XY(x0, y0, color);
        }

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;

        if (e2 > -dy)
        {
            err = err - dy;
            x0 = x0 + sx;
        }

        if (e2 < dx)
        {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

void CIRCLE(int x0, int y0, int radius,
            unsigned int color)
{
    int x, y, p;

    x = radius;
    y = 0;
    p = 1 - radius;

    while (x >= y)
    {
        COLOR_XY(x0 + x, y0 + y, color);
        COLOR_XY(x0 + y, y0 + x, color);
        COLOR_XY(x0 - y, y0 + x, color);
        COLOR_XY(x0 - x, y0 + y, color);
        COLOR_XY(x0 - x, y0 - y, color);
        COLOR_XY(x0 - y, y0 - x, color);
        COLOR_XY(x0 + y, y0 - x, color);
        COLOR_XY(x0 + x, y0 - y, color);

        y++;

        if (p <= 0)
        {
            p = p + 2 * y + 1;
        }
        else
        {
            x--;
            p = p + 2 * y - 2 * x + 1;
        }
    }
}
const unsigned char font8x8[36][8] =
{
    {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00},
    {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
    {0x3C,0x66,0x06,0x0C,0x30,0x60,0x7E,0x00},
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
    {0x0C,0x1C,0x2C,0x4C,0x7E,0x0C,0x0C,0x00},
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
    {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00},
    {0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00},
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00},

    {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
    {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00},
    {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00},
    {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
    {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00},
    {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
    {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
    {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
    {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00},
    {0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00},
    {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
    {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
    {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
    {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00},
    {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}
};

void LCD_CHAR(char symbol, unsigned char x, unsigned char y,
              unsigned int foreground, unsigned int background)
{
    unsigned char index;
    unsigned char row;
    unsigned char col;
    unsigned char bits;

    if (symbol >= '0' && symbol <= '9')
        index = symbol - '0';
    else if (symbol >= 'A' && symbol <= 'Z')
        index = symbol - 'A' + 10;
    else
        return;

    for (row = 0; row < 8; row++)
    {
        bits = font8x8[index][row];

        for (col = 0; col < 8; col++)
        {
            if (bits & 0x80)
                COLOR_XY(x + col, y + row, foreground);
            else
                COLOR_XY(x + col, y + row, background);

            bits = bits << 1;
        }
    }
}

void LCD_TEXT(char *text, unsigned char x, unsigned char y,
              unsigned int foreground, unsigned int background)
{
    while (*text != '\0')
    {
        LCD_CHAR(*text, x, y, foreground, background);
        x = x + 8;
        text++;
    }
}

void main(void)
{
    Init_MK();
    LCD_INIT();

    BOX_DRW(0, 0, 131, 131, BLACK);

    BOX_DRW(10, 10, 121, 45, BLUE);
    BOX_DRW(15, 15, 116, 40, RED);

    LINE_XY(10, 60, 120, 60, GREEN);
    LINE_XY(10, 60, 60, 110, YELLOW);
    CIRCLE(90, 85, 20, WHITE);

    LCD_TEXT("LCD6610", 10, 115, WHITE, BLACK);

    while (1)
    {
    }
}
