#include "PoolReaderDevice.h"

PoolReaderDevice::PoolReaderDevice(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7) : OneWireItem(ID1, ID2, ID3, ID4, ID5, ID6, ID7)
{
  state.T = 0;
  state.PH = 0;
  state.ORP = 0;
  state.wLevel = 0;
  state.interval = 10;
  updateCRC();
}

void PoolReaderDevice::updateCRC()
{
  state.crc = crc8(reinterpret_cast<uint8_t *>(&state), sizeof(pool_reader_register_t) - 1);
}

void PoolReaderDevice::duty(OneWireHub *hub)
{
  uint8_t cmd;
  //Serial.write('*');
  if (hub->recv(&cmd, 1))
  {
    //Serial.write('0');
    return;
  }

  switch (cmd)
  {
  case 0x6B: // Read Mesurments
    hub->send(reinterpret_cast<uint8_t *>(&state), sizeof(pool_reader_register_t));
    //Serial.write('1');
    break;

  case 0x6C: // Define sample interval
             //Serial.write('2');
    uint8_t v;
    hub->recv(&v, 1);
    hub->send(setSampleInterval(v));
    break; // send 1s, is passive ...

  default:
    //Serial.write('3');
    hub->raiseSlaveError(cmd);
  }
  //Serial.write('E');
}

void PoolReaderDevice::setTemperatureRaw(int16_t t)
{
  state.T = t;
  updateCRC();
}

void PoolReaderDevice::setPhRaw(uint16_t ph)
{
  state.PH = ph;
  updateCRC();
}

void PoolReaderDevice::setOrpRaw(uint16_t orp)
{
  state.ORP = orp;
  updateCRC();
}

void PoolReaderDevice::setWaterLevelRaw(uint8_t level)
{
  state.wLevel = level;
  updateCRC();
}

void PoolReaderDevice::setTemperature(float t)
{
  float value = t;
  // if (value > 125) value = 125;
  // if (value < -55) value = -55;
  setTemperatureRaw(static_cast<int16_t>(value * 16.0f));
}

void PoolReaderDevice::setPh(float ph)
{
  // 73 steps per Ph unit (14*73 = 1022)
  setPhRaw(static_cast<uint16_t>(ph * 73.0f));
}

void PoolReaderDevice::setOrp(int16_t orp)
{ //between -2000 and 2000mV (res 3.2mV)
  setOrpRaw(static_cast<uint16_t>((orp + 2000) * 0.256f));
}

void PoolReaderDevice::setWaterLevel(float level)
{
  setWaterLevelRaw(static_cast<uint8_t>(level * 10.0f));
}

uint8_t PoolReaderDevice::setSampleInterval(uint8_t interval)
{
  if (interval > 255 || interval < 1)
    return POOL_READER_RESPONSE_NOK;

  state.interval = interval;
  updateCRC();
  return POOL_READER_RESPONSE_OK;
}
