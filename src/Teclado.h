#ifndef TECLADO_H
#define	TECLADO_H

#include "I2C.h"

#define B_MENU 8
#define B_MENOS 32
#define B_MAIS 64
#define B_OK 2
#define B_CIMA 1
#define B_BAIXO 16
#define B_SILENCIA 4

unsigned char bt_flag = 0;

void readButtons(void);
char btPress(char bt);

void readButtons() {
    I2C_Start();
    I2C_Write(0x4F);
    bt_flag = ~I2C_Read(0);
    I2C_Stop();
}

char btPress(char bt) {
    if (bt_flag == bt) {
        if (PORTBbits.RB2) {//Somente quando for solto o botao
            bt_flag = 0;
            return 1;
        }
    }
    return 0;
}


#endif	/* XC_HEADER_TEMPLATE_H */

