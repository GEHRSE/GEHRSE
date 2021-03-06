#include <string.h>

#include "max7219.h"                                  // MAX7219 header file

#define _XTAL_FREQ 8000000
/*
*********************************************************************************************************
* Constants
*********************************************************************************************************
*/
#define REG_DECODE        0x09                        // "decode mode" register
#define REG_INTENSITY     0x0a                        // "intensity" register
#define REG_SCAN_LIMIT    0x0b                        // "scan limit" register
#define REG_SHUTDOWN      0x0c                        // "shutdown" register
#define REG_DISPLAY_TEST  0x0f                        // "display test" register

#define INTENSITY_MIN     0x00                        // minimum display intensity
#define INTENSITY_MAX     0x0f                        // maximum display intensity
#define NO_OP             0x00


char Am[8]={0x7E,0x13,0x7E,0x70,0x10,0x30,0x10,0x70};
char Pm[8]={0x7F,0x09,0x06,0x70,0x10,0x30,0x10,0x70};
/*
*********************************************************************************************************
* Macros
*********************************************************************************************************
*/
#define DATA_PORT     PORTCbits.RC4                              // assume "DATA" is on P3.5
#define DATA_DDR      TRISCbits.RC4

#define DATA_0()      (DATA_PORT =0)
#define DATA_1()      (DATA_PORT =1)
#define CLK_PORT      PORTCbits.RC3                              // assume "CLK" is on P3.4
#define CLK_DDR       TRISCbits.RC3
#define bittest(D,i) (D & (0x01 << i))
#define CLK_0()       (CLK_PORT = 0)
#define CLK_1()       (CLK_PORT = 1)
#define LOAD_PORT     PORTCbits.RC5                              // assume "LOAD" is on P3.3
#define LOAD_DDR      TRISCbits.RC5

#define LOAD_0()      (LOAD_PORT = 0)
#define LOAD_1()      (LOAD_PORT = 1)


/*
*********************************************************************************************************
* Private Data
*********************************************************************************************************
*/
// ... none ...


/*
*********************************************************************************************************
* Private Function Prototypes
*********************************************************************************************************
*/

static void MAX7219_SendByte (unsigned char data);
static unsigned char MAX7219_LookupCode (char character);
static void MAX7219_Write (char n,unsigned char reg_number, unsigned char data);
void ejemplo();
// ...................................... Public Functions ..............................................


