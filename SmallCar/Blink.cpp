// 
// 
// 

#include "Blink.h"

void Blink::begin(uint64_t timeout) { //采用默认参数时，仅声明处能填写默认参数，定义函数是不能再填写默认参数
	timeOut = timeout;
	pinMode(LED_BUILTIN, OUTPUT);
}

void Blink::run() {
	if (millis() - timeCnt >= timeOut) {

		if(digitalRead(LED_BUILTIN))
			digitalWrite(LED_BUILTIN, LOW);
		else       
			digitalWrite(LED_BUILTIN, HIGH);
		timeCnt = millis();

		
	}
}
