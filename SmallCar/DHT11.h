// DHT11.h

#ifndef _DHT11_h
#define _DHT11_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK        0
#define DHTLIB_ERROR_CHECKSUM -1
#define DHTLIB_ERROR_TIMEOUT  -2

//默认使用 3 脚作为输入

class DHT11
{
public:
	DHT11();
	void begin(int dht11_pin = 12);
	int read();
	float GetHumidity();
	float GetTemp();
private:
	int Dht11_pin;
	int humidity;
	int temperature;
};
#endif

