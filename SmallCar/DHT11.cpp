// 
// 
// 

#include "DHT11.h"

// Return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT

DHT11::DHT11()
{}

void DHT11::begin(int dht11_pin) {
	Dht11_pin = dht11_pin;
}

int DHT11::read()
{
	int pin = Dht11_pin;
	// 缓冲接收
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// 空闲缓冲区
	for (int i = 0; i< 5; i++) bits[i] = 0;

	// 请求样品
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// 确认或超时
	unsigned int loopCnt = 10000;
	while (digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	loopCnt = 10000;
	while (digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i = 0; i<40; i++)
	{
		loopCnt = 10000;
		while (digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		loopCnt = 10000;
		while (digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // 重新启动 MSB
			idx++;      // 下一个字节
		}
		else cnt--;
	}

	// 写到正确的版本
	// 因为位[1]和位[3]总是为零，所以在公式中省略。
	humidity = bits[0];
	temperature = bits[2];

	uint8_t sum = bits[0] + bits[2];

	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;
}

float DHT11::GetHumidity()
{
	return (float)humidity;
}
float DHT11::GetTemp()
{
	return (float)temperature;
}
//
// 文件结尾
//

