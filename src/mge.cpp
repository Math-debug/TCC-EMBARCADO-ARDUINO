#include <MGE.h>

String equipmentId;
String ipJMS;
String SSID;
String senhaRede;
String isEnabledSystem;
String tipoRede;
String neutroAtivo;
String terraAtivo;
String gateway;
String subrede;
char sep = ';';

String mensagem;

int numeroAleatorio(int menor, int maior);
void loadConfig();

void setup() {
  Serial.begin(9600);
  loadConfig();
  MGE().wifiInit(SSID,senhaRede,ipJMS,gateway,subrede);
}

void loop() {

  //001;00192.168.100.26;0000000000000000Home;0000MMTTUSXAVIER@123;1;M;1;1;000192.168.100.1;000255.255.255.0;

  if (Serial.available() > 0) {
    String comand = Serial.readString();
    Serial.println(comand);
    if(comand == "N"){
      while (true) {
        if(Serial.available() > 0){
        mensagem = Serial.readString();
        MGE().salvaConfig(mensagem);
        loadConfig();
        break;
        }
      };
    }
    else if (comand == "R"){
      MGE().limpaEEPROM();
    }
  }

  if(isEnabledSystem == "0"){
    return;
  }
  typedef struct
  {
    int id;
  } Equipment;

  typedef struct
  {
    float current1;
    float current2;
    float current3;
  } Current;

  typedef struct
  {
    float fase1fase2;
    float fase2fase3;
    float fase3fase1;
  } Voltage;

  struct
  {
    Equipment equipment;
    Current current;
    Voltage voltage;
    String type;
    String toString() {
      return String("{\"equipment\":{ \"id\":") + equipment.id + " }, \"current\": { \"current1\":" + current.current1 + ", \"current2\":"+ current.current2 +", \"current3\":"+ current.current3 +"  },\"voltage\":{ \"fase1fase2\":"+ voltage.fase1fase2 +", \"fase2fase3\":"+ voltage.fase2fase3 +", \"fase3fase1\":"+ voltage.fase3fase1 +"}, \"type\":\""+ "B" +"\"}";
    }
  } keepAlive;

  keepAlive.equipment.id = equipmentId.toInt();
  int current = numeroAleatorio(1, 3);
  keepAlive.current.current1 = current;
  keepAlive.current.current2 = current;
  keepAlive.current.current3 = current;
  int voltage = numeroAleatorio(200, 220);
  keepAlive.voltage.fase1fase2 = voltage;
  keepAlive.voltage.fase2fase3 = voltage;
  keepAlive.voltage.fase3fase1 = voltage;
  if(tipoRede == "M"){
    tipoRede = "M";
  } else if(tipoRede == "B"){
    tipoRede = "B";
  }else {
    tipoRede = "T";
  }
  keepAlive.type = tipoRede;
  Serial.println(keepAlive.toString());
  MGE().sendToSync(keepAlive.toString());
  delay(1000);
}

int numeroAleatorio(int menor, int maior) {
  return rand() % (maior - menor + 1) + menor;
}

void loadConfig(){
  equipmentId = MGE().loadConfig(1).toInt();
  ipJMS = MGE().loadConfig(2);
  SSID = MGE().loadConfig(3);
  senhaRede = MGE().loadConfig(4);
  isEnabledSystem = MGE().loadConfig(5);
  tipoRede = MGE().loadConfig(6);
  neutroAtivo = MGE().loadConfig(7).toInt();
  terraAtivo = MGE().loadConfig(8).toInt();
  gateway = MGE().loadConfig(9);
  subrede = MGE().loadConfig(10);
}