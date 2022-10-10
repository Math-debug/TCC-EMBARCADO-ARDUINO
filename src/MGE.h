#ifndef MGE_H
#define MGE_H
 
#include <Arduino.h>
#include <EEPROM.h>
 
 class MGE {
    public:
        void limpaEEPROM();
        void salvaConfig(String mensagem);
        String loadConfig(int idconfig);
    private:
        void escreveEEPROM(int enderecoInicio, int enderecoFim, String mensagem);
        String lerEEPROM(int enderecoInicio, int enderecoFim);
        String removeZero(String value);
 };
 
#endif
