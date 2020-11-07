#include <OneWire.h>
#include <PoolReaderClient.h>

#define ONEWIRE_PIN 4

pool_reader_address_t addr = {0xB7, 0x00, 0x00, 0xB2, 0xDA, 0x18, 0x00, 0x4F};
OneWire ow(ONEWIRE_PIN);

PoolReaderClient reader(&ow, addr);
//PoolReader reader(&ow);

void setup()
{
  Serial.begin(115200);
  Serial.println("//\n// Start PoolReaderClient \n//");

  Serial.println("Show Values");
  reader.begin();
  reader.setCalibrationValue(25.0, 7.0, 509);
}

void loop()
{

  if (!reader.read())
  {
    Serial.println("Error while reading");
    delay(2000);
    return;
  }

  Serial.println("Temp: " + String(reader.getTemperature()));
  Serial.println("Ph  : " + String(reader.getPh()));
  Serial.println("Ph Raw : " + String(reader.getPhRaw()));
  Serial.println("WL  : " + String(reader.getWaterLevel()));
  Serial.println("ORP : " + String(reader.getOrp()));
  Serial.println("ORP Raw: " + String(reader.getOrpRaw()));
  Serial.println("Interval : " + String(reader.getSampleInterval()));

  delay(2000);
}