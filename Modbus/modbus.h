/*
 * Released under the GPL
 * (C) 2018 by Cukier (mauriciocukier@gmail.com)
 */

#ifndef _SRC_MODBUS_H
#define _SRC_MODUBS_H

#include <vector>
#include <stdint.h>

namespace serialNetwork {
class Modbus;
}

class Modbus {
  public:
    enum modbus_commands {
      NO_COMMAND,
      READ_COILS_COMMAND,
      READ_DISCRETE_INPUT_COMMAND,
      READ_HOLDING_REGISTERS_COMMAND,
      READ_INPUT_REGISTERS_COMMAND,
      WRITE_SINGLE_COIL_COMMAND,
      WRITE_SINGLE_REGISTER_COMMAND,
      READ_EXCEPTION_STATUS_COMMAND,
      DIAGNOSTICS_COMMAND,
      GET_COMM_EVENT_COUNTER_COMMAND = 0x0B,
      GET_COMM_EVENT_LOG_COMMAND,
      WRITE_MULTIPLE_COILS_COMMAND = 0x0F,
      WRITE_MULTIPLE_REGISTERS_COMMAND,
      REPORT_SLAVE_ID_COMMAND,
      READ_FILE_RECORD_COMMAND = 0x14,
      WRITE_FILE_RECORD_COMMAND,
      MASK_WRITE_REGISTER_COMMAND,
      READ_WRITE_MULTIPLE_REGISTERS_COMMAND,
      READ_FIFO_QUEQUE_COMMAND,
      READ_DEVICE_IDENT_COMMAND = 0x2B
    };

    Modbus();
    ~Modbus();

    void init(uint32_t baud);
    std::vector<uint16_t> readHoldingRegisters(const uint16_t &slv_addr,
        const uint16_t &from, const uint16_t &len) const;
    //int writeHoldingRegisters(uint16_t slv_addr, std::vector<uint16_t> data);

  private:
    static const uint16_t wCRCTable[];
    static const uint16_t REQUEST_SIZE = 8;

    uint16_t swap_w(const uint16_t i_word) const;
    uint16_t make16(const uint8_t h_b, const uint8_t l_b) const;
    uint8_t make8(const uint32_t i_word, const uint8_t index) const;
    uint16_t CRC16(const uint8_t *nData, const uint16_t wLength) const;
    bool check_CRC(const std::vector<uint8_t> resp, uint8_t command) const;
    std::vector<uint8_t> make_request(const uint8_t& dev_addr, const uint16_t& from,
                                      const uint16_t& i_size,  const uint8_t& byte_count,
                                      std::vector<uint16_t>& data, const uint8_t& command) const;
    bool read_modbus_response(const std::vector<uint8_t>& req,
                              std::vector<uint8_t>& resp) const;
    bool make_transaction(const uint8_t& dev_addr, const uint16_t& from,
                          const uint16_t& i_size, const uint8_t& byte_count,
                          const std::vector<uint16_t>& data, const uint8_t& command,
                          std::vector<uint16_t>& modbus_response) const;
    bool mount_modbus_response(std::vector<uint16_t>& modbus_response,
                               const std::vector<uint8_t>& resp) const;
};

#endif

