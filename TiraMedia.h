#ifndef TIRAMEDIA_H
#define TIRAMEDIA_H
// Estado: 0=coleta, 1=flash, 2=normal, 3=failsafe
#define CALIBRANDO 0
#define CONFIRMANDO 1
#define NORMAL 2
#define FALHA_SINAL 3

unsigned char EstadoSistema;
unsigned char Amostras1, Amostras2, Amostras3;
unsigned long Soma1, Soma2, Soma3;
unsigned int Media1, Media2, Media3;
unsigned int Min1, Max1, Max2;

void TiraMedia_Iniciar() {
    Amostras1 = 0; Amostras2 = 0; Amostras3 = 0;
    Soma1 = 0; Soma2 = 0; Soma3 = 0;
    EstadoSistema = CALIBRANDO;
}

// Chamado apenas quando o sinal dos 3 canais estiver presente.
// A mascara "novos" indica pulsos completos desde a ultima leitura.
void TiraMedia_Atualizar(unsigned char novos,
                        unsigned int c1, unsigned int c2, unsigned int c3,
                        unsigned int agora) {
    if (EstadoSistema != CALIBRANDO) return;
    if ((novos & 1) && (Amostras1 < 100)) {
        Soma1 += c1; Amostras1++;
    }
    if ((novos & 2) && (Amostras2 < 100)) {
        Soma2 += c2; Amostras2++;
    }
    if ((novos & 4) && (Amostras3 < 100)) {
        Soma3 += c3; Amostras3++;
    }
    if ((Amostras1 == 100) && (Amostras2 == 100) &&
        (Amostras3 == 100)) {
        Media1 = Soma1 / 100;
        Media2 = Soma2 / 100;
        Media3 = Soma3 / 100;
        Min1 = Media1 - Media1 / 20;
        Max1 = Media1 + Media1 / 20;
        Max2 = Media2 + Media2 / 20;
        EstadoSistema = CONFIRMANDO;
    }
}

// Confirmacao visual: 10 piscadas (50 ms aceso + 50 ms apagado).
// Delay bloqueia o loop por ~1 s; interrupcoes continuam habilitadas.
unsigned char TiraMedia_Confirmacao(unsigned int agora) {
    unsigned char i;
    if (EstadoSistema != CONFIRMANDO) return 0;
    for (i = 0; i < 10; i++) {
        SetaEsquerda = 1; SetaDireita = 1;
        FarolDireito = 1; FarolEsquerdo = 1;
        Neon_BrakeLight = 1; Re_Lanterna = 1;
        FreioPWM(PWM_ACIONADO);
        Delay_ms(50);
        SetaEsquerda = 0; SetaDireita = 0;
        FarolDireito = 0; FarolEsquerdo = 0;
        Neon_BrakeLight = 0; Re_Lanterna = 0;
        FreioPWM(PWM_SOLTO);
        Delay_ms(50);
    }
    EstadoSistema = NORMAL;
    return 1;
}
#endif
