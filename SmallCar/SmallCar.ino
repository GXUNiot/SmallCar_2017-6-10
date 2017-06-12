
#include "Blink.h"
#include "Motor.h"
#include "SR04.h"
#include "DHT11.h"
//#include <Servo.h>
#include <LiquidCrystal.h>

#include <avr/wdt.h>

/****************************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 13

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second

/*****************************************************/

SR04 sr04;	//超声波
float SR04_Distance;

DHT11 dht11;//温湿度传感器 //使用默认引脚时无需（）
float Temp, humidity;

Blink blink;//软件定时闪烁

Motor motor;//马达

uint16_t Steering_Angle = 300;//大概90度

//Servo Duoji;//旋转舵机

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);//LCD1602

enum Runing_Mode{Auto,SemiAuto,Manual}Runmode;//枚举类型 用于小车控制模式选择 自动/半自动/手动 初始化为自动

enum Detection_Distance{near,far}Detectiondistance;//枚举类型 用于选择距离标尺 近模式/远模式 初始化为近模式

const unsigned int DistanceStandard[2][3] = {	{2,50,100},	//距离标尺 近模式
												{20,200,300}	//远模式，即题目要求距离
												};

unsigned char ColorArray[NUMPIXELS][3] = {	//灯带 颜色数组【灯序号】【G/R/B值】 改变灯颜色必须是修改改数组元素
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
	{ 0,0,0 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 } };

void setup()
{
	wdt_enable(WDTO_2S); //开启看门狗，并设置溢出时间为两秒
	Serial.begin(9600); // 初始化串口通信
	while (!Serial);	//等待串口初始化完成

	sr04.begin();	//超声波启动
	dht11.begin();	//D温湿度启动
	blink.begin();	//软件闪烁启动
	motor.begin();	//马达驱动启动

	
	

	

#if defined (__AVR_ATtiny85__)
	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
	pixels.begin(); //这将初始化NeoPixel库
	for (int i = 0; i < NUMPIXELS; i++) {
		//pixels.Color获取RGB值，从0,0,0到255,255,255
		pixels.setPixelColor(i, pixels.Color(150, 0, 150)); 
		pixels.show(); //将更新的像素颜色发送到硬件。
		delay(delayval); //延迟一段时间（以毫秒为单位）。
	}
	lcd.begin(16, 2);//LCD尺寸为 16*2
	lcd.print("System Ready!"); //输出到LCD显示
	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("Auto");

	lcd.setCursor(0, 1);
	lcd.print("^");
}

