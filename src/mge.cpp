#include <MGE.h>
#include "EmonLib.h"

#define VOLT_CAL 606.00

EnergyMonitor emon1;
EnergyMonitor emon2;

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

void setup()
{
  Serial.begin(9600);
  loadConfig();
  MGE().wifiInit(SSID, senhaRede, ipJMS, gateway, subrede);
  emon1.voltage(0, VOLT_CAL, 1.7);
  emon2.current(1, 15);
}

void loop()
{

  // 001;00192.168.100.26;0000000000000000Home;0000MMTTUSXAVIER@123;1;M;1;1;000192.168.100.1;000255.255.255.0;

  if (Serial.available() > 0)
  {
    String comand = Serial.readString();
    Serial.println(comand);
    if (comand == "N")
    {
      while (true)
      {
        if (Serial.available() > 0)
        {
          mensagem = Serial.readString();
          MGE().salvaConfig(mensagem);
          loadConfig();
          break;
        }
      };
    }
    else if (comand == "R")
    {
      MGE().limpaEEPROM();
    }
  }

  if (isEnabledSystem == "0")
  {
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
    String toString()
    {
      return String("{\"equipment\":{\"id\":") + equipment.id + "},\"current\":{\"a\":" + current.current1 + ",\"b\":" + current.current2 + ",\"c\":" + current.current3 + "},\"voltage\":{\"ab\":" + voltage.fase1fase2 + ",\"bc\":" + voltage.fase2fase3 + ",\"ca\":" + voltage.fase3fase1 + "},\"type\":\"" + tipoRede + "\"}";
    }
  } keepAlive;

  keepAlive.equipment.id = equipmentId.toInt();

  if (tipoRede == "M")
  {
    tipoRede = "M";
    double current = emon2.calcIrms(1480);
    keepAlive.current.current1 = current;
    keepAlive.current.current2 = current;
    keepAlive.current.current3 = current;
    emon1.calcVI(17, 2000);
    double voltage = emon1.Vrms;
    keepAlive.voltage.fase1fase2 = voltage;
    keepAlive.voltage.fase2fase3 = voltage;
    keepAlive.voltage.fase3fase1 = voltage;
  }
  else if (tipoRede == "B")
  {
    tipoRede = "B";
    double current = emon2.calcIrms(1480);
    keepAlive.current.current1 = current;
    keepAlive.current.current2 = current;
    keepAlive.current.current3 = current;
    emon1.calcVI(17, 2000);
    double voltage = emon1.Vrms;
    keepAlive.voltage.fase1fase2 = voltage;
    keepAlive.voltage.fase2fase3 = voltage;
    keepAlive.voltage.fase3fase1 = voltage;
  }
  else
  {
    tipoRede = "T";
  }
  keepAlive.type = tipoRede;
  // Serial.println(keepAlive.toString());
  MGE().sendToSync(keepAlive.toString());
  delay(1000);
}

int numeroAleatorio(int menor, int maior)
{
  return rand() % (maior - menor + 1) + menor;
}

void loadConfig()
{
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