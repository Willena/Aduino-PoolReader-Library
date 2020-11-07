
#include <OneWireHub.h>
#include <OneWireHub_config.h>
#include <OneWireItem.h>
#include <platform.h>
#include <PoolReaderDevice.h>

#define pin_onewire 8

auto hub = OneWireHub(pin_onewire);
auto board = PoolReaderDevice(PoolReaderDevice::family_code, 0x00, 0x00, 0xB2, 0xDA, 0x18, 0x00);

void setup()
{
  Serial.begin(115200);
  hub.attach(board);
  Serial.println("Setup OK");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // following function must be called periodically
  hub.poll();
  // this part is just for debugging (USE_SERIAL_DEBUG in OneWire.h must be enabled for output)
  if (hub.hasError())
  {
    auto _error = hub.getError();
    if (_error == Error::NO_ERROR)
      return;
    Serial.println("Error !  ");
  }

  // Values can be taken from analogRead, another sensor or coded
  board.setTemperature(12.3);
  board.setPh(10.36);
  board.setOrp(1534);
  board.setWaterLevel(10.3);
}