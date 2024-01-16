#include "humidity_sensor.h"
#include "analog.h"






int16_t HumiditySensor_Convert(uint8_t type, float volt)
{
	uint16_t percHumidity = 0;
  
	// Partitore sull'ingresso
	volt = volt * 3.564;

	switch (type)
	{
		default:
		case (HUMIDITY_SENSOR_TYPE_EE10_ELEKTRONIK):
		
			percHumidity = (int16_t)(volt * 10.0);	 // 0..10V -> 0..100%
		
		break;
  
	}
	
	if (percHumidity > 100)
		percHumidity = 100;


	return percHumidity;
}