void loop()
{
	blink.run();	//软件闪烁默认要有这一句
	wdt_reset(); //喂狗操作，使看门狗定时器复位
	char receive;
	if(Serial.available()>0){//实时检测串口数据

        receive = Serial.read();
		if( receive =='A' ){	//切换控制模式指令
			if(Runmode == Auto) {
				Runmode = SemiAuto;
				lcd.setCursor(0, 0);
				lcd.print("SemiAuto");
			}	
			else if(Runmode == SemiAuto) {
				Runmode = Manual;
				lcd.setCursor(0, 0);
				lcd.print("Manual  ");
			}	
			else{
				Runmode = Auto;
				lcd.setCursor(0, 0);
				lcd.print("Auto    ");
			}
				
		}
		if( receive =='D' ){	//切换距离标尺指令
			lcd.setCursor(0, 1);
			if (Detectiondistance == near) {
				Detectiondistance = far;
				lcd.print(" ");
			}
			else {
				Detectiondistance = near;
				lcd.print("^");
			}
				
		}
		
		if( Runmode == Auto ){

			if (receive == 'G' ) lcd.print("Auto");
			if (receive == 'L' ) lcd.print("Auto");
			if (receive == 'S' ) lcd.print("Auto");
			if (receive == 'R' ) lcd.print("Auto");
			if (receive == 'B' ) lcd.print("Auto");
		}
		else if( (Runmode == SemiAuto ) ){
			if (receive == 'G' ) motor.Go_Straight();
			if (receive == 'L' ) motor.Go_Left();
			if (receive == 'S' ) motor.Stop();
			if (receive == 'R' ) motor.Go_Right();
			if (receive == 'B' ) motor.Go_Back();
		}
		else {
			if (receive == 'G') motor.Go_Straight();
			if (receive == 'L') motor.Go_Left();
			if (receive == 'S') motor.Stop();
			if (receive == 'R') motor.Go_Right();
			if (receive == 'B') motor.Go_Back();
			if (receive == 'F') motor.Stop();
		}
        
	}
	lcd.setCursor(8, 0);

	if( Runmode == Manual ){	//手动模式，产生距离报警，但不会控制停止
		if( Detectiondistance == near ){//距离境界标尺为 近模式
			if( (sr04.GetDistance() <= DistanceStandard[0][1])&& (sr04.GetDistance() >= DistanceStandard[0][0])){//近

				WS2812_Red();
				lcd.print("Waring  ");
			}
			else if( sr04.GetDistance() >= DistanceStandard[0][2] ){//远
				

				WS2812_Green();
				lcd.print("Security");
			}
			else{//中
				
				WS2812_Yellow();
				lcd.print("Just Go ");
			}			
			
		}else{//距离境界标尺为 远模式 即题目要求距离
			if ((sr04.GetDistance() <= DistanceStandard[0][1]) && (sr04.GetDistance() >= DistanceStandard[0][0])) {//近

				

				WS2812_Red();
				lcd.print("Waring  ");
			}
			else if( sr04.GetDistance() >= DistanceStandard[1][2] ){//远
				

				WS2812_Green();
				lcd.print("Security");
			}
			else{//中
				
				WS2812_Yellow();
				lcd.print("Just Go ");
			}	
		}
	}
	else if ( Runmode == Auto) {//自动，自动不允许接受蓝牙控制，仅运行在安全距离内；
		if (Detectiondistance == near) {//距离境界标尺为 近模式
			if ((sr04.GetDistance() <= DistanceStandard[0][1]) && (sr04.GetDistance() >= DistanceStandard[0][0])) {//近用近距离标尺
				WS2812_Red();
				lcd.print("Waring  ");

				motor.Stop();	//停车
				motor.Go_Left();	//转向	
				delay(Steering_Angle);
				motor.Stop();

				
				
			}
			else if (sr04.GetDistance() >= DistanceStandard[0][2]) {//远 用近距离标尺
				motor.Go_Straight();	//直行

				WS2812_Green();
				lcd.print("Security");
			}
			else {													//中 用近距离标尺
				motor.Go_Straight();	//直行

				WS2812_Yellow();
				lcd.print("Just Go ");
			}

		}
		else {//距离境界标尺为 远模式 即题目要求距离
			if ((sr04.GetDistance() <= DistanceStandard[1][1]) && (sr04.GetDistance() >= DistanceStandard[1][0])) {//近采用远距离标尺
				WS2812_Red();
				lcd.print("Waring  ");

				motor.Stop();	//停车
				motor.Go_Left();	//转向	
				delay(Steering_Angle);
				motor.Stop();
			}
			else if (sr04.GetDistance() >= DistanceStandard[1][2]) {//远 采用远距离标尺
				motor.Go_Straight();	//直行


				WS2812_Green();
				lcd.print("Security");
			}
			else {													//中 采用远距离标尺
				motor.Go_Straight();	//直行

				WS2812_Yellow();
				lcd.print("Waring ");
			}
		}
	}
    else{	//半自动模式，半自动模式允许蓝牙控制，但不允许超过距离警戒
		if( Detectiondistance == near ){//距离境界标尺为 近模式
			if ((sr04.GetDistance() <= DistanceStandard[0][1]) && (sr04.GetDistance() >= DistanceStandard[0][0])) {//近 用近距离标尺
				WS2812_Red();
				lcd.print("Waring  ");
				
				motor.Stop();	//停车
				motor.Go_Left();	//转向	
				delay(Steering_Angle);
				motor.Stop();
			}
			else if( sr04.GetDistance() >= DistanceStandard[0][2] ){//远 用近距离标尺
				//motor.Go_Straight();	//直行
				
				WS2812_Green();
				lcd.print("Security");
			}
			else{													//中 用近距离标尺
				//motor.Go_Straight();	//直行
				

				WS2812_Yellow();
				lcd.print("Just Go ");
			}			
			
		}else{//距离境界标尺为 远模式 即题目要求距离
			if ((sr04.GetDistance() <= DistanceStandard[1][1]) && (sr04.GetDistance() >= DistanceStandard[1][0])) {//近 采用远距离标尺
				WS2812_Red();
				lcd.print("Waring  ");
				
				motor.Stop();	//停车
				motor.Go_Left();	//转向	
				delay(Steering_Angle);
				motor.Stop();
			}
			else if( sr04.GetDistance() >= DistanceStandard[1][2] ){//远 采用远距离标尺
				//motor.Go_Straight();	//直行
				
				WS2812_Green();
				lcd.print("Security");
			}
			else{													//中 采用远距离标尺
				//motor.Go_Straight();	//直行
				WS2812_Yellow();
				
				lcd.print("Waring ");
			}	
		}
	}
	DHT11_Update();

	lcd.setCursor(1, 1);
	lcd.print("          "); lcd.print("          ");
	lcd.setCursor(1, 1);

	if( ( Temp > 0 )&&( Temp < 50 ) ){		//有正常温度则使用温度补偿
		lcd.print( (sr04.GetDistance(Temp)/100.0));
	}	
	else
		lcd.print( (sr04.GetDistance()/100.0) );
	lcd.print("m");

	lcd.setCursor(7, 1);
	lcd.print(sr04.Speed());
	lcd.print("m/s");

	
	WS2812B_Display();
}