/*
*********************************************************************************************************
* MAX7219_Init()
*
* Description: Initialize MAX7219 module; must be called before any other MAX7219 functions.
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_Init (char n)
{
  DATA_DDR = 0;                               // configure "DATA" as output
  CLK_DDR  = 0;                                // configure "CLK"  as output
  LOAD_DDR = 0;                               // configure "LOAD" as output
  for(char i=1; i<=n;i++){
  MAX7219_Write(i,REG_SCAN_LIMIT, 7);                   // set up to scan all eight digits
  MAX7219_Write(i,REG_DECODE, 0x00);                    // set to "no decode" for all digits
  MAX7219_ShutdownStop(i);                             // select normal operation (i.e. not shutdown)
  MAX7219_DisplayTestStop(i);                          // select normal operation (i.e. not test mode)
  MAX7219_Clear(i);                                    // clear all digits
  MAX7219_SetBrightness(i,INTENSITY_MAX);               // set to maximum intensity
  }
  
}


/*
*********************************************************************************************************
* MAX7219_ShutdownStart()
*
* Description: Shut down the display.
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_ShutdownStart (char i)
{
  MAX7219_Write(i,REG_SHUTDOWN, 0);                     // put MAX7219 into "shutdown" mode
}


/*
*********************************************************************************************************
* MAX7219_ShutdownStop()
*
* Description: Take the display out of shutdown mode.
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_ShutdownStop (char i)
{
  MAX7219_Write(i,REG_SHUTDOWN, 1);                     // put MAX7219 into "normal" mode
}


/*
*********************************************************************************************************
* MAX7219_DisplayTestStart()
*
* Description: Start a display test.
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_DisplayTestStart (char i)
{
  MAX7219_Write(i,REG_DISPLAY_TEST, 1);                 // put MAX7219 into "display test" mode
}


/*
*********************************************************************************************************
* MAX7219_DisplayTestStop()
*
* Description: Stop a display test.
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_DisplayTestStop (char i)
{
  MAX7219_Write(i,REG_DISPLAY_TEST, 0);                 // put MAX7219 into "normal" mode
}


/*
*********************************************************************************************************
* MAX7219_SetBrightness()
*
* Description: Set the LED display brightness
* Arguments  : brightness (0-15)
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_SetBrightness (char i,char brightness)
{
  brightness &= 0x0f;                                 // mask off extra bits
  MAX7219_Write(i,REG_INTENSITY, brightness);           // set brightness
}


/*
*********************************************************************************************************
* MAX7219_Clear()
*
* Description: Clear the display (all digits blank)
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void MAX7219_Clear (char n)
{
  char i;
  for (i=0; i < 8; i++)
    MAX7219_Write(n,i, 0x00);                           // turn all segments off
}

/*
*********************************************************************************************************
* Fuente
*
* Description: Codigo ascci en 8 bits
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
const unsigned char Fuente[] = {
    
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , // ' '
0x00, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x1e, 0x0c ,
0x00, 0x3f, 0x66, 0x66, 0x3e, 0x66, 0x66, 0x3f ,
0x00, 0x3c, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3c ,
0x00, 0x1f, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1f ,
0x00, 0x7f, 0x46, 0x16, 0x1e, 0x16, 0x46, 0x7f ,
0x00, 0x0f, 0x06, 0x16, 0x1e, 0x16, 0x46, 0x7f ,
0x00, 0x7c, 0x66, 0x73, 0x03, 0x03, 0x66, 0x3c ,
0x00, 0x33, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x33 ,
0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e ,
0x00, 0x1e, 0x33, 0x33, 0x30, 0x30, 0x30, 0x78 ,
0x00, 0x67, 0x66, 0x36, 0x1e, 0x36, 0x66, 0x67 ,
0x00, 0x7f, 0x66, 0x46, 0x06, 0x06, 0x06, 0x0f ,
0x00, 0x63, 0x63, 0x6b, 0x7f, 0x7f, 0x77, 0x63 ,
0x00, 0x63, 0x63, 0x73, 0x7b, 0x6f, 0x67, 0x63 ,
0x00, 0x1c, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1c ,
0x00, 0x0f, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3f ,
0x00, 0x38, 0x1e, 0x3b, 0x33, 0x33, 0x33, 0x1e ,
0x00, 0x67, 0x66, 0x36, 0x3e, 0x66, 0x66, 0x3f ,
0x00, 0x1e, 0x33, 0x38, 0x0e, 0x07, 0x33, 0x1e ,
0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x2d, 0x3f ,
0x00, 0x3f, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 ,
0x00, 0x0c, 0x1e, 0x33, 0x33, 0x33, 0x33, 0x33 ,
0x00, 0x63, 0x77, 0x7f, 0x6b, 0x63, 0x63, 0x63 ,
0x00, 0x63, 0x36, 0x1c, 0x1c, 0x36, 0x63, 0x63 ,
0x00, 0x1e, 0x0c, 0x0c, 0x1e, 0x33, 0x33, 0x33 ,
0x00, 0x7f, 0x66, 0x4c, 0x18, 0x31, 0x63, 0x7f ,
0x00, 0x6e, 0x33, 0x3e, 0x30, 0x1e, 0x00, 0x00 ,
0x00, 0x3b, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x07 ,
0x00, 0x1e, 0x33, 0x03, 0x33, 0x1e, 0x00, 0x00 ,
0x00, 0x6e, 0x33, 0x33, 0x3e, 0x30, 0x30, 0x38 ,
0x00, 0x1e, 0x03, 0x3f, 0x33, 0x1e, 0x00, 0x00 ,
0x00, 0x0f, 0x06, 0x06, 0x0f, 0x06, 0x36, 0x1c ,
0x1f, 0x30, 0x3e, 0x33, 0x33, 0x6e, 0x00, 0x00 ,
0x00, 0x67, 0x66, 0x66, 0x6e, 0x36, 0x06, 0x07 ,
0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0e, 0x00, 0x0c ,
0x1e, 0x33, 0x33, 0x30, 0x30, 0x30, 0x00, 0x30 ,
0x00, 0x67, 0x36, 0x1e, 0x36, 0x66, 0x06, 0x07 ,
0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0e ,
0x00, 0x63, 0x6b, 0x7f, 0x7f, 0x33, 0x00, 0x00 ,
0x00, 0x33, 0x33, 0x33, 0x33, 0x1f, 0x00, 0x00 ,
0x00, 0x1e, 0x33, 0x33, 0x33, 0x1e, 0x00, 0x00 ,
0x0f, 0x06, 0x3e, 0x66, 0x66, 0x3b, 0x00, 0x00 ,
0x78, 0x30, 0x3e, 0x33, 0x33, 0x6e, 0x00, 0x00 ,
0x00, 0x0f, 0x06, 0x66, 0x6e, 0x3b, 0x00, 0x00 ,
0x00, 0x1f, 0x30, 0x1e, 0x03, 0x3e, 0x00, 0x00 ,
0x00, 0x18, 0x2c, 0x0c, 0x0c, 0x3e, 0x0c, 0x08 ,
0x00, 0x6e, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00 ,
0x00, 0x0c, 0x1e, 0x33, 0x33, 0x33, 0x00, 0x00 ,
0x00, 0x36, 0x7f, 0x7f, 0x6b, 0x63, 0x00, 0x00 ,
0x00, 0x63, 0x36, 0x1c, 0x36, 0x63, 0x00, 0x00 ,
0x1f, 0x30, 0x3e, 0x33, 0x33, 0x33, 0x00, 0x00 ,
0x00, 0x3f, 0x26, 0x0c, 0x19, 0x3f, 0x00, 0x00 ,

};

/*unsigned char Fuente [800] = {
  
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , // ' '
   0x00, 0x06, 0x5F, 0x5F, 0x06, 0x00, 0x00, 0x00 , // '!'
   0x00, 0x07, 0x07, 0x00, 0x07, 0x07, 0x00, 0x00 , // '"'
   0x14, 0x7F, 0x7F, 0x14, 0x7F, 0x7F, 0x14, 0x00 , // '#'
   0x24, 0x2E, 0x6B, 0x6B, 0x3A, 0x12, 0x00, 0x00 , // '$'
   0x46, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x62, 0x00 , // '%'
   0x30, 0x7A, 0x4F, 0x5D, 0x37, 0x7A, 0x48, 0x00 , // '&'
   0x04, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 , // '''
   0x00, 0x1C, 0x3E, 0x63, 0x41, 0x00, 0x00, 0x00 , // '('
   0x00, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00, 0x00 , // ')'
   0x08, 0x2A, 0x3E, 0x1C, 0x1C, 0x3E, 0x2A, 0x08 , // '*'
   0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00, 0x00 , // '+'
   0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00 , // ','
   0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00 , // '-'
   0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00 , // '.'
   0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00 , // '/'
   0x3E, 0x7F, 0x71, 0x59, 0x4D, 0x7F, 0x3E, 0x00 , // '0'
   0x40, 0x42, 0x7F, 0x7F, 0x40, 0x40, 0x00, 0x00 , // '1'
   0x62, 0x73, 0x59, 0x49, 0x6F, 0x66, 0x00, 0x00 , // '2'
   0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00 , // '3'
   0x18, 0x1C, 0x16, 0x53, 0x7F, 0x7F, 0x50, 0x00 , // '4'
   0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00, 0x00 , // '5'
   0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30, 0x00, 0x00 , // '6'
   0x03, 0x03, 0x71, 0x79, 0x0F, 0x07, 0x00, 0x00 , // '7'
   0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00 , // '8'
   0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E, 0x00, 0x00 , // '9'
   0x00, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00 , // ':'
   0x00, 0x80, 0xE6, 0x66, 0x00, 0x00, 0x00, 0x00 , // ';'
   0x08, 0x1C, 0x36, 0x63, 0x41, 0x00, 0x00, 0x00 , // '<'
   0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00, 0x00 , // '='
   0x00, 0x41, 0x63, 0x36, 0x1C, 0x08, 0x00, 0x00 , // '>'
   0x02, 0x03, 0x51, 0x59, 0x0F, 0x06, 0x00, 0x00 , // '?'
   0x3E, 0x7F, 0x41, 0x5D, 0x5D, 0x1F, 0x1E, 0x00 , // '@'
   //0x7C, 0x7E, 0x13, 0x13, 0x7E, 0x7C, 0x00, 0x00 , // 'A'
   0x00, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x1e, 0x0c , // 'A'
   //0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , // 'A'
   0x41, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00 , // 'B'
   0x1C, 0x3E, 0x63, 0x41, 0x41, 0x63, 0x22, 0x00 , // 'C'
   0x41, 0x7F, 0x7F, 0x41, 0x63, 0x3E, 0x1C, 0x00 , // 'D'
   0x41, 0x7F, 0x7F, 0x49, 0x5D, 0x41, 0x63, 0x00 , // 'E'
   0x41, 0x7F, 0x7F, 0x49, 0x1D, 0x01, 0x03, 0x00 , // 'F'
   0x1C, 0x3E, 0x63, 0x41, 0x51, 0x73, 0x72, 0x00 , // 'G'
   0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00, 0x00 , // 'H'
   0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00, 0x00 , // 'I'
   0x30, 0x70, 0x40, 0x41, 0x7F, 0x3F, 0x01, 0x00 , // 'J'
   0x41, 0x7F, 0x7F, 0x08, 0x1C, 0x77, 0x63, 0x00 , // 'K'
   0x41, 0x7F, 0x7F, 0x41, 0x40, 0x60, 0x70, 0x00 , // 'L'
   0x7F, 0x7F, 0x0E, 0x1C, 0x0E, 0x7F, 0x7F, 0x00 , // 'M'
   0x7F, 0x7F, 0x06, 0x0C, 0x18, 0x7F, 0x7F, 0x00 , // 'N'
   0x1C, 0x3E, 0x63, 0x41, 0x63, 0x3E, 0x1C, 0x00 , // 'O'
   0x41, 0x7F, 0x7F, 0x49, 0x09, 0x0F, 0x06, 0x00 , // 'P'
   0x1E, 0x3F, 0x21, 0x71, 0x7F, 0x5E, 0x00, 0x00 , // 'Q'
   0x41, 0x7F, 0x7F, 0x09, 0x19, 0x7F, 0x66, 0x00 , // 'R'
   0x26, 0x6F, 0x4D, 0x59, 0x73, 0x32, 0x00, 0x00 , // 'S'
   0x03, 0x41, 0x7F, 0x7F, 0x41, 0x03, 0x00, 0x00 , // 'T'
   0x7F, 0x7F, 0x40, 0x40, 0x7F, 0x7F, 0x00, 0x00 , // 'U'
   0x1F, 0x3F, 0x60, 0x60, 0x3F, 0x1F, 0x00, 0x00 , // 'V'
   0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F, 0x00 , // 'W'
   0x43, 0x67, 0x3C, 0x18, 0x3C, 0x67, 0x43, 0x00 , // 'X'
   0x07, 0x4F, 0x78, 0x78, 0x4F, 0x07, 0x00, 0x00 , // 'Y'
   0x47, 0x63, 0x71, 0x59, 0x4D, 0x67, 0x73, 0x00 , // 'Z'
   0x00, 0x7F, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00 , // '['
   0x01, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00 , // backslash
   0x00, 0x41, 0x41, 0x7F, 0x7F, 0x00, 0x00, 0x00 , // ']'
   0x08, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x08, 0x00 , // '^'
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 , // '_'
   0x00, 0x00, 0x03, 0x07, 0x04, 0x00, 0x00, 0x00 , // '`'
   0x20, 0x74, 0x54, 0x54, 0x3C, 0x78, 0x40, 0x00 , // 'a'
   0x41, 0x7F, 0x3F, 0x48, 0x48, 0x78, 0x30, 0x00 , // 'b'
   0x38, 0x7C, 0x44, 0x44, 0x6C, 0x28, 0x00, 0x00 , // 'c'
   0x30, 0x78, 0x48, 0x49, 0x3F, 0x7F, 0x40, 0x00 , // 'd'
   0x38, 0x7C, 0x54, 0x54, 0x5C, 0x18, 0x00, 0x00 , // 'e'
   0x48, 0x7E, 0x7F, 0x49, 0x03, 0x02, 0x00, 0x00 , // 'f'
   0x98, 0xBC, 0xA4, 0xA4, 0xF8, 0x7C, 0x04, 0x00 , // 'g'
   0x41, 0x7F, 0x7F, 0x08, 0x04, 0x7C, 0x78, 0x00 , // 'h'
   0x00, 0x44, 0x7D, 0x7D, 0x40, 0x00, 0x00, 0x00 , // 'i'
   0x60, 0xE0, 0x80, 0x80, 0xFD, 0x7D, 0x00, 0x00 , // 'j'
   0x41, 0x7F, 0x7F, 0x10, 0x38, 0x6C, 0x44, 0x00 , // 'k'
   0x00, 0x41, 0x7F, 0x7F, 0x40, 0x00, 0x00, 0x00 , // 'l'
   0x7C, 0x7C, 0x18, 0x38, 0x1C, 0x7C, 0x78, 0x00 , // 'm'
   0x7C, 0x7C, 0x04, 0x04, 0x7C, 0x78, 0x00, 0x00 , // 'n'
   0x38, 0x7C, 0x44, 0x44, 0x7C, 0x38, 0x00, 0x00 , // 'o'
   0x84, 0xFC, 0xF8, 0xA4, 0x24, 0x3C, 0x18, 0x00 , // 'p'
   0x18, 0x3C, 0x24, 0xA4, 0xF8, 0xFC, 0x84, 0x00 , // 'q'
   0x44, 0x7C, 0x78, 0x4C, 0x04, 0x1C, 0x18, 0x00 , // 'r'
   0x48, 0x5C, 0x54, 0x54, 0x74, 0x24, 0x00, 0x00 , // 's'
   0x00, 0x04, 0x3E, 0x7F, 0x44, 0x24, 0x00, 0x00 , // 't'
   0x3C, 0x7C, 0x40, 0x40, 0x3C, 0x7C, 0x40, 0x00 , // 'u'
   0x1C, 0x3C, 0x60, 0x60, 0x3C, 0x1C, 0x00, 0x00 , // 'v'
   0x3C, 0x7C, 0x70, 0x38, 0x70, 0x7C, 0x3C, 0x00 , // 'w'
   0x44, 0x6C, 0x38, 0x10, 0x38, 0x6C, 0x44, 0x00 , // 'x'
   0x9C, 0xBC, 0xA0, 0xA0, 0xFC, 0x7C, 0x00, 0x00 , // 'y'
   0x4C, 0x64, 0x74, 0x5C, 0x4C, 0x64, 0x00, 0x00 , // 'z'
   0x08, 0x08, 0x3E, 0x77, 0x41, 0x41, 0x00, 0x00 , // '{'
   0x00, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00, 0x00 , // '|'
   0x41, 0x41, 0x77, 0x3E, 0x08, 0x08, 0x00, 0x00 , // '}'
   0x02, 0x03, 0x01, 0x03, 0x02, 0x03, 0x01, 0x00 , // '~'
}; //  end of MAX7219_Dot_Matrix_font
*/


