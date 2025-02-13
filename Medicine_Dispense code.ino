#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>
#include "RTClib.h"
#include <Keypad.h>
#include "DHT.h"
#include <ESP32_Servo.h>
#define REMOTEXY_MODE__WIFI_CLOUD

#include <WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "Delicious Koffee"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_CLOUD_TOKEN "0890beace39fb2acd01bc439c1bf8dce"


#include <RemoteXY.h>

Servo myservo;
int servoPin = 4;
int hbPin = 39;
#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int time1 = 0, time2 = 0;
int pulse = 0;
long timecounter = 0;
#define debug 0


LiquidCrystal_I2C lcd(0x27, 16, 2);
int Hr_Start_Set[8] = {0};
int Min_Start_Set[8] = {0};
int Sec_Start_Set[8] = {0};

int Hr_Stop_Set[8] = {0};
int Min_Stop_Set[8] = {0};
int Sec_Stop_Set[8] = {0};

int TotalMin[8] = {0};

char key1;
String STR = "", buff = "";
int Counter = 0;
int diffmin = 0;

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {13, 26, 14, 27}; //connect to the row pinouts of the keypad
byte colPins[4] = {12, 25, 33, 32}; //connect to the column pinouts of the keypad

const int buzz  = 2;

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

RTC_DS1307 rtc;



char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int Hr, Min, Sec, Dt, Mn, Yr, MinHold = 0, Prev_Min = 0;
//int Hr_set = 0, Min_set = 0, Sec_set = 0, Hr_set1 = 0, Min_set1 = 0, Sec_set1 = 0, TotalMin = 0;
bool OneTimeFlag = 0, OneTimeFlag1 = 0;

void SETtime();

void LCDDISP(String R1, String R2, int D)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(R1);
  lcd.setCursor(0, 1);
  lcd.print(R2);
  delay(500);
}



void SETtime()
{
  int ADDRCount = 0;
  for (int i = 0; i <= 1; i++)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET " + String(i) + " START TIME");
    delay(100);
    while (1)
    {
      key1 = keypad.getKey();
      if (key1)
      {
        digitalWrite(buzz, HIGH);
        delay(100);
        digitalWrite(buzz, LOW);
        if (key1 == 'A')
        {
          delay(100);
          EEPROM.write(ADDRCount, Hr_Start_Set[i]);
          EEPROM.commit();
          ++ADDRCount;
          break;
        }

        if (key1 == 'C')STR = "";

        STR += key1;
        Hr_Start_Set[i] = STR.toInt();
      }
      lcd.setCursor(0, 1);
      lcd.print("                ");
      buff = String(Hr_Start_Set[i]) + ": " + String(Min_Start_Set[i]) + ": " + String(Sec_Start_Set[i]);
      lcd.setCursor(0, 1);
      lcd.print(buff);
      lcd.print(" Hr");
      delay(250);
    }
    STR = "";
    while (1)
    {
      key1 = keypad.getKey();
      if (key1)
      {
        digitalWrite(buzz, HIGH);
        delay(100);
        digitalWrite(buzz, LOW);
        if (key1 == 'A')
        {
          delay(100);
          EEPROM.write(ADDRCount, Min_Start_Set[i]);
          ++ADDRCount;
          EEPROM.commit();
          break;
        }

        if (key1 == 'C')STR = "";

        STR += key1;
        Min_Start_Set[i] = STR.toInt();
      }
      lcd.setCursor(0, 1);
      lcd.print("                ");
      buff = String(Hr_Start_Set[i]) + ": " + String(Min_Start_Set[i]) + ": " + String(Sec_Start_Set[i]);
      lcd.setCursor(0, 1);
      lcd.print(buff);
      lcd.print(" Min");
      delay(250);
    }
    STR = "";
    while (1)
    {
      key1 = keypad.getKey();
      if (key1)
      {
        digitalWrite(buzz, HIGH);
        delay(100);
        digitalWrite(buzz, LOW);
        if (key1 == 'A')
        {
          delay(100);
          EEPROM.write(ADDRCount, Sec_Start_Set[i]);
          ++ADDRCount;
          EEPROM.commit();
          break;
        }
        if (key1 == 'C')STR = "";

        STR += key1;
        Sec_Start_Set[i] = STR.toInt();
      }
      lcd.setCursor(0, 1);
      lcd.print("                ");
      buff = String(Hr_Start_Set[i]) + ": " + String(Min_Start_Set[i]) + ": " + String(Sec_Start_Set[i]);
      lcd.setCursor(0, 1);
      lcd.print(buff);
      lcd.print(" Sec");
      delay(250);

    }
  }
  STR = "";
  int ADDR = 0;
  for (int k = 0; k <= 1; k++)
  
  OneTimeFlag = 0;
  OneTimeFlag1 = 0;
}


