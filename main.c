#include "Configuracao.h"
#include "RC_Canais.h"

// Teste inicial: RB2 (FarolDireito) alterna a cada ~500 ms.
// Os canais RC sao capturados em segundo plano pela interrupcao.
// Use RC_LerCanal(1), (2) e (3) para ler os pulsos.

void interrupt() {
    if (INTCON.T0IF) {
        RC_Interrupcao();
    }
}

void main() {
    unsigned int anterior = 0;
    unsigned int agora;
    unsigned int ch1, ch2, ch3;

    ConfigurarPortas();
    RC_Inicializar();

    while (1) {
        ch1 = RC_LerCanal(1);
        ch2 = RC_LerCanal(2);
        ch3 = RC_LerCanal(3);

        // Os valores estao disponiveis para os proximos modulos.
        // Nesta etapa nao controlam as luzes.
        agora = RC_Ticks; // Leitura de teste: proteger se usar como tempo preciso.
        if ((unsigned int)(agora - anterior) >= 5000) {
            anterior = agora;
            FarolDireito = !FarolDireito;
        }
    }
}
