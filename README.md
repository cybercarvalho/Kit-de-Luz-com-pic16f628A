# Kit de Luz RC PIC16F628A v0.4.2 (experimental)

Mudancas em relacao a v0.4.1:
- Nova biblioteca FailSafe.h: timeout de 100 ms dos tres canais,
  sinalizacao por setas e freio a cada 100 ms e inicio de recalibracao
  quando os tres canais voltarem a entregar pulsos validos.
- main.c: rotina FailSafe removida; apenas chamadas a biblioteca.
- TiraMedia.h: confirmacao de calibracao com 10 piscadas de 50 ms
  acesas e 50 ms apagadas, usando for e Delay_ms(50).
- Mantidos os filtros de 3 pulsos e histerese de TrataCanal1/2.

AVISOS:
- Durante ~1 segundo da confirmacao, o loop principal fica bloqueado.
  Interrupcoes continuam, mas uma perda de sinal so e tratada depois.
- O receptor deve interromper os pulsos ao perder radio para que o
  timeout detecte a falha.
- Ainda nao compilado nem validado em hardware; verificar RAM/ROM no mikroC.
- GitHub nao alterado.
