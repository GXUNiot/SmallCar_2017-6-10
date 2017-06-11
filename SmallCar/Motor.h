// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Motor {
public:
	Motor();

	void begin(	int Before_Left_A = 4,
				int Before_Left_B = 5,
				int Before_Right_A = 7,
				int Before_Right_B = 6,
				int Rear_Left_A = 8,
				int Rear_Left_B = 9,
				int Rear_Right_A = 10,
				int Rear_Right_B = 11);

	void Go_Straight();//直走
	void Go_Back();
	void Go_Left();
	void Go_Right();
	void Stop();
	void test();

protected:
	int Before_Left_A = 4;
	int Before_Left_B = 5;
	int Before_Right_A = 7;
	int Before_Right_B = 6;
	int Rear_Left_A = 8;
	int Rear_Left_B = 9;
	int Rear_Right_A = 10;
	int Rear_Right_B = 11;

};

#endif

