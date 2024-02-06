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
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>


int j;
void busy(void)
{
    char RD,RS;
    int STATUS_TRISE;
    int portMap;
    RD=PORTDbits.RD5;
    RS=PORTBbits.RB15;
    STATUS_TRISE=TRISE;
    PORTDbits.RD5 = 1;//w/r
    PORTBbits.RB15 = 0;//rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do
    {
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
    }
    while(PORTEbits.RE7); // BF ?????
    PORTDbits.RD5=RD; 
    PORTBbits.RB15=RS;
    TRISE=STATUS_TRISE;   
}

void delay(int speed)
{
    if(speed == 0){
        for(j=0;j<64000;j++);
    }
    else{
        for(j=0;j<640000;j++);
    }
}

void write_to_first_line(char msg[], int len)
{
    PORTBbits.RB15=0;
    PORTDbits.RD5=0;
    PORTE = 0x80;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
    busy();
    PORTBbits.RB15=1;
    PORTDbits.RD5=0;

    //clear lcd
//    PORTE &= 0x001;
//    PORTDbits.RD4=1;
//    PORTDbits.RD4=0;
//    busy();


    for(int i=0; i<len; i++){
        PORTE = msg[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void write_to_secod_line(char msg[], int len)
{
    PORTBbits.RB15=0;
    PORTDbits.RD5=0;
    PORTE = 0xc0;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
    busy();
    PORTBbits.RB15=1;
    PORTDbits.RD5=0;
    
    //clear lcd
//    PORTE &= 0x001;
//    PORTDbits.RD4=1;
//    PORTDbits.RD4=0;
//    busy();
        
    for(int i=0; i<len; i++){
        PORTE = msg[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
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
    
    //*****
    int LCD_i, LCD_j;
    // 0x80-0x8f, 0x90-0x9f
//    char string[]="Menachem Epstein";
    char control[]={0x38,0x38,0x38,0xe,0x6,0x1};
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    //TRISEbits.TRISE0 = 1; // RE0 (DB0) set as input (change 1 to 0 for
    TRISE&=0xff00;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    ANSELEbits.ANSE7 = 0;
    
 /*   for(LCD_i=0;LCD_i<6;LCD_i++){
        PORTE=control[LCD_i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        //for(j=0;j<32000;j++);
        busy();
    }
    PORTBbits.RB15=1;//rs=0
    PORTDbits.RD5=0;//w=0
    for(LCD_i=0;LCD_i<16;LCD_i++){
        PORTE=string[LCD_i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        //for(j=0;j<32000;j++);
        busy();
    }
        */
    
    //write something to first line
    char name1[] = "hello           ";
    write_to_first_line(name1, 16);
    
    // try to write to 2 line
    char name[] = "Anan            ";
    write_to_secod_line(name, 16);
    //*****
    
    unsigned char SW2_Numbers[ARRAY_LEN] = {0x18, 0x24, 0x42, 0x81};
    int speed = 0; // default speed set to slow
    int j;
    
    while(1){
        if(PORTBbits.RB11){ // if SW5
            char string[]="Mode 5";
            char msg[] = "Halt";
            write_to_first_line(string, 6);
            write_to_secod_line(msg, 4);
            PORTA = 0x00;
            // doing nothing
            // but not going to the other operations
        }        
        else{ // if SW5=0
            if(PORTDbits.RD14) // if SW4
                speed = 1;
            else 
                speed = 0;
            //******** SW0 ********
            if(PORTFbits.RF3){ // if SW0
               char string[]="Mode 0";
               write_to_first_line(string, 6);
               if(PORTDbits.RD15){// if SW3
                   PORTA --;
               if(speed){
                   char msg[] = "Counter Down Slow";
                   write_to_secod_line(msg, 17);                   
               }else{
                   char msg[] = "Counter Down Fast";
                   write_to_secod_line(msg, 17);
                }
               }
               else{
                   PORTA ++;
               if(speed){
                   char msg[] = "Counter Up Slow";
                   write_to_secod_line(msg, 17);                    
               }else{
                   char msg[] = "Counter Up Fast";
                   write_to_secod_line(msg, 17);                    
                }
               }
               delay(speed);
            }//******** SW0 ********
            //******** SW1 ********
            else if(PORTFbits.RF5){ // if SW1
                // SHIFT
                char string[]="Mode 1";
                write_to_first_line(string, 6);
                delay(speed);
                if(PORTDbits.RD15){//if SW3
                    if(speed){
                        char msg[] = "Shift Right Slow";
                        write_to_secod_line(msg, 16);
                    }else{
                        char msg[] = "Shift Right Fast";
                        write_to_secod_line(msg, 16);
                    }
                    PORTA = 0x80;
                    delay(speed);
                    for(j=0; j<8; j++){
                        PORTA >>= 1;   
                        delay(speed);
                    } 
                }
                else{
                    if(speed){
                        char msg[] = "Shift Left Slow";
                        write_to_secod_line(msg, 15);
                    }else{
                        char msg[] = "Shift Left Fast";
                        write_to_secod_line(msg, 15);
                    }
                    PORTA = 0x01;
                    delay(speed);
                    for(j=0; j<8; j++){
                     PORTA <<= 1;   
                      delay(speed);
                    }
                }
            }//******** SW1 ********
            //******** SW2 ********
            else if(PORTFbits.RF4){ // if SW2
                char string[]="Mode 2";
                write_to_first_line(string, 6);
                if(PORTDbits.RD15){
                    if(speed){
                        char msg[] = "Swing Up Slow";
                        write_to_secod_line(msg, 15);
                    }else{
                        char msg[] = "Swing Up Fast";                    
                        write_to_secod_line(msg, 15);
                    }
                    for(j=0; j<4; j++){
                        PORTA = SW2_Numbers[j];
                        delay(speed);
                    }
                }
                else{
                    if(speed){
                        char msg[] = "Swing Down Slow";
                        write_to_secod_line(msg, 15);
                    }else{
                        char msg[] = "Swing Down Fast";
                        write_to_secod_line(msg, 15);
                    }
                    for(j=0; j<4; j++){
                    PORTA = SW2_Numbers[ARRAY_LEN-1-j];
                    delay(speed);
                    }
                }
            }//******** SW2 ********
            //******** SW6 ********
            if(PORTBbits.RB10){ // if SW6
                // buzzer
                char string[]="Mode 6";
                write_to_first_line(string, 6);
                if(speed){
                    char msg[] = "Beep Mode Slow";
                    write_to_secod_line(msg, 14);
                }else{
                    char msg[] = "Beep Mode Fast";                   
                    write_to_secod_line(msg, 14);
                }
                PORTBbits.RB14 = 0;
                delay(speed);
                PORTBbits.RB14 = 1;
                delay(speed);            
            }//******** SW6 ********
            //******** SW7 ********
            if(PORTBbits.RB9){ // if SW7
                // exit
                char string[]="Mode 7";
                write_to_first_line(string, 6);
                PORTA = 0xff;
                delay(speed);
                PORTA = 0x00;
                break;
            }//******** SW7 ********
        }//else(SW5 = 0) the whole system 
    }// while(1)
}//main
