
#ifndef CONTROLE_H
#define	CONTROLE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "FuncoesControlador.h"
#include "conexaoWifi.h"
#include "Led_display.h"


unsigned char _acao_flap = 0;
unsigned char f_buzzer_falta_energia = 0;

unsigned char t_controle_fim_fases = 0;
unsigned int t_controle_flap = 0;
unsigned char t_controle_buzzer = 0;
unsigned int t_controle_religar_buzzer = 0;
unsigned int t_controle_religar_ventoinha = 0;
unsigned int t_controle_modo_seguranca = 0;
unsigned char t_controle_blink = 0;
unsigned char t_controle_temp_automatica = 0;
unsigned char t_leitura_adc = 0;
bit md_seguranca = 0; //Altera a acao do flap, abrir e fechar no modo de segurnaça;
bit contador_fase_automatica = 0; //Sinaliza quando esta ocorrendo uma contagem de fim de fase


#define temp_fim_fase_amarelacao 100
#define temp_fim_fase_murchamento 110
#define temp_fim_fase_sec_folha 133


#define out_FLAP_RL1 LATAbits.LA5 
#define out_FLAP_RL2 LATCbits.LC2
#define out_VENTOINHA_RL1 LATDbits.LD1
#define out_ABAFADOR_RL1 LATEbits.LE0
#define out_ABAFADOR_RL2 LATDbits.LD0
#define out_BUZZER LATCbits.LC1

void timer() { // 100ms
    t_controle_flap = (t_controle_flap > 1) ? --t_controle_flap : 0; //TIMER PARA ACIONAMENTO DO FLAP
    t_controle_buzzer = (t_controle_buzzer > 1) ? --t_controle_buzzer : 0; //TIMER PARA CONTROLE DE LIGAR E DESLIGAR ALARME A CADA SEGUNDO
    t_controle_modo_seguranca = (t_controle_modo_seguranca > 1) ? --t_controle_modo_seguranca : 0; //TIMER PARA INVERTER ESTADO DO FLAP QUANDO ESTIVER EM MODO DE SEGUNRANÇA
    t_leitura_adc = (t_leitura_adc > 1) ? --t_leitura_adc : 0; //TIMER PARA INTERVALO ENTRE LEITURAS DO CANAL ADC



    if (t_controle_blink > 1) { //TIMER PARA INVERTER ESTADO DA VARIAVEL BLINK, FAZ EFEITO PISCAR NO LCD
        --t_controle_blink;
    } else {
        blinkk = ~blinkk;
        t_controle_blink = 5;
    }

    base_sec++;
    if (base_sec > 9) {
        base_sec = 0;
        sec++;
        t_wifi_restart = (t_wifi_restart > 1) ? --t_wifi_restart : 0; // TIMER PARA RESTARTAR WIFI

        //        if (PORTCbits.RC0 == 0) { //Verifica rede elétrica //Verificar circuito
        //            f_falta_energia = 0;
        //        } else {
        //            f_falta_energia = 1;
        //        }

        if (sec > 59) {
            t_controle_religar_buzzer = (t_controle_religar_buzzer > 1) ? --t_controle_religar_buzzer : 0; //TIMER PARA RELIGAR ALARME
            t_controle_religar_ventoinha = (t_controle_religar_ventoinha > 1) ? --t_controle_religar_ventoinha : 0; //TIMER PARA RELIGAR VENTOINHA

            //////////////////// SUBIR TEMEPRATURA AUTOMATICA ////////////
            // SOBE 1 grau de acordo com a histerise de temp atumatica pelo tempo definido em t_temp_automatica ---> Contador de minutos
            if (tempajt != temp_fim_fase_amarelacao && tempajt != temp_fim_fase_murchamento && tempajt != temp_fim_fase_sec_folha && f_falta_energia == 1) { //SOMENTE ENTRA SE TIVER ENERGIA E NAO TIVER NO FIM DAS FASES
                if ((((temp + h_subir_temp_automatica) >= tempajt) && ((temp - h_subir_temp_automatica) <= tempajt)) && (tempajt < 150 && t_temp_automatica != 0) && trava_fase == 0) { // SOEMNTE SE TIVER DENTRO DA FAIXA DE AJUSTE E MODO AUTOMATICO E O AJUSTE < 150 O TIMER CONTA
                    //         SOMENTE SE TIVER DENTRO DOS AJUSTES DA HISTERISE DE TEMP AUTOMATICA                  //SOBE ATÈ 150 GRAUS AUTOMATICO DEPOIS SOMENTE MANUAL // SOMENTE SOBE SE NAO TIVER TRAVADO
                    t_controle_temp_automatica++; //TIMER DO CONTADOR DE MINUTOS PARA TEMP AUTOMATICA; 
                    if (t_controle_temp_automatica >= t_temp_automatica) {
                        setRangeValor('+', &tempajt, 85, 151, 1);
                        t_controle_temp_automatica = 0;
                    }
                }
            }// Fim TEMEPRATURA automatica 

            sec = 0;
            min++;
            if (min > 59) {
                min = 0;
                hor++;

                //////////////////// SUBIR FASE AUTOMATICA  %%  TEMPO FIM DE FASE  ////////////
                if (trava_fase == 0 && t_temp_automatica != 0 && f_falta_energia == 1) {
                    if (tempajt == temp_fim_fase_amarelacao && fase == 1) { // Da start no fim de fase 1
                        t_controle_fim_fases++;
                        if (t_controle_fim_fases >= t_fim_fase1 && t_fim_fase1 != 0) {
                            setRangeValor('+', &tempajt, 85, 151, 1);
                            t_controle_fim_fases = 0;
                        }
                    } else if (tempajt == temp_fim_fase_murchamento && fase == 2) { // Da start no fim de fase 2
                        t_controle_fim_fases++;
                        if (t_controle_fim_fases >= t_fim_fase2 && t_fim_fase2 != 0) {
                            setRangeValor('+', &tempajt, 85, 151, 1);
                            t_controle_fim_fases = 0;
                        }
                    } else if (tempajt == temp_fim_fase_sec_folha && fase == 3) { // Da start no fim de fase 2
                        t_controle_fim_fases++;
                        if (t_controle_fim_fases >= t_fim_fase3 && t_fim_fase3 != 0) {
                            setRangeValor('+', &tempajt, 85, 151, 1);
                            t_controle_fim_fases = 0;
                        }
                    } else {
                        t_controle_fim_fases = 0;
                    }
                }// Fim FASE automatica 

                if (hor > 23) {
                    hor = 0;
                }
            }

        }
    }
}

