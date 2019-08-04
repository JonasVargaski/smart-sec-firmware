

//char buffer[35] = 0; // Tamanho do buffer de recepçao da serial
//unsigned char buffer_posi = 0;
//unsigned char armazena_buffer = 0;
//unsigned char str_recebida[7] = {'\0'}; // tamanho da string passada por parametro
//unsigned char str_posicao = 0, str_recebido_ok = 0, checar_serial = 0;

void UART_Init() {
    TRISCbits.TRISC6 = 0; // configura TX como saida
    TRISCbits.TRISC7 = 1; // configura RX como entradaF  

    //TXSTA
    TXSTAbits.TXEN = 1; //Habilita Transmissao pela EUSART
    TXSTAbits.BRGH = 1; // modo velocidade alta
    TXSTAbits.SYNC = 0; // Modo assincrono

    //RCSTA
    RCSTAbits.SPEN = 1; // Configura pinos tx/rx
    RCSTAbits.CREN = 1; //Habilita Recepçao pela EUSART

    //9600 = 51  0.160% err @8mhz
    //19200 = 25  0.160% err @8mhz

    SPBRG = 25; // crystal 8mhz baudrate 19200bps

    //Interrupçao UART
    INTCONbits.GIE = 1; // Habilita interrupçao global
    INTCONbits.PEIE = 1; // Habilita interrupçao por perifericos

    PIR1bits.RCIF = 0; // Limpa flag de recepçao uart
    PIE1bits.RCIE = 1; // Habilita interrupção por recepçao uart
}

void putch(char c) {

    while (!TXSTAbits.TRMT); // verifica se ainda tem dados para serem transmitidos
    TXREG = c; // envia cacacter
}

/*
char* getRespostaUART(char *txt, unsigned int timeout, char *salva_buffer) {
    if (salva_buffer) {
        char i = 0;
        for (i = 0; i < strlen(buffer); i++) { // Limpa o buffer da serial
            buffer[i] = "\0";
        }
        armazena_buffer = salva_buffer;
    }
    str_posicao = 0;
    strcpy(str_recebida, txt);
    checar_serial = 1;
    unsigned int i = 0;
    for (i = 0; i < 20000 && i < timeout && str_recebido_ok == 0; i++) {
        delay(1);
    }
    if (i == timeout) {
        checar_serial = 0;
        str_recebido_ok = 0;
        return 0;
    }
    str_recebido_ok = 0;
    return 1; // ACHOU OQUE FOI PASSADO
}

void trataSerial(char c) {

    if (checar_serial == 1) {
        if (c == str_recebida[str_posicao]) {
            str_posicao++;
        } else {
            str_posicao = 0;
        }
        if (strlen(str_recebida) == str_posicao) { // encontrou oque foi passado por parametro
            if (armazena_buffer != 0) {
                armazena_buffer = 2;
                buffer_posi = 0;
                return;
            } else {
                str_recebido_ok = 1;
                checar_serial = 0;
            }
        }
        if (armazena_buffer == 2) {
            if (c != '\"' && c != '}' && c != '\r' && c != ' ') { // RETIRA CARACTERES(nao usar buffer a toa)
                buffer[buffer_posi++] = (c == '%' ? ' ' : c);
            }
            if (c == '}' || c == '\r' || (c == ' ' && buffer_posi > 2)) {
                buffer[buffer_posi] = '\0';
                armazena_buffer = 0;
                str_recebido_ok = 1;
                checar_serial = 0;
            }
        }
    }
}

char lerBuffer(char posi, char delim, char*arr) {
    char r = 0, b = 0, p = 0;
    char resultado[20];
    buffer[strlen(buffer)] = delim;
    buffer[(strlen(buffer) + 1)] = '\0';
    while (buffer[b] != '\0') {
        resultado[r] = buffer[b];
        r++;
        if (buffer[b] == delim) {
            if (p == posi) {
                resultado[--r] = '\0';
                //memset(arr, 0, sizeof (arr));
                strcpy(arr, resultado);
                //break;
                return 1;
            }
            // memset(resultado, 0, sizeof (resultado));
            p++;
            r = 0;
        }
        b++;
    }
    return 0;
}

 */

/*   
   char fim;
   if(strlen(buffer)<5){ // Verifica se o buffer esta vazio ou com lixo;
       return -1;
   }
   char resultado[20]; // tamanho da variavel+valor
   //char i = 0;
   strcpy(resultado, strstr(buffer, "ssid")); // retorna ponteiro para a primeira ocorrencia da palavra passada
   return resultado;
   strcpy(resultado, strtok(resultado,",}")); // retorna um ponteriro até a posição onde encontrar ocorrencia dos delimitadores passados
   for (i = 0; i < strlen(resultado); i++) { // Percorre variavel recebida para extrair o valor
       if (resultado[i] == inicio) { // Se encontrar delimitador entre nome da variavel e valor
           strcpy(resultado, &resultado[++i]); // copia a partir do ponteiro que contem o caracter : do valor
           return resultado;
       }
   }
   return -1;
 * */






