
#ifndef LED_DISPLAY_H
#define	LED_DISPLAY_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include "I2C.h"

//NUMERO CORRESPONDENTE DOS LEDS
char leds = 0;
char _led = 1;

#define led_1 32
#define led_2 16
#define led_3 1
#define led_4 2
#define led_5 4
#define led_6 8

void setLed(char led, char acao) {
    if (acao) {
        leds = leds | led;

    } else {
        leds = leds&~led;
    }
}

void updateLeds() {
    //atualiza led somente quando modificar
    if (_led != leds) {
        I2C_Start();
        I2C_Write(0x46);
        I2C_Write(((char) leds));
        I2C_Stop();
        _led = leds;
    }
}



#endif	/* LED_DISPLAY_H */

