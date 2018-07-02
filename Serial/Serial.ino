int led_azul = 13;
int led_vermelho = 12;
int led_verde = 14;

void setup() {
  Serial.begin(115200);
  pinMode(led_azul, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
}

void loop() {
  Serial.println("Hello");
  digitalWrite(pin, HIGH);
  delay(300);
  digitalWrite(pin, LOW);
  delay(300);
}
