
#ifndef SERIAL_H
#define	SERIAL_H

#include <xc.h>
#include <stdbool.h>

unsigned char bufferSerial[100] = "";
unsigned char _BF_start[10] = "";
unsigned bool _BF_enable_receive = false;
unsigned char _BF_i_count = 0;
unsigned char _BF_current = 0;

void setKeyStartReceiveSerial(char start[]);
void receiveByteSerial(char byte);
bool findBufferSerial(char key[]);

#endif	

