#ifndef TRATACANAL1_H
#define TRATACANAL1_H

// 3 leituras consecutivas para trocar de estado.
// 5% para acionar; 3% para liberar (histerese).
unsigned int TempoSetas;
unsigned char FaseSetas;
unsigned char DirecaoSeta;  // 0=nenhuma, 1=esquerda, 2=direita
unsigned char CandidatoSeta;
unsigned char ContadorSeta;

// PWM por software dos dois farois: Timer2 a 4 kHz,
// 16 passos => 250 Hz. Duty 10 (escala 0..255) vira 1/16.
volatile unsigned char FasePWMFarois;
volatile unsigned char NivelPWMFarois;

void FaroisPWM_Interrupcao() {
    FasePWMFarois++;
    if (FasePWMFarois >= 16) FasePWMFarois = 0;
    FarolDireito = (FasePWMFarois < NivelPWMFarois);
    FarolEsquerdo = FarolDireito;
}

void TrataCanal1_Programa() {
    if (Programa == 0) NivelPWMFarois = 0;
    else if (Programa == 1) NivelPWMFarois = 1;
    else NivelPWMFarois = 16;
}

void TrataCanal1_Iniciar(unsigned int agora) {
    TempoSetas = agora;
    FasePWMFarois = 0;
    NivelPWMFarois = 0;
    FaseSetas = 0;
    DirecaoSeta = 0;
    CandidatoSeta = 0;
    ContadorSeta = 0;
    SetaEsquerda = 0;
    SetaDireita = 0;
}

void TrataCanal1(unsigned int pulso, unsigned int agora,
                 unsigned char novaLeitura) {
    unsigned char candidato;

    TrataCanal1_Programa();

    // Avaliar apenas uma vez por pulso RC recebido, nunca por giro do while.
    if (novaLeitura) {
        candidato = DirecaoSeta;

        if (DirecaoSeta == 1) {
            if (pulso >= (Media1 - Margem1Soltar)) candidato = 0;
        } else if (DirecaoSeta == 2) {
            if (pulso <= (Media1 + Margem1Soltar)) candidato = 0;
        } else {
            if (pulso < Min1) candidato = 1;
            else if (pulso > Max1) candidato = 2;
            else candidato = 0;
        }

        if (candidato == DirecaoSeta) {
            ContadorSeta = 0;
            CandidatoSeta = candidato;
        } else {
            if (candidato != CandidatoSeta) {
                CandidatoSeta = candidato;
                ContadorSeta = 1;
            } else if (ContadorSeta < 3) {
                ContadorSeta++;
            }
            if (ContadorSeta >= 3) {
                DirecaoSeta = candidato;
                ContadorSeta = 0;
                // Comecar piscada imediatamente ao confirmar comando.
                if (DirecaoSeta) {
                    FaseSetas = 1;
                    TempoSetas = agora;
                }
            }
        }
    }

    if (DirecaoSeta || Programa == 3) {
        if ((unsigned int)(agora - TempoSetas) >= 500) {
            TempoSetas = agora;
            FaseSetas = !FaseSetas;
        }
    } else {
        FaseSetas = 0;
        TempoSetas = agora;
    }
    SetaEsquerda = FaseSetas && ((DirecaoSeta == 1) || (Programa == 3));
    SetaDireita = FaseSetas && ((DirecaoSeta == 2) || (Programa == 3));
}
#endif
