#include "config.c"
#include <pic18f26k20.h>
#include <stdlib.h>


#define _XTAL_FREQ 16000000



void Initialize_UART()
{
    TRISC7 = 1; //Rx
    TRISC6 = 0;   //Tx 
    
    SPBRG=0X67;  //Baudrate
    BAUDCONbits.BRG16 = 0; //8-bit Baud Rate Generator is used
    BRGH = 1; //high speed
    
    TXSTAbits.SYNC = 0; //Asynchronous mode
    RCSTAbits.SPEN = 1; //Serial port enabled
    
    TXSTAbits.TXEN = 1; //Transmit enabled
    RCSTAbits.CREN = 1; //Enables receiver
    
    TXSTAbits.TX9 = 0; //8 bit tx
    RCSTAbits.RX9 = 0; //8 bit rx
      
}

void UART_send_char(char data)  //write a char
{
    while(!TRMT);
    TXREG = data;
}

void UART_send_string(char *st_pt)                                        //send
{
    int i;
    for(i=0;st_pt[i]!='\0';i++)
    UART_send_char(st_pt[i]);
}

unsigned int ADC_Read()
{ 
    __delay_us(16);                                 //acquisition delay 16us
    ADCON0bits.GO_DONE = 1;                         //Initializes A/D conversion
    while(ADCON0bits.GO_DONE==0);           //Waiting for conversion to complete
    return ((ADRESH<<8)+ADRESL);                                 //Return result
}

void main()
{
    OSCCONbits.IRCF = 7;
    
    Initialize_UART();

    TRISA= 1;
    ANSEL= 1;
    
    /*Initialize ADC*/
    ADCON2bits.ADCS = 2;               //clock derived from a dedicated internal oscillator
    ADCON1bits.VCFG0 = 0;               //Positive voltage reference supplied internally by VDD.
    ADCON1bits.VCFG1 = 0;               //Negative voltage reference supplied internally by VSS.
    ADCON0bits.CHS=0b0000;                   //select ADC input chanel 0
    ADCON2bits.ADFM=1;                  //select result format Right justified
    ADCON2bits.ACQT=4;                  //select acquisition delay 
    ADCON0bits.ADON = 1;                    //Turn on ADC module
    
    do
    {
        char s[10];
        char c[10];
        int t,h,l;
        t = ADC_Read()*33;
        h = t/10000;
        l = t%10000;
        itoa(s,h,10);
        itoa(c,l,10);
        UART_send_string(s);
        UART_send_string(".");
        UART_send_string(c);
        UART_send_char(10);
        __delay_ms(1000);            //Delay
    }
    while(1);                    //Infinite Loop
}
