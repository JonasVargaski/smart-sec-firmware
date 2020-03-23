#ifndef FUNCOESCONTROLADOR_H
#define	FUNCOESCONTROLADOR_H

#include "TabelasUmidade.h";
#include "variaveisGlobais.h" 
#include "Eeprom_ext.h"

char *faseAtual(char tp, char *_fase, char *_tempajt) {
    char _fasee = *_fase;
    char tmpajt = *_tempajt;
    if (tp == '?') { // Retorna nome da fase atual
        switch (_fasee) {
            case 1: return "Amarelacao";
            case 2: return "Murchamento";
            case 3: return "Sec. Folha";
            case 4: return "Sec. Talo";
            default: return "- -";
        }
    } else
        if (tp == 'F') { // Altera numero da fase de acordo com a temperatura
        if (tmpajt <= 100) *_fase = 1;
        else if (tmpajt >= 101 && tmpajt <= 110) *_fase = 2;
        else if (tmpajt >= 111 && tmpajt <= 133) *_fase = 3;
        else if (tmpajt >= 134 && tmpajt <= 160) *_fase = 4;
    } else
        if (tp == 'A') { //quando Alterar a fase seta a temperatura
        if (fase == 1) *_tempajt = 85;
        else if (fase == 2) *_tempajt = 101;
        else if (fase == 3) *_tempajt = 111;
        else if (fase == 4) *_tempajt = 134;
    }
}

char *climaAtual(char c) {
    switch (c) {

        case 1:return "Umido";
        case 2:return "Normal";
        case 3:return "Seco";
        default: return "--";
    }
}

void setUmidade(char tp, char *_umdajt, char *_tempajt, char *_clima) {
    char _umd = *_umdajt;
    char _tmpajt = *_tempajt;
    char _clim = *_clima;

    switch (tp) {
        case '+':
            if (tipo_sensor_umidade == 1) {
                _umd = (_umd >= 100) ? 10 : ++_umd;
            } else {
                _umd = (_umd >= 120) ? 60 : ++_umd;
            }
            writeEEPROM_ext(2, _umd);

            break;
        case '-':
            if (tipo_sensor_umidade == 1) {
                _umd = (_umd <= 10) ? 100 : --_umd;
            } else {
                _umd = (_umd <= 60) ? 120 : --_umd;
            }
            writeEEPROM_ext(2, _umd);

            break;
        case 'A':
            _umd = tabelaUmidade(_tmpajt, _clim, tipo_sensor_umidade);
            break;
    }
    // NAO ESTA SALVANO QUANDO NO AUTOMATICO
    *_umdajt = _umd;
}

void setRangeValor(char _acao, char *_var_ajt, char _min, char _max, char end_eeprom) {
    int _aux = *_var_ajt;
    switch (_acao) {
        case '+':
            _aux = (_aux >= _max) ? _min : ++_aux;
            break;
        case '-':
            _aux = (_aux <= _min) ? _max : --_aux;
            break;
    }
    if (end_eeprom != 0) { // passar 0 para nao salvar
        writeEEPROM_ext(end_eeprom, _aux);
    }
    *_var_ajt = _aux;
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
#endif /* __cplusplus */
#endif	/* XC_HEADER_TEMPLATE_H */

