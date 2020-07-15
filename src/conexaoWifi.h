
#ifndef CONEXAOWIFI_H
#define	CONEXAOWIFI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <string.h>
#include "variaveisGlobais.h"
#include "function.h"
#include "Eeprom_ext.h"
#include "controle.h"
#include "serial.h"
#include "json.h"

unsigned char t_wifi_restart = 0;
unsigned char f_wifi_processo = 0;

void alterarDadosConexaoConfigWifi() {
    if (t_wifi_restart == 0) {
        menu = 8;
        opcaomenu = 1;
        return;
    } else { // tempo para reiniciar o modulo wifi
        delay(10);
        switch (f_wifi_processo) {
            case 0:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CWMODE=2\r\n");
                wifi_conectado = 0;
                f_wifi_processo++;
                break;
            case 1:
                if (findBufferSerial("OK")) {
                    f_wifi_processo++;
                }
                break;
            case 2:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPMUX=1\r\n");
                f_wifi_processo++;
                break;
            case 3:
                if (findBufferSerial("OK")) {
                    f_wifi_processo++;
                }
                break;
            case 4:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CWSAP=\"CONTROLADOR\",\"\",11,0,4\r\n");
                f_wifi_processo++;
                break;
            case 5:
                if (findBufferSerial("OK")) {
                    f_wifi_processo++;
                }
                break;
            case 6:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPAP=\"10.1.1.1\"\r\n");
                f_wifi_processo++;
                break;
            case 7:
                if (findBufferSerial("OK")) {
                    f_wifi_processo++;
                }
                break;
            case 8:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPSERVER=1,80\r\n");
                f_wifi_processo++;
                break;
            case 9:
                if (findBufferSerial("OK")) {
                    setKeyStartReceiveSerial("Accept");
                    f_wifi_processo++;
                    t_wifi_restart = 180; // 3 minutos para se conectar a rede!!
                }
                break;
            case 10:
                sprintf(line1, "Conecte-se a rede e");
                sprintf(line2, "acesse o IP para");
                sprintf(line3, "configurar o WIFI...");
                sprintf(line4, "10.1.1.1");
                if (findBufferSerial("Accept")) {
                    delay(400);
                    f_wifi_processo++;
                }
                break;
            case 11:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPSENDEX=0,800\r\n");
                f_wifi_processo++;
                break;
            case 12:
                if (strstr(bufferSerial, ">") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 13:
                setKeyStartReceiveSerial("ALL");
                printf("HTTP 1.1/200 OK\r\nContent-type: text/html\r\n\r\n");
                delay(100);
                printf("<html><style>div{margin:25px auto;max-width:350px;display:flex;flex-direction:column;}input,button{padding:7px;margin-bottom:25px;border:1px solid #888;border-radius:3px;}</style><body><div> <h2>Configuração Wifi</h2> SSID: <input id='s'> Senha: <input id='p'> <button onclick='s()'>Confirmar</button> Servidor: <input id='t' value='%s:%s'></div></body></html><script>function s(){const g=i=>document.querySelector('#' + i).value let sv=i('t').split(':');fetch('/',{method:'post',body:JSON.stringify({'$#':'',ss:i('#s'),pw:i('#p'),sv:[0],pt:sv[1],'':'#$'})})}</script>\r\n\r\n\\0", ipServer, portServer);
                delay(800); // tempo para envio da pagina
                printf("AT+CIPCLOSE=0\r\n");
                delay(300);
                t_wifi_restart = 180; // 3 minutos para colocar dados da rede!!
                setKeyStartReceiveSerial("$#"); // INICIO JSON
                f_wifi_processo++;
                break;
            case 14:
                sprintf(line4, "Aguardando...");
                if (findBufferSerial("#$")) { // FIM JSON
                    getStringJSON(bufferSerial, "ss", ssid);
                    getStringJSON(bufferSerial, "pw", senha);
                    getStringJSON(bufferSerial, "ip", ipServer);
                    getStringJSON(bufferSerial, "pt", portServer);

                    sprintf(line1, "%s", ssid);
                    sprintf(line2, "%s", senha);
                    sprintf(line3, "%s:%s", ipServer, portServer);
                    sprintf(line4, "OK reiniciando...");
                    atualizaLCD(line1, line2, line3, line4);

                    if (strlen(ssid) > 3) {
                        unsigned char i = 0;
                        for (i = 0; i < 15; i++) {
                            writeEEPROM_ext((40 + i), ssid[i]);
                            writeEEPROM_ext((60 + i), senha[i]);
                        }
                        for (i = 0; i < 20; i++) {
                            writeEEPROM_ext((80 + i), ipServer[i]);
                        }
                        for (i = 0; i < 5; i++) {
                            writeEEPROM_ext((105 + i), portServer[i]);
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
                setKeyStartReceiveSerial("ALL");
                printf("AT+CWMODE=1\r\n");
                wifi_conectado = 0;
                f_wifi_processo++;
                break;
            case 1:
                if (findBufferSerial("OK")) {
                    f_wifi_processo++;
                }
                break;
            case 2:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPMUX=0\r\n");
                f_wifi_processo++;
                break;
            case 3:
                if (findBufferSerial("OK") || findBufferSerial("link")) {
                    f_wifi_processo++;
                }
                break;
            case 4:
                setKeyStartReceiveSerial("ALL");
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
                setKeyStartReceiveSerial("ALL");
                if (t_wifi_restart <= 15) { //ENVIA A CADA 10 SEGUNDOS OS DADOS, tem 10 segundos para enviar, caso de erro, reinicia o wifi
                    printf("AT+CIPSTATUS\r\n");
                    f_wifi_processo++;
                }
                break;
            case 7:
                if (findBufferSerial("2") || findBufferSerial("4")) {
                    wifi_conectado = 1;
                    f_wifi_processo++;
                }
                break;
            case 8:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ipServer, portServer);
                f_wifi_processo++;
                break;
            case 9:
                if (findBufferSerial("OK") != NULL) {
                    f_wifi_processo++;
                }
                break;
            case 10:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPSENDEX=260\r\n"); /// VER SE RPECISA DE TXT
                f_wifi_processo++;
                break;
            case 11:
                if (findBufferSerial(">")) {
                    f_wifi_processo++;
                }
                break;
            case 12:
                setKeyStartReceiveSerial("200");
                char i = 0;
                char AUX[15];
                sprintf(AUX, "%s", __DATE__);
                for (i = 0; i < strlen(AUX); i++) {
                    if (AUX[i] == ' ') AUX[i] = '-';
                }
                //                printf("GET /device/integration?i=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%d&p=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d&k=1z4df68iw1fp5 HTTP/1.1\r\nHost: 192.168.0.125\r\n\r\n\\0", temp, tempajt, umid, umidajt, fornalha, buzzer, tipo_modo_trabalho, trava_fase, fase, clima, tipo_sensor_umidade,wifi_MAC, (wifi_SENHA * 39), AUX, f_falta_energia, tipo_sensor_umidade,h_abafador,h_flap,h_venotinha,h_modo_seguranca,h_buzzer,t_flap_ligado,t_flap_intervalo,t_buzzer_religar,t_temp_automatica,wifi_habilita_alteracao,wifi_SENHA,h_subir_temp_automatica,t_religar_ventoinha_automatica);
                printf("GET /device/integration?i=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%d&p=%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d&k=1z4df68iw1fp5 HTTP/1.1\r\nHost: 18.228.154.248\r\n\r\n\\0", temp, tempajt, umid, umidajt, fornalha, buzzer, tipo_modo_trabalho, trava_fase, fase, clima, tipo_sensor_umidade, wifi_MAC, (wifi_SENHA * 39), AUX, f_falta_energia, tipo_sensor_umidade, h_abafador, h_flap, h_venotinha, h_modo_seguranca, h_buzzer, t_flap_ligado, t_flap_intervalo, t_buzzer_religar, t_temp_automatica, wifi_habilita_alteracao, wifi_SENHA, h_subir_temp_automatica, t_religar_ventoinha_automatica);
                f_wifi_processo++;
                break;
            case 13:
                if (findBufferSerial("save")) {
                    f_wifi_processo++;
                }
                break;
            case 14:
                setKeyStartReceiveSerial("ALL");
                printf("AT+CIPCLOSE\r\n"); ///
                f_wifi_processo++;
                break;
            case 15:
                if (findBufferSerial("OK") || findBufferSerial("CLOSED")) {
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
                setKeyStartReceiveSerial("STAMAC");
                printf("AT+CIFSR\r\n");
                f_wifi_processo++;
                break;
            case 1:
                if (findBufferSerial("OK")) {
                    unsigned char i = 0;
                    unsigned char j = 0;
                    unsigned char men = 0;
                    for (i = 0; i < strlen(bufferSerial); i++) {
                        if (bufferSerial[i] == '"' && men == 0) {
                            men = 1;
                        } else if (bufferSerial[i] == '"' && men == 1) {
                            return;
                        } else if (men && bufferSerial[i] != ':' && strlen(wifi_MAC) < 12) {
                            wifi_MAC[j++] = bufferSerial[i];
                        }
                    }
                }
                break;
            default:
                f_wifi_processo = 0;
                break;
        }
    }
}

#endif	/* XC_HEADER_TEMPLATE_H */

