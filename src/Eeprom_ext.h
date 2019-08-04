#ifndef EEPROM_EXT_H
#define	EEPROM_EXT_H
#ifdef	__cplusplus
#endif 

#include "I2C.h"
#include "config.h"

#define SLAVE_ADRESS_WRITE 0xA0
#define SLAVE_ADRESS_READ 0xA1


//SALVAR DADOS NA EEMPROM (ENDERE�O, VALOR))
void writeEEPROM_ext(unsigned char adress, unsigned char data) {

    I2C_Start();
    I2C_Write(SLAVE_ADRESS_WRITE);
    //I2C_Write(adress >> 8);     // SOMENTE PARA 24LC256
    I2C_Write(adress);
    I2C_Write(data);
    I2C_Stop();
    delay(105); // tempo necessario entre escritas e grava�oes
}

//LER DADOS DA EEPROM,  (Endere�o))
unsigned char readEEPROM_ext(char adress) {
    I2C_Start();
    I2C_Write(SLAVE_ADRESS_WRITE);
    //I2C_Write(adress >> 8);    //   SOMENTE PARA 24LC256
    I2C_Write(adress);
    I2C_Restart();
    I2C_Write(SLAVE_ADRESS_READ);
    unsigned char data = I2C_Read(0);
    I2C_Stop();
    return data;
}

//SALVAR DADOS de 16bits  NA EEMPROM (ENDERE�O, VALOR))
void writeEEPROM16_ext(unsigned char adress, unsigned int data) {
    writeEEPROM_ext(adress, data);
    data>> = 8;
    writeEEPROM_ext(adress + 1, data);
}

//LER DADOS de 16bits DA EEPROM,  (Endere�o))
unsigned int readEEPROM16_ext(unsigned char adress) {
    unsigned int data;
    data = readEEPROM_ext(adress + 1);
    data <<= 8;
    data |= readEEPROM_ext(adress);
    return data;
}





#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

