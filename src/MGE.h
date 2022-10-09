#ifndef MGE_H
#define MGE_H
 
#include <Arduino.h>
#include <EEPROM.h>
 
 class MGE {
    public:
        String lerEEPROM(int enderecoInicio, int enderecoFim);
        void limpaEEPROM();
        String removeZero(String value);
        void salvaConfig(String mensagem);
    private:
        void escreveEEPROM(int enderecoInicio, int enderecoFim, String mensagem);
 };
 
#endif
