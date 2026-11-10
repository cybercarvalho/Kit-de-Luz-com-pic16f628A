#ifndef TIRAMEDIA_H
#define TIRAMEDIA_H
// Estado: 0=coleta, 1=flash, 2=normal, 3=failsafe
#define CALIBRANDO 0
#define CONFIRMANDO 1
#define NORMAL 2
#define FALHA_SINAL 3

unsigned char EstadoSistema;
unsigned char Amostras1, Amostras2;
unsigned int Soma1, Soma2;
unsigned int Media1, Media2;
unsigned int Min1, Max1, Min2, Max2, Margem1Soltar, Limite2Soltar, Limite2SoltarInv, Limite2Prog;

void TiraMedia_Iniciar() {
    Amostras1 = 0; Amostras2 = 0;
    Soma1 = 0; Soma2 = 0;
    EstadoSistema = CALIBRANDO;
}

// Chamado quando os canais 1 e 2 estiverem presentes.
// A mascara "novos" indica pulsos completos desde a ultima leitura.
void TiraMedia_Atualizar(unsigned char novos,
                        unsigned int c1, unsigned int c2,
                        unsigned int agora) {
    if (EstadoSistema != CALIBRANDO) return;
    if ((novos & 1) && (Amostras1 < 20)) {
        Soma1 += c1; Amostras1++;
    }
    if ((novos & 2) && (Amostras2 < 20)) {
        Soma2 += c2; Amostras2++;
    }
    if ((Amostras1 == 20) && (Amostras2 == 20)) {
        unsigned int margem;
        Media1 = Soma1 / 20;
        Media2 = Soma2 / 20;
        margem = Media1 / 20;
        Min1 = Media1 - margem;
        Max1 = Media1 + margem;
        margem = Media2 / 20;
        Max2 = Media2 + margem;
        Min2 = Media2 - margem;
        Margem1Soltar = Media1 / 33;
        margem = Media2 / 33;
        Limite2SoltarInv = Media2 - margem;
        Limite2Soltar = Media2 + margem;
        Limite2Prog = Media2 + Media2 / 10; // 10% so na programacao
        EstadoSistema = CONFIRMANDO;
    }
}

// Confirmacao visual: 10 piscadas (10 ms aceso + 10 ms apagado).
// Delay bloqueia o loop por ~200 ms; interrupcoes continuam habilitadas.
unsigned char TiraMedia_Confirmacao(unsigned int agora) {
    unsigned char i;
    if (EstadoSistema != CONFIRMANDO) return 0;
    for (i = 0; i < 10; i++) {
        SetaEsquerda = 1; SetaDireita = 1;
        FarolDireito = 1; FarolEsquerdo = 1;
        Neon_BrakeLight = 1; Re_Lanterna = 1;
        FreioPWM(PWM_ACIONADO);
        Delay_ms(10);
        SetaEsquerda = 0; SetaDireita = 0;
        FarolDireito = 0; FarolEsquerdo = 0;
        Neon_BrakeLight = 0; Re_Lanterna = 0;
        FreioPWM(PWM_SOLTO);
        Delay_ms(10);
    }
    // Garantia final: todas as luzes apagadas apos o for.
    SetaEsquerda = 0; SetaDireita = 0;
    FarolDireito = 0; FarolEsquerdo = 0;
    Neon_BrakeLight = 0; Re_Lanterna = 0;
    FreioPWM(0);
    EstadoSistema = NORMAL;
    return 1;
}
#endif
