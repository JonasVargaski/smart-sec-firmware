
#ifndef CONEXAOWIFI_H
#define	CONEXAOWIFI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <string.h>

#include "variaveisGlobais.h"
#include "function.h"
#include "Eeprom_ext.h"
#include "Controle.h"

unsigned char t_wifi_restart = 0;
unsigned char f_wifi_processo = 0;
unsigned char cbf = 0;
unsigned char bufferSerial[80];
unsigned char ssid[15];
unsigned char senha[15];

void limpaBuffer() {
    delay(5);
    memset(bufferSerial, 0, sizeof (bufferSerial));
    cbf = 0;
    delay(5);
}

void getMac(char mac[], char var[]) {
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char men = 0;
    for (i = 0; i < strlen(mac); i++) {
        if (mac[i] == '"' && men == 0) {
            men = 1;
        } else if (mac[i] == '"' && men == 1) {
            var[j] = '\0';
            return;
        } else if (men && mac[i] != ':' && j < 12) {
            var[j++] = mac[i];
        }
    }
}

void filtroStr(char inicio[], char fim[], char str[], char strdest[]) {
    char i = 0;
    char x = 0;
    char y = 0;
    char count = 0;
    char *p;
    char _str[82];
    char aux[60];

    strncpy(_str, str, 80);
    p = strstr(_str, inicio);
    strcpy(p, strtok(p, fim));
    sprintf(aux, "%s", p);

    x = strlen(inicio);
    y = strlen(aux);

    for (i = x; i < (x + y); i++) {
        strdest[count++] = (char) (aux[i] == '+') ? ' ' : aux[i];
    }
    sprintf(strdest, "%s", strdest);
}

void alterarConfigWifi() {

    if (t_wifi_restart == 0) {
        menu = 8;
        opcaomenu = 1;
        return;
    } else { // tempo para reiniciar o modulo wifi
        delay(10);
        switch (f_wifi_processo) {
            case 0:
                limpaBuffer();
                printf("AT+CWMODE=2\r\n");
                wifi_conectado = 0;
                f_wifi_processo++;
                break;
            case 1:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 2:
                limpaBuffer();
                printf("AT+CIPMUX=1\r\n");
                f_wifi_processo++;
                break;
            case 3:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 4:
                limpaBuffer();
                printf("AT+CWSAP=\"CONTROLADOR\",\"\",11,0,4\r\n");
                f_wifi_processo++;
                break;
            case 5:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 6:
                limpaBuffer();
                printf("AT+CIPAP=\"10.1.1.1\"\r\n");
                f_wifi_processo++;
                break;
            case 7:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 8:
                limpaBuffer();
                printf("AT+CIPSERVER=1,8080\r\n");
                f_wifi_processo++;
                break;
            case 9:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                    t_wifi_restart = 180; // 3 minutos para se conectar a rede!!
                }
                break;
            case 10:
                sprintf(line1, "Conecte-se a rede e");
                sprintf(line2, "acesse o IP para");
                sprintf(line3, "configurar o WIFI...");
                sprintf(line4, "10.1.1.1:8080");
                if ((strstr(bufferSerial, "+IPD") != NULL) || (strstr(bufferSerial, "Accept") != NULL) || (strstr(bufferSerial, "CONNECT") != NULL)) {
                    delay(400);
                    f_wifi_processo++;
                }
                break;
            case 11:
                limpaBuffer();
                printf("AT+CIPSENDEX=0,800\r\n");
                f_wifi_processo++;
                break;
            case 12:
                if (strstr(bufferSerial, ">") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 13:
                limpaBuffer();
                printf("HTTP 1.1/200 OK\r\nContent-type: text/html\r\n\r\n");
                delay(100);
                printf("<html><head><title>WIFI</title><style>*{font-size: 35px;font-family:sans-serif;}form{height:450px;display:flex;flex-direction:column;justify-content:space-around;align-items:center;margin:0 auto;}button{width:280px;height:60px;color:White;background-color:#4FCE00;border: 2px rgb(21, 196, 21) solid;border-radius:5px;}input{height:60px;width:600px; border-radius:5px;border: 3px rgb(132, 136, 138) solid;}body{background:#F4F7F5;}</style></head><body><form action=\"/wifi\"method=\"post\"><h1>Configuração Wifi</h1>NOME DO WIFI:<input type=\"text\"name=\"ssid\" required>SENHA DO WIFI:<input type=\"text\"name=\"senha\"><input type=\"hidden\" name=\"fimstr\"><button type=\"submit\">OK</button></form></body></html>\r\n\r\n\\0");
                delay(800); // tempo para envio da pagina
                printf("AT+CIPCLOSE=0\r\n");
                delay(300);
                t_wifi_restart = 180; // 3 minutos para colocar dados da rede!!
                limpaBuffer();
                f_wifi_processo++;
                break;
            case 14:
                sprintf(line4, "Aguardando...");
                if ((strstr(bufferSerial, "ssid") != NULL) && (strstr(bufferSerial, "fimstr") != NULL)) {
                    filtroStr("ssid=", "&", bufferSerial, ssid);
                    filtroStr("senha=", "&", bufferSerial, senha);

                    sprintf(line1, "%s", ssid);
                    sprintf(line2, "%s", senha);
                    sprintf(line4, "OK...        ");
                    atualizaLCD(line1, line2, line3, line4);

                    if (strlen(ssid) > 3) {
                        unsigned char i = 0;
                        for (i = 0; i < 15; i++) {
                            writeEEPROM_ext((40 + i), ssid[i]);
                        }
                        for (i = 0; i < 15; i++) {
                            writeEEPROM_ext((60 + i), senha[i]);
                        }
                    }
                    while (1) { // parta reiniciar o controlador. WATCHDOG
                    }
                }
                break;
            default: f_wifi_processo = 0;
        }
    }
}

