#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H
// PIC16F628A, 4 MHz interno. Comparadores desligados.
sbit Canal1 at RB7_bit;
sbit Canal2 at RB6_bit;
sbit Canal3 at RB5_bit;
sbit Freio at RB3_bit;
sbit FarolDireito at RB2_bit;
sbit FarolEsquerdo at RB1_bit;
sbit SetaEsquerda at RB0_bit;
sbit SetaDireita at RA3_bit;
sbit Botao at RA4_bit;
sbit Neon_BrakeLight at RA2_bit;
sbit Re_Lanterna at RA0_bit;

void ConfigurarPortas() {
    CMCON = 7;
    PORTA = 0;
    PORTB = 0;
    TRISA = 0x30;
    TRISB = 0xE0;
}
#endif
