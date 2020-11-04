#ifndef POOL_READER_TYPES_H
#define POOL_READER_TYPES_H

//Device Commands
#define POOL_READER_COMMAND_READ 0x6B
#define POOL_READER_COMMAND_SAMPLE_TIME 0x6C
#define POOL_READER_RESPONSE_OK 0x01
#define POOL_READER_RESPONSE_NOK 0x02

//Main structure
typedef struct __attribute__((packed))
{
    int16_t T;        //Value as DS18B20 doc
    uint16_t PH;      //value between (0-1024 => -500mv  500mv value)
    uint16_t ORP;     //value between (0-1024 => -2000-2000mV)
    uint8_t wLevel;   //Water level (0-255 => +1 = +0.1cm. ; Max range 25.5cm)
    uint8_t interval; //Sample interval (1-255; Min is 1s, max is 255s)
    uint8_t crc;      //CRC of the content
} pool_reader_register_t;

#endif