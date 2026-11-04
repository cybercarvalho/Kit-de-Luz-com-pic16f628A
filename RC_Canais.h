#ifndef RC_CANAIS_H
#define RC_CANAIS_H

// PIC16F628A - Fosc 4 MHz - Timer1 = 1 tick/us
// Tres valores independentes (6 bytes no total).
volatile unsigned int ValorCanal1 = 0;
volatile unsigned int ValorCanal2 = 0;
volatile unsigned int ValorCanal3 = 0;

// Instantes das bordas de subida (6 bytes).
volatile unsigned int InicioCanal1 = 0;
volatile unsigned int InicioCanal2 = 0;
volatile unsigned int InicioCanal3 = 0;
volatile unsigned char EstadoAnteriorRC = 0;

// Leitura do contador de 16 bits; usada somente na interrupcao.
// Timer1 roda em modo temporizador interno (Fosc/4).
unsigned int RC_Tempo() {
    unsigned char h, l;
    h = TMR1H;
    l = TMR1L;
    return ((unsigned int)h << 8) | l;
}

void RC_Inicializar() {
    T1CON = 0x00;
    TMR1H = 0;
    TMR1L = 0;
    T1CON = 0x01;          // Timer1 ON, clock interno, prescaler 1:1
    EstadoAnteriorRC = PORTB & 0xE0;
    INTCON.RBIF = 0;
    INTCON.RBIE = 1;
    INTCON.GIE = 1;
}

// Chamar no interrupt() do programa principal quando RBIF = 1.
// Detecta subida e descida, calcula largura com aritmetica modular
// de 16 bits (funciona mesmo quando Timer1 passa de 65535 para 0).
void RC_Interrupcao() {
    unsigned char estado;
    unsigned char alterados;
    unsigned int agora;
    unsigned int largura;

    estado = PORTB & 0xE0;  // Le PORTB para encerrar a condicao de mismatch
    alterados = estado ^ EstadoAnteriorRC;
    EstadoAnteriorRC = estado;
    agora = RC_Tempo();

    if (alterados & 0x80) {
        if (estado & 0x80) InicioCanal1 = agora;
        else {
            largura = (unsigned int)(agora - InicioCanal1);
            if ((largura >= 900) && (largura <= 2100))
                ValorCanal1 = largura;
            else ValorCanal1 = 0;
        }
    }

    if (alterados & 0x40) {
        if (estado & 0x40) InicioCanal2 = agora;
        else {
            largura = (unsigned int)(agora - InicioCanal2);
            if ((largura >= 900) && (largura <= 2100))
                ValorCanal2 = largura;
            else ValorCanal2 = 0;
        }
    }

    if (alterados & 0x20) {
        if (estado & 0x20) InicioCanal3 = agora;
        else {
            largura = (unsigned int)(agora - InicioCanal3);
            if ((largura >= 900) && (largura <= 2100))
                ValorCanal3 = largura;
            else ValorCanal3 = 0;
        }
    }
    INTCON.RBIF = 0;
}

// Para copiar um valor de 16 bits no main sem leitura interrompida:
// desabilite GIE, copie ValorCanalN, restaure GIE.
// Nesta primeira versao nao ha timeout por ausencia de pulsos.
#endif
