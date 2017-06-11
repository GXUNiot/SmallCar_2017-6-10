// SR04.h

#ifndef _SR04_h
#define _SR04_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SR04 {
public:
	SR04();
	void begin(int TP = 2, int EP = 3);
	float GetDistance();
	float GetDistance(float temp);
  float Speed();
private:
	int Trig_pin;
	int Echo_pin;
	float distance;

	float distance1;
	float distance2 = 0;
	uint64_t timeout = 0;
	float speed;
	float distance_temp;
	uint64_t timetemp = 0;
};

#endif

