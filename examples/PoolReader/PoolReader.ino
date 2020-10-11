#include <OneWire.h>
#include <PoolReader.h>

pool_reader_address_t addr = {0xB7, 0x00, 0x00, 0xB2, 0xDA, 0x18, 0x00, 0x4F};
OneWire ow(4);

// If you know the adresse of the device or if you have multiple device of the same family on the bus
//PoolReader reader(&ow, addr);

//Use auto detection 
PoolReader reader(&ow);

void setup() {
  Serial.begin(115200);
  reader.begin();
}

void loop() {

  if (!reader.read())
  {
    Serial.println("Error while reading");
    delay(2000);
    return;
  }

  Serial.println("Temp: " + String(reader.getTemperature()) );
  Serial.println("Ph  : " + String(reader.getPh()));
  Serial.println("WL  : " + String(reader.getWaterLevel()));
  Serial.println("ORP : " + String(reader.getOrp()));
  Serial.println("Interval : " + String(reader.getSampleInterval()));

  delay(2000);
  
}