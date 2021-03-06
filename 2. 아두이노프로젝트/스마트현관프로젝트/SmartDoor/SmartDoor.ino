#include <Arduino.h>

typedef struct weatherObj {
  String temp;
  String weather;
}myWeather;

#include <TM1637Display.h>
#include "LedControlMS.h"
#include <ESP8266WiFi.h>
#define NBR_MTX 4
#define CLK 4
#define DIO 5
#define ONE 0x06
#define TWO 0x5B
#define THREE 0x4F
#define FOUR 0x66
#define FIVE 0x6D
#define SIX 0x7D
#define SEVEN 0x27
#define EIGHT 0x7F
#define NINE 0x67
#define ZERO 0x3F
#define NONE 0x00



LedControl lc=LedControl(14,0,2, NBR_MTX);
LedControl lc2=LedControl(15,12,13, NBR_MTX);
TM1637Display display(CLK, DIO);

void setMyLed(LedControl lc ,int loc, char ch, bool flag);
void setMyLedTemp(LedControl lc ,int loc, char ch, bool flag);
String getTime();
myWeather getMyWeather();
void setTM1637(char num1, char num2);

unsigned long delaytime=1000;
const char* ssid     = "2.4G";
const char* password = "01093106215";
void setup() {
  Serial.begin(115200);
  for (int i=0; i<NBR_MTX; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
    lc2.shutdown(i, false);
    lc2.setIntensity(i, 8);
    lc2.clearDisplay(i);
  }


   // AP 연결
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    // put your main code here, to run repeatedly:
    
    Serial.println(getTime());
    String curTime=getTime().substring(0,25);
    Serial.println(curTime);
    String DayOfWeek = curTime.substring(0,3);
    String Day = curTime.substring(5,7);
    String Month = curTime.substring(8,11);
    String Year = curTime.substring(12,16);
    String rcv_hour = curTime.substring(17,19);
    int tmp = rcv_hour.toInt();
    tmp += 9; //구글의 시간대여서 +9시간 추가해줘야한다.
    String hour(tmp%24);
    String minu = curTime.substring(20,22);
    String second = curTime.substring(23,25);
    char hourCh[2], minCh[2], secCh[2];
    for (int i=0; i<2; i++) {
      hourCh[i] = hour.charAt(i);
      minCh[i] = minu.charAt(i);
      secCh[i] = second.charAt(i);
    }
    Serial.print(hourCh[1]);
    Serial.print(hourCh[1]);
    Serial.print(hourCh[1]);Serial.print(hourCh[1]);
    Serial.print(hourCh[1]);
    Serial.print(hourCh[1]);
    
    if(hourCh[1]==' ') { //  이거 스페이스도 아니고 뭐냐..??
      hourCh[1] = hour[0];
      hourCh[0] = '0';
    }
    setMyLed(lc ,3, hourCh[0], true);
    setMyLed(lc ,2, hourCh[1], true);
    setMyLed(lc ,1, minCh[0], true);
    setMyLed(lc ,0, minCh[1], true);
  
    delay(delaytime);
  
    setMyLed(lc ,3, hourCh[0], false);
    setMyLed(lc ,2, hourCh[1], false);
    setMyLed(lc ,1, minCh[0], false);
    setMyLed(lc ,0, minCh[1], false);
  
    myWeather mw;
    mw = getMyWeather();
    //만약 영하라면 영하플래그 ON
    //도 플래그 ON
    int val1, val2;
    if(mw.temp.charAt(2+3)) {
      setMyLedTemp(lc2, 2, mw.temp.charAt(0), true);
      setMyLedTemp(lc2, 1, mw.temp.charAt(1), true);
      setMyLedTemp(lc2, 0, mw.temp.charAt(2), true);
      val1 = mw.temp.charAt(4);
      val2 = mw.temp.charAt(5);
    }


    
    else if(mw.temp.charAt(1+3)) {
      setMyLedTemp(lc2, 1, mw.temp.charAt(0), true);
      setMyLedTemp(lc2, 0, mw.temp.charAt(1), true);
      Serial.print(mw.temp.charAt(1));
      val1 = mw.temp.charAt(3);
      val2 = mw.temp.charAt(4);
      
    } else {
      setMyLedTemp(lc2, 0, mw.temp.charAt(0), true);
      val1 = mw.temp.charAt(2);
      val2 = mw.temp.charAt(3);
    }
    
    setMyLed(lc2, 3, 'h', true);
    //setMyLed(lc2, 2, 'h', true);
    display.setBrightness(0x0f);
    setTM1637(val1,val2);
}
