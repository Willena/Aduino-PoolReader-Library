/* Pool Reader library implementation 
 *  Author: Guillaume VILLENA
 *  Date: 10-2020
 */
#include "PoolReader.h"

PoolReader::PoolReader(OneWire * ow){
  this->ow = ow;
  hasAddr = false;
}

PoolReader::PoolReader(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7){
  currentaddr[0] = ID1;
  currentaddr[1] = ID2;
  currentaddr[2] = ID3;
  currentaddr[3] = ID4;
  currentaddr[4] = ID5;
  currentaddr[5] = ID6;
  currentaddr[6] = ID7;
  hasAddr = true;
}
 
PoolReader::PoolReader(OneWire * ow, pool_reader_address_t addr){
    memcpy(currentaddr, addr, 8); 
    this->ow = ow;
    hasAddr = true;
}

void PoolReader::begin(){
  if (ow != NULL){
    //Acting as client mode.
    
    if (!hasAddr){
      //If address not specified try to find the first one return false if could not find a device
	    pool_reader_address_t address;
		uint8_t count = 0;
		
		ow->reset_search();
		while(ow->search(address)){
			
			if (address[0] != PoolReader::family_code)
				continue;
			
			count++;

			Serial.println("Found one valid addr: ");
		    for (uint8_t i = 0; i < 8; i++)
		    {
		  	  Serial.print("0x");
		  	  if (address[i] < 0x10) Serial.print("0");
		  	  Serial.print(address[i], HEX);
		  	  Serial.print(" ");
		    }
			Serial.println();
		    
		}
		Serial.println();
		Serial.print(count);
		Serial.println(" Compatibles devices found !");
		memcpy(currentaddr, address, 8); 
		hasAddr = true;
    }
	
	
	Serial.print("Will be using the following device address :");
	for (uint8_t i = 0; i < 8; i++)
	{
	  Serial.print("0x");
	  if (currentaddr[i] < 0x10) Serial.print("0");
	  Serial.print(currentaddr[i], HEX);
	  Serial.print(" ");
	}
	Serial.println();
  }
}

bool PoolReader::readBus(uint8_t * dest, uint8_t len){
  if (ow == NULL) return false;
  
  for (int i = 0; i < len; i++) { 
    dest[i] = ow->read();
  }

  return true;
}


void PoolReader::setCalibrationValue(float temperature, float bufferValue, uint16_t adcValue){
  calibrationData.cal_temp = temperature;
  calibrationData.cal_ph_buffer = bufferValue;
  calibrationData.cal_mv_value = ((adcValue * ADC_STEP_V) / PROBE_AMP_GAIN) - PROBE_SWING_HALF;	
}

bool PoolReader::read(){
  
  if (!hasAddr && ow == NULL) return false;
  
  uint8_t * s = reinterpret_cast<uint8_t *>(&state);
  int b = ow->reset();
  if (b==0) return false;
  
  ow->select(currentaddr);
  ow->write(POOL_READER_COMMAND_READ);
  //readBus(s,sizeof(pool_reader_register_t));
  for (int i = 0; i < sizeof(pool_reader_register_t); i++) { 
    s[i] = ow->read();
  }

  Serial.print("buffer: ");
  for (int i =0; i< sizeof(pool_reader_register_t); i++){
	if (s[i] < 0x10) Serial.write('0');
    Serial.print(s[i], HEX);
	Serial.write(' ');
  }
  Serial.println();

  //Check CRC
  if (ow->crc8(s, sizeof(pool_reader_register_t)-1 ) != state.crc) { 
      Serial.println("Invalid CRC");
      return false;
  }
  return true;
}

bool PoolReader::writeSampleInterval(uint8_t interval){
  if (!hasAddr && ow == NULL) return false;

  if (interval > 255 || interval < 1) return false;
  
  int b = ow->reset();
  if (b==0) return false;
  
  ow->select(currentaddr);
  ow->write(POOL_READER_COMMAND_SAMPLE_TIME);
  ow->write(interval);
  
  if (ow->read() != POOL_READER_RESPONSE_OK) return false;
  
  b = ow->reset();  
  return (b==1);
}


uint16_t PoolReader::getTemperatureRaw() const {
  return state.T;
}

uint16_t PoolReader::getPhRaw() const{
 return state.PH ;
}

uint16_t PoolReader::getOrpRaw() const{
  return state.ORP;
}

uint8_t PoolReader::getWaterLevelRaw() const{
  return state.wLevel;
}

float PoolReader::getTemperature() const{
  return static_cast<int16_t>(getTemperatureRaw()) /16.0f;
}

float PoolReader::getPh() const{
	
  float pHmV = ((getPhRaw() * ADC_STEP_V) / PROBE_AMP_GAIN) - PROBE_SWING_HALF;
  float pH = calibrationData.cal_ph_buffer + (( ( calibrationData.cal_mv_value + pHmV ) * FARADAY_CST ) / ( GAS_CST * (ZERO_C_K*calibrationData.cal_temp)*log(10)));
	
  return pH;
}

float PoolReader::getWaterLevel() const{
  return getWaterLevelRaw() / 10.0f;
}

int16_t PoolReader::getOrp() const{
  return (getOrpRaw() / 0.256f) - 2000;
}

uint8_t PoolReader::getSampleInterval() const{
  return state.interval;
}

