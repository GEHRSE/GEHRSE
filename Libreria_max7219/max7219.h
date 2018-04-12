/* 
 * File:   max7219.h
 * Author: camil
 *
 * Created on 8 de abril de 2018, 01:28 PM
 */

#ifndef MAX7219_H
#define	MAX7219_H
#include <xc.h> // include processor files - each processor file is guarded.

void MAX7219_Init (char n);
void MAX7219_ShutdownStart (char n);
void MAX7219_ShutdownStop (char n);
void MAX7219_DisplayTestStart (char n);
void MAX7219_DisplayTestStop (char n);
void MAX7219_SetBrightness (char n ,char brightness);
void MAX7219_Clear (char n);
void Mostrar(void);
void WriteTexto(char texto,char n);
void WriteChar7219 (char n,unsigned char myChar);
void Enviar_MAX72xx(unsigned char num,unsigned char direc,unsigned char dato );

#endif	/* MAX7219_H */

