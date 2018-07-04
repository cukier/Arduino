#include "modbus.h"

Modbus mstr;

void setup() {
  mstr.init(9600);
}

void loop() {
  mstr.readHoldingRegisters(0x01, 0x00, 0x0A);
  delay(1000);
}

