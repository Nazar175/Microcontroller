#include <htc.h>

#define _XTAL_FREQ 4000000

#define OW_PIN PORTBbits.RB0
#define OW_TRIS TRISBbits.TRISB0

unsigned char scratchpad[5];
unsigned char sensor_present;

void ow_release(void)
{
    OW_PIN = 0;
    OW_TRIS = 1;
}

void ow_low(void)
{
    OW_PIN = 0;
    OW_TRIS = 0;
}

unsigned char ow_reset(void)
{
    unsigned char presence;

    ow_low();
    __delay_us(480);

    ow_release();
    __delay_us(70);

    presence = (OW_PIN == 0);

    __delay_us(410);

    return presence;
}

void ow_write_bit(unsigned char bit)
{
    ow_low();

    if (bit)
    {
        __delay_us(6);
        ow_release();
        __delay_us(64);
    }
    else
    {
        __delay_us(60);
        ow_release();
        __delay_us(10);
    }
}

unsigned char ow_read_bit(void)
{
    unsigned char bit;

    ow_low();
    __delay_us(3);

    ow_release();
    __delay_us(10);

    bit = OW_PIN;

    __delay_us(53);

    return bit;
}

void ow_write_byte(unsigned char data)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        ow_write_bit(data & 0x01);
        data >>= 1;
    }
}

unsigned char ow_read_byte(void)
{
    unsigned char i;
    unsigned char data = 0;

    for (i = 0; i < 8; i++)
    {
        if (ow_read_bit())
        {
            data |= (1 << i);
        }
    }

    return data;
}

void ds18b20_write_settings(void)
{
    ow_reset();

    ow_write_byte(0xCC);

    ow_write_byte(0x4E);

    ow_write_byte(0x7D);

    ow_write_byte(0xC9);

    ow_write_byte(0x7F);
}

void ds18b20_read_scratchpad(void)
{
    unsigned char i;

    ow_reset();

    ow_write_byte(0xCC);

    ow_write_byte(0xBE);

    for (i = 0; i < 5; i++)
    {
        scratchpad[i] = ow_read_byte();
    }
}

void main(void)
{
    PORTB = 0x00;
    TRISB = 0xFF;
    ow_release();

    sensor_present = ow_reset();

    if (sensor_present)
    {
        ds18b20_write_settings();

        ds18b20_read_scratchpad();
    }

    while (1)
    {
    }
}