/*
*********************************************************************************************************
* MAX7219_LookupCode()
*
* Description: Convert an alphanumeric character to the corresponding 7-segment code.
* Arguments  : character to display
* Returns    : segment code
*********************************************************************************************************
*/


 void WriteChar7219 (char n,unsigned char myChar)
 {
    unsigned char Column;
    int   Start_Byte;
    
    Start_Byte = ((myChar-64) *8);// 65 represents the letter "A" in ASCII code.
    
    // We are using only columns from 2 through 7 FOR displaying the character.
    for (Column = 8; Column > 0 ; Column--)
    {
        MAX7219_Write (n,Column, Fuente[Start_Byte++]);
    }
 }
 
 

 
 void Enviar_MAX72xx(unsigned char num,unsigned char direc,unsigned char dato ){
    unsigned char MAX_DATO[10]={0,0,0,0,0,0,0,0,0,0};  
    unsigned char rep;
    unsigned char cont;
    rep = 1*2;
    MAX_DATO[num*2-1]=direc;
    MAX_DATO[num*2-2]=dato;
    LOAD_0(); 
    for(cont=rep; cont>0;cont--){
      SPI_MAX72xx(MAX_DATO[cont-1]);
    }
    LOAD_1(); 
}

/*
*********************************************************************************************************
* MAX7219_Write()
*
* Description: Write to MAX7219
* Arguments  : reg_number = register to write to
*              dataout = data to write to MAX7219
* Returns    : none
*********************************************************************************************************
*/
static void MAX7219_Write (char n,unsigned char reg_number, unsigned char dataout)
{
  char i;
  if(n==1)
  {
  LOAD_1();                                           // take LOAD high to begin
  MAX7219_SendByte(reg_number);                       // write register number to MAX7219
  MAX7219_SendByte(dataout);                          // write data to MAX7219
  LOAD_0();                                           // take LOAD low to latch in data
  LOAD_1();                                           // take LOAD high to end
  }
  if(n>1)
  {
  LOAD_1();                                           // take LOAD high to begin
  MAX7219_SendByte(reg_number);                       // write register number to MAX7219
  MAX7219_SendByte(dataout);                          // write data to MAX7219
  for(i=2;i<=n;i++)
  {
  MAX7219_SendByte(NO_OP);                       // write register number to MAX7219
  MAX7219_SendByte(0x00);
  }
  LOAD_0();                                           // take LOAD low to latch in data
  LOAD_1(); 
  }
}


