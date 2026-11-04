#ifndef TIRAMEDIA_H
#define TIRAMEDIA_H
#define CALIBRANDO 0
#define CONFIRMANDO 1
#define NORMAL 2
#define FALHA_SINAL 3
unsigned char EstadoSistema;
unsigned char Amostras1, Amostras2, Amostras3;
unsigned long Soma1, Soma2, Soma3;
unsigned int Media1, Media2, Media3;
unsigned int Min1, Max1, Max2;
unsigned int InicioFlash;
void TiraMedia_Iniciar() {
    Amostras1 = 0; Amostras2 = 0; Amostras3 = 0;
    Soma1 = 0; Soma2 = 0; Soma3 = 0;
    EstadoSistema = CALIBRANDO;
}
void TiraMedia_Atualizar(unsigned char novos,
                        unsigned int c1, unsigned int c2, unsigned int c3,
                        unsigned int agora) {
    if (EstadoSistema != CALIBRANDO) return;
    if ((novos & 1) && (Amostras1 < 100)) { Soma1 += c1; Amostras1++; }
    if ((novos & 2) && (Amostras2 < 100)) { Soma2 += c2; Amostras2++; }
    if ((novos & 4) && (Amostras3 < 100)) { Soma3 += c3; Amostras3++; }
    if ((Amostras1 == 100) && (Amostras2 == 100) && (Amostras3 == 100)) {
        Media1 = Soma1 / 100;
        Media2 = Soma2 / 100;
        Media3 = Soma3 / 100;
        Min1 = Media1 - Media1 / 20;
        Max1 = Media1 + Media1 / 20;
        Max2 = Media2 + Media2 / 20;
        InicioFlash = agora;
        EstadoSistema = CONFIRMANDO;
    }
}
unsigned char TiraMedia_Confirmacao(unsigned int agora) {
    if (EstadoSistema != CONFIRMANDO) return 0;
    if ((unsigned int)(agora - InicioFlash) < 150) {
        SetaEsquerda = 1; SetaDireita = 1;
        FarolDireito = 1; FarolEsquerdo = 1;
        Neon_BrakeLight = 1; Re_Lanterna = 1;
        FreioPWM(PWM_ACIONADO);
    } else {
        SetaEsquerda = 0; SetaDireita = 0;
        FarolDireito = 0; FarolEsquerdo = 0;
        Neon_BrakeLight = 0; Re_Lanterna = 0;
        FreioPWM(PWM_SOLTO);
        EstadoSistema = NORMAL;
    }
    return 1;
}
#endif
