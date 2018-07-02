void setup() {
  Serial1.begin(115200);
}

void loop() {  
  Serial1.println("Hello");

  if(Serial1.available()) {
    Serial1.println(Serial1.read());
  }
  
  delay(1000);
}
