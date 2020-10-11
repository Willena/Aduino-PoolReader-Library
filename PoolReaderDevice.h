#ifndef POOL_READER_DEVICE_H
#define POOL_READER_DEVICE_H

#include "OneWireItem.h"
#include "PoolReader.h"

class PoolReaderDevice : public PoolReader, public OneWireItem
{
  private:

    void updateCRC(void);

  public:

    PoolReaderDevice(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7);

    void duty(OneWireHub * hub) final;

    void setTemperatureRaw(int16_t t);
    void setPhRaw(uint16_t ph);
    void setOrpRaw(uint16_t orp);
    void setWaterLevelRaw(uint8_t level);
    uint8_t setSampleInterval(uint8_t interval);

    void setTemperature(float t);
    void setPh(float ph);
    void setOrp(int16_t orp);
    void setWaterLevel(float level);    

};


#endif
