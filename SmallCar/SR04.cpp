// 
// 
// 

#include "SR04.h"

SR04::SR04()
{}

void SR04::begin(int TP, int EP) { 
	pinMode(TP, OUTPUT);
	pinMode(EP, INPUT);
	Trig_pin = TP;
	Echo_pin = EP;
}

float SR04::GetDistance()
{
	
	while( millis() - timetemp <= 60){//当获取当前时间对比上次时间小于60ms
		;	//等待
	}
	timetemp = millis();//更新本次时间
	
	digitalWrite(Trig_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(Trig_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(Trig_pin, LOW);
	// 检测脉冲宽度，并计算出距离
	distance = pulseIn(Echo_pin, HIGH) / 58.00;//为什么除以58等于厘米，  Y米=（X秒*344）/2
												   // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
	distance1 = distance;
	return distance;
}

float SR04::GetDistance(float temp){ //改进为温度补偿模式 参数为温度
	while( millis() - timetemp <= 60){//当获取当前时间对比上次时间小于60ms
		;	//等待
	}
	timetemp = millis();//更新本次时间
	digitalWrite(Trig_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(Trig_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(Trig_pin, LOW);
	// 检测脉冲宽度，并计算出距离
	
	double v = temp/273;	//补偿公式 V=331*(1+T/273)^(1/2) 后面这个是1/2次方
	v = 1 + v;
	v = sqrt(v);
	v = 331*v;
	v = 2/v;
	distance = pulseIn(Echo_pin, HIGH) / 58.00;//为什么除以58等于厘米，  Y米=（X秒*344）/2
	distance1 = distance;							   // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
	return distance;
}

float SR04::Speed() {
	if(millis() - timeout < 1000) {//当获取当前时间对比上次时间小于500ms
		return 0;
	}

	timeout = millis() - timeout;//更新本次时间
	if (distance2 != 0) {
		speed = (distance1  - distance2 ) / (timeout/10);
	}
	Serial.println(speed);


	distance2 = distance1;
	timeout = millis();
	return speed;

}

