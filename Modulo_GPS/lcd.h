#ifndef LCD_H
#define	LCD_H

#include <stdlib.h>
#define _XTAL_FREQ 8000000

//Macros para los pines de conexi�n de la LCD
#define LCD_RD7     PORTDbits.RD7       // D7
#define TRISRD7     TRISDbits.TRISD7

#define LCD_RD6     PORTDbits.RD6       // D6
#define TRISRD6     TRISDbits.TRISD6

#define LCD_RD5     PORTDbits.RD5       // D5
#define TRISRD5     TRISDbits.TRISD5

#define LCD_RD4     PORTDbits.RD4       // D4
#define TRISRD4     TRISDbits.TRISD4

#define LCD_EN      PORTDbits.RD0       // EN
#define TRISEN      TRISDbits.TRISD0

#define LCD_RS      PORTDbits.RD1       // RS
#define TRISRS      TRISDbits.TRISD1

//Macros para los comandos disponibles
#define      LCD_FIRST_ROW           128
#define      LCD_SECOND_ROW          192
#define      LCD_THIRD_ROW           148
#define      LCD_FOURTH_ROW          212
#define      LCD_CLEAR               1
#define      LCD_RETURN_HOME         2
#define      LCD_CURSOR_OFF          12
#define      LCD_BLINK_CURSOR_OFF    14
#define      LCD_BLINK_CURSOR_ON     15
#define      LCD_MOVE_CURSOR_LEFT    16
#define      LCD_MOVE_CURSOR_RIGHT   20
#define      LCD_TURN_OFF            0
#define      LCD_TURN_ON             8
#define      LCD_SHIFT_LEFT          24
#define      LCD_SHIFT_RIGHT         28

//******************************************************************************
//Prototipos de funciones
void Lcd_Init(void);
void Lcd_Out(unsigned char y, unsigned char x, const char *buffer);
void Lcd_Out2(unsigned char y, unsigned char x, char *buffer);
void Lcd_Chr_CP(char data);
void Lcd_Cmd(unsigned char data);
void Lcd_string(const char *str);
void Lcd_Write_Float(float f);

//******************************************************************************
void Lcd_Init(void)
{
    unsigned char data;
    TRISRD7 = 0;
    TRISRD6 = 0;
    TRISRD5 = 0;
    TRISRD4 = 0;
    TRISEN = 0;
    TRISRS = 0;
    LCD_RD7 = 0;
    LCD_RD6 = 0;
    LCD_RD5 = 0;
    LCD_RD4 = 0;
    LCD_EN = 0;
    LCD_RS = 0;
    __delay_us(5500);
    __delay_us(5500);
    __delay_us(5500);
    __delay_us(5500);
    __delay_us(5500);
    __delay_us(5500);
    
    for(data = 1; data < 4; data ++)
    {
        LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1; LCD_EN = 0;
        LCD_RS = 0; LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1;
        LCD_EN = 1; LCD_RS = 0;
        __delay_us(5);
        LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1; LCD_EN = 0;
        LCD_RS = 0;
        __delay_us(5500);
    }
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 0; LCD_RS = 0;
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 1; LCD_RS = 0;
    __delay_us(5);
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 0; LCD_RS = 0;
    __delay_us(5500);
    data = 40; Lcd_Cmd(data);
    data = 16; Lcd_Cmd(data);
    data = 1;  Lcd_Cmd(data);
    data = 15; Lcd_Cmd(data);
}
//******************************************************************************
void Lcd_Out(unsigned char y, unsigned char x, const char *buffer)
{
    unsigned char data;

    switch (y)
    {
        case 1: data = 128 + x; break;
        case 2: data = 192 + x; break;
        case 3: data = 148 + x; break;
        case 4: data = 212 + x; break;
        default: break;
    }
    Lcd_Cmd(data);
    while(*buffer)              // Write data to LCD up to null
    {
        Lcd_Chr_CP(*buffer);
        buffer++;               // Increment buffer
    }
    return;
}
//******************************************************************************
void Lcd_Out2(unsigned char y, unsigned char x, char *buffer)
{
    unsigned char data;

    switch (y)
    {
        case 1: data = 128 + x; break;
        case 2: data = 192 + x; break;
        case 3: data = 148 + x; break;
        case 4: data = 212 + x; break;
        default: break;
    }
    Lcd_Cmd(data);
    while(*buffer)              // Write data to LCD up to null
    {
        Lcd_Chr_CP(*buffer);
        buffer++;               // Increment buffer
    }
    return;
}
//******************************************************************************
void Lcd_Chr_CP(char data)
{
    if (data=='�') data=238;
    LCD_EN = 0; LCD_RS = 1;
    LCD_RD7 = (data & 0b10000000)>>7; LCD_RD6 = (data & 0b01000000)>>6;
    LCD_RD5 = (data & 0b00100000)>>5; LCD_RD4 = (data & 0b00010000)>>4;
    _delay(10);
    LCD_EN = 1; __delay_us(5); LCD_EN = 0;
    LCD_RD7 = (data & 0b00001000)>>3; LCD_RD6 = (data & 0b00000100)>>2;
    LCD_RD5 = (data & 0b00000010)>>1; LCD_RD4 = (data & 0b00000001);
    _delay(10);
    LCD_EN = 1; __delay_us(5); LCD_EN = 0;
    __delay_us(5); __delay_us(5500);
}
//******************************************************************************
void Lcd_Cmd(unsigned char data)
{
    LCD_EN = 0; LCD_RS = 0;
    LCD_RD7 = (data & 0b10000000)>>7; LCD_RD6 = (data & 0b01000000)>>6;
    LCD_RD5 = (data & 0b00100000)>>5; LCD_RD4 = (data & 0b00010000)>>4;
    _delay(10);
    LCD_EN = 1; __delay_us(5); LCD_EN = 0;
    LCD_RD7 = (data & 0b00001000)>>3; LCD_RD6 = (data & 0b00000100)>>2;
    LCD_RD5 = (data & 0b00000010)>>1; LCD_RD4 = (data & 0b00000001);
    _delay(10);
    LCD_EN = 1; __delay_us(5); LCD_EN = 0;
    __delay_us(5500);//Delay_5us();
}
//******************************************************************************
void LCD_String (const char *str)							/* Send string to LCD function */
{
	int i;
	while((*str)!=0)							/* Send each char of string till the NULL */
	{
		Lcd_Chr_CP (*str);							/* Call LCD data write */
        str++;
    }
}

#endif	/* LCD_H */

void Lcd_Write_Float(float f)
{
char* buf11;
int status;
buf11 = ftoa(f, &status);
LCD_String(buf11);
}