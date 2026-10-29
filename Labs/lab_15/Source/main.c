#include <htc.h>

#define _XTAL_FREQ 4000000

#define EEPROM_WRITE 0xA0
#define EEPROM_READ  0xA1

unsigned char received_data;

void I2C_Wait(void)
{
    while (!SSPIF)
    {
    }

    SSPIF = 0;
}

void I2C_Init(void)
{
    TRISC3 = 1;
    TRISC4 = 1;

    GIE = 0;
    PEIE = 0;
    SSPIE = 0;

    SSPADD = 9;

    SMP = 1;
    CKE = 0;

    SSPCON = 0x28;

    SSPCON2 = 0x00;

    SSPIF = 0;
}

void I2C_Start(void)
{
    SSPIF = 0;
    SEN = 1;
    I2C_Wait();
}

void I2C_Stop(void)
{
    SSPIF = 0;
    PEN = 1;
    I2C_Wait();
}

void I2C_Trans(unsigned char data)
{
    SSPIF = 0;
    SSPBUF = data;

    I2C_Wait();
}

unsigned char RECIEVE_ACK(void)
{
    unsigned char data;

    SSPIF = 0;
    RCEN = 1;

    I2C_Wait();

    data = SSPBUF;

    ACKDT = 0;
    SSPIF = 0;
    ACKEN = 1;
    I2C_Wait();

    return data;
}

unsigned char RECIEVE_NACK(void)
{
    unsigned char data;

    SSPIF = 0;
    RCEN = 1;

    I2C_Wait();

    data = SSPBUF;

    ACKDT = 1;
    SSPIF = 0;
    ACKEN = 1;
    I2C_Wait();

    return data;
}

void EEPROM_Write(void)
{
    I2C_Start();

    I2C_Trans(EEPROM_WRITE);
    I2C_Trans(0x00);
    I2C_Trans(0x5A);

    I2C_Stop();

    __delay_ms(10);
}

unsigned char EEPROM_Read(void)
{
    unsigned char data;

    I2C_Start();
    I2C_Trans(EEPROM_WRITE);
    I2C_Trans(0x00);
    I2C_Stop();

    I2C_Start();
    I2C_Trans(EEPROM_READ);

    data = RECIEVE_NACK();

    I2C_Stop();

    return data;
}

void main(void)
{
    I2C_Init();

    EEPROM_Write();

    received_data = EEPROM_Read();

    while (1)
    {
    }
}
