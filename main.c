#include "Configuracao.h"
#include "RC_Canais.h"

// Interrupcao centralizada no programa principal.
void interrupt() {
    if (INTCON.RBIF) {
        RC_Interrupcao();
    }
}

void main() {
    ConfigurarPortas();
    RC_Inicializar();

    while (1) {
        // ValorCanal1, ValorCanal2 e ValorCanal3
        // sao atualizados pela interrupcao RBIF.
        // Valores em microssegundos, ou 0 para pulso invalido.
        // Nenhum efeito de luz nesta versao de teste.
    }
}
