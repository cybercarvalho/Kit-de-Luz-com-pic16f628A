# Kit de Luz RC — PIC16F628A v0.4.1 (experimental)
Base v0.4.0 com melhorias contra piscadas espontaneas:
- RC_Canais.h: leitura consistente do Timer1 com verificacao de rollover.
- TrataCanal1.h: 3 pulsos consecutivos para confirmar troca de direcao;
  histerese (aciona fora de 5%, libera dentro de ~3%);
  pisca apenas quando existe comando confirmado.
- TrataCanal2.h: 3 pulsos consecutivos para acionar/desacionar freio;
  histerese (aciona acima de 5%, libera abaixo de ~3%).
- Filtros atualizados apenas com pulso novo (mascara NovosRC),
  nao a cada iteracao do while.
- Fail-safe: 100 ms sem pulso valido em qualquer canal; setas e freio
  alternam a cada 100 ms.
- Recalibracao: 100 leituras por canal; flash de confirmacao 150 ms.
- Timer0 e Timer1, pinagem e PWM mantidos da v0.4.0.

ATENCAO: versao nao compilada nem testada no PIC fisico.
Os filtros nao eliminam erros repetidos, jitter grande, problemas
eletricos, mau aterramento ou falhas na captura de bordas RBIF.
Com Timer1 a 4 MHz e interrupcao compartilhada, verificar os pulsos
em osciloscopio/analizador logico caso persistam acionamentos.
Conferir uso de RAM/ROM e compatibilidade com mikroC PRO.