/*
   modbus.h

    Created on: 1 de out de 2015
        Author: cuki


  #ifndef MODBUS_H_
  #define MODBUS_H_

  #include <stdint.h>
  #include <stdbool.h>

  #define RDA_TIMER_RESET          0
  #define RDA2_TIMER_RESET        0
  #define REQUEST_SIZE          8
  #define RDA_BUFFER_SIZE         256
  #define RDA2_BUFFER_SIZE        256
  #define UART1_BAUD_RATE         19200
  #define UART2_BAUD_RATE         19200
  #define UC_TIMER_0            11111
  #define UC_TIMER_1            22222
  #define SL_MASTER           33333
  #define SL_MULTI_MASTER         44444
  #define UC_UART_1           88888
  #define UC_UART_2           99999

  #ifndef SL1_TYPE
  #define SL1_TYPE SL_MASTER_SLAVE
  #endif

  #ifndef SL2_TYPE
  #define SL2_TYPE SL_MASTER_SLAVE
  #endif

  #ifdef USE_UART1
  #define uc_set_timer_rda()        set_timer0(RDA_TIMER_RESET)
  #define uc_setup_timer_rda()      setup_timer_0(T0_INTERNAL|T0_DIV_8)
  #define uc_clear_rda_interrupt()    clear_interrupt(INT_TIMER0)
  #define uc_turn_off_timer_rda()     setup_timer_0(T0_OFF)
  #define RDA_INTERRUPT         INT_TIMER0
  #endif

  #ifdef USE_UART2
  #define uc_set_timer_rda2()       set_timer1(RDA2_TIMER_RESET)
  #define uc_setup_timer_rda2()     setup_timer_1(T1_INTERNAL|T1_DIV_BY_8)
  #define uc_clear_rda2_interrupt()     clear_interrupt(INT_TIMER1)
  #define uc_turn_off_timer_rda2()    setup_timer_1(T1_DISABLED)
  #define RDA2_INTERRUPT          INT_TIMER1
  #endif

  typedef enum doors_enum {
  NO_DOOR, SL1_DOOR, SL2_DOOR
  } door_t;

  typedef enum modbus_commands {
  NO_COMMAND,
  READ_COILS_COMMAND,
  READ_DISCRETE_INPUT_COMMAND,
  READ_HOLDING_REGISTERS_COMMAND,
  READ_INPUT_REGISTERS_COMMAND,
  WRITE_SINGLE_COIL_COMMAND,
  WRITE_SINGLE_REGISTER_COMMAND,
  READ_EXCEPTION_STATUS_COMMAND,
  DIAGNOSTICS_COMMAND,
  GET_COMM_EVENT_COUNTER_COMMAND = 0x0B,
  GET_COMM_EVENT_LOG_COMMAND,
  WRITE_MULTIPLE_COILS_COMMAND = 0x0F,
  WRITE_MULTIPLE_REGISTERS_COMMAND,
  REPORT_SLAVE_ID_COMMAND,
  READ_FILE_RECORD_COMMAND = 0x14,
  WRITE_FILE_RECORD_COMMAND,
  MASK_WRITE_REGISTER_COMMAND,
  READ_WRITE_MULTIPLE_REGISTERS_COMMAND,
  READ_FIFO_QUEQUE_COMMAND,
  READ_DEVICE_IDENT_COMMAND = 0x2B
  } modbus_command_t;

  typedef enum modbus_fields {
  MODBUS_FIELDS_ADDRESS,
  MODBUS_FIELDS_FUNCTION,
  MODBUS_FIELDS_REGISTER_ADDRESS_H,
  MODBUS_FIELDS_REGISTER_ADDRESS_L,
  MODBUS_FIELDS_REGISTER_VALUE_H,
  MODBUS_FIELDS_REGISTER_VALUE_L,
  MODBUS_FIELDS_BYTE_COUNT
  } modbus_fields_t;

  typedef enum modbus_command_exception_code {
  EXCEPTION_CODE_0,
  EXCEPTION_CODE_1,
  EXCEPTION_CODE_2,
  EXCEPTION_CODE_3,
  EXCEPTION_CODE_4
  } modbus_command_exception_code_t;

  typedef enum read_holding_registers_exceptions {
  FUNCTION_CODE_NOT_SUPPORTED_EXCEPTION = 0x01,
  ADDRESS_EXCEPTION,
  QUANTITY_OF_REGISTERS_EXCEPTION,
  READ_MULTIPLE_REGISTER_EXCEPTION
  } read_holding_registers_exceptions_t;

  typedef enum write_single_register_exceptions {
  OUTPUT_ADDRESS_EXCEPTION = 0x02,
  OUTPUT_VALUE_EXCEPTION,
  WRITE_SINGLE_OUTPUT_EXCEPTION
  } write_single_register_exceptions_t;

  typedef struct modbus_response {
  uint8_t address;
  uint8_t function;
  uint8_t response_size;
  uint8_t *data;
  uint16_t crc;
  } modbus_rx_t;

  typedef struct modbus_request {
  uint8_t address;
  uint8_t function;
  uint16_t start_address;
  uint16_t size;
  uint8_t byte_count;
  uint8_t *data;
  uint16_t crc;
  } modbus_rq_t;

  static const uint16_t wCRCTable[] = { 0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301,
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
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

  static short slave_respond;
  static uint16_t index_rda, index_rda2;

  uint8_t buffer_rda[RDA_BUFFER_SIZE];
  uint8_t buffer_rda2[rda2_BUFFER_SIZE];


  #ifdef USE_UART1
  extern isr_rda();
  #if (SL1_TYPE == SL_MULTI_MASTER)
  extern void isr_rda_timer();
  #endif
  #endif
  #ifdef USE_UART2
  extern isr_rda2();
  #if (SL2_TYPE == SL_MULTI_MASTER)
  extern void isr_rda2_timer();
  #endif
  #endif

  extern uint16_t swap_w(uint16_t word);
  extern uint32_t get_dword(uint16_t address);
  extern uint8_t set_dword(uint16_t address, uint32_t value);
  extern uint16_t get_register(uint16_t address);
  extern uint8_t set_register(uint16_t address, uint16_t value);
  extern uint8_t get_byte(uint16_t address);
  extern uint8_t set_byte(uint16_t address, uint8_t value);
  extern bool get_bool(uint16_t address);
  extern bool set_bool(uint16_t address, bool value);
  extern exception_t slave_response(void);
  extern uint8_t modbus_init(void);
  extern uint8_t parse_error(exception_t error, char *msg);
  extern uint16_t CRC16(uint8_t *nData, uint16_t wLength);
  extern uint8_t make_request(uint8_t dev_addr, uint16_t from, uint16_t size,
    uint8_t byte_count, uint8_t *data, modbus_command_t command,
    uint8_t *req);
  extern void flush_buffer(uint8_t *buffer, uint16_t *index);
  extern exception_t read_modbus_response(uint8_t *req, uint8_t *resp, door_t sl);
  extern exception_t mount_modbus_response(modbus_rx_t *modbus_response,
    uint8_t *resp);
  extern bool check_CRC(uint8_t *resp, modbus_command_t command);
  extern exception_t make_transaction(uint8_t dev_addr, uint16_t from,
    uint16_t size, uint8_t byte_count, uint8_t *data,
    modbus_command_t command, modbus_rx_t *modbus_response, door_t sl);
  extern uint8_t get_word_mem(modbus_rx_t *device, uint16_t *resp);
  extern uint8_t get_byte_mem(modbus_rx_t *device, uint16_t *resp);
  extern exception_t transport(uint8_t dev_addr, uint16_t from, uint16_t size,
    uint8_t byte_count, uint8_t *data, uint16_t *resp,
    modbus_command_t command, door_t sl);
  extern exception_t read_holding_registers(uint8_t dev_addr, uint16_t from,
    uint16_t size, uint16_t *to, door_t sl);
  extern exception_t read_discrete_inputs(uint8_t dev_addr, uint16_t from,
    uint16_t size, uint16_t *to, door_t sl);
  extern exception_t read_coils(uint8_t dev_addr, uint16_t from, uint16_t size,
    uint16_t *to, door_t sl);
  extern exception_t send_modbus(uint8_t *data, uint16_t size);
  extern exception_t return_error(uint8_t dev_addr, modbus_command_t command,
    modbus_command_exception_code_t error);
  extern exception_t write_multiple_registers(uint8_t dev_addr, uint16_t from,
    uint16_t size, uint16_t byte_count, uint8_t *data, door_t sl);

  #endif
*/
