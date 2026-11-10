#ifndef RC_CANAIS_H
#define RC_CANAIS_H
// Timer1 1 us/tick. RBIF detecta bordas de RB7/RB6/RB5.
// Variaveis de 16 bits precisam de copia atomica no main.
volatile unsigned int ValorCanal1, ValorCanal2, ValorCanal3;
unsigned int InicioCanal1, InicioCanal2, InicioCanal3;
unsigned char EstadoAnteriorRC;
volatile unsigned char NovosRC;
volatile unsigned char Idade1, Idade2, Idade3; // ms, saturados em 255

unsigned int RC_Tempo() {
    unsigned char hi1, hi2, lo;
    // Leitura consistente mesmo na virada de 0xXXFF para 0x(X+1)00.
    // Leitura dos bytes pode ocorrer em qualquer ponto da contagem.
    do {
        hi1 = TMR1H;
        lo = TMR1L;
        hi2 = TMR1H;
    } while (hi1 != hi2);
    return ((unsigned int)hi2 << 8) | lo;
}
void RC_Inicializar() {
    ValorCanal1 = 0; ValorCanal2 = 0; ValorCanal3 = 0;
    NovosRC = 0;
    Idade1 = 255; Idade2 = 255; Idade3 = 255;
    T1CON = 0;
    TMR1H = 0; TMR1L = 0;
    T1CON = 1; // Fosc/4, prescaler 1:1, Timer1 ON
    EstadoAnteriorRC = PORTB & 0xE0;
    INTCON.RBIF = 0;
    INTCON.RBIE = 1;
}
void RC_Interrupcao() {
    unsigned char estado, mudou;
    unsigned int agora, largura;
    estado = PORTB & 0xE0;
    mudou = estado ^ EstadoAnteriorRC;
    EstadoAnteriorRC = estado;
    agora = RC_Tempo();

    if (mudou & 0x80) {
        if (estado & 0x80) InicioCanal1 = agora;
        else {
            largura = agora - InicioCanal1;
            if ((largura >= 900) && (largura <= 2100)) {
                ValorCanal1 = largura; Idade1 = 0; NovosRC |= 1;
            }
        }
    }
    if (mudou & 0x40) {
        if (estado & 0x40) InicioCanal2 = agora;
        else {
            largura = agora - InicioCanal2;
            if ((largura >= 900) && (largura <= 2100)) {
                ValorCanal2 = largura; Idade2 = 0; NovosRC |= 2;
            }
        }
    }
    if (mudou & 0x20) {
        if (estado & 0x20) InicioCanal3 = agora;
        else {
            largura = agora - InicioCanal3;
            if ((largura >= 900) && (largura <= 2100)) {
                ValorCanal3 = largura; Idade3 = 0; NovosRC |= 4;
            }
        }
    }
    INTCON.RBIF = 0;
}
#endif
