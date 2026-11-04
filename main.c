#include "Configuracao.h"
#include "RC_Canais.h"
#include "Freio_Setas.h"

// Timer0: ~1 ms por interrupcao (4 MHz, prescaler 1:4).
volatile unsigned int Milissegundos;
unsigned int UltimoPisca;
unsigned int InicioFlash;
unsigned char Modo; // 0 = calibracao, 1 = normal, 2 = flash de confirmacao
unsigned char Pisca;
unsigned char Cont1, Cont2, Cont3;
unsigned long Soma1, Soma2, Soma3;
unsigned int Media1, Media2, Media3;
unsigned int Min1, Max1, Max2;

void interrupt() {
    if (INTCON.RBIF) RC_Interrupcao();
    if (INTCON.T0IF) {
        TMR0 += 6;             // ~1 ms; verificar calibracao em hardware
        INTCON.T0IF = 0;
        Milissegundos++;
        if (Idade1 != 255) Idade1++;
        if (Idade2 != 255) Idade2++;
        if (Idade3 != 255) Idade3++;
    }
}

void IniciarCalibracao() {
    Modo = 0;
    Cont1 = 0; Cont2 = 0; Cont3 = 0;
    Soma1 = 0; Soma2 = 0; Soma3 = 0;
    INTCON.GIE = 0;
    NovosRC = 0;
    INTCON.GIE = 1;
}

void main() {
    unsigned char novos, perdido;
    unsigned int c1, c2, c3, tempo;
    unsigned char intervalo;
    ConfigurarPortas();
    Milissegundos = 0;
    OPTION_REG = 0x81;  // TMR0 clock interno, prescaler 1:4
    TMR0 = 6;
    INTCON.T0IF = 0;
    INTCON.T0IE = 1;
    RC_Inicializar();
    FreioInicializar();
    IniciarCalibracao();
    UltimoPisca = 0;
    Pisca = 0;
    INTCON.GIE = 1;

    while (1) {
        // Snapshot atomico: 16 bits nao sao atomicos no PIC de 8 bits.
        INTCON.GIE = 0;
        c1 = ValorCanal1; c2 = ValorCanal2; c3 = ValorCanal3;
        novos = NovosRC;
        NovosRC = 0;
        perdido = (Idade1 >= 100) || (Idade2 >= 100) || (Idade3 >= 100);
        tempo = Milissegundos;
        INTCON.GIE = 1;

        if (perdido) {
            if (Modo) IniciarCalibracao();
            // Se a calibracao for interrompida, reiniciar acumuladores.
            if (Cont1 || Cont2 || Cont3) IniciarCalibracao();
            intervalo = 100;
            if ((unsigned int)(tempo - UltimoPisca) >= intervalo) {
                UltimoPisca = tempo;
                Pisca = !Pisca;
            }
            SetaEsquerda = Pisca;
            SetaDireita = Pisca;
            FreioPWM(Pisca ? PWM_ACIONADO : PWM_SOLTO);
            continue;
        }

        if (!Modo) {
            // Coleta ate 100 pulsos VALIDOS de cada canal.
            if ((novos & 1) && (Cont1 < 100)) { Soma1 += c1; Cont1++; }
            if ((novos & 2) && (Cont2 < 100)) { Soma2 += c2; Cont2++; }
            if ((novos & 4) && (Cont3 < 100)) { Soma3 += c3; Cont3++; }
            SetaEsquerda = 0;
            SetaDireita = 0;
            FreioPWM(PWM_SOLTO);
            if ((Cont1 == 100) && (Cont2 == 100) && (Cont3 == 100)) {
                Media1 = Soma1 / 100;
                Media2 = Soma2 / 100;
                Media3 = Soma3 / 100;
                Min1 = Media1 - Media1 / 20;
                Max1 = Media1 + Media1 / 20;
                Max2 = Media2 + Media2 / 20;
                Modo = 2;           // Confirmacao visual da media
                InicioFlash = tempo;
                Pisca = 0;
                SetaEsquerda = 1;
                SetaDireita = 1;
                FarolDireito = 1;
                FarolEsquerdo = 1;
                Neon_BrakeLight = 1;
                Re_Lanterna = 1;
                FreioPWM(PWM_ACIONADO);
            }
            continue;
        }

        if (Modo == 2) {
            // Flash unico de 150 ms, sem delay e sem novas variaveis.
            if ((unsigned int)(tempo - InicioFlash) < 150) {
                SetaEsquerda = 1;
                SetaDireita = 1;
                FarolDireito = 1;
                FarolEsquerdo = 1;
                Neon_BrakeLight = 1;
                Re_Lanterna = 1;
                FreioPWM(PWM_ACIONADO);
            } else {
                SetaEsquerda = 0;
                SetaDireita = 0;
                FarolDireito = 0;
                FarolEsquerdo = 0;
                Neon_BrakeLight = 0;
                Re_Lanterna = 0;
                FreioPWM(PWM_SOLTO);
                Pisca = 0;
                UltimoPisca = tempo;
                Modo = 1;
            }
            continue;
        }

        FreioPWM(c2 > Max2 ? PWM_ACIONADO : PWM_SOLTO);
        if ((unsigned int)(tempo - UltimoPisca) >= 500) {
            UltimoPisca = tempo;
            Pisca = !Pisca;
        }
        SetaEsquerda = (c1 < Min1) && Pisca;
        SetaDireita = (c1 > Max1) && Pisca;
    }
}
