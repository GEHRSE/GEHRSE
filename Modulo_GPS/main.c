/*
 * File:   main.c
 * Author: camil
 *
 * Created on 6 de marzo de 2018, 12:27 AM
 */


#include <xc.h>
#include "Configuration_Bits.h"
#include "lcd.h"
#include "uart.h"
#include <string.h>
#include <pic18f45k20.h>
#define _XTAL_FREQ 800000

// ============================================================================
// --- Variables globales ---

volatile char c;
volatile char d;
char* data;
static char uartBuffer[300];
int i;
char* terminator;
char conversionString[8];
double lat = 0.0;
double lon = 0.0;
double *longitude = &lon;
double *latitude = &lat;



// ============================================================================

// --- interrupciones ---
/*
void interrupt interrupcion()
{
     if (PIR1bits.RCIF == 1)                         //interrpcion UART?
     {                                         //Si...
          txt = UART_Read_Text();             //Lo guarda en el espacio siguiente de la string
          PORTBbits.RB0 = ~PORTBbits.RB0 ;              //invierte el estado de RB0
          if(i == 768) i = 0;                  //mira si i es 768? lo vuelve cero 
          T1CONbits.TMR1ON = 0;                        //apaga Timer
          T1CONbits.TMR1ON = 1;                        //prende el Timer1
          PIR1bits.RCIF = 0;                         //Limpia la bandera RCIF

     } //end if RCIF

} //end interrupt
*/
// ============================================================================
void read_gps();

// ============================================================================
// --- Funcion principal ---
void main()
{
      ADCON1   = 0x0F;                         // Canales Analogicos como digitales I/O
      OSCCONbits.IRCF = 7;
      TRISBbits.RB0 = 0;//configura RB0 como salida
      TRISBbits.RB1 = 0;
      Lcd_Init();                              //inicia LCD
      Lcd_Cmd(LCD_CURSOR_OFF);                  //Se apaga el cursor                
      Lcd_Cmd(LCD_CLEAR);                     //Limpa LCD
      Lcd_Out(1,1,"   PIC18F45K20   ");         //Envia mensaje
      Lcd_Out(2,1,"   Modulo GPS   ");         //
      __delay_ms(1000);                          //espera 1 segundo
      Lcd_Cmd(LCD_CLEAR);                     //Limpia LCD
      Lcd_Out(1,1,"Esperando señal");         //Envia mensaje

      UART_Init(9600);                        //inicia la comunicacion UART1 en 9600 baud rate
      //Habilita recepcion  de UART:
      PIE1bits.RCIE = 1;                             //RCIE = 1

      INTCONbits.GIE = 1;                           //GIE = 1 habilita interrupcion global
      INTCONbits.PEIE = 1;                           //PEIE = 1 habilita interrupcion por periféricos
      
      __delay_ms(100);
      while(1)                                 //Loop Infinito
      {
          read_gps();
          Lcd_Out(1,1,"");         //Envia mensaje
          Lcd_Write_Float(*latitude);         //Envia mensaje
          Lcd_Out(1,1,"");         //Envia mensaje
          Lcd_Write_Float(*latitude);         //Envia mensaje
          
      } //end while

} //end main

void read_gps(){
// Read characters from UART into buffer
for(i=0; i<sizeof(uartBuffer)-1; i++)
{
d = UART_Read_Char(c);
uartBuffer[i] = d;
}
// Last character is null terminator
uartBuffer[sizeof(uartBuffer)-1] = '\0';
// Look for needle in haystack to find string for GPGLL
data = strstr(uartBuffer, "$GPGLL");
// if null exit
if(data == NULL)
{
return;
}
// Find terminator
terminator = strstr(data,",");
// if null exit
if(terminator == NULL)
{
return;
}
// If the first byte of the latitude field is ',', there is noinfo
// so exit
if(data[7] == ',')
{
return;
}
///////////////////////////////////
// Search buffer data for Latitude
// and Longitude values
//////////////////////////////////
data = terminator+1;
terminator = strstr(data,",");
if(terminator == NULL)
{
return;
}
memset(conversionString,0,sizeof(conversionString));
memcpy(conversionString, data, 2);
*latitude = atof(conversionString);
data += 2;
*terminator = '\0';
*latitude += (atof(data)/60);
data = terminator+1;
terminator = strstr(data,",");
if(terminator == NULL)
{
return;
}
if(*data == 'S')
{
*latitude *= -1;
}
data = terminator+1;
terminator = strstr(data,",");
if(terminator == NULL)
{
return;
}
memset(conversionString,0,sizeof(conversionString));
memcpy(conversionString, data, 3);
*longitude = atof(conversionString);
data += 3;
*terminator = '\0';
*longitude += (atof(data)/60);
data = terminator+1;
terminator = strstr(data,",");
if(terminator == NULL)
{
return;
}
if(*data == 'W')
{
*longitude *= -1;
}
}