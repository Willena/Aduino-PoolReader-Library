
#include <OneWireHub.h>
#include <OneWireHub_config.h>
#include <OneWireItem.h>
#include <platform.h>
#include <pool_reader.h>
#include <pool_reader_device.h>


#define pin_onewire 8

auto hub    = OneWireHub(pin_onewire);
auto board = PoolReaderDevice(PoolReader::family_code, 0x00, 0x00, 0xB2, 0xDA, 0x18, 0x00);

void setup() {
  Serial.begin(115200);
  hub.attach(board);

  Serial.println("Self Tests ----- Temperature 0 °C");
  board.setTemperature(0);
  Serial.println("Temperature is " + String(board.getTemperature()));
  
  Serial.println("Self Tests ----- Temperature 4 °C");
  board.setTemperature(4);
  Serial.println("Temperature is " + String(board.getTemperature()));
  
  Serial.println("Self Tests ----- Temperature -21°C");
  board.setTemperature(-21.0f);
  Serial.println("Temperature is " + String(board.getTemperature()));

  Serial.println("Self Tests ----- Temperature -26.5°C");
  board.setTemperature(-26.5f);
  Serial.println("Temperature is " + String(board.getTemperature()));

  Serial.println("Self Tests ----- PH 0");
  board.setPh(0);
  Serial.println("Ph is " + String(board.getPh()));

  Serial.println("Self Tests ----- PH 8.63");
  board.setPh(8.63);
  Serial.println("Ph is " + String(board.getPh()));

  Serial.println("Self Tests ----- PH 14");
  board.setPh(14);
  Serial.println("Ph is " + String(board.getPh()));

  Serial.println("Self Tests ----- ORP 0");
  board.setOrp(0);
  Serial.println("Orp is " + String(board.getOrp()));

  Serial.println("Self Tests ----- ORP -2000");
  board.setOrp(-2000);
  Serial.println("Orp is " + String(board.getOrp()));


  Serial.println("Self Tests ----- ORP 2000");
  board.setOrp(2000);
  Serial.println("Orp is " + String(board.getOrp()));

  Serial.println("Self Tests ----- WLevel 25.4cm");
  board.setWaterLevel(25.4);
  Serial.println("WaterLevel is " + String(board.getWaterLevel()));

  Serial.println("Self Tests ----- WLevel 0.2cm");
  board.setWaterLevel(0.2);
  Serial.println("WaterLevel is " + String(board.getWaterLevel()));
  
  Serial.println("Self Tests ----- WLevel 17.53cm");
  board.setWaterLevel(17.53);
  Serial.println("WaterLevel is " + String(board.getWaterLevel()));

  Serial.println("Done! ");


  Serial.println("Sizeof struct" + String(sizeof(pool_reader_register_t)));
  Serial.println("WaterLevel is " + String(board.getWaterLevel()));
  Serial.println("Orp is " + String(board.getOrp()));
  Serial.println("Ph is " + String(board.getPh()));
  Serial.println("Temperature is " + String(board.getTemperature()));

  
}

void loop() {
  // put your main code here, to run repeatedly:
  // following function must be called periodically
    hub.poll();
    // this part is just for debugging (USE_SERIAL_DEBUG in OneWire.h must be enabled for output)
    if (hub.hasError()){
      auto _error = hub.getError();  
      if (_error == Error::NO_ERROR) return; 
      Serial.println("Error !  ");
    }
}