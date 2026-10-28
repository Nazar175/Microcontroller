#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CP_OFF);

#define OW_PIN PORTBbits.RB0
#define OW_DIR TRISBbits.TRISB0

unsigned char ow_reset(void);
void ow_write_bit(unsigned char bit_value);
unsigned char ow_read_bit(void);
void ow_write_byte(unsigned char data);
unsigned char ow_read_byte(void);

void ds18b20_write_config(void);
signed int ds18b20_read_temperature(void);

void ow_low(void)
{
    OW_PIN = 0;
    OW_DIR = 0;
}

void ow_release(void)
{
    OW_DIR = 1;
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

void ow_write_bit(unsigned char bit_value)
{
    ow_low();

    if (bit_value)
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
    unsigned char bit_value;

    ow_low();
    __delay_us(2);

    ow_release();
    __delay_us(10);

    bit_value = OW_PIN;

    __delay_us(50);

    return bit_value;
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
void ds18b20_write_config(void)
{
    if (!ow_reset())
        return;

    ow_write_byte(0xCC);
    ow_write_byte(0x4E);

    ow_write_byte(0x7D);
    ow_write_byte(0xC9);
    ow_write_byte(0x7F);
}

signed int ds18b20_read_temperature(void)
{
    unsigned char low_byte;
    unsigned char high_byte;
    signed int raw_temperature;

    if (!ow_reset())
        return 0;

    ow_write_byte(0xCC);
    ow_write_byte(0x44);

    __delay_ms(750);

    if (!ow_reset())
        return 0;

    ow_write_byte(0xCC);
    ow_write_byte(0xBE);

    low_byte = ow_read_byte();
    high_byte = ow_read_byte();

    raw_temperature = ((unsigned int)high_byte << 8) | low_byte;

    return raw_temperature;
}

void main(void)
{
    signed int temperature_raw;
    signed int temperature_c;

    PORTB = 0x00;
    TRISB = 0x01;

    __delay_ms(100);

    ds18b20_write_config();

    while (1)
    {
        temperature_raw = ds18b20_read_temperature();
        temperature_c = temperature_raw / 16;

        __delay_ms(500);
    }
}