void ComapreFUN()
{
  if ((Hr == Hr_Start_Set[0]) && (Min == Min_Start_Set[0]))
  {
    if (OneTimeFlag == 0)
    {
      OneTimeFlag = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite(buzz, HIGH);
      lcd.print("Time Matched");
      delay(2000);
      digitalWrite(buzz, LOW);
      Serial.println("Time Matched");
      time1 = 1;
    }
  }
  else if ((Hr == Hr_Start_Set[1]) && (Min == Min_Start_Set[1]))
  {
    if (OneTimeFlag1 == 0)
    {
      OneTimeFlag1 = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite(buzz, HIGH);
      lcd.print("Time Matched");
      delay(2000);
      digitalWrite(buzz, LOW);
      Serial.println("Time Matched");
      time2 = 1;
    }
  }
  else
  {
    OneTimeFlag = 0;
    OneTimeFlag1 = 0;
  }
}

//void IRAM_ATTR isr()
//{
//  detachInterrupt(hbPin);
//  pulse++;
//  attachInterrupt(hbPin, isr, RISING);
//}

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 80 bytes
{ 255, 0, 0, 22, 0, 73, 0, 19, 0, 0, 0, 0, 19, 1, 106, 200, 1, 1, 4, 0,
  67, 18, 49, 71, 17, 69, 2, 26, 11, 67, 18, 106, 71, 16, 69, 2, 26, 11, 129, 20,
  39, 65, 8, 64, 36, 66, 111, 100, 121, 32, 84, 101, 109, 112, 101, 114, 97, 116, 117, 114,
  101, 0, 129, 35, 97, 36, 8, 64, 36, 72, 101, 97, 114, 116, 32, 114, 97, 116, 101, 0
};

// this structure defines all the variables and events of your control interface
struct {

  // output variables
  char value_01[11]; // string UTF8 end zero
  char value_02[11]; // string UTF8 end zero

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

void DELAY(int D)
{
  for(int i = 0;i<D;i++)
  {
    delay(1);
    RemoteXY_Handler ();
  }
}

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(200);
  pinMode(buzz, OUTPUT);
  pinMode(hbPin, INPUT);
  digitalWrite(buzz, LOW);
  //attachInterrupt(hbPin, isr, RISING);
  myservo.attach(servoPin);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    WEL-COME    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(1000);
  int ADDRCount = 0;
  if (debug)
  {
    EEPROM.write(ADDRCount, 17);
    ++ADDRCount;
    EEPROM.commit();
    EEPROM.write(ADDRCount, 34);
    ++ADDRCount;
    EEPROM.commit();
    EEPROM.write(ADDRCount, 0);
    ++ADDRCount;
    EEPROM.commit();

    EEPROM.write(ADDRCount, 17);
    ++ADDRCount;
    EEPROM.commit();
    EEPROM.write(ADDRCount, 38);
    ++ADDRCount;
    EEPROM.commit();
    EEPROM.write(ADDRCount, 0);
    ++ADDRCount;
    EEPROM.commit();
  }
  int ADDR = 0;
  for (int k = 0; k <= 1; k++)
  {
    Hr_Start_Set[k]  = EEPROM.read(ADDR);
    ++ADDR;
    Min_Start_Set[k] = EEPROM.read(ADDR);
    ++ADDR;
    Sec_Start_Set[k] = EEPROM.read(ADDR);
    ++ADDR;
  }

