#include <pic.h>
__CONFIG(0x3FF1);
#define _XTAL_FREQ 4000000UL

const unsigned char SEGMENTS[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
volatile unsigned char count=0, digit=0;

void interrupt isr(void) {
    if (T0IE && T0IF) {
        T0IF=0; TMR0=100;
        RA2=0; RA3=0;
        if (digit==0) { PORTB=SEGMENTS[count/10]; RA2=1; digit=1; }
        else { PORTB=SEGMENTS[count%10]; RA3=1; digit=0; }
    }
}
void main(void) {
    unsigned char previous_button=1, current_button;
    TRISA=0b11100001; TRISB=0x00; PORTA=0; PORTB=0;
    OPTION_REG=0x05; TMR0=100; T0IF=0; T0IE=1; GIE=1;
    while(1) {
        current_button=RA0;
        if (previous_button==1 && current_button==0) {
            delay_ms(20);
            if (RA0==0) {
                count++;
                if (count>=100) count=0;
                while(RA0==0) { ; }
                delay_ms(20);
            }
        }
        previous_button=current_button;
    }
}
