#include "config.c"
#include <pic18f26k20.h>

char value = 0;


void interrupt timer_isr()
{  
    if(TMR0IF==1)
    {
        value=!value;   // complement the value for blinking the LEDs
        TMR0L=99;     // Load the time value(101) for 10ms delay
        TMR0IF=0;       // Clear timer interrupt flag
    } 
}


void main()
{    
    OSCCONbits.IRCF = 7;
    TRISC=0;    //COnfigure PORTC as output to blink the LEDs
    T0CONbits.T08BIT = 1;  //8bit timer/counter
    //T0CONbits.PSA =0;    //Timer Clock Source is from Prescaler
    T0CONbits.T0CS =0;    //Prescaler gets clock from FCPU 
    T0CONbits.T0PS=7; //prescale set =8 ;
    TMR0L=99;     // Load the time value(101) for 10ms delay       
    TMR0IE=1;       //Enable timer interrupt bit in PIE0 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR0ON = 1;     //Start Timer0    

    while(1)
    {
        PORTCbits.RC4 = value;
    }
}