  if (! rtc.begin())
  {
    Serial.print("Couldn't find RTC");
    //    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  dht.begin();
  RemoteXY_Init ();
  myservo.write(90);
}

void loop()
{
  RemoteXY_Handler ();
  buff = "";
  int t = dht.readTemperature();
  Serial.println("TEMP:" + String(t));
  DateTime now = rtc.now();
  Hr = now.hour();
  Min = now.minute();
  Sec = now.second();
  Serial.println("H:" + String(Hr) + ":M:" + String(Min) + ":S:" + String(Sec));

  //  lcd.clear();
  //  lcd.setCursor(3, 0);
  //  lcd.print(Hr);
  //  lcd.print(': ');
  //  lcd.print(Min);
  //  lcd.print(': ');
  //  lcd.print(Sec);
  if ((OneTimeFlag == 0) || (OneTimeFlag1 == 0))
  {
    lcd.clear();
    LCDDISP("  " + String(Hr) + ":" + String(Min) + ":" + String(Sec), "  ", 1);
    DELAY(100);
  }

  key1 = keypad.getKey();// Read the key
  Serial.println(key1);
  if (key1 == 'A')
  {
    digitalWrite(buzz, HIGH);
    delay(200);
    digitalWrite(buzz, LOW);
    SETtime();
  }

  ComapreFUN();
  if (time1 == 1)
  {
    OneTimeFlag1 = 0;
    LCDDISP("1st Tablet", "dispensing...!", 1000);
    for (int pos = 90; pos <= 180; pos += 1)
    {
      myservo.write(pos);
      delay(15);
    }
    DELAY(3000);
    for (int pos = 180; pos >= 90; pos -= 1)
    {
      myservo.write(pos);
      delay(15);
    }
    time1 = 0;
    LCDDISP("1st Tablet", "Dispensed.", 1000);
  }

  if (time2 == 1)
  {
    OneTimeFlag = 0;
    LCDDISP("2nd Tablet", "dispensing...!", 1000);
    for (int pos = 90; pos >= 0; pos -= 1)
    {
      myservo.write(pos);
      delay(15);
    }
    DELAY(3000);
    for (int pos = 0; pos <= 90; pos += 1)
    {
      myservo.write(pos);
      delay(15);
    }
    LCDDISP("2nd Tablet", "Dispensed.", 1000);
    time2 = 0;
  }
  ++timecounter;
  if (timecounter > 30)
  {
    timecounter = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heart rate");
    lcd.setCursor(0, 1);
    lcd.print("Reading Start");
    pulse = 0;
    int flag = 0;
    //  attachInterrupt(hbPin, isr, RISING);
    for (int i = 0; i < 500; i++)
    {
      while (digitalRead(hbPin) == 1)
      {
        flag = 1;
      }
      if (flag == 1)
      {
        flag = 0;
        pulse++;
      }
      DELAY(10);
    }
    //  detachInterrupt(hbPin);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heart rate");
    lcd.setCursor(0, 1);
    lcd.print("Reading Stop");
    DELAY(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heart rate:");
    lcd.setCursor(0, 1);
    pulse = pulse * 8;
    lcd.print(pulse);
    DELAY(3000);
    if(pulse>120)
    {
      digitalWrite(buzz, HIGH);
      delay(200);
      digitalWrite(buzz, LOW);
      delay(200);
      digitalWrite(buzz, HIGH);
      delay(200);
      digitalWrite(buzz, LOW);
      delay(200);
      digitalWrite(buzz, HIGH);
      delay(200);
      digitalWrite(buzz, LOW);
      delay(200);
      digitalWrite(buzz, HIGH);
      delay(200);
      digitalWrite(buzz, LOW);
      delay(200);
      digitalWrite(buzz, HIGH);
      delay(200);
      digitalWrite(buzz, LOW);
      delay(200);
    }
  }
  sprintf(RemoteXY.value_01, "%d", t);
  sprintf(RemoteXY.value_02, "%d", pulse);
}
