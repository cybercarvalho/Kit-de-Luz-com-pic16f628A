# Kit de Luz RC com PIC16F628A — v0.3.1 (teste)
mikroC PRO for PIC, clock interno 4 MHz. Não compilado nem validado em hardware.

- RB7: canal 1; RB6: canal 2; RB5: canal 3.
- Timer1 a 1 us/tick, captura por interrupção de mudança de PORTB.
- Timer0: base de tempo aproximada de 1 ms (verificar precisão em bancada).
- Timer2/CCP1/RB3: PWM de freio a 1 kHz, duty 20 (solto) ou 255 (acionado).
- Seta esquerda RB0, direita RA3: alternância a cada 500 ms.
- Falta de pulsos válidos por 100 ms em qualquer canal: fail-safe,
  ambas as setas e freio alternam a cada 100 ms.
- Ao recuperar sinal, 100 amostras por canal e média inteira.
- Canal 1 abaixo de 95% da média: esquerda; acima de 105%: direita.
- Canal 2 acima de 105% da média: freio acionado.
- Canal 3 participa da validação de sinal e calibração.
- Ao concluir as 100 medições de cada canal, todas as saídas de luz
  (faróis, setas, neon, ré/lanterna e freio) acendem juntas por 150 ms,
  uma única vez, sem delay. Depois o modo normal é liberado.
- O flash também acontece após recuperação de fail-safe e recalibração.

LIMITAÇÕES IMPORTANTES:
1. Receptor que continua gerando pulsos durante perda do rádio pode não ser
   detectado como falha. Configurar o failsafe do receptor adequadamente.
2. RBIF não captura múltiplas bordas simultâneas com precisão perfeita.
3. A temporização Timer0 é aproximada, pois a latência da ISR afeta o período.
4. Se o loop principal não processar uma amostra antes da próxima, ela é
   substituída; em operação normal, o loop deve ser suficientemente rápido.
5. A calibração ocorre também ao ligar. Manter os controles em neutro
   durante as 100 medições.
6. Configurar PIC16F628A, oscilador interno 4 MHz, WDT OFF, LVP OFF,
   MCLR OFF. Conferir os nomes de registradores conforme versão do mikroC.
7. A RAM e o tamanho do firmware só podem ser confirmados após compilar.

Versão experimental publicada no GitHub a pedido do usuário.
