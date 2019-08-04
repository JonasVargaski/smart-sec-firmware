#ifndef I2C_H
#define	I2C_H
#ifdef	__cplusplus
#endif 


/*
 * File:   I2C.h
 * Author: Jonas Vargaski
 *
 * Created on 11 de Julho de 2018, 19:08
 */
void I2C_Master_Init(const unsigned long c);
void I2C_Wait();
void I2C_Start();
void I2C_Restart();
void I2C_Stop();
void I2C_Write(unsigned d);
unsigned short I2C_Read(unsigned short a);





#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* XC_HEADER_TEMPLATE_H */


