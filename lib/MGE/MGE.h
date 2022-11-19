#ifndef MGE_H
#define MGE_H
 
#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
 
 class MGE {
    public:
        void limpaEEPROM();
        void salvaConfig(String mensagem);
        String loadConfig(int idconfig);
        void wifiInit(String SSID, String password);
        void sendToSync(String dados);
    private:
        void escreveEEPROM(int enderecoInicio, int enderecoFim, String mensagem);
        String lerEEPROM(int enderecoInicio, int enderecoFim);
        String removeZero(String value);
        String sendData(String command, const int timeout, boolean debug);
 };
 
#endif
