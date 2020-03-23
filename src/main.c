#include <xc.h>
#include "config.h"
#include "variaveisGlobais.h"
#include "Led_display.h"
#include "lcd.h"
#include "Teclado.h"
#include <stdio.h>
#include "Controle.h"
#include "FuncoesControlador.h"
#include "USART.h"
#include "conexaoWifi.h"
#include "Eeprom_ext.h"
#include "ADC.h"

void interrupt TIMER0() { // INTERRUPÇÂO TIMER 0
    if (TMR0IF) {
        TMR0IF = 0x00;
        TMR0H = 0x3C;
        TMR0L = 0xB0;
        timer();
    }
    if (RCIF) { //INTERRUPÇÂO SERIAL
        char c = RCREG;
        if (c != '\n' && c != '\r') {
            bufferSerial[cbf++] = c;
        }
        if (cbf >= 80) cbf = 0;
    }
    if (INT2IF) { // INTERRUPÇÃO PCF LEITURA DOS BOTOES   
        INT2IF = 0;
        readButtons();
    }
}

void main() {

    //## CONFIGURAÇÂO SAIDAS
    TRISAbits.RA5 = 0; // rl flap 1
    TRISCbits.RC2 = 0; // rl flap 2
    TRISEbits.RE0 = 0; // rl abafador 1
    TRISDbits.RD0 = 0; // rl abafador 2
    TRISCbits.RC1 = 0; // rl buzzer
    TRISDbits.RD1 = 0; // rl ventoinha
    TRISCbits.RC0 = 1; // input bateria

    PORTAbits.RA5 = 0;
    PORTCbits.RC2 = 0;
    PORTEbits.RE0 = 0;
    PORTDbits.RD0 = 0;
    PORTCbits.RC1 = 0;
    PORTDbits.RD1 = 0;


    //##RESET WIFI
    TRISAbits.RA4 = 0;
    LATAbits.LATA4 = 0; // liga wifi


    //##Blackligth do display
    TRISBbits.RB5 = 0;
    LATBbits.LATB5 = 0; // led lcd


    //##INTERRUPÇAO RB2 borda descida
    TRISBbits.RB2 = 1; // interrupt botao
    INTCON2bits.INTEDG2 = 0;
    INTCON2bits.RBIP = 0;
    INTCON3bits.INT2IE = 1;

    //## ADC ENTRADAS
    TRISAbits.RA0 = 1;
    PORTAbits.AN0 = 0;
    TRISAbits.RA1 = 1;
    PORTAbits.AN1 = 0;
    TRISAbits.RA2 = 1;
    PORTAbits.AN2 = 0;



    //## TIMER
    T0CON = 0x81;
    INTCON = 0xE4;

    //## ADC
    ADCON0 = 0x01;
    ADCON1 = 0x0C; // AN0, AN1, AN2 analogico
    ADCON2 = 0xB5;


    UART_Init();
    printf("AT+RST\r\n");
    delay(200);

    Lcd_Init();
    Lcd_Clear();
    criaCaracteresCGRAM();
    I2C_Master_Init(1000000); //Inicializa I2C modo MASTER em 100KHz clock/
    delay(150);

    //// TELA DE BOAS VINDAS /////
    sprintf(line1, " TECHNOW Tecnologia");
    sprintf(line3, "Versao Soft.:");
    sprintf(&line4[9], "%s", __DATE__);
    atualizaLCD(line1, line2, line3, line4);
    delay(300);
    LATBbits.LATB5 = 1; // led lcd
    delay(2450);

    tempajt = readEEPROM_ext(1);
    umidajt = readEEPROM_ext(2);
    trava_fase = readEEPROM_ext(3);
    fase = readEEPROM_ext(4);
    clima = readEEPROM_ext(5);
    tipo_modo_trabalho = readEEPROM_ext(6);
    tipo_sensor_umidade = readEEPROM_ext(7);
    t_fim_fase1 = readEEPROM_ext(8);
    t_fim_fase2 = readEEPROM_ext(9);
    t_fim_fase3 = readEEPROM_ext(10);
    h_venotinha = readEEPROM_ext(11);
    h_abafador = readEEPROM_ext(12);
    h_flap = readEEPROM_ext(13);
    buzzer = readEEPROM_ext(14);
    fornalha = readEEPROM_ext(15);
    h_modo_seguranca = readEEPROM_ext(16);
    h_buzzer = readEEPROM_ext(17);
    t_flap_ligado = readEEPROM_ext(18);
    t_flap_intervalo = readEEPROM_ext(20);
    t_buzzer_religar = readEEPROM_ext(21);
    t_temp_automatica = readEEPROM_ext(22);
    wifi_habilita_alteracao = readEEPROM_ext(23);
    wifi_SENHA = readEEPROM_ext(25);
    h_subir_temp_automatica = readEEPROM_ext(27);
    t_religar_ventoinha_automatica = readEEPROM_ext(28);

    char i = 0;
    for (i = 0; i < 15; i++) {
        ssid[i] = (char) readEEPROM_ext(40 + i);
    }
    for (i = 0; i < 15; i++) {
        senha[i] = (char) readEEPROM_ext(60 + i);
    }

    i = readEEPROM_ext(30); // confere se esta fazendo a leitura correta da memoria externa, caso nao esteja entao nao deixa o controlador funcionar
    if (i != 187) { // numero aleatorio
        writeEEPROM_ext(30, 187);
        while (1) {
            asm("CLRWDT");
            sprintf(line4, "ERRO EEPROM");
            atualizaLCD(line1, line2, line3, line4);
            delay(3000);
        }
    }

    while (1) {

        ///////////////////////////////  ESPAÇO TESTEs //////////////////////////////////////////////


        /////////////////////////////// FIM ESPAÇO TESTEs //////////////////////////////////////////////
        if (RCSTAbits.OERR) { // reset da serial caso travar
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
        asm("CLRWDT"); // WTD 5 segundos  
        getSensors(); //Leitura dos sensores
        controle(); // controle de saidas e logica
        //////////////////////////////////// ROTINAS DE CONTROLE //////////////////////////////////////////

        if (tipo_modo_trabalho == 1) { //Se tiver no modo Aumtomatico a umidade ajusta sozinha
            setUmidade('A', &umidajt, &tempajt, &clima);
        }

        faseAtual('F', &fase, &tempajt); // Ajusta fase conforme a temperatura.

        if (menu != 10) { // ENVIA DADOS PARA NUVEM SE NAO TIVER SENDO CONFIGURANDO
            enviaDadosWifi();
        }

        ///////////////////////////////  VISUALIZACAO DAS TELAS ///////////////////////////////////////////////////////

        if (btPress(B_SILENCIA)) setRangeValor('+', &fornalha, 0, 1, 15); // LIGA E DESLIGA VENTOINHA

        switch (menu) {
            case 0:
                sprintf(&line1[6], "|Sensor|Ajuste");
                line1[0] = 0x02;
                if (!buzzer) blink(line1, 1, 2);
                line1[2] = 'F';
                if (!fornalha) blink(line1, 2, 3);
                line1[4] = (wifi_conectado) ? 0x05 : ' ';
                line1[5] = (trava_fase) ? 0x01 : ' ';
                sprintf(line2, "TEMP  |  %3d | %3d F", temp, tempajt);
                line2[5] = 0x04;
                sprintf(line3, "UMID  |  %3d | %3d %c", umid, umidajt, (tipo_sensor_umidade) ? '%' : 'F');
                line3[5] = 0x03;
                sprintf(line4, "Fase ~ %s", faseAtual('?', &fase, &tempajt)); // retorna string da fase atual
                if (!f_falta_energia) {
                    sprintf(line4, " *FALTA DE ENERGIA*");
                };

                if (btPress(B_MENU)) {
                    menu = 1;
                    opcaomenu = 1;
                }
                if (btPress(B_OK)) {
                    ajuste_parametro++;
                }
                if (btPress(B_CIMA)) setRangeValor('+', &buzzer, 1, 1, 14);
                if (btPress(B_BAIXO)) setRangeValor('+', &buzzer, 0, 0, 14);


                switch (ajuste_parametro) { // Ajuste rapido tela inicial
                    case 1:
                        blink(line2, 15, 20);
                        if (btPress(B_MAIS)) setRangeValor('+', &tempajt, 85, 160, 1);
                        if (btPress(B_MENOS)) setRangeValor('-', &tempajt, 85, 160, 1);
                        break;
                    case 2:
                        if (tipo_modo_trabalho == 0) {
                            blink(line3, 15, 20);
                            if (btPress(B_MAIS)) setUmidade('+', &umidajt, &tempajt, &clima);
                            if (btPress(B_MENOS)) setUmidade('-', &umidajt, &tempajt, &clima);
                        } else {
                            ajuste_parametro = 0;
                        }
                        break;
                    default:
                        ajuste_parametro = 0;
                }
                break;
            case 1: // 1 tela do menu
                sprintf(&line1[1], "TEMPERATURA: %d F", tempajt);
                sprintf(&line2[1], "UMIDADE: %d %c", umidajt, (tipo_sensor_umidade) ? '%' : 'F');
                sprintf(&line3[1], "FASE: %s", faseAtual('?', &fase, &tempajt));
                sprintf(&line4[1], "CLIMA: %s", climaAtual(clima));
                break;
            case 2: // 2 tela do menu
                sprintf(&line1[1], "MODO: %s", (!tipo_modo_trabalho) ? "Manual" : "Automatico");
                sprintf(&line2[1], "TRAVA: %s", (trava_fase) ? "Travado" : "Destravado");
                sprintf(&line3[1], "ALARME: %s", (buzzer) ? "Ligado" : "Desligado");
                sprintf(&line4[1], "TEMPO FIM FASE   >>");
                break;
            case 3: // 3 tela do menu
                sprintf(&line1[1], "CONFIG WIFI      >>");
                sprintf(&line2[1], "CONFIG AVANCADAS >>");
                break;
            case 5: // Menu de config avancadas 1
                sprintf(&line1[1], "P1:%4d |  P5:%4d", senha_desbloqueio_func, h_venotinha);
                sprintf(&line2[1], "P2:%4d |  P6:%4d", tipo_sensor_umidade, h_modo_seguranca);
                sprintf(&line3[1], "P3:%4d |  P7:%4d", h_abafador, h_buzzer);
                sprintf(&line4[1], "P4:%4d |  P8:%4d", h_flap, t_flap_ligado);
                break;
            case 6: // Menu de config avancadas 2
                sprintf(&line1[1], "P9 :%4d | P13:%4d", t_flap_intervalo, wifi_SENHA);
                sprintf(&line2[1], "P10:%4d | P14:%4d", t_buzzer_religar, h_subir_temp_automatica);
                sprintf(&line3[1], "P11:%4d | P15:%4d", t_temp_automatica, t_religar_ventoinha_automatica);
                sprintf(&line4[1], "P12:%4d |", wifi_habilita_alteracao);
                break;
            case 8: // submenu WIFI 1
                sprintf(line1, "REDE:%s", ssid);
                sprintf(line2, "SENHA:%s", senha);
                //                sprintf(line3, "NUMERO SERIE: %d", (int) (wifi_NS * 39));
                sprintf(line3, "NS: %s", wifi_MAC);
                sprintf(line4, "SENHA REMOTA: %d", (int) (wifi_SENHA * 39));
                break;
            case 9: // Sub menu ajuste de configuraçoes do wifi 2.
                sprintf(&line1[1], "CONFIGURAR       >>");
                break;
            case 10: // CONFIGURAÇAO DO WIFI EM ANDAMENTO
                alterarConfigWifi();
                break;
            case 11: // Sub menu ajuste de tempos de fim de fase.
                sprintf(&line1[1], "Amarelacao : %d hr", t_fim_fase1);
                sprintf(&line2[1], "Murchamento: %d hr", t_fim_fase2);
                sprintf(&line3[1], "Sec. Folha : %d hr", t_fim_fase3);
                sprintf(line4, "*0 nao passa fase");
                break;
            default: menu = 0;
                break;
        }

        /////////////// ACOES DO MENU NORMAL ////////////////////////////

        if (menu == 1 | menu == 2 | menu == 3) {
            ajuste_parametro = 0;

            switch (opcaomenu) {// açoes da do MENU
                case 0:
                    opcaomenu = 10; // ultima opçao do menu
                    break;
                case 1: // ação ajuste temperatura
                    menu = 1;
                    line1[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &tempajt, 60, 160, 1);
                    if (btPress(B_MENOS)) setRangeValor('-', &tempajt, 60, 160, 1);
                    break;
                case 2: //ação ajuste umidade
                    line2[0] = '>';
                    if (tipo_modo_trabalho == 0) {
                        if (btPress(B_MAIS)) setUmidade('+', &umidajt, &tempajt, &clima);
                        if (btPress(B_MENOS)) setUmidade('-', &umidajt, &tempajt, &clima);
                    }
                    break;
                case 3: //ação ajuste fase
                    line3[0] = '>';
                    if (btPress(B_MAIS)) {
                        setRangeValor('+', &fase, 1, 4, 4);
                        faseAtual('A', &fase, &tempajt);
                        setUmidade('A', &umidajt, &tempajt, &clima); //SETA A UMIDADE PARA VOLTAR OS PARAMETROS
                    }
                    if (btPress(B_MENOS)) {
                        setRangeValor('-', &fase, 1, 4, 4);
                        faseAtual('A', &fase, &tempajt);
                        setUmidade('A', &umidajt, &tempajt, &clima);
                    }
                    break;
                case 4: //ação ajuste clima
                    menu = 1;
                    line4[0] = '>';
                    if (btPress(B_MAIS))setRangeValor('+', &clima, 1, 3, 5);
                    if (btPress(B_MENOS))setRangeValor('-', &clima, 1, 3, 5);
                    break;
                case 5: //ação ajuste modo de trabalho
                    menu = 2;
                    line1[0] = '>';
                    if (btPress(B_MAIS) || btPress(B_MENOS)) setRangeValor('+', &tipo_modo_trabalho, 0, 1, 6);
                    break;
                case 6: //ação ajuste trava fase
                    line2[0] = '>';
                    if (btPress(B_MAIS) || btPress(B_MENOS)) setRangeValor('+', &trava_fase, 0, 1, 3);
                    break;
                case 7: //ação ajuste buzzer liga/ desliga
                    line3[0] = '>';
                    if (btPress(B_MAIS) || btPress(B_MENOS)) setRangeValor('+', &buzzer, 0, 1, 14);
                    break;
                case 8:// acçao entra no menu de configuraçao de tempos de fim de fase. 
                    menu = 2;
                    line4[0] = '>';
                    if (btPress(B_OK)) menu = 11;
                    break;
                case 9: //ação ajuste Wifi
                    menu = 3;
                    line1[0] = '>';
                    if (btPress(B_OK)) menu = 8;
                    break;
                case 10: //ação ajuste paramteros avancados
                    menu = 3;
                    line2[0] = '>';
                    if (btPress(B_OK)) {
                        menu = 5;
                        opcaomenu = 1;
                    }
                    break;
                default:
                    opcaomenu = 1;
                    break;
            }
            //Controle dos cursores no menu
            if (btPress(B_BAIXO)) opcaomenu++;
            if (btPress(B_CIMA)) opcaomenu--;
            if (btPress(B_MENU)) menu = 0;
        } else

            /////////////// ACOES DO MENU ANACADO ////////////////////////////

            if (menu == 5 | menu == 6) {
            if (senha_desbloqueio_func != 48) opcaomenu = 1; // senha para liberar menu
            switch (opcaomenu) {
                case 1: //P1 --> senha de desbloqueio
                    menu = 5;
                    line1[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &senha_desbloqueio_func, 0, 50, 0);
                    if (btPress(B_MENOS)) setRangeValor('-', &senha_desbloqueio_func, 0, 50, 0);
                    break;
                case 2: //P2 --> tipo do sensor de umidade, 1 relativo , 0 Bulbo Umido
                    line2[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &tipo_sensor_umidade, 0, 1, 7);
                    if (btPress(B_MENOS)) setRangeValor('-', &tipo_sensor_umidade, 0, 1, 7);
                    break;
                case 3: // P3 --> histerise do abafador (quantos graus abaixo do ajuste ele abre)
                    line3[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_abafador, 1, 10, 12);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_abafador, 1, 10, 12);
                    break;
                case 4: // P4 --> histerise do flap (quantos graus abaixo do ajuste ele abre)
                    line4[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_flap, 1, 10, 13);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_flap, 1, 10, 13);
                    break;
                case 5: // P5 --> histerise da ventoinha (quantos graus abaixo do ajuste ela liga)
                    line1[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_venotinha, 1, 10, 11);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_venotinha, 1, 10, 11);
                    break;
                case 6: // P6 --> Histerise do modo de segunrança (Quantos graus abaixo ou acima entra no modo de segurança)
                    line2[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_modo_seguranca, 0, 15, 16);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_modo_seguranca, 0, 15, 16);
                    break;
                case 7: // P7 --> Histerise do alarme (liga quando tiver abaixo ou acima da temp ajuste)
                    line3[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_buzzer, 1, 15, 17);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_buzzer, 1, 15, 17);
                    break;
                case 8: // P8 -- > Tempo em que a saida do flap permanece acionada
                    menu = 5;
                    line4[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_flap_ligado, 1, 250, 18);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_flap_ligado, 1, 250, 18);
                    break;
                case 9: // P9 -- > Tempo do intervalo entre acionamentos do flap (s))
                    menu = 6;
                    line1[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_flap_intervalo, 0, 250, 20);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_flap_intervalo, 0, 250, 20);
                    break;
                case 10: // P10 -- > Tempo para religar o alarme automaticamente (M))
                    line2[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_buzzer_religar, 0, 30, 21);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_buzzer_religar, 0, 30, 21);
                    break;
                case 11: // P11 -- > Tempo para subir temperatura automatica (M))
                    line3[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_temp_automatica, 0, 250, 22);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_temp_automatica, 0, 250, 22);
                    break;
                case 12: // P12 -- > Controle de alteração de parametros pela internet.
                    line4[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', (&wifi_habilita_alteracao), 0, 1, 23);
                    if (btPress(B_MENOS)) setRangeValor('-', &wifi_habilita_alteracao, 0, 1, 23);
                    break;
                case 13: // P13 -- > Senha de acesso remoto wifi
                    line1[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &wifi_SENHA, 1, 250, 25);
                    if (btPress(B_MENOS)) setRangeValor('-', &wifi_SENHA, 1, 250, 25);
                    break;
                case 14: // P14 -- > Histerise para começar a funcionar o contador de subida de temperatura automatica
                    line2[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &h_subir_temp_automatica, 0, 15, 27);
                    if (btPress(B_MENOS)) setRangeValor('-', &h_subir_temp_automatica, 0, 15, 27);
                    break;
                case 15: // P15 -- > Tempo para religar a ventoinha automaticamente
                    line3[11] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_religar_ventoinha_automatica, 0, 60, 28);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_religar_ventoinha_automatica, 0, 60, 28);
                    break;
                default: opcaomenu = 1;
            }
            if (btPress(B_BAIXO)) opcaomenu++;
            if (btPress(B_CIMA)) opcaomenu--;
            if (btPress(B_MENU)) {
                menu = 0;
                senha_desbloqueio_func = 0;
            }
        } else

            /////////////// ACOES DO MENU WIFI ////////////////////////////

            if (menu == 8 || menu == 9) {
            switch (opcaomenu) {
                case 1:
                    menu = 8; // menu de informaçoes do wifi
                    break;
                case 2: //configurar wifi
                    menu = 9; // botao de acao pra entrar na config do wifi
                    line1[0] = '>';
                    if (btPress(B_OK)) {
                        LATAbits.LATA4 = 1; // reiniciar wifi
                        delay(100);
                        LATAbits.LATA4 = 0;
                        delay(800); // tempo para iniciar o modulo novamente
                        menu = 10;
                        f_wifi_processo = 0;
                        t_wifi_restart = 15;
                    }
                    break;
                default:
                    opcaomenu = 1;

                    break;
            }
            if (btPress(B_BAIXO)) opcaomenu++;
            if (btPress(B_CIMA)) opcaomenu--;
            if (btPress(B_MENU)) {
                menu = 3;
            }
        } else

            /////////////// ACOES DO MENU TEMPO DE FASES ////////////////////////////

            if (menu == 11) { // AÇAO DO MENU DE AJUSTE DE TEMPO DE FASE menu=8
            switch (opcaomenu) {
                case 1: //tempo  Fase 1
                    line1[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_fim_fase1, 0, 30, 8);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_fim_fase1, 0, 30, 8);
                    break;
                case 2: //Tempo fase 2
                    line2[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_fim_fase2, 0, 30, 9);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_fim_fase2, 0, 30, 9);
                    break;
                case 3: // Tempo de fase 3
                    line3[0] = '>';
                    if (btPress(B_MAIS)) setRangeValor('+', &t_fim_fase3, 0, 30, 10);
                    if (btPress(B_MENOS)) setRangeValor('-', &t_fim_fase3, 0, 30, 10);
                    break;
                default: opcaomenu = 1;
            }
            if (btPress(B_BAIXO)) opcaomenu++;
            if (btPress(B_CIMA)) opcaomenu--;
            if (btPress(B_MENU)) {
                menu = 2;
            }
        }
        updateLeds();
        atualizaLCD(line1, line2, line3, line4);
    }
}

