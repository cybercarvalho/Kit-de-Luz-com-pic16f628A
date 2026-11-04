# Kit de Luz com PIC16F628A

Firmware modular para **mikroC PRO for PIC**, PIC16F628A com oscilador interno de **4 MHz**.

## Versao 0.2.0 - leitura RC por interrupcao

Arquivos:
- `Configuracao.h`: mapeamento das portas.
- `RC_Canais.h`: captura dos tres canais por interrupcao RBIF e Timer1.
- `main.c`: inicializacao e tratamento centralizado de interrupcoes.

| Canal | Entrada | Variavel | Tipo |
| --- | --- | --- | --- |
| 1 | RB7 | ValorCanal1 | unsigned int (2 bytes) |
| 2 | RB6 | ValorCanal2 | unsigned int (2 bytes) |
| 3 | RB5 | ValorCanal3 | unsigned int (2 bytes) |

Timer1: clock interno Fosc/4, prescaler 1:1, **1 us por tick**.
Cada valor e atualizado na borda de descida, aceitando larguras de 900 a 2100 us.
Os tres resultados usam **6 bytes de RAM**; os instantes iniciais usam mais 6 bytes e o estado anterior 1 byte.

## Configuracao
- Device: PIC16F628A.
- Clock: 4 MHz.
- Oscilador interno com RA6/RA7 como I/O.
- WDT OFF, LVP OFF, MCLR OFF.
- Incluir os tres arquivos na mesma pasta do projeto mikroC.

## Limitacoes desta versao
- Ainda **nao testada em compilador ou hardware**.
- **Nao ha timeout de perda de sinal**: se o receptor parar de emitir pulsos, o ultimo valor valido permanece armazenado. Nao usar para controlar luzes ou seguranca ate implementar failsafe.
- A interrupcao compartilhada RBIF nao registra varias transicoes que ocorram antes do atendimento; a precisao depende da latencia.
- Leituras de variaveis de 16 bits no codigo principal devem ser atomicas (proteger temporariamente com GIE=0).
- RB6/RB7 compartilham funcoes de programacao ICSP; verificar compatibilidade eletrica da placa.
