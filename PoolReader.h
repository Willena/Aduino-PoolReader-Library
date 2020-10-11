#ifndef POOL_READER_H
#define POOL_READER_H

#include <OneWire.h>

//Device Commands
#define POOL_READER_COMMAND_READ 0x6B
#define POOL_READER_COMMAND_SAMPLE_TIME 0x6C
#define POOL_READER_RESPONSE_OK 0x01
#define POOL_READER_RESPONSE_NOK 0x02

//Main structure
typedef struct __attribute__((packed)) {
  int16_t T;            //Value as DS18B20 doc
  uint16_t PH;          //value between (0-1024 => 0-14pH)
  uint16_t ORP;         //value between (0-1024 => -2000-2000mV)
  uint8_t wLevel;       //Water level (0-255 => +1 = +0.1cm. ; Max range 25.5cm)
  uint8_t interval;     //Sample interval (1-255; Min is 1, max is 255)
  uint8_t crc;          //CRC of the content
} pool_reader_register_t;


typedef uint8_t pool_reader_address_t[8];

class PoolReader{

  private:

  bool hasAddr = false;
  OneWire * ow = NULL;

  bool readBus(uint8_t * dest, uint8_t len);
  
  protected:

  pool_reader_register_t state;
  pool_reader_address_t currentaddr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  public:

  static constexpr uint8_t family_code { 0xB7 }; 

  
  PoolReader(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7); //Constructor for Hub usage
  PoolReader(OneWire * ow, pool_reader_address_t addr ); //Constructor to address single device on pin
  PoolReader(OneWire * ow); // Default constructor. Use if only one device
  void begin(); 
  bool read(); //Read register on the bus

  uint16_t getTemperatureRaw() const;
  uint16_t getPhRaw() const;
  uint16_t getOrpRaw() const;
  uint8_t getWaterLevelRaw() const;
  
  float getTemperature() const;
  float getPh() const;
  float getWaterLevel() const;
  int16_t getOrp() const;
  
  uint8_t getSampleInterval() const;
  bool writeSampleInterval(uint8_t interval);
  
};

#endif
