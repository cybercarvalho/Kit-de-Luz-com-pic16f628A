#ifndef FREIO_SETAS_H
#define FREIO_SETAS_H
#define PWM_SOLTO 0
#define PWM_ACIONADO 255
// CCP1/RB3: 4 kHz, PR2=249, prescaler 1:1.
// Duty de 8 bits, mapeado para 10 bits: CCPR1L=duty; DC1B=0.
void FreioPWM(unsigned char duty) {
    CCPR1L = duty;
}
void FreioInicializar() {
    TRISB.F3 = 1;
    PR2 = 249;
    CCPR1L = PWM_SOLTO;
    CCP1CON = 0x0C; // CCP1 PWM, DC1B=0
    T2CON = 0x04;   // Timer2 ligado, prescaler 1:1 (4 kHz)
    PIR1.TMR2IF = 0;
    PIE1.TMR2IE = 1;
    TRISB.F3 = 0;
}
#endif
