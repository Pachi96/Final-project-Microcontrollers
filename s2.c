   #asm
    .equ __lcd_port=0x0B
    .equ __lcd_EN=1
    .equ __lcd_RS=0
    .equ __lcd_D4=2
    .equ __lcd_D5=3
    .equ __lcd_D6=4
    .equ __lcd_D7=5
   #endasm

#include <mega328p.h>
#include <stdio.h>
#include <delay.h>
#include <stdlib.h>
#include <display.h>

int times = 50;
int endflag = 0;
int i, j, x, ready, score, cuentas;
float frecuencia;
unsigned int selection, inicio;
unsigned char Puntaje[16];
int plays[50];
int aleatorios[50];

char cero[8]={0x00,0x00,0x1B,0x1F,0x1F,0x0E,0x04,0x00};
char uno[8]={0x0E,0x1F,0x15,0x1F,0x1F,0x15,0x15,0x15};
char dos[8]={0x0A,0x0A,0x0A,0x04,0x00,0x0E,0x11,0x00};

void tonito(float frecuencia)
{
    cuentas = (int)(500000/frecuencia - 1);
    DDRB.3 = 1;
    TCCR2A = 0x42;
    TCCR2B = 0x02;
    OCR2A = cuentas;
}

void acciones(void)
{
    delay_ms(500);
    PORTC = 0; 
    PORTB.7 = 0;  
    TCCR2B = 0;
    delay_ms(250);
}

void TurnLed(int color)
{
    switch (color)
    {
        case 0: frecuencia = 261.6;
                tonito(frecuencia); 
                PORTB.7 = 1;
                PORTC = 0x01;
                acciones();
                break;

        case 1: frecuencia = 329.6;
                tonito(frecuencia);
                PORTB.7 = 1;
                PORTC = 0x01;
                PORTC = 0x02;
                acciones();
                break;

        case 2: frecuencia = 391.9;
                tonito(frecuencia);
                PORTB.7 = 1;
                PORTC = 0x04;
                acciones();
                break;

        case 3: frecuencia = 523.2;
                tonito(frecuencia);
                PORTB.7 = 1;
                PORTC = 0x03;
                acciones();
                break;

        default: break;
    }
}

void PrintPlayingLCD(void)
{
    EraseLCD();
    MoveCursor(0,0);
    CharLCD(0);
    CharLCD(1);
    MoveCursor(4,0);
    StringLCD("Playing");
    MoveCursor(14,0);
    CharLCD(1);
    CharLCD(0); 
}

void PrintPress2PlayAgain(void)
{
    CharLCD(2);       
    delay_ms(5000);   
    EraseLCD();
    MoveCursor(0,0);
    StringLCD("Press a button");
    MoveCursor(0,1);
    StringLCD("to play again ");
    CharLCD(0);
}

void main(void)
{
    DDRC = 0x07;
    PORTB = 0x47;
    TCCR0B = 0x01;  
    inicio = PINB & 0x47;
       
    SetupLCD();         
    CreateChar(0,cero);
    CreateChar(1,uno); 
    CreateChar(2, dos);
    MoveCursor(0,0);
    StringLCD(" Press a button");
    MoveCursor(0,1);
    StringLCD("    to play");
    
    while(inicio == 71)
    {                  
        delay_ms(100);
        inicio = PINB & 0x47;
    }         
    
    while(1)
    { 
        PrintPlayingLCD();  
        srand(TCNT0);
        
        for (j=0 ; j < times + 1; j++)
        {
            if(endflag == 1)
            break;   
                  
            aleatorios[i] = rand()%4;
                        
            for (i=0 ; i < j; i++)
            {                           
                TurnLed(aleatorios[i]);
            }

            if (j>0)
            {
                for (x=0; x < j; x++)
                {    
                    ready = 0;
                    while(ready == 0)
                    {               
                        selection = PINB & 0x47;
                        switch (selection)
                        {
                            case 70: if(ready == 0)
                                      {
                                          TurnLed(0);//rojo
                                          plays[x] = 0;
                                          ready = 1;
                                        }
                                        break;

                            case 69: if(ready == 0)
                                      {
                                          TurnLed(1);//verde
                                          plays[x] = 1;
                                          ready = 1;
                                        }
                                        break;

                            case 67: if(ready == 0)
                                      {
                                          TurnLed(2);//azul
                                          plays[x] = 2;
                                          ready = 1;
                                      }     
                                        break;

                            case 7: if(ready == 0)
                                    {
                                        TurnLed(3);//amarillo
                                        plays[x] = 3;
                                        ready = 1;
                                    }
                                    break;

                            default: break;
                        }
                    }
                    
                    if(aleatorios[x] == plays[x])  
                    {
                        endflag = 0;     
                    }
                    else
                    {
                        endflag = 1;
                        break;
                    }
                }
            }
        }
        score = i-1;    
        EraseLCD();
        MoveCursor(0,0);
        StringLCD("    Game Over   ");
        MoveCursor(1,1);  
        CharLCD(2);
        StringLCD("  Score: ");  
        sprintf(Puntaje,"%i  ",score);
        StringLCDVar(Puntaje);
        PrintPress2PlayAgain(); 
        
        inicio = PINB & 0x47;   
        while(inicio == 71)
        {                  
            delay_ms(50);
            inicio = PINB & 0x47;
        }    
        endflag = 0;
    } 
}