/*
*********************************************************************************************************
* MAX7219_SendByte()
*
* Description: Send one byte to the MAX7219
* Arguments  : dataout = data to send
* Returns    : none
*********************************************************************************************************
*/
static void MAX7219_SendByte (unsigned char dataout)
{
  char i;
  for (i=8; i>0; i--) {
    unsigned char mask = 1 << (i - 1);                // calculate bitmask
    CLK_0();                                          // bring CLK low
    if (dataout & mask)                               // output one data bit
      DATA_1();                                       //  "1"
    else                                              //  or
      DATA_0();                                       //  "0"
    CLK_1();                                          // bring CLK high
	}
}



char Vram[80]={
    0, 0, 0, 0, 0, 0, 0, 0 ,//1
    0, 0, 0, 0, 0, 0, 0, 0 ,//2
    0, 0, 0, 0, 0, 0, 0, 0 ,//3
    0, 0, 0, 0, 0, 0, 0, 0 ,//4
    0, 0, 0, 0, 0, 0, 0, 0 ,//5
    0, 0, 0, 0, 0, 0, 0, 0 ,//6
    0, 0, 0, 0, 0, 0, 0, 0 ,//7
    0, 0, 0, 0, 0, 0, 0, 0 ,//8
    0, 0, 0, 0, 0, 0, 0, 0 ,//9
    0, 0, 0, 0, 0, 0, 0, 0 ,//10

};

