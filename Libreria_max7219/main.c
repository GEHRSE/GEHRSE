/*
 * File:   main.c
 * Author: camil
 *
 * Created on 8 de abril de 2018, 01:29 PM
 */


#include <xc.h>
#include "max7219.h"
#include "Configuration_Bits.h"

const char men[6]={"GHRSE"};

void main(void) {
    MAX7219_Init(4);
    for (char i=0; i<7;i++){
        WriteTexto(men[i],i+1);
        }
    while (1){
       Mostrar();
     //// WriteChar7219(2,'A');
       // WriteChar7219(3,'A');
       // WriteChar7219(4,'A');
    }
    return;
}
