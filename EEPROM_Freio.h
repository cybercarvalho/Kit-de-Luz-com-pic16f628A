#ifndef EEPROM_FREIO_H
#define EEPROM_FREIO_H
// Acesso direto a EEPROM do PIC16F628A, sem biblioteca EEPROM do mikroC.
// Endereco 0: 0xA5 = normal; 0x5A = invertido.
unsigned char LerFreioEEPROM() {
    EEADR = 0;
    EECON1.RD = 1;
    return EEDATA;
}
void SalvarFreioEEPROM(unsigned char invertido) {
    unsigned char gieAnterior;
    EEADR = 0;
    EEDATA = invertido ? 0x5A : 0xA5;
    EECON1.WREN = 1;
    gieAnterior = INTCON.GIE;
    INTCON.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1.WR = 1;
    INTCON.GIE = gieAnterior;
    while (EECON1.WR) { }
    EECON1.WREN = 0;
}
#endif
