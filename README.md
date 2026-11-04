# Kit de Luz com PIC16F628A

Firmware modular em **mikroC PRO for PIC** para PIC16F628A com oscilador interno de **4 MHz**.

## Versao 0.1.0 (experimental)
- `Configuracao.h`: mapeamento das entradas e saidas.
- `RC_Canais.h`: amostragem por Timer0 dos tres canais RC (RB7, RB6, RB5).
- `main.c`: inicializacao, interrupcao e teste de pisca em RB2.

## Teste
1. Criar projeto no mikroC PRO, dispositivo PIC16F628A, clock 4 MHz.
2. Selecionar oscilador interno com RA6/RA7 como I/O; WDT OFF; LVP OFF; MCLR OFF.
3. Adicionar `main.c` e colocar os arquivos `.h` na mesma pasta.
4. Compilar e gravar. RB2 deve alternar aproximadamente a cada 500 ms.
5. `RC_LerCanal(1..3)` retorna largura nominal em microssegundos, ou zero se nao houver sinal valido.

## Limitacoes
- Versao experimental, ainda nao testada no hardware.
- Resolucao nominal 100 us; a latencia de interrupcao pode afetar o periodo.
- Sem UART de diagnostico nesta versao; a captura dos canais ainda precisa ser validada.
- RB6/RB7 tambem sao usados por programadores ICSP: observar o circuito durante a gravacao.
