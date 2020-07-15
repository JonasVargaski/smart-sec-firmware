#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serial.h"

void setKeyStartReceiveSerial(char start[]) {
    _BF_enable_receive = false;
    _BF_i_count = 0;
    _BF_current = 0;
    memset(bufferSerial, 0, sizeof (bufferSerial));
    sprintf(_BF_start, "%s", start);
    if(strstr("ALL", start)){
        _BF_enable_receive = true;
    }
}

void receiveByteSerial(char byte) {
    if (_BF_enable_receive) {
        bufferSerial[_BF_current++] = byte;
        if (_BF_current >= 100) {
            _BF_current = 0;
        }
    } else if (_BF_start[_BF_i_count] == byte) {
        _BF_i_count++;
        if(_BF_i_count >= strlen(_BF_start)){
            _BF_enable_receive = true;
        }
    } else {
        _BF_i_count = 0;
    }
}

bool findBufferSerial(char key[]){
    return strstr(bufferSerial, key) != NULL;
}