#ifndef RC_CANAIS_H
#define RC_CANAIS_H

// Amostragem nominal: 100 us com Fosc=4 MHz.
// Todos os dados de 16 bits sao copiados com GIE=0.
// A rotina RC_Interrupcao() deve ser chamada pelo interrupt() do main.

volatile unsigned int RC_Ticks = 0;
volatile unsigned int RC_Inicio[3];
volatile unsigned int RC_Ultimo[3];
volatile unsigned int RC_Largura[3];
volatile unsigned char RC_Valido[3];
volatile unsigned char RC_Anterior = 0;

void RC_Inicializar() {
    unsigned char i;
    RC_Ticks = 0;
    RC_Anterior = PORTB & 0xE0;
    for (i = 0; i < 3; i++) {
        RC_Inicio[i] = 0;
        RC_Ultimo[i] = 0;
        RC_Largura[i] = 0;
        RC_Valido[i] = 0;
    }

    // TMR0 interno, prescaler 1:1 (PSA=1), Fosc/4=1 MHz
    OPTION_REG = 0b10001000;
    TMR0 = 156;            // 100 contagens por interrupcao
    INTCON.T0IF = 0;
    INTCON.T0IE = 1;
    INTCON.GIE = 1;
}

// Executada a cada overflow de Timer0.
void RC_Interrupcao() {
    unsigned char atual;
    unsigned char mascara;
    unsigned char i;
    unsigned int duracao;

    // Compensa parcialmente a latencia da interrupcao.
    TMR0 = TMR0 + 156;
    INTCON.T0IF = 0;
    RC_Ticks++;

    atual = PORTB & 0xE0;
    mascara = 0x80;
    for (i = 0; i < 3; i++) {
        if ((atual & mascara) && !(RC_Anterior & mascara)) {
            RC_Inicio[i] = RC_Ticks;
        }
        if (!(atual & mascara) && (RC_Anterior & mascara)) {
            duracao = (unsigned int)(RC_Ticks - RC_Inicio[i]);
            if ((duracao >= 8) && (duracao <= 22)) {
                RC_Largura[i] = duracao;
                RC_Ultimo[i] = RC_Ticks;
                RC_Valido[i] = 1;
            } else {
                RC_Valido[i] = 0;
            }
        }
        if ((unsigned int)(RC_Ticks - RC_Ultimo[i]) > 300)
            RC_Valido[i] = 0;

        mascara = mascara >> 1;
    }
    RC_Anterior = atual;
}

// Canal: 1, 2 ou 3. Retorna 0 se o sinal estiver ausente.
// Retorno em us, quantizado em passos nominais de 100 us.
unsigned int RC_LerCanal(unsigned char canal) {
    unsigned int largura;
    unsigned char valido;
    unsigned char gie;
    unsigned char i;

    if ((canal < 1) || (canal > 3)) return 0;
    i = canal - 1;
    gie = INTCON.GIE;
    INTCON.GIE = 0;
    largura = RC_Largura[i];
    valido = RC_Valido[i];
    if (gie) INTCON.GIE = 1;

    if (!valido) return 0;
    return largura * 100;
}
#endif
