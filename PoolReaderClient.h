#ifndef POOL_READER_H
#define POOL_READER_H

#include "Arduino.h"
#include "PoolReaderTypes.h"
#include <OneWire.h>
#include <math.h>

constexpr float V_MV = 1.0;
constexpr float ADC_STEPS = 1024.0;
constexpr float ADC_MAX_V = 5.0;
constexpr float ADC_STEP_V = ADC_MAX_V / ADC_STEPS;
constexpr float PROBE_ADC_MIN = -0.500;
constexpr float PROBE_ADC_MAX = 0.500;
constexpr float PROBE_SWING = 1.0;
constexpr float PROBE_AMP_GAIN = (ADC_MAX_V / PROBE_SWING);
constexpr float PROBE_SWING_HALF = (PROBE_SWING / 2.0);
constexpr float FARADAY_CST = 9.6485309E4; // C.mol-1
constexpr float GAS_CST = 8.314510;        // J.K-1.mol-1
constexpr float ZERO_C_K = 273.15;         // K

/*
 ***********************************************
 *                    PH formula 
 ***********************************************
 *
 * 
 *                    (E(Ref) - E(x)) * F
 * pH(x) = pH(Ref) + ---------------------
 *                       R * T * ln(10)
 *
 *
 * pH(x) : Desired pH of unknown solution
 * pH(Ref) : pH of the reference solution (used for calibration)
 * E(Ref) : Electric potential (mV) of the reference solution (used for calibration)
 * E(x) : Electric potential (mV) of the unknown solution
 * F : Faraday constant
 * R : Universal Gas constant
 * T : temperation of the solution in Kelvin
 *
 *
 * References : https://www.ti.com/lit/an/snoa529a/snoa529a.pdf
 *              https://www.hach.com/asset-get.download.jsa?id=17525673904
 *
 */

typedef struct
{
  float cal_temp;
  float cal_ph_buffer;
  float cal_mv_value;
} calibration_t;

typedef uint8_t pool_reader_address_t[8];

class PoolReaderClient
{

private:
  bool hasAddr = false;
  OneWire *ow = NULL;

  //Default theorical values
  calibration_t calibrationData = {25, 6.86, 502};

  bool readBus(uint8_t *dest, uint8_t len);

protected:
  pool_reader_register_t state;
  pool_reader_address_t currentaddr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

public:
  static constexpr uint8_t family_code{0xB7};

  PoolReaderClient(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7); //Constructor for Hub usage
  PoolReaderClient(OneWire *ow, pool_reader_address_t addr);                                                   //Constructor to address single device on pin
  PoolReaderClient(OneWire *ow);                                                                               // Default constructor. Use if only one device
  void begin();
  bool read(); //Read register on the bus

  uint16_t getTemperatureRaw() const;
  uint16_t getPhRaw() const;
  uint16_t getOrpRaw() const;
  uint8_t getWaterLevelRaw() const;

  float getTemperature() const;
  float getPh() const;
  float getPh(float currentTemperature) const;
  float getWaterLevel() const;
  int16_t getOrp() const;

  void setCalibrationValue(float temperature, float bufferValue, uint16_t adcValue);

  uint8_t getSampleInterval() const;
  bool writeSampleInterval(uint8_t interval);
};

#endif
