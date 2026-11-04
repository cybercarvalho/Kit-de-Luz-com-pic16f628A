#ifndef TRATACANAL1_H
#define TRATACANAL1_H
unsigned int TempoSetas;
unsigned char FaseSetas;
unsigned char DirecaoSeta;
unsigned char CandidatoSeta;
unsigned char ContadorSeta;
void TrataCanal1_Iniciar(unsigned int agora) {
    TempoSetas = agora;
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
    unsigned int margem;
    if (novaLeitura) {
        margem = Media1 / 33;
        candidato = DirecaoSeta;
        if (DirecaoSeta == 1) {
            if (pulso >= (Media1 - margem)) candidato = 0;
        } else if (DirecaoSeta == 2) {
            if (pulso <= (Media1 + margem)) candidato = 0;
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
                if (DirecaoSeta) {
                    FaseSetas = 1;
                    TempoSetas = agora;
                }
            }
        }
    }
    if (DirecaoSeta) {
        if ((unsigned int)(agora - TempoSetas) >= 500) {
            TempoSetas = agora;
            FaseSetas = !FaseSetas;
        }
    } else {
        FaseSetas = 0;
        TempoSetas = agora;
    }
    SetaEsquerda = (DirecaoSeta == 1) && FaseSetas;
    SetaDireita = (DirecaoSeta == 2) && FaseSetas;
}
#endif