void acaoFlap(char acao, int tempo_ativado, int tempo_aguardar) {
    if (acao != _acao_flap) { // RESET DO TEMPORIZADOR QUANDO MUDA DE ESTADO PRA NAO CADUCAR O FLAP 
        t_controle_flap = 0;
        _acao_flap = acao;
        return;
    }
    if (acao) {
        setLed(led_5, 1);
    } else {
        setLed(led_5, 0);
    }

    if (!t_controle_flap) {
        t_controle_flap = (tempo_ativado + tempo_aguardar);
    }

    if (t_controle_flap > tempo_aguardar) {
        out_FLAP_RL1 = !acao;
        out_FLAP_RL2 = acao;
    } else {
        out_FLAP_RL1 = 0;
        out_FLAP_RL2 = 0;
    }
}

void controle() { // Controle das saidas 

    if (f_falta_energia == 0) { // ##### CONTROLE SE FALTAR ENERGIA #####
        LATBbits.LATB5 = blinkk; // pisca display
        out_ABAFADOR_RL1 = 0; // fecha abafador
        out_ABAFADOR_RL2 = 1; // fecha abafador
        out_VENTOINHA_RL1 = 0; // desliga ventoinha
        acaoFlap(1, 10, 0); //ABRE O FLAP
        if (f_buzzer_falta_energia == 1 && buzzer == 0) { // se alarme tiver delsigado entao liga se faltar luz
            buzzer = 1; // liga o alarme
            f_buzzer_falta_energia = 0; // reseta para se a pessoa desligar o alarme ele poder ficar desligado
        }
        if (buzzer) {
            out_BUZZER = blinkk; // APITA O buzzer mais rapido indicando falta energia
        } else {
            out_BUZZER = 0;
        }
        return; // nao deixa fazer controle normal dos atuadores
    } else {
        f_buzzer_falta_energia = 1; // se tiver energia seta flag para ligar o alarme se faltar
    }


    //ROTINA NORMAL
    LATBbits.LATB5 = 1; // LIGA LED do BLACKLIGHT LCD ;

    if (((temp + h_modo_seguranca) <= tempajt) | ((temp - h_modo_seguranca) >= tempajt) && h_modo_seguranca != 0) { // Modo de segurança... só entra quando atingir histerise
        setLed(led_6, 1); //led modo segurança liga
        acaoFlap(md_seguranca, 10, 0); // abre tudo e fecha tudo
        if (t_controle_modo_seguranca == 0) {
            t_controle_modo_seguranca = 1800; // altera estado a cada 3 minutos
            md_seguranca = ~md_seguranca;
        }
    } else {
        setLed(led_6, 0); //led modo segurança desliga
        if (umid >= (umidajt + h_flap)) { //CONTROLE FLAP
            acaoFlap(1, t_flap_ligado, (t_flap_intervalo * 10)); // abrir
        } else if (umid <= umidajt) {
            acaoFlap(0, 200, 600); // fechar 20 segundo ligado 60 desligado
        }
    }
    if (temp > 0) { //Verifica sensor, caso seja desconectado ou haja algum problema nao deixa temperatura subir
        if ((temp + h_abafador) <= tempajt) { // Controle do abafador
            out_ABAFADOR_RL1 = 1;
            out_ABAFADOR_RL2 = 0;
        } else if (temp >= tempajt) {
            out_ABAFADOR_RL1 = 0;
            out_ABAFADOR_RL2 = 1;
        } else {
            out_ABAFADOR_RL1 = 0;
            out_ABAFADOR_RL2 = 0;
        }

        if (fornalha == 1) { // Controle da ventoinha
            if ((temp + h_venotinha) <= tempajt) {
                out_VENTOINHA_RL1 = 1;

            } else if (temp >= tempajt) {
                out_VENTOINHA_RL1 = 0;
            }
        } else {
            out_VENTOINHA_RL1 = 0;
        }

    } else { //Fecha abafador, desliga ventoinha se o sensor for desconectado
        out_ABAFADOR_RL1 = 0;
        out_ABAFADOR_RL2 = 1;
        out_VENTOINHA_RL1 = 0;
    }


    if (fornalha == 1 && t_religar_ventoinha_automatica != 0) { // RELIGAR VENTOINHA AUTOMATICA
        t_controle_religar_ventoinha = t_religar_ventoinha_automatica; //reset no contador da ventoinha
    } else if (t_controle_religar_ventoinha == 0 && t_religar_ventoinha_automatica != 0) {
        fornalha = 1;
    }


    if (buzzer == 1 && t_controle_buzzer == 0) { // CONTROLE BUZZER (alterna entre ligado e desligado)
        if (((temp + h_buzzer) <= tempajt) | ((temp - h_buzzer) >= tempajt)) { // acima ou abaixo do ajuste
            t_controle_buzzer = 10; // 1 segundo
            out_BUZZER = ~out_BUZZER;
        } else {
            out_BUZZER = 0;
        }
    }
    if (buzzer == 1 && t_buzzer_religar != 0) { // RELIGAR BUZZER AUTOMATICO
        t_controle_religar_buzzer = t_buzzer_religar;
    } else if (t_controle_religar_buzzer == 0 && t_buzzer_religar != 0) {
        buzzer = 1;
    }
    if (!buzzer) {
        out_BUZZER = 0;
    }

    //CONTROLE DOS LEDS DISPLAY

    if (temp + 3 < tempajt) { // temp baixa
        setLed(led_2, 1);
        setLed(led_1, 0);
    } else if (temp - 3 > tempajt) {
        setLed(led_1, 1);
        setLed(led_2, 0);
    } else {
        setLed(led_1, 0);
        setLed(led_2, 0);
    }

    if (umid + 3 < umidajt) { // umid baixa
        setLed(led_3, 0);
        setLed(led_4, 1);
    } else if (umid - 3 > umidajt) {
        setLed(led_3, 1);
        setLed(led_4, 0);
    } else {
        setLed(led_3, 0);
        setLed(led_4, 0);
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

