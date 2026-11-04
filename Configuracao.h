#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

// Entradas do receptor RC
sbit Canal1 at RB7_bit;
sbit Canal2 at RB6_bit;
sbit Canal3 at RB5_bit;

// Entrada do botao
sbit Botao at RA4_bit;

// Saidas
sbit Freio at RB3_bit;
sbit FarolDireito at RB2_bit;
sbit FarolEsquerdo at RB1_bit;
sbit SetaEsquerda at RB0_bit;
sbit SetaDireita at RA3_bit;
sbit Neon_BrakeLight at RA2_bit;
sbit Re_Lanterna at RA0_bit;

void ConfigurarPortas() {
    CMCON = 7;             // Comparadores desligados
    PORTA = 0;
    PORTB = 0;
    TRISA = 0b00110000;   // RA4 botao; RA5 somente entrada
    TRISB = 0b11100000;   // RB7, RB6, RB5 entradas RC
}
#endif
