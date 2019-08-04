#ifndef LCD_H
#define	LCD_H
#ifdef	__cplusplus
#endif 

#include "config.h"
#include "function.h"

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char* a);
void Lcd_Set_Cursor(char a, char b);
void lcd_Init();
//void Lcd_Clear();

void Lcd_Init() {

#define _XTAL_FREQ 8000000

    TRISDbits.RD7 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD5 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD2 = 0;

#define RS PORTDbits.RD2
#define EN PORTDbits.RD3
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(11);
    Lcd_Cmd(0x03);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06);
}

void Lcd_Port(char a) {
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    switch (a) {
        case 1:
            temp = 0x80 + b - 1;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 2:
            temp = 0xC0 + b - 1;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 3:
            temp = 0x94 + b;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 4:
            temp = 0xD4 + b;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        default:return;
    }
}

void Lcd_Write_Char(char a) {
    char tempp, y;
    tempp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    Lcd_Port(tempp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a) {
    unsigned char i = 0;
    //for (i = 0; a[i] != '\0' && i < 20; i++) { // anterior nao funciona a posicao 0 CGRAM
    for (i = 0; i < 20; i++) { // anterior nao funciona a posicao 0 CGRAM
        Lcd_Write_Char(a[i]);
        //        Lcd_Write_Char((a[i] != '\0') ? a[i] : ' ');
    }
}
/*
void Lcd_Shift_Right() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}
 */

//Atualiza LCD 20x4

void atualizaLCD(char *line1, char *line2, char *line3, char *line4) {


    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String(line2);
    Lcd_Set_Cursor(3, 0);
    Lcd_Write_String(line3);
    Lcd_Set_Cursor(4, 0);
    Lcd_Write_String(line4);

    char i = 0;
    for (i = 0; i < 20; i++) {
        line1[i] = ' ';
        line2[i] = ' ';
        line3[i] = ' ';
        line4[i] = ' ';
    }
}

void criaCaracteresCGRAM() {
#define ICO_CADEADO 0x01 
#define ICO_BUZZER 0x02 
#define ICO_UMID 0x03 
#define ICO_TEMP 0x04 
#define ICO_WIFI 0x05

    const char character[7][8] = {
        14, 17, 17, 31, 27, 27, 31, 31, //  cadeado 0x01    
        1, 3, 7, 29, 29, 7, 3, 1, // buzzer  0x02
        4, 4, 14, 14, 31, 29, 27, 14, // gota umidade 0x03
        4, 10, 10, 10, 14, 31, 31, 14, // ternometro 0x04
        // 10, 10, 31, 31, 31, 14, 4, 4, // tomada 
        0, 14, 17, 4, 10, 0, 4, 0 // wifi   0x05
        //4,14,31,4,4,4,4,0, //seta p cima
        //0,4,4,4,4,31,14,4 // seta p baixo 
    };
    Lcd_Cmd(0b0100);
    Lcd_Cmd(0b0000);
    __delay_us(50);
    char i = 0, j = 0, k = 0;
    for (i = 0; i < 5; i++) {
        if (k == 1) i = 0;
        for (j = 0; j < 8; j++) {
            Lcd_Write_Char((k) ? character[i][j] : 0); // adianta um valor da CGRAM
        }
        k++;
    }
}

//Pisca display na linha e espaços passados

void blink(char *line, char start, char end) {
    if (blinkk) {
        char i = 0;
        for (i = (--start); i < end; i++) {
            line[i] = ' ';
        }
    }
}






#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

