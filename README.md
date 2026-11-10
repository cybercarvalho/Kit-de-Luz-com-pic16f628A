# Kit de Luz PIC16F628A — v0.5.6

Mudancas sobre v0.5.5:
- Programa 0: farois 0; freio 0 em repouso e 255 quando acionado.
- Programa 1: farois 10; freio 10 em repouso e 255 quando acionado.
- Programa 2: farois 255; freio 10 em repouso e 255 quando acionado.
- Mantida a confirmacao de 3 amostras e histerese do canal 2.
- Corrigida ordem dos includes: TrataCanal3.h antes de TrataCanal1.h e TrataCanal2.h.

PWM dos farois permanece em 16 passos (duty 10 aproximado para 1/16).
Arquivo principal: kit_de_Luz_16f628A_2026.c.

Fonte nao compilada neste ambiente: requer mikroC PRO for PIC.
Verificar consumo de ROM e funcionamento na placa.
Versao publicada no GitHub.
