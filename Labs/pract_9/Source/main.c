#include <pic.h>
#include <htc.h>

#define _XTAL_FREQ 4000000

CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CP_OFF);

#define EEPROM_WRITE 0
#define EEPROM_READ  1

unsigned char read_data;

void I2C_Init(void)
{
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    INTCON = 0;
    PIE1 = 0;
    PIE2 = 0;

    SSPADD = 9;
    SSPSTAT = 0x80;
    SSPCON = 0x28;
    SSPCON2 = 0x00;

    PIR1bits.SSPIF = 0;
}

void I2C_Wait(void)
{
    while ((SSPCON2 & 0x1F) != 0)
    {
        ;
    }

    while (SSPSTATbits.R_W)
    {
        ;
    }
}

void I2C_Start(void)
{
    I2C_Wait();

    PIR1bits.SSPIF = 0;
    SSPCON2bits.SEN = 1;

    while (SSPCON2bits.SEN)
    {
        ;
    }

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    PIR1bits.SSPIF = 0;
}

void I2C_Stop(void)
{
    I2C_Wait();

    PIR1bits.SSPIF = 0;
    SSPCON2bits.PEN = 1;

    while (SSPCON2bits.PEN)
    {
        ;
    }

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    PIR1bits.SSPIF = 0;
}

void I2C_Trans(char data)
{
    I2C_Wait();

    PIR1bits.SSPIF = 0;
    SSPBUF = data;

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    PIR1bits.SSPIF = 0;

    while (SSPCON2bits.ACKSTAT)
    {
        ;
    }
}

char RECIEVE_ACK(void)
{
    char data;

    I2C_Wait();

    PIR1bits.SSPIF = 0;
    SSPCON2bits.RCEN = 1;

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    data = SSPBUF;
    PIR1bits.SSPIF = 0;

    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;

    while (SSPCON2bits.ACKEN)
    {
        ;
    }

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    PIR1bits.SSPIF = 0;

    return data;
}

char RECIEVE_NACK(void)
{
    char data;

    I2C_Wait();

    PIR1bits.SSPIF = 0;
    SSPCON2bits.RCEN = 1;

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    data = SSPBUF;
    PIR1bits.SSPIF = 0;

    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;

    while (SSPCON2bits.ACKEN)
    {
        ;
    }

    while (!PIR1bits.SSPIF)
    {
        ;
    }

    PIR1bits.SSPIF = 0;

    return data;
}

void EEPROM_WriteByte(unsigned int address, unsigned char data)
{
    unsigned char control;

    control = 0xA0 | ((address >> 7) & 0x0E);

    I2C_Start();
    I2C_Trans(control | EEPROM_WRITE);
    I2C_Trans((unsigned char)address);
    I2C_Trans(data);
    I2C_Stop();

    delay_ms(10);
}

unsigned char EEPROM_ReadByte(unsigned int address)
{
    unsigned char control;
    unsigned char data;

    control = 0xA0 | ((address >> 7) & 0x0E);

    I2C_Start();
    I2C_Trans(control | EEPROM_WRITE);
    I2C_Trans((unsigned char)address);

    I2C_Start();
    I2C_Trans(control | EEPROM_READ);

    data = RECIEVE_NACK();

    I2C_Stop();

    return data;
}

void main(void)
{
    unsigned char test_data = 0x5A;

    I2C_Init();

    EEPROM_WriteByte(0x000, test_data);

    read_data = EEPROM_ReadByte(0x000);

    while (1)
    {
    }
}
