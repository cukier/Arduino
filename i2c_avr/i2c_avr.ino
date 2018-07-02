#undef F_CPU
#define F_CPU 8000000ULL

#include  <Wire.h>

void setup() {
  Serial.begin(19200);
  Wire.begin();
  Wire.setClock(400000ULL);
}

void loop() {

}
