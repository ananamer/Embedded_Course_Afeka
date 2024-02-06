#include <xc.h>
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC =FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
#define ARRAY_LEN 4
int j;
void delay(int speed)
{
    if(speed == 0){
        for(j=0;j<64000;j++);
    }
    else{
        for(j=0;j<640000;j++);
    }
}
void main()
{
    TRISA &= 0xff00;
    TRISFbits.TRISF3 = 1; //  RF3  (SW0) configured as input
    TRISFbits.TRISF5 = 1; //  RF5  (SW1) configured as input
    TRISFbits.TRISF4 = 1; //  RF4  (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; //  RB9  (SW7) configured as input
    ANSELBbits.ANSB9 = 0; //  RB9  (SW7) disabled analog
    
    TRISBbits.TRISB14 = 0; // RB14 (Buzzer) configured as output
    ANSELBbits.ANSB14 = 0; // RB14 disabled analog
    
    
    unsigned char SW2_Numbers[ARRAY_LEN] = {0x18, 0x24, 0x42, 0x81};
    int speed = 0; // default speed set to slow
    int j;
    
    while(1){
        if(PORTBbits.RB11){ // if SW5
            PORTA = 0x00;
            // doing nothing
            // but not going to the other operations
        }        
        else{ // if SW5=0
            if(PORTDbits.RD14) // if SW4
                speed = 1;
            else 
                speed = 0;
            
            if(PORTFbits.RF3){ // if SW0
               if(PORTDbits.RD15)// if SW3
                   PORTA --;
               else
                   PORTA ++;
               delay(speed);
            }
            else if(PORTFbits.RF5){ // if SW1
                // SHIFT
                delay(speed);
                if(PORTDbits.RD15){
                    PORTA = 0x80;
                    delay(speed);
                    for(j=0; j<8; j++){
                        PORTA >>= 1;   
                        delay(speed);
                    } 
                }
                else{
                    PORTA = 0x01;
                    delay(speed);
                    for(j=0; j<8; j++){
                     PORTA <<= 1;   
                      delay(speed);
                    }
                }
            }
            else if(PORTFbits.RF4){ // if SW2
                if(PORTDbits.RD15){
                    for(j=0; j<4; j++){
                        PORTA = SW2_Numbers[j];
                        delay(speed);
                    }
                }
                else{
                    for(j=0; j<4; j++){
                    PORTA = SW2_Numbers[ARRAY_LEN-1-j];
                    delay(speed);
                    }
                }
            }
            if(PORTBbits.RB10){ // if SW6
                // buzzer
                PORTBbits.RB14 = 0;
                delay(speed);
                PORTBbits.RB14 = 1;
                delay(speed);            
            }
            if(PORTBbits.RB9){ // if SW7
                // exit
                PORTA = 0xff;
                delay(speed);
                PORTA = 0x00;
                break;
            }
        }//else(SW5 = 0) the whole system 
    }// while(1)
}//main
