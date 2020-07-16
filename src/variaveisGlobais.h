#ifndef VARIAVEISGLOBAIS_H
#define	VARIAVEISGLOBAIS_H

unsigned char lcd_rfsh, base_sec, _sec = 0;


unsigned char buzzer = 0; //Status de alarme 1=ligado  0=desligado
unsigned char fornalha = 0; //Status de ventoinha  1=ligado  0=desligado
unsigned char trava_fase = 0; //Status de trava de fase 1=ligado  0=desligado
unsigned char wifi_conectado = 0; //Status de alarme 1=conectado  0=desconectado
unsigned char tipo_sensor_umidade = 0; // Tipo do sensor utilizado 1= UR% 0 = ºF
unsigned char tipo_modo_trabalho = 0; // Tipo de trabalho do controlador, utilizado para umidade automatica 0=Manual  1=Automatico
unsigned char f_falta_energia = 1; // flag que sinaliza falta de energia


unsigned char sec = 0; // Contador de segundos
unsigned char min = 0; //Contador de minutos
unsigned char hor = 0; // Contador de horas

unsigned char t_fim_fase1 = 0; // tempo de fim de fase 1 (H)
unsigned char t_fim_fase2 = 0; // tempo de fim de fase 2 (H)
unsigned char t_fim_fase3 = 0; // tempo de fim de fase 3 (H)
unsigned char t_flap_ligado = 0; // tempo que o flap permanece ligado (TEMPO EM MS)
unsigned char t_flap_intervalo = 0; // tempo de intervalo de acionamento do flap (TEMPO EM S)
unsigned char t_buzzer_religar = 1; // tempo Para religar o alarme automaticamente (TEMPO EM M)
unsigned char t_temp_automatica = 0; // tempo Para subir de temperatura automaticamente 1 grau (TEMPO EM M)
unsigned char t_religar_ventoinha_automatica = 0; // tempo Para religar a ventoinha apos ter sido desligada (TEMPO EM M)


//unsigned char f_falta_energia = 0; // flag para sinalizar falta de energia
unsigned char h_modo_seguranca = 0; // histerise para entrar no modo de segurança
unsigned char h_venotinha = 0; // histerise de diferença de ligar a ventoinha
unsigned char h_abafador = 0; // histerise de diferença para abrir flap
unsigned char h_flap = 0; // histerise de diferenca para abrir flap
unsigned char h_buzzer = 0; // histerise de diferenca para tocar alarme
unsigned char h_subir_temp_automatica = 0; // histerise de diferenca para subir a temperatura sozinha (Modo automatico)


unsigned char wifi_habilita_alteracao = 0; // variavel de controle para habilitar alterar configuraçao do controlador pela internet
unsigned char wifi_MAC[12] = "ERRO"; // Endereço MAC da placa wifi usado para conexao remota wifi
unsigned int wifi_SENHA = 0; // Senha usada para conexao remota wifi
unsigned char ssid[15] = "";
unsigned char senha[15] = "";
unsigned char ipServer[20] = "";
unsigned int portServer = 0;

unsigned char fase = 0; // Fases
unsigned char clima = 0; // Climas 
unsigned char temp = 99; // Temperatura lida do sensor
unsigned char tempajt = 0; // Setpoint de ajuste de temperatura
unsigned char umid = 100; // Umidade lida do sensor
unsigned char umidajt = 0; // Setpiont de ajuste de umidade 
bit blinkk = 0; // flag para piscar o display, inverte o estado pelo timer

unsigned char line1[21] = ""; // linhas do lcd
unsigned char line2[21] = ""; // linhas do lcd
unsigned char line3[21] = ""; // linhas do lcd
unsigned char line4[21] = ""; // linhas do lcd


unsigned char menu = 15;
char opcaomenu = 0;
unsigned char ajuste_parametro = 0;
unsigned char senha_desbloqueio_func = 0;


#endif	/* XC_HEADER_TEMPLATE_H */

