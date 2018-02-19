#include "config.c"
#include <pic18f26k20.h>

char value = 0;


void interrupt timer_isr()
{  
    if(TMR1IF==1)
    {
        value=!value;   // complement the value for blinking the LEDs
        TMR1H=0x3C;     // Load the time value(0xBDC) for 100ms delay
        TMR1L=0xB0;
        TMR1IF=0;       // Clear timer interrupt flag
    } 
}


void main()
{    
    OSCCONbits.IRCF = 7;
    TRISC=0;    //COnfigure PORTD as output to blink the LEDs

    T1CONbits.T1CKPS=3; //prescale set =8 ;
    TMR1H=0x3C;     // Load the time value(0xBDC) for 100ms delay
    TMR1L=0xB0;       
    TMR1IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR1ON = 1;     //Start Timer1    

    while(1)
    {
        PORTCbits.RC4 = value;
    }
}