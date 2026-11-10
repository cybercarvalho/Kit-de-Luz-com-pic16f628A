#ifndef TRATACANAL3_H
#define TRATACANAL3_H

// Versao reduzida: somente troca de programa pelo Canal 3.
// Canal 3 NAO participa da calibracao.
#define QTD_PROGRAMAS 4
#define CH3_LIMIAR_ALTO_US 1500

unsigned char Programa;
unsigned char Canal3Armado;
unsigned char Canal3Pressionado;

void ZeraEstados(); // Implementada em FuncoesGerais.h

void TrataCanal3_Iniciar(unsigned int agora) {
    Programa = 0;
    ZeraEstados();
}

void TrataCanal3_Cancelar(unsigned int agora) {
    // Nao altera Programa; exige voltar a chave para baixo.
    ZeraEstados();
}

void TrataCanal3(unsigned int pulso, unsigned int agora) {
    // Um ciclo baixo -> alto -> baixo incrementa uma vez.
    // O valor e validado pelo receptor antes de chegar aqui.
    if (pulso <= CH3_LIMIAR_ALTO_US) {
        if (!Canal3Armado) {
            Canal3Armado = 1;
        } else if (Canal3Pressionado) {
            Canal3Pressionado = 0;
            Programa++;
            if (Programa >= QTD_PROGRAMAS) Programa = 0;
        }
    } else if (Canal3Armado) {
        Canal3Pressionado = 1;
    }
}

// Funcionalidades desativadas para economizar ROM:
// - espera de 10s e modo de configuracao de 12s
// - pisca de confirmacao do Canal 3
// - temporizadores e filtros de estado do Canal 3
// Codigo anterior preservado abaixo, excluido da compilacao.
#if 0
#ifndef TRATACANAL3_H
#define TRATACANAL3_H

// Canal 3: toque curto troca programa; manter alto por 10s entra
// em espera e por mais 2s ativa modo de configuracao.
// Valores ajustaveis. Programa inicial = 0; QtdProgramas = 3.
// O modo de configuracao fica sinalizado, mas comandos de ajuste
// ainda dependem das rotinas de cada programa (nao portadas).
#define QTD_PROGRAMAS 3
#define CH3_CONF_ESPERA_MS 10000
#define CH3_CONF_ATIVA_MS 12000
#define CH3_DEBOUNCE_MS 80
#define CH3_ARMAR_MS 300
#define CH3_LIMIAR_ALTO_US 1500
#define CH3_PISCA_MS 500
#define CH3_PISCA_CONFIG_MS 50

#define CH3_INATIVA 0
#define CH3_ESPERA 1
#define CH3_ATIVA 2

unsigned char Programa;
unsigned char ModoConfiguracao;
unsigned char Canal3Armado;
unsigned char Canal3AltoEstavel;
unsigned char Canal3Candidato;
unsigned char Canal3Pressionado;
unsigned char Canal3Luzes;
unsigned int Canal3Mudanca;
unsigned int Canal3InicioPressao;
unsigned int Canal3UltimaPiscada;

void TrataCanal3_Luzes(unsigned char acesas) {
    // Sinaliza com os dois farois, sem interferir no PWM do freio.
    FarolDireito = acesas;
    FarolEsquerdo = acesas;
}

void TrataCanal3_Iniciar(unsigned int agora) {
    Programa = 0;
    ModoConfiguracao = CH3_INATIVA;
    Canal3Armado = 0;
    Canal3AltoEstavel = 0;
    Canal3Candidato = 0;
    Canal3Pressionado = 0;
    Canal3Luzes = 0;
    Canal3Mudanca = agora;
    Canal3InicioPressao = agora;
    Canal3UltimaPiscada = agora;
    TrataCanal3_Luzes(0);
}

void TrataCanal3_Cancelar(unsigned int agora) {
    ModoConfiguracao = CH3_INATIVA;
    Canal3Armado = 0;
    Canal3Pressionado = 0;
    Canal3AltoEstavel = 0;
    Canal3Candidato = 0;
    Canal3Mudanca = agora;
    Canal3Luzes = 0;
    TrataCanal3_Luzes(0);
}

void TrataCanal3(unsigned int pulso, unsigned int agora) {
    unsigned char alto;
    unsigned int decorrido;

    // Chave do canal 3: limiar fixo, sem calibracao.
    alto = (pulso > CH3_LIMIAR_ALTO_US);

    if (alto != Canal3Candidato) {
        Canal3Candidato = alto;
        Canal3Mudanca = agora;
    }
    decorrido = (unsigned int)(agora - Canal3Mudanca);

    if (decorrido >= CH3_DEBOUNCE_MS)
        Canal3AltoEstavel = Canal3Candidato;

    if (!Canal3Armado) {
        if (!Canal3AltoEstavel && !Canal3Candidato &&
            decorrido >= CH3_ARMAR_MS)
            Canal3Armado = 1;
        return;
    }

    if (!Canal3Pressionado) {
        if (Canal3AltoEstavel) {
            Canal3Pressionado = 1;
            Canal3InicioPressao = agora;
            Canal3UltimaPiscada = agora;
            Canal3Luzes = 1;
            TrataCanal3_Luzes(1);
        }
        return;
    }

    // Soltou a chave: troca apenas se ainda nao entrou no menu.
    if (!Canal3AltoEstavel) {
        if (ModoConfiguracao == CH3_INATIVA)
            if (++Programa >= QTD_PROGRAMAS) Programa = 0;
        ModoConfiguracao = CH3_INATIVA;
        Canal3Pressionado = 0;
        Canal3Luzes = 0;
        TrataCanal3_Luzes(0);
        return;
    }

    decorrido = (unsigned int)(agora - Canal3InicioPressao);
    if ((ModoConfiguracao == CH3_INATIVA) &&
        (decorrido >= CH3_CONF_ESPERA_MS)) {
        ModoConfiguracao = CH3_ESPERA;
        Canal3Luzes = 0;
        TrataCanal3_Luzes(0);
    }
    if ((ModoConfiguracao == CH3_ESPERA) &&
        (decorrido >= CH3_CONF_ATIVA_MS)) {
        ModoConfiguracao = CH3_ATIVA;
        Canal3UltimaPiscada = agora;
        Canal3Luzes = 1;
        TrataCanal3_Luzes(1);
    }
    if (ModoConfiguracao != CH3_ESPERA) {
        if ((unsigned int)(agora - Canal3UltimaPiscada) >=
            ((ModoConfiguracao == CH3_ATIVA) ?
             CH3_PISCA_CONFIG_MS : CH3_PISCA_MS)) {
            Canal3UltimaPiscada = agora;
            Canal3Luzes = !Canal3Luzes;
            TrataCanal3_Luzes(Canal3Luzes);
        }
    }
}
#endif

#endif /* codigo anterior desativado */
#endif /* TRATACANAL3_H */