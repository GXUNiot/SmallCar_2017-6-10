// Blink.h

#ifndef _BLINK_h
#define _BLINK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Blink
{
public:
	
	void begin(uint64_t timeout = 500);
	void run(void);
private:
	uint64_t timeOut = 0;
	uint64_t timeCnt = 0;
};


#endif

