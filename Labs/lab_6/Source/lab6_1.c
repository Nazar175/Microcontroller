#include <pic.h>
__CONFIG(0x3FF1);
#define _XTAL_FREQ 4000000UL

#define DISPLAY_TENS 1
#define DISPLAY_UNITS 5
const unsigned char SEGMENTS[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
volatile unsigned char digit=0;

void interrupt isr(void) {
    if (T0IE && T0IF) {
        T0IF=0;
        TMR0=100;
        RA2=0; RA3=0;
        if (digit==0) { PORTB=SEGMENTS[DISPLAY_TENS]; RA2=1; digit=1; }
        else { PORTB=SEGMENTS[DISPLAY_UNITS]; RA3=1; digit=0; }
    }
}
void main(void) {
    TRISA=0b11100001;
    TRISB=0x00; PORTA=0; PORTB=0;
    OPTION_REG=0x05;
    TMR0=100; T0IF=0; T0IE=1; GIE=1;
    while(1) {
        RA1=1; delay_ms(500);
        RA1=0; delay_ms(500);
    }
}