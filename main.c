#include "Configuracao.h"
#include "RC_Canais.h"
#include "Freio_Setas.h"
#include "TiraMedia.h"
#include "TrataCanal1.h"
#include "TrataCanal2.h"
#include "TrataCanal3.h"
volatile unsigned int Milissegundos;
unsigned int TempoFailSafe;
unsigned char FaseFailSafe;
void interrupt() {
    if (INTCON.RBIF) RC_Interrupcao();
    if (INTCON.T0IF) {
        TMR0 += 6;
        INTCON.T0IF = 0;
        Milissegundos++;
        if (Idade1 != 255) Idade1++;
        if (Idade2 != 255) Idade2++;
        if (Idade3 != 255) Idade3++;
    }
}
void FalhaSinal(unsigned int agora) {
    if (EstadoSistema != FALHA_SINAL) {
        EstadoSistema = FALHA_SINAL;
        TempoFailSafe = agora;
        FaseFailSafe = 1;
    }
    if ((unsigned int)(agora - TempoFailSafe) >= 100) {
        TempoFailSafe = agora;
        FaseFailSafe = !FaseFailSafe;
    }
    SetaEsquerda = FaseFailSafe;
    SetaDireita = FaseFailSafe;
    FreioPWM(FaseFailSafe ? PWM_ACIONADO : PWM_SOLTO);
}
void main() {
    unsigned int c1, c2, c3, agora;
    unsigned char novos, perdido;
    unsigned char estadoAnterior;
    ConfigurarPortas();
    Milissegundos = 0;
    OPTION_REG = 0x81;
    TMR0 = 6;
    INTCON.T0IF = 0;
    INTCON.T0IE = 1;
    RC_Inicializar();
    FreioInicializar();
    EstadoSistema = FALHA_SINAL;
    TempoFailSafe = 0;
    FaseFailSafe = 1;
    INTCON.GIE = 1;
    while (1) {
        INTCON.GIE = 0;
        c1 = ValorCanal1; c2 = ValorCanal2; c3 = ValorCanal3;
        novos = NovosRC;
        NovosRC = 0;
        perdido = (Idade1 >= 100) || (Idade2 >= 100) || (Idade3 >= 100);
        agora = Milissegundos;
        INTCON.GIE = 1;
        if (perdido) {
            FalhaSinal(agora);
            continue;
        }
        if (EstadoSistema == FALHA_SINAL) {
            TiraMedia_Iniciar();
            SetaEsquerda = 0; SetaDireita = 0;
            FreioPWM(PWM_SOLTO);
        }
        if (EstadoSistema == CALIBRANDO) {
            TiraMedia_Atualizar(novos, c1, c2, c3, agora);
            continue;
        }
        estadoAnterior = EstadoSistema;
        if (TiraMedia_Confirmacao(agora)) {
            if ((estadoAnterior == CONFIRMANDO) &&
                (EstadoSistema == NORMAL)) {
                TrataCanal1_Iniciar(agora);
                TrataCanal2_Iniciar();
            }
            continue;
        }
        TrataCanal1(c1, agora, novos & 1);
        TrataCanal2(c2, novos & 2);
        TrataCanal3(c3);
    }
}
