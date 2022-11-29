#include <MGE.h>

SoftwareSerial esp8266(2, 3);

void MGE::escreveEEPROM(int enderecoInicio, int enderecoFim, String mensagem){
    if (mensagem.length() > EEPROM.length() || (enderecoFim + mensagem.length()) > EEPROM.length()) {  // verificamos se a string cabe na memória a partir do endereço desejado
    Serial.println("A sua String não cabe na EEPROM");                                               // Caso não caiba mensagem de erro é mostrada
  } else {
    int i = 0;  // Caso seja possível armazenar
    for (enderecoInicio; enderecoInicio < enderecoFim; enderecoInicio++) {
      if (enderecoInicio < enderecoFim) {
        EEPROM.write(enderecoInicio, mensagem[i]);  // Escrevemos cada byte da string de forma sequencial na memória
      }
      i++;
    }
    // Serial.println("Salvando mensagem: " + mensagem);
    EEPROM.write(enderecoFim, '\0');
  }
};

String MGE::lerEEPROM(int enderecoInicio, int enderecoFim){
  String mensagem = "";
  if (enderecoFim > EEPROM.length()) {  // Se o endereço base for maior que o espaço de endereçamento da EEPROM retornamos uma string vazia
    return mensagem;
  } else {  // Caso contrário, lemos byte a byte de cada endereço e montamos uma nova String
    char pos;
    do {
      pos = EEPROM.read(enderecoInicio);  // Leitura do byte com base na posição atual
      enderecoInicio++;                   // A cada leitura incrementamos a posição a ser lida
      mensagem = mensagem + pos;
    } while (pos != '\0');  // Fazemos isso até encontrar o marcador de fim de string
  }
  return mensagem;  // Retorno da mensagem
};

void MGE::limpaEEPROM(){
    for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  escreveEEPROM(5, 6, "0");
};

String MGE::removeZero(String value) {
  String retorno = value;
  for (int i = 0; i < value.length(); i++) {
    if (value[i] != '0') {
      retorno = retorno.substring(i);
      break;
    }
  }
  return retorno;
};

void MGE::salvaConfig(String mensagem){

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

    int index = mensagem.indexOf(";");
    equipmentId = mensagem.substring(0, index);
    escreveEEPROM(1, 4, equipmentId);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    ipJMS = mensagem.substring(0, index);
    escreveEEPROM(7, 23, ipJMS);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    SSID = mensagem.substring(0, index);
    escreveEEPROM(24, 44, SSID);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    senhaRede = mensagem.substring(0, index);
    escreveEEPROM(45, 65, senhaRede);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    isEnabledSystem = mensagem.substring(0, index);
    escreveEEPROM(5, 6, isEnabledSystem);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    tipoRede = mensagem.substring(0, index);
    escreveEEPROM(66, 67, tipoRede);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    neutroAtivo = mensagem.substring(0, index);
    escreveEEPROM(68, 69, neutroAtivo);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    terraAtivo = mensagem.substring(0, index);
    escreveEEPROM(70, 71, terraAtivo);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    gateway = mensagem.substring(0, index);
    escreveEEPROM(72, 88, gateway);
    index = index + 1;
    mensagem = mensagem.substring(index);
    index = mensagem.indexOf(";");
    subrede = mensagem.substring(0, index);
    escreveEEPROM(89, 105, subrede);
};

String MGE::loadConfig(int idconfig){
  switch (idconfig)
  {
  case 1:
    return removeZero(lerEEPROM(1, 4));
    break;
  case 2:
    return removeZero(lerEEPROM(7, 23));
  case 3:
    return removeZero(lerEEPROM(24, 44));
  case 4:
    return removeZero(lerEEPROM(45, 65));
  case 5:
    return lerEEPROM(5, 6);
  case 6:
    return lerEEPROM(66, 67);
  case 7:
    return lerEEPROM(68, 69);
  case 8:
    return lerEEPROM(70, 71);
  case 9:
    return removeZero(lerEEPROM(72, 88));
  case 10:
    return removeZero(lerEEPROM(89, 105));
  default:
    break;
  }
    
};

void MGE::wifiInit(String SSID, String password, String ip, String gateway, String subrede){
  esp8266.begin(19200);
 
  sendData("AT+RST\r\n", 2000, false); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=\""+SSID+"\",\""+password+"\"\r\n", 2000, true);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, false);
  // seta ip Fixo
  sendData("AT+CIPSTA_DEF=\""+ip+"\",\""+subrede+"\",\""+gateway+"\"\r\n", 1000, false);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, false);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 2000, true);
};

String MGE::sendData(String command, const int timeout, boolean debug){
   // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
};

void MGE::sendToSync(String dados){
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;
      String webpage = "HTTP/1.1 200 OK \n";
      webpage += "Content-Type: application/json \n";
      webpage += "Connection: keep-alive \n\n"; 
      webpage += dados;
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, false);
      sendData(webpage, 1000, false);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, false);
    }
  }
};