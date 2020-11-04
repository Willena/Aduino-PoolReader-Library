/* Pool Reader library implementation 
 *  Author: Guillaume VILLENA
 *  Date: 10-2020
 */

#include "PoolReaderClient.h"

PoolReaderClient::PoolReaderClient(OneWire *ow)
{
  this->ow = ow;
  hasAddr = false;
}

PoolReaderClient::PoolReaderClient(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7)
{
  currentaddr[0] = ID1;
  currentaddr[1] = ID2;
  currentaddr[2] = ID3;
  currentaddr[3] = ID4;
  currentaddr[4] = ID5;
  currentaddr[5] = ID6;
  currentaddr[6] = ID7;
  hasAddr = true;
}

PoolReaderClient::PoolReaderClient(OneWire *ow, pool_reader_address_t addr)
{
  memcpy(currentaddr, addr, 8);
  this->ow = ow;
  hasAddr = true;
}

void PoolReaderClient::begin()
{
  if (ow != NULL)
  {
    //Acting as client mode.

    if (!hasAddr)
    {
      //If address not specified try to find the first one return false if could not find a device
      pool_reader_address_t address;
      uint8_t count = 0;

      ow->reset_search();
      while (ow->search(address))
      {

        if (address[0] != PoolReaderClient::family_code)
          continue;

        break;
      }
      memcpy(currentaddr, address, 8);
      hasAddr = true;
    }
  }
}

bool PoolReaderClient::readBus(uint8_t *dest, uint8_t len)
{
  if (ow == NULL)
    return false;

  for (int i = 0; i < len; i++)
  {
    dest[i] = ow->read();
  }

  return true;
}

void PoolReaderClient::setCalibrationValue(float temperature, float bufferValue, uint16_t adcValue)
{
  calibrationData.cal_temp = temperature;
  calibrationData.cal_ph_buffer = bufferValue;
  calibrationData.cal_mv_value = (((adcValue * ADC_STEP_V) / PROBE_AMP_GAIN) - PROBE_SWING_HALF) * V_MV;
}

bool PoolReaderClient::read()
{

  if (!hasAddr && ow == NULL)
    return false;

  uint8_t *s = reinterpret_cast<uint8_t *>(&state);
  int b = ow->reset();
  if (b == 0)
    return false;

  ow->select(currentaddr);
  ow->write(POOL_READER_COMMAND_READ);
  //readBus(s,sizeof(pool_reader_register_t));
  for (int i = 0; i < sizeof(pool_reader_register_t); i++)
  {
    s[i] = ow->read();
  }

  //Check CRC
  if (ow->crc8(s, sizeof(pool_reader_register_t) - 1) != state.crc)
    return false;

  return true;
}

bool PoolReaderClient::writeSampleInterval(uint8_t interval)
{
  if (!hasAddr && ow == NULL)
    return false;

  if (interval > 255 || interval < 1)
    return false;

  int b = ow->reset();
  if (b == 0)
    return false;

  ow->select(currentaddr);
  ow->write(POOL_READER_COMMAND_SAMPLE_TIME);
  ow->write(interval);

  if (ow->read() != POOL_READER_RESPONSE_OK)
    return false;

  b = ow->reset();
  return (b == 1);
}

uint16_t PoolReaderClient::getTemperatureRaw() const
{
  return state.T;
}

uint16_t PoolReaderClient::getPhRaw() const
{
  return state.PH;
}

uint16_t PoolReaderClient::getOrpRaw() const
{
  return state.ORP;
}

uint8_t PoolReaderClient::getWaterLevelRaw() const
{
  return state.wLevel;
}

float PoolReaderClient::getTemperature() const
{
  return static_cast<int16_t>(getTemperatureRaw()) / 16.0f;
}

float PoolReaderClient::getPh() const
{
  return getPh(calibrationData.cal_temp);
}

float PoolReaderClient::getPh(float currentTemperature) const
{

  //Serial.print("CAL: ");
  //Serial.println(calibrationData.cal_mv_value, 6);

  //Automatic compensation using temperature (dtermine the compensation factor)
  float factor = (currentTemperature + ZERO_C_K) / (ZERO_C_K + calibrationData.cal_temp);
  //Serial.print("FACTROR: ");
  //Serial.println(factor,6);

  float mesuredmV = (((getPhRaw() * ADC_STEP_V) / PROBE_AMP_GAIN) - PROBE_SWING_HALF) * V_MV / factor;
  //Serial.print("MESURED: ");
  //Serial.println(mesuredmV, 6);

  //Ph value using calibration.
  float pHCal = calibrationData.cal_ph_buffer + (((calibrationData.cal_mv_value - mesuredmV) * FARADAY_CST) / (GAS_CST * (ZERO_C_K + calibrationData.cal_temp) * log(10)));
  //Serial.println("Ph " + String(pH));

  if (pHCal < 0)
    return 0;

  if (pHCal > 14)
    return 14;

  return pHCal;
}

float PoolReaderClient::getWaterLevel() const
{
  return getWaterLevelRaw() / 10.0f;
}

int16_t PoolReaderClient::getOrp() const
{
  return (getOrpRaw() / 0.256f) - 2000;
}

uint8_t PoolReaderClient::getSampleInterval() const
{
  return state.interval;
}
