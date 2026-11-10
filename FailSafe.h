#ifndef FAILSAFE_H
#define FAILSAFE_H

#define FAILSAFE_TIMEOUT_MS 100
#define FAILSAFE_PISCA_MS 100

unsigned int TempoFailSafe;
unsigned char FaseFailSafe;

void FailSafe_Inicializar() {
    EstadoSistema = FALHA_SINAL;
    TempoFailSafe = 0;
    FaseFailSafe = 1;
}

// Idades capturadas atomicamente no main.
unsigned char FailSafe_SinalPerdido(unsigned char idade1,
                                   unsigned char idade2,
                                   unsigned char idade3) {
    return (idade1 >= FAILSAFE_TIMEOUT_MS) ||
           (idade2 >= FAILSAFE_TIMEOUT_MS) ||
           (idade3 >= FAILSAFE_TIMEOUT_MS);
}

void FailSafe_Executar(unsigned int agora) {
    if (EstadoSistema != FALHA_SINAL) {
        EstadoSistema = FALHA_SINAL;
        TempoFailSafe = agora;
        FaseFailSafe = 1;
    }
    if ((unsigned int)(agora - TempoFailSafe) >= FAILSAFE_PISCA_MS) {
        TempoFailSafe = agora;
        FaseFailSafe = !FaseFailSafe;
    }
    SetaEsquerda = FaseFailSafe;
    SetaDireita = FaseFailSafe;
    FreioPWM(FaseFailSafe ? PWM_ACIONADO : PWM_SOLTO);
}

// Retorna 1 apenas no primeiro ciclo apos a recuperacao.
unsigned char FailSafe_Recuperou() {
    if (EstadoSistema != FALHA_SINAL) return 0;
    TiraMedia_Iniciar();
    SetaEsquerda = 0;
    SetaDireita = 0;
    FreioPWM(PWM_SOLTO);
    return 1;
}
#endif
