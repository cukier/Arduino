#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

const char* ssid = "Moto";
const char* password = "mauricio1";
const char* mqtt_server = "iot.eclipse.org";
const char* user = "";
const char* pass = "";
const int pin = 14;
bool ctrl = true;

WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  Serial.print("Connecting to ");

  if (!wifiManager.autoConnect("Teste")) {
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void verificaconexoes() {
  if (!WiFi.isConnected()) {
    Serial.println("Reconectando WIFI");
    WiFi.reconnect();
  }

  if (!client.connected()) {
    reconnect();
    Serial.println("Reconectando MQTT");
  } else {
    client.loop();
  }
}

boolean reconnect() {
  //if (client.connect("IoT Teste", user, pass)) {
  if (client.connect("IoT Teste")) {
    Serial.print("Conectado ");
    client.subscribe("/IoT/Topico1");
  }

  return client.connected();
}

void loop() {
  verificaconexoes();

  if (digitalRead(pin) == LOW) {
    if (ctrl) {
      ctrl = false;
      Publicar("/IoT/TestePin", "Botao acionado", false);
    }
  } else if (!ctrl) {
    ctrl = true;
    Publicar("/IoT/TestePin", "Botao solto", true);
  }
}

void Publicar(String Topic, String Conteudo, bool Retain) {
  char fila[50];
  char xvalor[30];

  Topic.toCharArray(fila, 50);
  Conteudo.toCharArray(xvalor, 30);
  client.publish(fila, xvalor, Retain);
}

