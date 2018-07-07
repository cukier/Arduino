/*
 * Released under the GPL
 * (C) 2018 by Cukier (mauriciocukier@gmail.com)
 */

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

  Serial.setTimeout(3000);
  Serial.write(&req[0], req.size());
  int readBytesNr = Serial.readBytes(aux, expected_size);

  for (int cont = 0; cont < readBytesNr; ++cont) {
    resp.push_back((uint8_t) aux[cont]);
  }

  return readBytesNr == expected_size;
}

bool Modbus::make_transaction(const uint8_t& dev_addr, const uint16_t& from,
                              const uint16_t& i_size, const uint8_t& byte_count,
                              const std::vector<uint16_t>& data, const uint8_t& command,
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

  if ((!(resul)) || (!(check_CRC(resp, command))))
    return false;

  m_aux.clear();

  if (!(mount_modbus_response(m_aux, resp))) {
    return false;
  }

  modbus_response.clear();
  modbus_response.insert(modbus_response.end(), m_aux.begin(), m_aux.end());

  return true;
}

bool Modbus::mount_modbus_response(std::vector<uint16_t>& modbus_response,
                                   const std::vector<uint8_t>& resp) const {

  if (!resp.size())
    return false;

  modbus_response.clear();

  for (uint16_t cont = 3; cont < resp.size(); ++cont) {
    modbus_response.push_back(resp.at(cont));
  }

  return true;
}

std::vector<uint16_t> Modbus::readHoldingRegisters(const uint16_t& slv_addr,
    const uint16_t& from, const uint16_t& len) const {
  std::vector<uint8_t> resp;
  std::vector<uint16_t> data;

  std::vector<uint8_t> req = make_request(slv_addr, from, len, 0x00,
                                          data,
                                          READ_HOLDING_REGISTERS_COMMAND);
  //  Serial.write(&req[0], req.size());
  bool r = read_modbus_response(req, resp);

  if (r) {
    r = mount_modbus_response(data, resp);
  } else {
    return std::vector<uint16_t>();
  }

  if (r) {
    return data;
  }

  return std::vector<uint16_t>();
}
