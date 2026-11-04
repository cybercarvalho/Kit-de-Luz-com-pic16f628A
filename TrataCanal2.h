#ifndef TRATACANAL2_H
#define TRATACANAL2_H
unsigned char FreioAtivo;
unsigned char CandidatoFreio;
unsigned char ContadorFreio;
void TrataCanal2_Iniciar() {
    FreioAtivo = 0;
    CandidatoFreio = 0;
    ContadorFreio = 0;
    FreioPWM(PWM_SOLTO);
}
void TrataCanal2(unsigned int pulso, unsigned char novaLeitura) {
    unsigned char candidato;
    if (novaLeitura) {
        candidato = FreioAtivo;
        if (!FreioAtivo) {
            if (pulso > Max2) candidato = 1;
        } else {
            if (pulso < (Media2 + Media2 / 33)) candidato = 0;
        }
        if (candidato == FreioAtivo) {
            CandidatoFreio = candidato;
            ContadorFreio = 0;
        } else {
            if (candidato != CandidatoFreio) {
                CandidatoFreio = candidato;
                ContadorFreio = 1;
            } else if (ContadorFreio < 3) {
                ContadorFreio++;
            }
            if (ContadorFreio >= 3) {
                FreioAtivo = candidato;
                ContadorFreio = 0;
            }
        }
    }
    FreioPWM(FreioAtivo ? PWM_ACIONADO : PWM_SOLTO);
}
#endif
