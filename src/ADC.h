#include "config.h"
#include "function.h"
#include "variaveisGlobais.h"
#include "controle.h "

unsigned char media_adc = 0;
float _temp[11] = 0;
float _umid[11] = 0;

unsigned int readADC(char canal) {
    ADRESH = 0;
    ADRESL = 0;
    int digital;
    ADCON0 = (ADCON0 & 0b11000011) | ((canal << 2) & 0b00111100);
    __delay_us(30);
    ADCON0 |= ((1 << ADON) | (1 << GO));
    while (ADCON0bits.GO_nDONE == 1);
    digital = (ADRESH * 256) | (ADRESL);
    return (digital);
}

getSensors() {
    unsigned char i = 0;
    float aux = 0;

    if (t_leitura_adc == 0) {
        t_leitura_adc = 10; //1000 ms cada leitura
        media_adc = (media_adc >= 10) ? 0 : ++media_adc;


        aux = 0;
        for (i = 0; i < 12; i++) {
            aux += (((readADC(0)* 0.48828125)*1.8) + 32); // graus F
        }
        aux = (aux / 12);
        _temp[media_adc] = (aux <= 37) ? 0 : aux+1; ///+1 para corrigir leitura 

        /////////////////////////////////////////////////////////////////////////

        aux = 0;
        if (tipo_sensor_umidade == 1) { // %
            float auxx = 0;
            for (i = 0; i < 12; i++) {
                auxx = (((readADC(1)*0.0048828125) - 0.826) / 0.0315);
                if (auxx < 10) {
                    aux += 0;
                } else {
                    aux += auxx;
                }
            }
        } else { // F
            for (i = 0; i < 12; i++) {
                aux += (((readADC(2)* 0.48828125)*1.8) + 32); // graus F
            }
        }

        aux = (aux / 12);
        if (tipo_sensor_umidade == 1) {
            if (aux > 100) aux = 100;
            if (aux < 10) aux = 0;
        }
        _umid[media_adc] = (aux < 40 && tipo_sensor_umidade == 0) ? 0 : aux;
    }

    ////////// - - TEMPERATURA - - ////////////

    i = 0;
    aux = 0;
    for (i = 0; i < 10; i++) {
        aux += _temp[i];
    }
    temp = (aux / 10); //TEMPERATURA

    ////////// - - UMIDADE - - ////////////

    i = 0;
    aux = 0;
    for (i = 0; i < 10; i++) {
        aux += _umid[i];
    }
    umid = (aux / 10); //UMIDADE 
}