#include <MGE.h>

String equipmentId;
String ipJMS;
String SSID;
String senhaRede;
String isEnabledSystem;
String tipoRede;
String neutroAtivo;
char sep = ';';

String mensagem;

int numeroAleatorio(int menor, int maior);

void setup() {
  Serial.begin(9600);
}

void loop() {

  //001;000192.168.18.26;0000000000000000HOME;000000000000senha123;1;M;1;
  equipmentId = MGE().loadConfig(1).toInt();
  ipJMS = MGE().loadConfig(2);
  SSID = MGE().loadConfig(3);
  senhaRede = MGE().loadConfig(4);
  isEnabledSystem = MGE().loadConfig(5);
  tipoRede = MGE().loadConfig(6);
  neutroAtivo = MGE().loadConfig(7);

  if (Serial.available() > 0) {
    String comand = Serial.readString();
    Serial.println(comand);
    if(comand == "N"){
      while (true) {
        if(Serial.available() > 0){
        mensagem = Serial.readString();
        MGE().salvaConfig(mensagem);
        break;
        }
      };
    }
    else if (comand == "R"){
      MGE().limpaEEPROM();
    }
    else if (comand == "C"){
      Serial.println(String("if_") + sep + equipmentId + sep + ipJMS + sep + SSID + sep + senhaRede + sep + tipoRede + sep + neutroAtivo + sep);
    }
  }
  if(isEnabledSystem == "0"){
    return;
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
      return String("{\"equipment\":{ \"id\":") + equipment.id + " }, \"current\": " + current + ",\"voltage\":" + voltage + "}";
    }
  } keepAlive;

  keepAlive.equipment.id = equipmentId.toInt();
  int current = numeroAleatorio(1, 3);
  keepAlive.current = current;
  int voltage = numeroAleatorio(200, 220);
  keepAlive.voltage = voltage;
  Serial.println("============= Keep Alive ==============");
  Serial.println(keepAlive.toString());
  delay(1000);
}

int numeroAleatorio(int menor, int maior) {
  return rand() % (maior - menor + 1) + menor;
}