void WS2812B_Display(){
	for (int i = 0; i < NUMPIXELS; i++) {
		//pixels.Color获取RGB值，从0,0,0到255,255,255
		pixels.setPixelColor(i, pixels.Color(ColorArray[i][0], ColorArray[i][1], ColorArray[i][2])); 
		pixels.show(); //将更新的像素颜色发送到硬件。
		//delay(delayval); //延迟一段时间（以毫秒为单位）。
	}
}

void WS2812_Red() {
	//改变灯带显示数组里第 15 个（灯）的颜色
	

	ColorArray[7][0] = 255;//G
	ColorArray[7][1] = 0;//R
	ColorArray[7][2] = 0;//B

	ColorArray[8][0] = 255;//G
	ColorArray[8][1] = 0;//R
	ColorArray[8][2] = 0;//B

	ColorArray[14][0] = 255;//G
	ColorArray[14][1] = 0;//R
	ColorArray[14][2] = 0;//B

	ColorArray[15][0] = 255;//G
	ColorArray[15][1] = 0;//R
	ColorArray[15][2] = 0;//B
	WS2812B_Display();
}

void WS2812_Yellow() {
	//改变灯带显示数组里第 15 个（灯）的颜色


	ColorArray[7][0] = 255;//G
	ColorArray[7][1] = 255;//R
	ColorArray[7][2] = 0;//B

	ColorArray[8][0] = 255;//G
	ColorArray[8][1] = 255;//R
	ColorArray[8][2] = 0;//B

	ColorArray[14][0] = 255;//G
	ColorArray[14][1] = 255;//R
	ColorArray[14][2] = 0;//B

	ColorArray[15][0] = 255;//G
	ColorArray[15][1] = 255;//R
	ColorArray[15][2] = 0;//B
}

void WS2812_Green() {
	//改变灯带显示数组里第 15 个（灯）的颜色


	ColorArray[7][0] = 0;//G
	ColorArray[7][1] = 255;//R
	ColorArray[7][2] = 0;//B

	ColorArray[8][0] = 0;//G
	ColorArray[8][1] = 255;//R
	ColorArray[8][2] = 0;//B

	ColorArray[14][0] = 0;//G
	ColorArray[14][1] = 255;//R
	ColorArray[14][2] = 0;//B

	ColorArray[15][0] = 0;//G
	ColorArray[15][1] = 255;//R
	ColorArray[15][2] = 0;//B
}




void DHT11_Update() {	//更新温湿度数据
	int chk = dht11.read();
	switch (chk)
	{
	case DHTLIB_OK:
		break;
	case DHTLIB_ERROR_CHECKSUM:
		Serial.println("DHT11--Checksum error");
		break;
	case DHTLIB_ERROR_TIMEOUT:
		Serial.println("DHT11--Time out error");
		break;
	default:
		Serial.println("DHT11--Unknown error");
		break;
	}
	Temp = dht11.GetTemp();
	humidity = dht11.GetHumidity();
}