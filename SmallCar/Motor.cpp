// 
// 
// 

#include "Motor.h"

Motor::Motor() {
};

void Motor::begin(	int Before_Left_A_pin,
				int Before_Left_B_pin,
				int Before_Right_A_pin,
				int Before_Right_B_pin,
				int Rear_Left_A_pin,
				int Rear_Left_B_pin,
				int Rear_Right_A_pin,
				int Rear_Right_B_pin ) {
	Before_Left_A = Before_Left_A_pin;
	Before_Left_B = Before_Left_B_pin;
	Before_Right_A = Before_Right_A_pin;
	Before_Right_B = Before_Right_B_pin;
	Rear_Left_A = Rear_Left_A_pin;
	Rear_Left_B = Rear_Left_B_pin;
	Rear_Right_A = Rear_Right_A_pin;
	Rear_Right_B = Rear_Right_B_pin;


	pinMode(Rear_Left_A, OUTPUT);
	pinMode(Rear_Left_B, OUTPUT);
	pinMode(Rear_Right_A, OUTPUT);
	pinMode(Rear_Right_B, OUTPUT);
	pinMode(Before_Left_A, OUTPUT);
	pinMode(Before_Left_B, OUTPUT);
	pinMode(Before_Right_A, OUTPUT);
	pinMode(Before_Right_B, OUTPUT);

}

void Motor::Go_Straight() {
	digitalWrite(Rear_Left_A, LOW);
	digitalWrite(Rear_Left_B, HIGH);
	digitalWrite(Rear_Right_A, LOW);
	digitalWrite(Rear_Right_B, HIGH);
	digitalWrite(Before_Left_A, LOW);
	digitalWrite(Before_Left_B, HIGH);
	digitalWrite(Before_Right_A, LOW);
	digitalWrite(Before_Right_B, HIGH);
}

void Motor::Go_Back() {
	digitalWrite(Rear_Left_A, HIGH);
	digitalWrite(Rear_Left_B, LOW);
	digitalWrite(Rear_Right_A, HIGH);
	digitalWrite(Rear_Right_B, LOW);
	digitalWrite(Before_Left_A, HIGH);
	digitalWrite(Before_Left_B, LOW);
	digitalWrite(Before_Right_A, HIGH);
	digitalWrite(Before_Right_B, LOW);

}

void Motor::Go_Left() {
	digitalWrite(Rear_Right_A, LOW);	//右后轮——前
	digitalWrite(Rear_Right_B, HIGH);
	digitalWrite(Before_Left_A, LOW);	//右前轮——前
	digitalWrite(Before_Left_B, HIGH);	
	digitalWrite(Rear_Left_A, HIGH);	//左后轮——后
	digitalWrite(Rear_Left_B, LOW);
	digitalWrite(Before_Right_A, HIGH);	//左前轮——后
	digitalWrite(Before_Right_B, LOW);
}

void Motor::Go_Right() {
	digitalWrite(Before_Right_A, LOW);	//左前轮——前
	digitalWrite(Before_Right_B, HIGH);
	digitalWrite(Before_Right_A, LOW);	//左前轮——前
	digitalWrite(Before_Right_B, HIGH);

	digitalWrite(Rear_Right_A, HIGH);	//右后轮——后
	digitalWrite(Rear_Right_B, LOW);
	digitalWrite(Before_Left_A, HIGH);	//右前轮——后
	digitalWrite(Before_Left_B, LOW);
}

void Motor::Stop() {
	digitalWrite(Before_Left_A, LOW);
	digitalWrite(Before_Left_B, LOW);
	digitalWrite(Before_Right_A, LOW);
	digitalWrite(Before_Right_B, LOW);
	digitalWrite(Rear_Left_A, LOW);
	digitalWrite(Rear_Left_B, LOW);
	digitalWrite(Rear_Right_A, LOW);
	digitalWrite(Rear_Right_B, LOW);
}

void Motor::test() {
	
	//digitalWrite(Rear_Left_A, LOW);
	//digitalWrite(Rear_Left_B, HIGH);	//左后轮——前

	//digitalWrite(Rear_Left_A, HIGH);	//左后轮——后
	//digitalWrite(Rear_Left_B, LOW);

	//digitalWrite(Rear_Right_A, LOW);	//右后轮——前
	//digitalWrite(Rear_Right_B, HIGH);

	//digitalWrite(Rear_Right_A, HIGH);	//右后轮——后
	//digitalWrite(Rear_Right_B, LOW);

	//digitalWrite(Before_Left_A, LOW);	//右前轮——前
	//digitalWrite(Before_Left_B, HIGH);	

	//digitalWrite(Before_Left_A, HIGH);	//右前轮——后
	//digitalWrite(Before_Left_B, LOW);

	//digitalWrite(Before_Right_A, LOW);	//左前轮——前
	//digitalWrite(Before_Right_B, HIGH);

	//digitalWrite(Before_Right_A, HIGH);	//左前轮——后
	//digitalWrite(Before_Right_B, LOW);
}