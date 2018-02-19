#include "config.c"
#include <pic18f46k20.h>
#define _XTAL_FREQ 16000000




char Initialize_UART(void)
{
    //SYNC = 0; //Asynchronous mode
    
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

char UART_get_char()    //receive
{   
    if(OERR)
    {
        CREN = 0;
        CREN = 1;
    }
    while(!RCIF);
    
    return RCREG;
}

void UART_send_string(char *st_pt)   //send
{
    int i;
    for(i=0;st_pt[i]!='\0';i++)
    UART_send_char(st_pt[i]);
}

void main()
{
    OSCCONbits.IRCF = 7;        //oscillator 16000000Hz
    int get_value;
    
    TRISCbits.RC4 = 0x00;                //B port output
    Initialize_UART();
       
    
    while(1)
    {
        get_value = UART_get_char();
        
        if (get_value == '1')          //when send 1 ,led on, receive "on"
        {
            UART_send_string("on");
            UART_send_char(10);
            PORTCbits.RC4=0xFF;   
        }
        
        if (get_value == '0')        //when send 0 ,led off, receive "off"
        {
            PORTCbits.RC4=0x00;
            UART_send_string("off");
            UART_send_char(10);
        }
    }
}