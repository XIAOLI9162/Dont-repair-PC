#include "config.c"
#include <pic18f26k20.h>
//#define _XTAL_FREQ 16000000

char value = 0 ;
void low_priority interrupt PIR(void)
{   
    
    if(INTCONbits.INT0IF == 1)
    {
        
        value=!value;	// Toggle LED on interrupt
        //__delay_ms(100);
        INTCONbits.INT0IF=0;
    }
    
}

void main()
{
    //OSCCONbits.IRCF = 7;		/* Set internal oscillator to 16MHz*/
    TRISCbits.RC4=0;                   /* Make PORTC4 as output*/           
	//PORTBbits.RB1= 0x00;
    /* Initialize External Interrupt*/ 
    TRISBbits.RB0=1;		// Make INT0 pin as an input pin
    INTCON2bits.INTEDG0=1;  // Set Interrupt on falling Edge
    INTCONbits.INT0IF=0;	// Clear INT0IF flag
    INTCONbits.INT0IE=1;	// Enable INT0 external interrupt
    INTCONbits.GIE=1;		// Enable Global Interrupt
    INTCONbits.PEIE=1;
    ANSELHbits.ANS12 =0;   //enable RB0 as I/O port
    //INTCON2bits.RBPU = 0;
    
    //RCONbits.IPEN = 1;
    //INTCONbits.GIEH=1;
    //INTCONbits.GIEL=1;
    //LATC= 0xff;	// Toggle LED on interrupt
    //__delay_ms(100);
    while(1)
    {
        PORTCbits.RC4= value;    
    }
}
    