void WriteTexto(char texto,char n){
    
    char A[8];
    int Inicio;
    Inicio = (texto-64)*8;
    for (char i=0; i<8; i++){
        A[i]= Fuente[Inicio++];
    }
    char uvram = n+1;
        for (char j=0; j<8; j++){
            
            Vram[8*uvram+j] = A[j];
        }
    
   

   }

void Mostrar(){
    char *pm4, *pm3, *pm2, *pm1; 
    char matrix_4, matrix_3, matrix_2, matrix_1;
    int cont;
    char x;
    pm4= (void*)&Vram[0]; 
    pm3= (void*)&Vram[8]; 
    pm2= (void*)&Vram[16]; 
    pm1= (void*)&Vram[24]; 
    for (cont=0; cont<48; cont++){
        for(x=1; x<9; x++){
            matrix_4=*pm4;
            MAX7219_Write(4,x,matrix_4);
            pm4=pm4+1;
            matrix_3=*pm3;
            MAX7219_Write(3,x,matrix_3);
            pm3=pm3+1;
            matrix_2=*pm2;
            MAX7219_Write(2,x,matrix_2);
            pm2=pm2+1;
            matrix_1=*pm1;
            MAX7219_Write(1,x,matrix_1);
            pm1=pm1+1;
        }
                               // delay sets speed of scrolling 
            pm4 = (void*)&Vram[cont + 8];     // allow the scrolling feature 
            pm3 = (void*)&Vram[cont + 16]; 
            pm2 = (void*)&Vram[cont + 24]; 
            pm1 = (void*)&Vram[cont + 32]; 
            if ((void*)*pm1 == "\0")            // when the pointer pm1 finds the null character 
            {                                   // at the end of message[], it resets all pointer      // addresses and clears counter variable cnt 
                pm4 = (void*)&Vram[8]; 
                pm3 = (void*)&Vram[16]; 
                pm2 = (void*)&Vram[24]; 
                pm1 = (void*)&Vram[32]; 
                cont = 0; 
    }
}
}

