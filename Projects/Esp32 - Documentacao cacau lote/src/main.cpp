#include <Arduino.h>
#include "SPIFFS.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String fileContent; 
bool disconnected = false; 

void setup() {
  Serial.begin(9600);
  
  if (!SPIFFS.begin(true)) {
    Serial.println("Ocorreu um erro ao montar o SPIFFS");
    return;
  }
  
  if (!SerialBT.begin("Totem Gênesis - CacauLote")) {  
    Serial.println("Ocorreu um erro ao inicializar o Bluetooth");
    return;
  }
  Serial.println("Bluetooth iniciado");

  File file = SPIFFS.open("/cacaulotedoc.txt");
  if (!file) {
    Serial.println("Falha ao abrir o arquivo para leitura");
    return;
  }
  
  Serial.println("Enviando conteúdo do arquivo para o terminal Bluetooth e Serial...");

  while (file.available()) {
    char c = file.read();
    Serial.write(c); 
    SerialBT.write(c); 
    fileContent += c;
  }
  
  file.close();
}

void loop() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c); 
  }

  if (!SerialBT.connected()) {
    if (!disconnected) {
      Serial.println("Celular desconectado. Tentando reconectar...");
      disconnected = true; 

      if (!SerialBT.begin("Totem Gênesis - CacauLote")) {
        Serial.println("Erro ao reconectar.");
      } else {
        Serial.println("Reconexão bem-sucedida!");
        SerialBT.print(fileContent);
        disconnected = false; 
      }
    }
  } else {
    disconnected = false;
  }
}