void enviaDadosWifi() {
    if (t_wifi_restart == 0) {
        f_wifi_processo = 0;
        t_wifi_restart = 10;
    } else if (strlen(wifi_MAC) == 12) {
        switch (f_wifi_processo) {
            case 0:
                limpaBuffer();
                printf("AT+CWMODE=1\r\n");
                wifi_conectado = 0;
                f_wifi_processo++;
                break;
            case 1:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 2:
                limpaBuffer();
                printf("AT+CIPMUX=0\r\n");
                f_wifi_processo++;
                break;
            case 3:
                if (strstr(bufferSerial, "OK") != NULL | strstr(bufferSerial, "link") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 4:
                limpaBuffer();
                delay(10);
                printf("AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, senha);
                f_wifi_processo++;
                t_wifi_restart = 35; // Aguarda 35 segundos até conectar
                break;
            case 5:
                //if (strstr(bufferSerial, "CON") || strstr(bufferSerial, "OK") || strstr(bufferSerial,"GOT")) {
                f_wifi_processo++;
                // }
                break;
            case 6:
                limpaBuffer();
                if (t_wifi_restart <= 15) { //ENVIA A CADA 10 SEGUNDOS OS DADOS, tem 10 segundos para enviar, caso de erro, reinicia o wifi
                    printf("AT+CIPSTATUS\r\n");
                    f_wifi_processo++;
                }
                break;
            case 7:
                if (strstr(bufferSerial, "2") != NULL | strstr(bufferSerial, "4") != NULL) {
                    wifi_conectado = 1;
                    f_wifi_processo++;
                }
                break;
            case 8:
                limpaBuffer();
//             printf("AT+CIPSTART=\"TCP\",\"192.168.0.125\",3333\r\n"); /// trocar servidor;
               printf("AT+CIPSTART=\"TCP\",\"18.228.154.248\",80\r\n"); /// trocar servidor;
                f_wifi_processo++;
                break;
            case 9:
                if (strstr(bufferSerial, "OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 10:
                limpaBuffer();
                printf("AT+CIPSENDEX=260\r\n"); /// VER SE RPECISA DE TXT
                f_wifi_processo++;
                break;
            case 11:
                if (strstr(bufferSerial, ">") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 12:
                limpaBuffer();
                char i = 0;
                char AUX[15];
                sprintf(AUX, "%s", __DATE__);
                for (i = 0; i < strlen(AUX); i++) {
                    if (AUX[i] == ' ') AUX[i] = '-';
                }
//                printf("GET /device/integration?i=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%d&p=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d&k=1z4df68iw1fp5 HTTP/1.1\r\nHost: 192.168.0.125\r\n\r\n\\0", temp, tempajt, umid, umidajt, fornalha, buzzer, tipo_modo_trabalho, trava_fase, fase, clima, tipo_sensor_umidade,wifi_MAC, (wifi_SENHA * 39), AUX, f_falta_energia, tipo_sensor_umidade,h_abafador,h_flap,h_venotinha,h_modo_seguranca,h_buzzer,t_flap_ligado,t_flap_intervalo,t_buzzer_religar,t_temp_automatica,wifi_habilita_alteracao,wifi_SENHA,h_subir_temp_automatica,t_religar_ventoinha_automatica);
                printf("GET /device/integration?i=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%d&p=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d&k=1z4df68iw1fp5 HTTP/1.1\r\nHost: 18.228.154.248\r\n\r\n\\0", temp, tempajt, umid, umidajt, fornalha, buzzer, tipo_modo_trabalho, trava_fase, fase, clima, tipo_sensor_umidade,wifi_MAC, (wifi_SENHA * 39), AUX, f_falta_energia, tipo_sensor_umidade,h_abafador,h_flap,h_venotinha,h_modo_seguranca,h_buzzer,t_flap_ligado,t_flap_intervalo,t_buzzer_religar,t_temp_automatica,wifi_habilita_alteracao,wifi_SENHA,h_subir_temp_automatica,t_religar_ventoinha_automatica);
                f_wifi_processo++;
                break;
            case 13:
                if (strstr(bufferSerial, "200") != NULL | strstr(bufferSerial, "save") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 14:
                limpaBuffer();
                printf("AT+CIPCLOSE\r\n"); ///
                f_wifi_processo++;
                break;
            case 15:
                if (strstr(bufferSerial, "OK") != NULL | strstr(bufferSerial, "CLOSED") != NULL) {
                    f_wifi_processo = 6;
                    t_wifi_restart = 25; // 10 segundos o intervalo de envio de informaçoes
                }
                break;
            default:
                f_wifi_processo = 0;
        }
    } else {
        switch (f_wifi_processo) {
            case 0:
                limpaBuffer();
                printf("AT+CIFSR\r\n");
                f_wifi_processo++;
                break;
            case 1:
                if (strstr(bufferSerial, "MAC") != NULL) {
                    getMac(strstr(bufferSerial, "MAC"), wifi_MAC);
                }
                break;
            default:
                f_wifi_processo = 0;
                break;
        }
    }
}




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

