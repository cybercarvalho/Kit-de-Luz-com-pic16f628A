#ifndef TRATACANAL3_H
#define TRATACANAL3_H
// ModoProg: 0=normal, 1=sequencia inicial, 2=apagado 2s,
// 3=pisca 100ms, 4=apagado apos confirmar inversao.
// LuzProg: 0=apagado, 1=setas, 2=farois+freio, 3=todas.
unsigned char Programa, ModoProg, Ch3Estado, Ch2Armado, Ch2Altos;
unsigned char FreioInvertido, LuzProg, PassoLuz;
unsigned int TempoCh3, TempoProg;

void TrataCanal3_Iniciar(unsigned int agora) {
    Programa = 0;
    ModoProg = 0;
    Ch3Estado = 0;
    Ch2Armado = 0;
    Ch2Altos = 0;
    LuzProg = 0;
    PassoLuz = 0;
    TempoCh3 = agora;
    TempoProg = agora;
    FreioInvertido = (LerFreioEEPROM() == 0x5A);
}
void TrataCanal3_Cancelar(unsigned int agora) {
    ModoProg = 0;
    Ch3Estado = 0;
    Ch2Armado = 0;
    Ch2Altos = 0;
    LuzProg = 0;
    PassoLuz = 0;
    TempoCh3 = agora;
}
void TrataCanal3(unsigned int c3, unsigned int c2,
                 unsigned int agora, unsigned char novo2) {
    unsigned int intervalo;
    if (c3 <= 1500) {
        // Pulso curto baixo-alto-baixo troca programa.
        if ((Ch3Estado == 2) && (ModoProg == 1)) {
            Programa++;
            if (Programa == 4) Programa = 0;
        }
        Ch3Estado = 1;
        ModoProg = 0;
        Ch2Armado = 0;
    Ch2Altos = 0;
        LuzProg = 0;
        PassoLuz = 0;
        return;
    }
    if (Ch3Estado != 2) {
        Ch3Estado = 2;
        TempoCh3 = agora;
        TempoProg = agora;
        ModoProg = 1;
        PassoLuz = 0;
        LuzProg = 1;
    }

    if (ModoProg == 1) {
        if ((unsigned int)(agora - TempoCh3) >= 10000) {
            ModoProg = 2;
            TempoProg = agora;
            LuzProg = 0;
            Ch2Armado = 0;
            Ch2Altos = 0;
            return;
        }
        // 0-50ms: setas; 50-100ms: farois e freio;
        // 100-350ms: tudo apagado; repete enquanto C3 alto.
        intervalo = (PassoLuz == 2) ? 250 : 50;
        if ((unsigned int)(agora - TempoProg) >= intervalo) {
            TempoProg = agora;
            PassoLuz++;
            if (PassoLuz == 3) PassoLuz = 0;
            LuzProg = (PassoLuz == 0) ? 1 : (PassoLuz == 1) ? 2 : 0;
        }
        return;
    }
    // Aos 10s: todas apagadas por 2s antes da piscada rapida.
    if (ModoProg == 2) {
        LuzProg = 0;
        if ((unsigned int)(agora - TempoProg) >= 2000) {
            ModoProg = 3;
            TempoProg = agora;
            LuzProg = 3;
            Ch2Armado = 0;
            Ch2Altos = 0;
        }
        return;
    }
    // Apos confirmar a inversao, todas as luzes ficam apagadas
    // ate o Canal 3 retornar a LOW.
    if (ModoProg == 4) {
        LuzProg = 0;
        return;
    }
    // Comando somente quando o kit esta aguardando programacao.
    // Exige Canal 2 baixo antes de uma nova transicao para alto.
    if (novo2) {
        // Limiar exclusivo da programacao: 10% acima da media.
        // O freio normal continua utilizando Max2 (5%).
        if (c2 <= Max2) {
            Ch2Armado = 1;
            Ch2Altos = 0;
        }
        else if (c2 > Limite2Prog) {
            if (Ch2Armado) {
                Ch2Altos++;
                if (Ch2Altos >= 3) {
                    Ch2Armado = 0;
                    Ch2Altos = 0;
                    FreioInvertido = !FreioInvertido;
                    SalvarFreioEEPROM(FreioInvertido);
                    ModoProg = 4;
                    LuzProg = 0;
                    return;
                }
            }
        }
        else {
            // Zona intermediaria: nao confirma HIGH.
            Ch2Altos = 0;
        }
    }
    intervalo = 100;
    if ((unsigned int)(agora - TempoProg) >= intervalo) {
        TempoProg = agora;
        LuzProg = LuzProg ? 0 : 3;
    }
}
#endif
