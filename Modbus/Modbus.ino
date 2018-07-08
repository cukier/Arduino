/*
   Released under the GPL
   (C) 2018 by Cukier (mauriciocukier@gmail.com)
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include "modbus.h"

#include <vector>

const char* ssid = "ConectionPoint";
const char* password = "";
const char* mqtt_server = "iot.eclipse.org";
const char* user = "";
const char* pass = "";

WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;
Modbus mstr;

void setup() {
  mstr.init(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
#ifdef DEBUG
  Serial.print("Connecting to ");
#endif

  if (!wifiManager.autoConnect("Teste")) {
#ifdef DEBUG
    Serial.println("failed to connect and hit timeout");
#endif
    ESP.restart();
  }

#ifdef DEBUG
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

void callback(char* topic, byte* payload, unsigned int length) {
#ifdef DEBUG
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
#endif
}

void verificaconexoes() {
  if (!WiFi.isConnected()) {
#ifdef DEBUG
    Serial.println("Reconectando WIFI");
#endif
    WiFi.reconnect();
  }

  if (!client.connected()) {
    reconnect();
#ifdef DEBUG
    Serial.println("Reconectando MQTT");
#endif
  } else {
    client.loop();
  }
}

boolean reconnect() {
  //if (client.connect("IoT Teste", user, pass)) {
  if (client.connect("IoT Teste")) {
#ifdef DEBUG
    Serial.print("Conectado ");
#endif
    client.subscribe("/IoT/Topico1");
  }

  return client.connected();
}

void loop() {
  verificaconexoes();

  if (!(millis() % 1000)) {
    std::vector<uint16_t> resp = mstr.readHoldingRegisters(0x01, 0x00, 50);
    Publicar("/IoT/modbus/size", String(resp.size()), false);
    //    Publicar("/IoT/modbus/valor 0", String(resp.at(0)), false);

    for (const auto i : resp) {
      String str(i);

      Publicar("/IoT/modbus/valor", str, false);
    }
  }
}

void Publicar(String Topic, String Conteudo, bool Retain) {
  char fila[50];
  char xvalor[50];

  Topic.toCharArray(fila, 50);
  Conteudo.toCharArray(xvalor, 50);
  client.publish(fila, xvalor, Retain);
}
