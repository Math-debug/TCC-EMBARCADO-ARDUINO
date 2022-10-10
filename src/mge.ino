#include <MGE.h>

// Endereco 1 a 4
String equipmentId;
// Endereco 5 a 6
String isEnabledSystem;
// Endereco 7 a 23
String ipJMS;
// Endereco 24 a 44
String SSID;
// Endereco 45 a 65
String senhaRede;

String mensagem;

void setup() {
  Serial.begin(9600);
}

void loop() {

  //001;000192.168.18.26;0000000000000000HOME;000000000000senha123;
  equipmentId = MGE().lerEEPROM(1, 4).toInt();
  ipJMS = MGE().removeZero(MGE().lerEEPROM(7, 23));
  SSID = MGE().removeZero(MGE().lerEEPROM(24, 44));
  senhaRede = MGE().removeZero(MGE().lerEEPROM(45, 65));

  //limpaEEPROM();

  if (Serial.available() > 0) {
    String comand = Serial.readString();
    Serial.println(comand);
    if(comand == "N"){
      Serial.println("Aguardando configuracoes");
      while (true) {
        if(Serial.available() > 0){
        mensagem = Serial.readString();
        MGE().salvaConfig(mensagem);
        break;
        }
      };
    }
  }

  typedef struct
  {
    int id;
  } Equipment;

  struct
  {
    Equipment equipment;
    float current;
    float voltage;
    String toString() {
      return String("{\"equipment\":{ \"id\":") + equipment.id + " }, \"current\": " + current + ",\"voltage\":" + voltage + "}_";
    }
  } keepAlive;

  keepAlive.equipment.id = equipmentId.toInt();
  int current = numeroAleatorio(1, 3);
  keepAlive.current = current;
  int voltage = numeroAleatorio(200, 220);
  keepAlive.voltage = voltage;

  Serial.println(keepAlive.toString());

  delay(1000);
}

int numeroAleatorio(int menor, int maior) {
  return rand() % (maior - menor + 1) + menor;
}