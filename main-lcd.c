#include "config.c"
#include <pic18f46k20.h>

#define _XTAL_FREQ 16000000
#define RS PORTEbits.RE2
#define EN PORTAbits.RA1
#define D4 PORTDbits.RD1
#define D5 PORTDbits.RD0
#define D6 PORTDbits.RD5
#define D7 PORTDbits.RD4

#include "lcd.h";

int get_value;
unsigned int a;
int count;

char Initialize_UART(void)
{
    TRISC7 = 1;                                                             //Rx
    TRISC6 = 0;                                                             //Tx 
    
    SPBRG=0X67;                                                       //Baudrate
    BAUDCONbits.BRG16 = 0;                   //8-bit Baud Rate Generator is used
    BRGH = 1;                                                       //high speed
    
    TXSTAbits.SYNC = 0;                                      //Asynchronous mode
    RCSTAbits.SPEN = 1;                                    //Serial port enabled
    
    TXSTAbits.TXEN = 1;                                       //Transmit enabled
    RCSTAbits.CREN = 1;                                       //Enables receiver
    
    TXSTAbits.TX9 = 0;                                                //8 bit tx
    RCSTAbits.RX9 = 0;                                                //8 bit rx  
}

void UART_send_char(char data)                                    //write a char
{
    while(!TRMT);
    TXREG = data;
}

char UART_get_char()                                                   //receive
{   
    if(OERR)
    {
        CREN = 0;
        CREN = 1;
    }
    while(!RCIF);
    
    return RCREG;
}

void UART_send_string(char *st_pt)                                        //send
{
    int i;
    for(i=0;st_pt[i]!='\0';i++)
    UART_send_char(st_pt[i]);
}

void interrupt isr()
{
    if (INTCONbits.INT0IF==1)                               //external interrupt
    {
        UART_send_string("INTERRUPT");
        UART_send_char(10);
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("INTERRUPT");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("!!!!!");
        __delay_ms(200);
        INTCONbits.INT0IF=0;
    }
    
    if (TMR1IF==1)                                  //every 15s have a interrupt
    {
        TMR1H=0x3C;                // Load the time value(0xBDC) for 100ms delay
        TMR1L=0xB0;
        TMR1IF=0;                                  // Clear timer interrupt flag
        if (count>=150)                                        //make 15s delays
        {
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("waiting for");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("COMMAND");
            count =0;
        }
        else
        {
            count++;
        }   
    }
}

void main()
{        
    OSCCONbits.IRCF = 7;                                 //oscillator 16000000Hz
    Initialize_UART();
 
    /* Initialize External Interrupt*/ 
    TRISBbits.RB0=1;                            // Make INT0 pin as an input pin
    INTCON2bits.INTEDG0=1;                      // Set Interrupt on falling Edge
    INTCONbits.INT0IF=0;                                    // Clear INT0IF flag
    INTCONbits.INT0IE=1;                       // Enable INT0 external interrupt
    INTCONbits.GIE=1;                                 // Enable Global Interrupt
    INTCONbits.PEIE=1;
    ANSELHbits.ANS12 =0;                                //enable RB0 as I/O port
    
    /* Initialize timer1 Interrupt*/ 
    T1CONbits.T1CKPS=3;                                      //prescale set =8 ;
    TMR1H=0x3C;                    // Load the time value(0xBDC) for 100ms delay
    TMR1L=0xB0;       
    TMR1IE=1;                      //Enable timer interrupt bit in PIE1 register
    TMR1ON = 1;                                                   //Start Timer1    
          
    /* Initialize the LCD pins*/ 
    TRISEbits.RE2 = 0x00;
    TRISAbits.RA1 = 0x00;
    TRISDbits.RD1 = 0x00;
    TRISDbits.RD0 = 0x00;
    TRISDbits.RD5 = 0x00;
    TRISDbits.RD4 = 0x00;
    Lcd_Init();
    
    while(1)
    {
        get_value = UART_get_char();
        if(get_value == '0')                        //when send 0 ,receive "off"
        {
            UART_send_string("off");
            UART_send_char(10);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("LCD IS OFF");
            
            for(a=0;a<10;a++)
            {
                __delay_ms(300);
                Lcd_Shift_Left();
            }
            for(a=0;a<10;a++)
            {
                __delay_ms(300);
                Lcd_Shift_Right();
            }            
       }
        
        if (get_value=='1')                          //when send 1 ,receive "on"
        {
            UART_send_string("on");
            UART_send_char(10);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("hello");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("world");
            __delay_ms(2000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("bonjour");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("anao");
            __delay_ms(2000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Xiao LI");
        }
    }
}
