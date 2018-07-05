#include "modbus.h"
#include <Arduino.h>

const uint16_t Modbus::wCRCTable[] = {
  0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301,
  0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1,
  0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81,
  0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
  0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00,
  0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1,
  0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641, 0XD201, 0X12C0, 0X1380,
  0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040, 0XF001, 0X30C0, 0X3180, 0XF141,
  0X3300, 0XF3C1, 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501,
  0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0,
  0X3E80, 0XFE41, 0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881,
  0X3840, 0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
  0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40, 0XE401,
  0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, 0X2200, 0XE2C1,
  0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, 0XA001, 0X60C0, 0X6180,
  0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740,
  0XA501, 0X65C0, 0X6480, 0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01,
  0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1,
  0XA881, 0X6840, 0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80,
  0XBA41, 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
  0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200,
  0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041, 0X5000, 0X90C1,
  0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241, 0X9601, 0X56C0, 0X5780,
  0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01, 0X5CC0, 0X5D80, 0X9D41,
  0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901,
  0X59C0, 0X5880, 0X9841, 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1,
  0X8A81, 0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80,
  0X8C41, 0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
  0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

Modbus::Modbus() {}
Modbus::~Modbus() {}

void Modbus::init(uint32_t baud) {
  Serial.begin(baud);
}

uint16_t Modbus::swap_w(const uint16_t i_word) const {
  uint16_t aux;

  aux = (i_word & 0xFF00) >> 8;
  aux |= (i_word & 0x00FF) << 8;

  return aux;
}

uint16_t Modbus::make16(const uint8_t h_b, const uint8_t l_b) const {
  uint16_t ret = 0;

  ret = (h_b << 8) & 0xFF00;
  ret |= l_b & 0xFF;

  return ret;
}

uint8_t Modbus::make8(const uint32_t i_word, const uint8_t index) const {
  uint8_t ret = 0;
  uint32_t mask = 0xFF << (index * 8);

  ret = ((i_word & mask) >> (index * 8) & 0xFF);

  return ret;
}

uint16_t Modbus::CRC16(const uint8_t *nData, const uint16_t wLength) const {
  uint16_t nTemp;
  uint16_t wCRCWord = 0xFFFF;
  uint16_t cont = wLength;

  while (cont--) {
    nTemp = *nData++ ^ wCRCWord;
    nTemp &= 0xFF;
    wCRCWord >>= 8;
    wCRCWord ^= wCRCTable[nTemp];
  }

  return wCRCWord;
}

bool Modbus::check_CRC(const std::vector<uint8_t> resp, uint8_t command) const {
  std::vector<uint8_t> arr;
  uint16_t ar_size, crc_check, crc_in, cont;

  switch (command) {
    case READ_HOLDING_REGISTERS_COMMAND:
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
      ar_size = resp.at(2) + 3;
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      ar_size = 6;
      break;
    default:
      break;
  }

  for (cont = 0; cont < ar_size; ++cont)
    arr.push_back(resp.at(cont));

  crc_in = make16(resp.at(ar_size + 1), resp.at(ar_size));
  crc_check = CRC16(&arr[0], ar_size);

  return crc_check == crc_in;;
}

std::vector<uint8_t> Modbus::make_request(const uint8_t& dev_addr, const uint16_t& from,
    const uint16_t& i_size,  const uint8_t& byte_count,
    std::vector<uint16_t>& data, const uint8_t& command) const {
  uint16_t crc, cont;
  std::vector<uint8_t> req;

  req.push_back(dev_addr);          //0
  req.push_back(command);           //1
  req.push_back(make8(from, 1));    //2
  req.push_back(make8(from, 0));    //3
  req.push_back(make8(i_size, 1));  //4
  req.push_back(make8(i_size, 0));  //5

  switch (command) {
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
    case READ_HOLDING_REGISTERS_COMMAND:
      crc = CRC16(&req[0], req.size());
      req.push_back(make8(crc, 0));
      req.push_back(make8(crc, 1));
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      req.push_back(byte_count);

      for (const auto i : data) {
        req.push_back(make8(i, 0));
        req.push_back(make8(i, 1));
      }

      crc = CRC16(&req[0], req.size());
      req.push_back(make8(crc, 0));
      req.push_back(make8(crc, 1));
      break;
    default:
      break;
  }

  return req;
}

bool Modbus::read_modbus_response(const std::vector<uint8_t>& req,
                                  std::vector<uint8_t>& resp) const {
  uint16_t i, time_out, expected_size, req_cont;
  uint8_t cmd;

  i = 0;
  time_out = 0;
  cmd = req.at(1);
  expected_size = 0;
  req_cont = 0;

  switch (cmd) {
    case READ_HOLDING_REGISTERS_COMMAND:
      expected_size = (make16(req.at(4), req.at(5)) << 1) + 5;
      req_cont = REQUEST_SIZE;
      break;
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
      expected_size = make16(req.at(4), req.at(5));
      if (expected_size <= 8)
        expected_size = (expected_size / 8) + 5;
      else
        expected_size = (expected_size / 8) + 6;
      req_cont = REQUEST_SIZE;
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      expected_size = 8;
      req_cont = req.at(6) + 9;
      break;
    default:
      break;
  }

  byte aux[expected_size];

  //  Serial.flush();
  Serial.setTimeout(3000);
  Serial.write(&req[0], req.size());
  int readBytesNr = Serial.readBytes(aux, expected_size);

  for (int cont = 0; cont < readBytesNr; ++cont) {
    resp.push_back((uint8_t) aux[cont]);
  }

  //  while (Serial.available() != expected_size) {
  //    time_out++;
  //    delay(1);
  //
  //    if (time_out >= 3000) {
  //      Serial.flush();
  //      Serial.println("Sem resposta " + String(Serial.available()));
  //      return false;
  //    }
  //  }

  //  for (int cont = 0; cont < Serial.available(); ++cont) {
  //    Serial.print(String(Serial.read()) + ' ');
  //  }
  //
  //  Serial.flush();

  return readBytesNr == expected_size;
}

bool Modbus::make_transaction(const uint8_t& dev_addr, const uint16_t& from,
                              const uint16_t& i_size, const uint8_t& byte_count,
                              std::vector<uint16_t>& data, const uint8_t& command,
                              std::vector<uint16_t>& modbus_response) const {
  std::vector<uint8_t> req, resp;
  std::vector<uint16_t> m_aux(data);
  uint16_t resp_size;
  bool resul;

  switch (command) {
    case READ_DISCRETE_INPUT_COMMAND:
    case READ_COILS_COMMAND:
      if (i_size <= 8)
        resp_size = (uint16_t)(i_size / 8 + 5);
      else
        resp_size = (uint16_t)(i_size / 8 + 6);
      break;
    case READ_HOLDING_REGISTERS_COMMAND:
      resp_size = (uint16_t)(i_size << 1) + 5;
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      resp_size = (uint16_t) 6;
      break;
  }

  req = make_request(dev_addr, from, i_size, byte_count, m_aux, command);
  resul = read_modbus_response(req, resp);

  if (!resul)
    return false;

  //if (
}

std::vector<uint8_t> Modbus::readHoldingRegisters(const uint16_t& slv_addr,
    const uint16_t& from, const uint16_t& len) const {
  std::vector<uint8_t> resp;
  std::vector<uint16_t> data;

  data.push_back(0);

  std::vector<uint8_t> req = make_request(slv_addr, from, len, 0x01,
                                          data, READ_HOLDING_REGISTERS_COMMAND);
  //  Serial.write(&req[0], req.size());
  bool r = read_modbus_response(req, resp);

  if (r) {
    Serial.println("Recebido " + String(resp.size()));

    for (const auto i : resp)
      Serial.print(String(i) + ' ');
  } else {
    Serial.println("Erro " + String(resp.size()));
  }

  Serial.println("");

  return resp;
}




/*
   modbus.c

    Created on: 20/08/2015
        Author: cuki


  #include "modbus.h"
  #include <stdlib.h>
  #include <STDDEF.H>

  #ifdef USE_UART1
  #use rs232(uart1, baud=UART1_BAUD_RATE, stream=sl1)
  #INT_RDA
  void isr_rda() {
  clear_interrupt(INT_RDA);
  uc_set_timer_rda();
  uc_setup_timer_rda();
  buffer_rda[index_rda++] = fgetc(sl1);
  if (index_rda >= RDA_BUFFER_SIZE)
    index_rda = 0;
  }
  #if (SL1_TYPE == SL_MULTI_MASTER)
  #uint8_t_TIMER0
  void isr_rda_timer() {
  clear_interrupt(RDA_INTERRUPT);
  index_rda = 0;
  uc_set_timer_rda();
  uc_turn_off_timer_rda();
  slave_respond = TRUE;
  }
  #endif
  #endif

  #ifdef USE_UART2
  #use rs232(uart2, baud=UART2_BAUD_RATE, stream=sl2)
  #INT_RDA2
  void isr_rda2() {
  clear_interrupt(INT_RDA2);
  uc_set_timer_rda2();
  uc_setup_timer_rda2();
  buffer_rda2[index_rda2++] = fgetc(sl2);
  if (index_rda2 >= RDA2_BUFFER_SIZE)
    index_rda2 = 0;
  }
  #if (SL2_TYPE == SL_MULTI_MASTER)
  #INT_TIMER1
  void isr_rda2_timer() {
  clear_interrupt(RDA2_INTERRUPT);
  uc_set_timer_rda2();
  uc_turn_off_timer_rda2();
  slave_respond = TRUE;
  }
  #endif
  #endif

  uint16_t swap_w(uint16_t word) {
  uint16_t aux;

  aux = (word & 0xFF00) >> 8;
  aux |= (word & 0x00FF) << 8;

  return aux;
  }

  uint32_t get_dword(uint16_t address) {
  uint8_t addr = address * 2;
  return make32(read_eeprom(addr), read_eeprom(addr + 1),
                read_eeprom(addr + 2), read_eeprom(addr + 3));
  }

  uint8_t set_dword(uint16_t address, uint32_t value) {
  uint8_t addr = address * 2;
  write_eeprom(addr, make8(value, 3));
  write_eeprom(addr + 1, make8(value, 2));
  write_eeprom(addr + 2, make8(value, 1));
  write_eeprom(addr + 3, make8(value, 0));
  return 0;
  }

  uint16_t get_register(uint16_t address) {
  uint16_t addr = address * 2;

  return make16(read_eeprom(addr), read_eeprom(addr + 1));
  }

  uint8_t set_register(uint16_t address, uint16_t value) {
  uint16_t addr = address * 2;

  write_eeprom(addr, make8(value, 1));
  write_eeprom(addr + 1, make8(value, 0));

  return 0;
  }

  uint8_t get_byte(uint16_t address) {
  uint16_t addr;

  addr = (uint8_t) address / 2;

  if (address % 2) {
    addr++;
  }

  return read_eeprom(addr);
  }

  uint8_t set_byte(uint16_t address, uint8_t value) {
  uint16_t addr;

  addr = (uint8_t) address / 2;

  if (address % 2) {
    addr++;
  }

  write_eeprom(addr, value);

  return 0;
  }

  bool get_bool(uint16_t address) {
  uint8_t addr;

  addr = (uint8_t) address / 8;

  return (bool) read_eeprom(addr) & (1 << (address - addr));
  }

  bool set_bool(uint16_t address, bool value) {
  uint8_t reg;
  uint8_t addr;

  addr = (uint8_t) address / 8;
  reg = read_eeprom(addr);

  if (value) {
    reg |= (1 << (address - addr));
  } else {
    reg &= ~(1 << (address - addr));
  }

  write_eeprom(addr, value);

  return FALSE;
  }

  exception_t slave_response() {
  short rbt;
  uint16_t cont, crc, register_value, register_address, b_count, size,
           m_index, aux;
  uint8_t *resp, *buff, my_address;

  #ifdef USE_UART1
  #if (SL1_TYPE == SL_MULTI_MASTER)
  buff = buffer_rda;
  m_index = index_rda;
  #endif
  #endif
  #ifdef USE_UART2
  #if (SL2_TYPE == SL_MULTI_MASTER)
  buff = buffer_rda2;
  m_index = index_rda2;
  #endif
  #endif

  size = 0;
  rbt = FALSE;
  my_address = get_byte(ADDR_MY_B);
  register_value = make16(buff[MODBUS_FIELDS_REGISTER_VALUE_H],
                          buff[MODBUS_FIELDS_REGISTER_VALUE_L]);
  register_address = make16(buff[MODBUS_FIELDS_REGISTER_ADDRESS_H],
                            buff[MODBUS_FIELDS_REGISTER_ADDRESS_L]);
  b_count = buff[MODBUS_FIELDS_BYTE_COUNT];

  if (my_address == buff[MODBUS_FIELDS_ADDRESS]) {
    switch (buff[MODBUS_FIELDS_FUNCTION]) {
      case READ_HOLDING_REGISTERS_COMMAND:

        if (register_value == 0 || register_value > 0x007D
            || m_index != 8) {
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc((size_t) (size * sizeof(uint8_t)));

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x83;
          resp[2] = 0x03;
          crc = CRC16(resp, 3);
        } else if (register_value + register_address >= UINT16_MAX) {
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc((size_t) (size * sizeof(uint8_t)));

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x83;
          resp[2] = 0x02;
          crc = CRC16(resp, 3);
        } else {
          size = (size_t)((register_value * 2) + 5);
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = READ_HOLDING_REGISTERS_COMMAND;
          resp[2] = (uint8_t) register_value * 2;

          aux = 0;
          for (cont = 0; cont < register_value; ++cont) {
            aux = get_register(cont + register_address);
            resp[2 * cont + 3] = make8(aux, 1);
            resp[2 * cont + 4] = make8(aux, 0);

          }

          crc = CRC16(resp, (register_value * 2) + 3);
        }

        resp[size - 2] = make8(crc, 0);
        resp[size - 1] = make8(crc, 1);
        break;

      case WRITE_SINGLE_REGISTER_COMMAND:

        if (m_index != 8) {
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc((size_t) (size * sizeof(uint8_t)));

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x86;
          resp[2] = 0x03;
          crc = CRC16(resp, 3);
          resp[4] = make8(crc, 1);
          resp[5] = make8(crc, 0);
        } else if (register_value + register_address >= 0x03F8) { //0x03F8 eh o tamanho da memoria do 46K22
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x86;
          resp[2] = 0x02;
          crc = CRC16(resp, 3);
          resp[4] = make8(crc, 1);
          resp[5] = make8(crc, 0);
        } else {

          size = (size_t)(8 * sizeof(uint8_t));
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          if (register_address < 14 || register_address > 15) //protecao para versao
            set_register(register_address, register_value);

          resp[0] = (uint8_t) my_address;
          resp[1] = WRITE_SINGLE_REGISTER_COMMAND;
          resp[2] = (uint8_t) make8(register_address, 1);
          resp[3] = (uint8_t) make8(register_address, 0);
          resp[4] = (uint8_t) make8(register_value, 1);
          resp[5] = (uint8_t) make8(register_value, 0);
          crc = CRC16(resp, 6);
          resp[6] = (uint8_t) make8(crc, 1);
          resp[7] = (uint8_t) make8(crc, 0);
        }

        break;

      case WRITE_MULTIPLE_REGISTERS_COMMAND:

        if (register_value == 0 || register_value > 0x007B
            || b_count != 2 * register_value
            || m_index != (b_count + 9)) {
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x90;
          resp[2] = 0x03;
          crc = CRC16(resp, 3);
        } else if (register_value + register_address >= 0x03F8) { //0x03F8 eh o tamanho da memoria do 46K22
          size = 5;
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = 0x90;
          resp[2] = 0x02;
          crc = CRC16(resp, 3);
        } else {

          size = (size_t)(8);
          resp = NULL;
          resp = (uint8_t *) malloc(size);

          if (resp == NULL) {
            free(resp);
            return OUT_OF_MEMORY_EXCEPTION;
          }

          for (cont = 0; cont < b_count; ++cont) {
            if (register_address < 14 || register_address > 15)
              write_eeprom(2 * register_address + cont,
                           buff[cont + MODBUS_FIELDS_BYTE_COUNT + 1]);
          }

          resp[0] = (uint8_t) my_address;
          resp[1] = WRITE_MULTIPLE_REGISTERS_COMMAND;
          resp[2] = (uint8_t) make8(register_address, 1);
          resp[3] = (uint8_t) make8(register_address, 0);
          resp[4] = (uint8_t) make8(register_value, 1);
          resp[5] = (uint8_t) make8(register_value, 0);
          crc = CRC16(resp, 6);
          resp[6] = (uint8_t) make8(crc, 1);
          resp[7] = (uint8_t) make8(crc, 0);
        }

        break;

      default:
        break;
    }
  }

  for (cont = 0; cont < size; ++cont) {
  #ifdef USE_UART1
  #if (SL1_TYPE == SL_MULTI_MASTER)
    fputc(resp[cont], sl1);
  #endif
  #endif

  #ifdef USE_UART2
  #if (SL2_TYPE == SL_MULTI_MASTER)
    fputc(resp[cont], sl2);
  #endif
  #endif
  }

  free(resp);

  #ifdef USE_UART1
  #if (SL1_TYPE == SL_MULTI_MASTER)
  flush_buffer(buffer_rda, &index_rda);
  #endif
  #endif
  #ifdef USE_UART2
  #if (SL2_TYPE == SL_MULTI_MASTER)
  flush_buffer(buffer_rda2, &index_rda2);
  #endif
  #endif

  if (rbt) {
    rbt = FALSE;
    reset_cpu();
  }

  return NO_EXCEPTION;
  }

  uint8_t modbus_init(void) {

  #ifdef USE_UART1
  uc_turn_off_timer_rda();
  uc_clear_rda_interrupt();
  uc_set_timer_rda();
  clear_interrupt(INT_RDA);
  enable_interrupts(INT_RDA);
  clear_interrupt(RDA_INTERRUPT);
  enable_interrupts(RDA_INTERRUPT);
  #endif

  #ifdef USE_UART2
  uc_clear_RDA2_INTERRUPT();
  uc_turn_off_timer_rda2();
  uc_set_timer_rda2();
  clear_interrupt(INT_RDA2);
  enable_interrupts(INT_RDA2);
  clear_interrupt(RDA2_INTERRUPT);
  enable_interrupts(RDA2_INTERRUPT);
  #endif

  enable_interrupts(GLOBAL);

  slave_respond = FALSE;

  return 0;
  }

  uint8_t parse_error(exception_t error, char *msg) {
  switch (error) {
    case NO_EXCEPTION:
      strcpy(msg, "no errors");
      break;
    case TIMEOUT_EXCEPTION:
      strcpy(msg, "time out");
      break;
    case OUT_OF_MEMORY_EXCEPTION:
      strcpy(msg, "out of memory");
      break;
    case CRC_EXCEPTION:
      strcpy(msg, "crc error");
      break;
    case WRONG_RESPONSE_EXCEPTION:
      strcpy(msg, "wrong response");
      break;
    case MODBUS_EXCEPTION:
      strcpy(msg, "modbus exception");
      break;
    case NOT_SUPPORTED_EXCEPTION:
      strcpy(msg, "operacao nao suportada");
      break;
    default:
      strcpy(msg, "unknown error");
      break;
  }
  return 0;
  }

  uint16_t CRC16(uint8_t *nData, uint16_t wLength) {
  uint16_t nTemp;
  uint16_t wCRCWord = 0xFFFF;

  while (wLength--) {
    nTemp = *nData++ ^ wCRCWord;
    wCRCWord >>= 8;
    wCRCWord ^= wCRCTable[nTemp];
  }

  return wCRCWord;
  }

  uint8_t make_request(uint8_t dev_addr, uint16_t from, uint16_t size,
                     uint8_t byte_count, uint8_t *data, modbus_command_t command,
                     uint8_t *req) {
  uint16_t crc, cont;

  req[0] = dev_addr;
  req[1] = (uint8_t) command;
  req[2] = make8(from, 1);
  req[3] = make8(from, 0);
  req[4] = make8(size, 1);
  req[5] = make8(size, 0);

  switch (command) {
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
    case READ_HOLDING_REGISTERS_COMMAND:
      crc = CRC16(req, 6);
      req[6] = make8(crc, 0);
      req[7] = make8(crc, 1);
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      req[6] = byte_count;
      for (cont = 0; cont < byte_count; cont += 2) {
        req[7 + cont] = data[cont + 1];
        req[8 + cont] = data[cont];
      }
      crc = CRC16(req, cont + 7);
      req[cont + 7] = make8(crc, 0);
      req[cont + 8] = make8(crc, 1);
      break;
    default:
      break;
  }

  return 0;
  }

  void flush_buffer(uint8_t *buffer, uint16_t *index) {
  uint8_t erase_cont;

  for (erase_cont = 0; erase_cont < *index; ++erase_cont)
    buffer[erase_cont] = 0;

   index = 0;
  }

  //funcao que leia a resposta do plc
  exception_t read_modbus_response(uint8_t *req, uint8_t *resp, door_t sl) {
  uint16_t i, time_out, expected_size, cont, req_cont;
  modbus_command_t cmd;

  i = 0;
  time_out = 0;
  cmd = (modbus_command_t) req[1];
  expected_size = 0;
  req_cont = 0;

  if (sl == SL1_DOOR) {
  #ifdef USE_UART1
    disable_interrupts(RDA_INTERRUPT);
    disable_interrupts(RDA_INTERRUPT);
    index_rda = 0;
  #endif
  } else {
  #ifdef USE_UART2
    disable_interrupts(RDA2_INTERRUPT);
    index_rda2 = 0;
  #endif
  }

  switch (cmd) {
    case READ_HOLDING_REGISTERS_COMMAND:
      expected_size = (make16(req[4], req[5]) << 1) + 5;
      req_cont = REQUEST_SIZE;
      break;
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
      expected_size = make16(req[4], req[5]);
      if (expected_size <= 8)
        expected_size = (expected_size / 8) + 5;
      else
        expected_size = (expected_size / 8) + 6;
      req_cont = REQUEST_SIZE;
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      expected_size = 8;
      req_cont = req[6] + 9;
      break;
    default:
      break;
  }

  if (sl == SL1_DOOR) {
  #ifdef USE_UART1
    for (i = 0; i < req_cont; ++i) {
      fputc(req[i], sl1);
    }

    while (!interrupt_active(RDA_INTERRUPT)) {
  #ifdef USE_WDT
      restart_wdt();
  #endif
      delay_us(10);
      time_out++;
      if (time_out == 0xFFFF) {
        flush_buffer(buffer_rda, &index_rda);
        enable_interrupts(RDA_INTERRUPT);
        return TIMEOUT_EXCEPTION;
      }
    }

    uc_clear_rda_interrupt();
    uc_turn_off_timer_rda();
    uc_set_timer_rda();

    if (index_rda != 0 && index_rda == expected_size) {
      for (cont = 0; cont < index_rda; ++cont) {
        resp[cont] = buffer_rda[cont];
        buffer_rda[cont] = 0;
      }
      index_rda = 0;
    } else if (index_rda != expected_size) {
      flush_buffer(buffer_rda, &index_rda);
      enable_interrupts(RDA_INTERRUPT);
      return WRONG_RESPONSE_EXCEPTION;
    }
    enable_interrupts(RDA_INTERRUPT);
  #endif
  } else {
  #ifdef USE_UART2
    for (i = 0; i < req_cont; ++i) {
      fputc(req[i], sl2);
    }

    while (!interrupt_active(RDA2_INTERRUPT)) {
  #ifdef USE_WDT
      restart_wdt();
  #endif
      delay_us(10);
      time_out++;
      if (time_out == 0xFFFF) {
        flush_buffer(buffer_rda2, &index_rda2);
        enable_interrupts(RDA2_INTERRUPT);
        return TIMEOUT_EXCEPTION;
      }
    }

    uc_clear_RDA2_INTERRUPT();
    uc_turn_off_timer_rda2();
    uc_set_timer_rda2();

    if (index_rda2 != 0 && index_rda2 == expected_size) {
      for (cont = 0; cont < index_rda2; ++cont) {
        resp[cont] = buffer_rda2[cont];
        buffer_rda2[cont] = 0;
      }
      index_rda2 = 0;
    } else if (index_rda2 != expected_size) {
      flush_buffer(buffer_rda2, &index_rda2);
      enable_interrupts(RDA2_INTERRUPT);
      return WRONG_RESPONSE_EXCEPTION;
    }
    enable_interrupts(RDA2_INTERRUPT);
  #endif
  }

  return NO_EXCEPTION;
  }
  exception_t mount_modbus_response(modbus_rx_t *modbus_response, uint8_t *resp) {
  uint8_t cont, size, *data;

  data = NULL;
  size = resp[2];

  modbus_response->address = resp[0];
  modbus_response->function = resp[1];
  modbus_response->response_size = size;

  free(modbus_response->data);
  data = (uint8_t *) malloc((size_t) size);

  if (data == NULL) {
    free(data);
    return OUT_OF_MEMORY_EXCEPTION;
  }

  for (cont = 0; cont < size; ++cont)
    data[cont] = resp[cont + 3];

  modbus_response->data = data;
  modbus_response->crc = make16(resp[size + 4], resp[size + 3]);

  return NO_EXCEPTION;
  }

  bool check_CRC(uint8_t *resp, modbus_command_t command) {
  uint8_t *arr;
  uint16_t ar_size, crc_check, crc_in, cont;

  switch (command) {
    case READ_HOLDING_REGISTERS_COMMAND:
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
      ar_size = resp[2] + 3;
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      ar_size = 6;
      break;
    default:
      break;
  }

  arr = NULL;
  arr = (uint8_t *) malloc((size_t) ar_size);

  if (arr == NULL) {
    free(arr);
    return FALSE;
  }

  for (cont = 0; cont < ar_size; ++cont)
    arr[cont] = resp[cont];

  crc_in = make16(resp[ar_size + 1], resp[ar_size]);
  crc_check = CRC16(arr, ar_size);
  free(arr);

  return crc_check == crc_in;;
  }

  exception_t make_transaction(uint8_t dev_addr, uint16_t from, uint16_t size,
                             uint8_t byte_count, uint8_t *data, modbus_command_t command,
                             modbus_rx_t *modbus_response, door_t sl) {
  uint8_t *req, *resp;
  uint16_t resp_size;
  exception_t resul;

  resul = NO_EXCEPTION;
  resp_size = 0;
  req = NULL;

  switch (command) {
    case READ_DISCRETE_INPUT_COMMAND:
    case READ_COILS_COMMAND:
      if (size <= 8)
        resp_size = (size_t)(size / 8 + 5);
      else
        resp_size = (size_t)(size / 8 + 6);
      req = (uint8_t *) malloc((size_t) REQUEST_SIZE);
      break;
    case READ_HOLDING_REGISTERS_COMMAND:
      resp_size = (size_t)(size << 1) + 5;
      req = (uint8_t *) malloc((size_t) REQUEST_SIZE);
      break;
    case WRITE_MULTIPLE_REGISTERS_COMMAND:
      resp_size = (size_t) 6;
      req = (uint8_t *) malloc((size_t)(9 + byte_count));
      break;
  }

  if (req == NULL) {
    free(req);
    return OUT_OF_MEMORY_EXCEPTION;
  }

  make_request(dev_addr, from, size, byte_count, data, command, req);

  resp = NULL;
  resp = (uint8_t *) malloc(resp_size * sizeof(uint8_t));

  if (resp == NULL) {
    free(req);
    free(resp);
    return OUT_OF_MEMORY_EXCEPTION;
  }

  resul = read_modbus_response(req, resp, sl);
  free(req);

  if (resul != NO_EXCEPTION) {
    free(resp);
    return resul;
  }

  if (!check_CRC(resp, command)) {
    free(resp);
    return CRC_EXCEPTION;
  }

  resul = mount_modbus_response(modbus_response, resp);
  free(resp);

  if (resul != NO_EXCEPTION)
    return resul;

  return NO_EXCEPTION;
  }

  uint8_t get_word_mem(modbus_rx_t *device, uint16_t *resp) {
  uint8_t cont, size_bytes, size_array;
  uint8_t aux_h, aux_l;

  size_bytes = device->response_size;
  size_array = device->response_size / sizeof(uint8_t);

  for (cont = 0; cont < size_array; ++cont) {
    aux_h = device->data[2 * cont];
    aux_l = device->data[(2 * cont) + 1];
    resp[cont] = make16(aux_h, aux_l);
  }

  return 0;
  }

  uint8_t get_byte_mem(modbus_rx_t *device, uint16_t *resp) {
  uint8_t cont;

  for (cont = 0; cont < device->response_size; ++cont)
    resp[cont] = device->data[cont];

  return 0;
  }

  exception_t transport(uint8_t dev_addr, uint16_t from, uint16_t size,
                      uint8_t byte_count, uint8_t *data, uint16_t *resp,
                      modbus_command_t command, door_t sl) {
  modbus_rx_t *modbus_response;
  exception_t exception;

  modbus_response = NULL;
  exception = NO_EXCEPTION;
  modbus_response = (modbus_rx_t *) malloc(sizeof(modbus_rx_t));

  if (modbus_response == NULL) {
    free(modbus_response->data);
    free(modbus_response);
    return OUT_OF_MEMORY_EXCEPTION;
  }

  exception = make_transaction(dev_addr, from, size, byte_count, data,
                               command, modbus_response, sl);

  if (exception != NO_EXCEPTION) {
    free(modbus_response->data);
    free(modbus_response);
    return exception;
  }

  switch (command) {
    case READ_COILS_COMMAND:
    case READ_DISCRETE_INPUT_COMMAND:
      get_byte_mem(modbus_response, resp);
      break;
    case READ_HOLDING_REGISTERS_COMMAND:
      get_word_mem(modbus_response, resp);
      break;
    default:
      break;
  }

  free(modbus_response->data);
  free(modbus_response);

  return NO_EXCEPTION;

  }

  exception_t read_holding_registers(uint8_t dev_addr, uint16_t from,
                                   uint16_t size, uint16_t *to, door_t sl) {
  return transport(dev_addr, from, size, 0, NULL, to,
                   READ_HOLDING_REGISTERS_COMMAND, sl);
  }

  exception_t read_discrete_inputs(uint8_t dev_addr, uint16_t from, uint16_t size,
                                 uint16_t *to, door_t sl) {

  return transport(dev_addr, from, size, 0, NULL, to,
                   READ_DISCRETE_INPUT_COMMAND, sl);
  }

  exception_t read_coils(uint8_t dev_addr, uint16_t from, uint16_t size,
                       uint16_t *to, door_t sl) {

  return transport(dev_addr, from, size, 0, NULL, to, READ_COILS_COMMAND, sl);
  }

  exception_t send_modbus(uint8_t *data, uint16_t size) {
  uint8_t cont;
  uint8_t *ptr;

  for (cont = 0, ptr = data; cont < size; cont++, ptr++) {
  #ifdef USE_UART1
  #if (SL1_TYPE == SL_MULTI_MASTER)
    fputc(*ptr, sl1);
  #endif
  #endif
  #ifdef USE_UART2
  #if (SL2_TYPE == SL_MULTI_MASTER)
    fputc(*ptr, sl2);
  #endif
  #endif
  }

  return NO_EXCEPTION;
  }

  exception_t return_error(uint8_t dev_addr, modbus_command_t command,
                         modbus_command_exception_code_t error) {
  uint8_t *resp;
  uint16_t crc, size;

  switch (command) {
    case WRITE_SINGLE_REGISTER_COMMAND:
      size = 5;
      resp = NULL;
      resp = (uint8_t *) malloc((size_t)(size * sizeof(uint8_t)));

      if (resp == NULL) {
        free(resp);
        return OUT_OF_MEMORY_EXCEPTION;
      }

      resp[0] = (uint8_t) get_byte(ADDR_MY_B);
      resp[1] = 0x86;
      resp[2] = error;
      crc = CRC16(resp, 3);
      resp[3] = make8(crc, 1);
      resp[4] = make8(crc, 0);

      break;
    default:
      break;
  }

  send_modbus(resp, size);
  free(resp);

  return NO_EXCEPTION;
  }

  exception_t write_multiple_registers(uint8_t dev_addr, uint16_t from,
                                     uint16_t size, uint16_t byte_count, uint8_t *data, door_t sl) {

  if (size < 3) {
    return NOT_SUPPORTED_EXCEPTION;
  }

  return transport(dev_addr, from, size, byte_count, data, NULL,
                   WRITE_MULTIPLE_REGISTERS_COMMAND, sl);
  }


*/

