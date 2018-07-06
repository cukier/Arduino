#include <ModbusRTUSlave.h>

ModbusRTUSlave slv(1, &Serial);

u16 _D[100];
u8 _M[50];

void setup() {
  slv.addWordArea(0x7000, _D, 100);
  slv.addBitArea(0x1000, _M, 50);
  slv.begin(9600);  
}

void loop() {
  slv.process();
}

//void setup() {
//  Serial.begin(9600);
//}
//
//void loop() {
//  Serial.println("Hello");
//  delay(1000);
//}

