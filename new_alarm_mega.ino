//test

#include <TTP229.h>
#include <uRTCLib.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <DHT_U.h>
#include <FastLED_NeoPixel.h>
#include <Adafruit_Fingerprint.h>
#include "Arduino.h"
#include "uRTCLib.h"
uRTCLib rtc;


#define RXPIN 15                        // change this to whatever  // RXPIN is IN from sensor (GREEN wire)
#define TXPIN 14                        // change this to whatever  // TXPIN is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(RXPIN, TXPIN);  // Set up the serial port to use softwareserial..
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define lazer 6

#define DATA_PIN A6
#define NUM_LEDS 8
#define DATA_PIN2 13
#define DATA_PIN3 A7
#define NUM_LEDS3 31
#define NUM_LEDS2 4
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB);
Adafruit_NeoPixel strip2(NUM_LEDS2, DATA_PIN2, NEO_GRB);
Adafruit_NeoPixel strip3(NUM_LEDS3, DATA_PIN3, NEO_GRB);

#define BLINK_TIME 50
#define BLINK_GAP 100

#define Micro 43
#define playe 8
#define rec 10
#define playl 9

#define SEND_PIN A12
#define RECV_PIN 33
#define relay 31

#define ldR A15
#define PIRmotion 41

#define sclpin A8  // orange  scl
#define sdopin A9  // yellow  sdo

#define DHTPIN A10
#define DHTTYPE DHT11  // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 20, 4);   // purple sda 20, blue scl 21
LiquidCrystal_I2C lcd2(0x26, 16, 2);  // purple sda 20, blue scl 21
IRrecv irrecv(RECV_PIN);
decode_results results;

#define Password_length 2    // define password legnth +1
char Data[Password_length];  // define password storage
char Master[Password_length] = "1";

int currentMillis = millis();
unsigned long previousMillis;
const long intervaltime = 21000;
const long interval = 20000;
const long ledtimer = 500;
const long ledtimer2 = 100;
const long lcdclear = 5;
const int sensorMin = 0.0;    // sensor minimum, discovered through experiment
const int sensorMax = 255.0;  // sensor maximum, discovered through experiment
int key = 0;
int i;
int val;
int val2 = 0;
int fingerprintID = 0;
int fingerprintstate = 1;
int a;  //LED strip
int b;  //LED strip2
int c;  //LED strip3
int fadeValue;
byte data_count = 0;
int data[500];
int brightness = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("Serial OK");
		URTCLIB_WIRE.begin();  
  // Only used once, then disabled
  //rtc.set(0, 27, 11, 5, 5, 11, 22);
  //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

  rtc.set_rtc_address(0x68);
  
  lcd.init();
  lcd2.init();
  lcd.backlight();
  lcd2.backlight();
  strip.begin();   // initialize strip (required!)
  strip2.begin();  // initialize strip (required!)
  strip3.begin();  // initialize strip (required!)
  strip.setBrightness(0);
  strip3.setBrightness(0);
  pinMode(ldR, INPUT);
  pinMode(PIRmotion, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(Micro, INPUT);
  pinMode(rec, OUTPUT);
  pinMode(playl, OUTPUT);
  pinMode(playe, OUTPUT);
  pinMode(lazer, OUTPUT);
  pinMode(sclpin, OUTPUT);
  pinMode(sdopin, INPUT);
  digitalWrite(sclpin, HIGH);
  digitalWrite(relay, LOW);
  digitalWrite(playl, LOW);
  digitalWrite(playe, LOW);
  digitalWrite(rec, HIGH);
  digitalWrite(lazer, LOW);

  dht.begin();

  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("??C"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("??C"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("??C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  irrecv.enableIRIn();  // Start the receiver

  delay(50);
}

void loop() {
  int currentMillis = millis();
  int Battery1 = analogRead(A1);
  int Battery2 = analogRead(A2);
  float voltage2 = Battery1 * (5.0 / 1023.0);
  float voltage = Battery2 * (5.0 / 1023.0);
 
  delay(100);

  lcd2.setCursor(0, 0);
  lcd2.print("A1 = ");
  lcd2.print(voltage2);
  lcd2.print("v");
  lcd2.setCursor(0, 1);
  lcd2.print("A2 = ");
  lcd2.print(voltage);
  lcd2.print("v");
  pir();
  temp();
  keys();
  Clock();
char minute[] = "45";
if (strcmp(rtc.minute(),minute) == 45){digitalWrite(LED_BUILTIN,0x1);
}
delay(100);

}
void keys() {
   int microValue = analogRead(A14);
      int sensorValue = analogRead(A15);
   float light = sensorValue * (100.0 / 1024.0);

  for (i = 1; i < 17; i++) {
    digitalWrite(sclpin, LOW);  //toggle clock
    digitalWrite(sclpin, HIGH);
    if (!digitalRead(sdopin)) {
      key = i;  //valid data found
      Serial.println(i);
    }
  }
  if (key == 1) {
  }
  if (key == 2) {
    lcd.clear();
  lcd.setCursor(0, 0);
lcd.print("Light: ");
  lcd.print(light);
  lcd.setCursor(0, 1);
  lcd.print("Sound: ");
  lcd.print(microValue);    
  }
  if (key == 3) {
  }
  if (key == 4) {
  }
  if (key == 5) {
  }
  if (key == 6) {
    digitalWrite(lazer, HIGH);
    delay(50);
    digitalWrite(lazer, LOW);
  }
  if (key == 7) {
  }
  if (key == 8) {
  }

  if (key == 9) {
    digitalWrite(lazer, LOW);
  }
  if (key == 10) {
    for (int c = 0; c < 31; c++) {
      strip3.setPixelColor(c, strip3.Color(0, 0, 255));  // set STRIP to blue
      strip3.show();
    }
    delay(100);
    for (int c = 0; c < 31; c++) {
      strip3.setPixelColor(c, strip3.Color(255, 0, 0));  // set STRIP to red
      strip3.show();
    }
    delay(100);
    for (int a = 0; a < 8; a++) {
      strip.setPixelColor(a, strip.Color(0, 0, 255));  // set STRIP to blue
      strip.show();
    }
    delay(100);
    for (int a = 0; a < 8; a++) {
      strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
      strip.show();
    }
    delay(100);
  }
  if (key == 11) {
  }
  if (key == 12) {
    led();
  }
  if (key == 13) {
    digitalWrite(playl, HIGH);  // start play
  }
  if (key == 14) {
    digitalWrite(rec, LOW);  // start recording
  }
  if (key == 15) {
    digitalWrite(playl, LOW);  // stop play
  }
  if (key == 16) {
    digitalWrite(rec, HIGH);  // stop recording
  } else {
    if (irrecv.decode(&results)) {
      switch (results.value) {
        case 0xFFA25D:
          Serial.println("CH-");
          digitalWrite(relay, 0);
          break;
        case 0xFFE21D:
          Serial.println("CH+");
          digitalWrite(relay, 1);
          break;
        case 0xFF906F:
          Serial.println("EQ");
          police();
          break;
        case 0xFFE01F:
          Serial.println("-");
          strip.setBrightness(15);
          strip3.setBrightness(15);
          break;
        case 0xFF629D:
          Serial.println("CH");
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(255, 255, 255));  // set STRIP to white
            strip.show();
          }
          break;
        case 0xFF22DD:
          Serial.println("PREVIOUS");
          strip.setBrightness(10);
          strip3.setBrightness(5);
          break;
        case 0xFFC23D:
          Serial.println("PLAY");
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(255, 255, 255));
            strip3.show();
          }
          break;
        case 0xFFA857:
          Serial.println("+");
          strip.setBrightness(100);   // set pixel brightness to 100 STRIP
          strip3.setBrightness(100);  // set pixel brightness to 100 STRIP3
          break;
        case 0xFF6897:
          Serial.println("0");
          strip.setBrightness(0);   // set pixel brightness to 0 STRIP
          strip3.setBrightness(0);  // set pixel brightness to 0 STRIP3
          strip.show();
          strip3.show();
          break;
        case 0xFF9867:
          Serial.println("100+");
          strip.setBrightness(150);   // set pixel brightness to 150 STRIP
          strip3.setBrightness(150);  // set pixel brightness to 150 STRIP3
          break;
        case 0xFFB04F:
          Serial.println("200+");
          strip.setBrightness(255);   // set pixel brightness to 255 STRIP
          strip3.setBrightness(255);  // set pixel brightness to 255 STRIP
          break;
        case 0xFF30CF:
          Serial.println("1");
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
            strip.show();
          }
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(255, 0, 0));  // set STRIP3 to red
            strip3.show();
          }
          break;
        case 0xFF18E7:
          Serial.println("2");
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(0, 0, 255));  // set STRIP to blue
            strip.show();
          }
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(0, 0, 255));  // set STRIP to blue
            strip3.show();
          }
          break;
        case 0xFF7A85:
          Serial.println("3");
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(0, 255, 0));  // set STRIP3 to green
            strip3.show();
          }
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(0, 255, 0));  // set STRIP3 to green
            strip.show();
          }
          break;
        case 0xFF10EF:
          Serial.println("4");
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(255, 255, 0));  // set STRIP3 to yellow
            strip3.show();
          }
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(255, 255, 0));  // set STRIP3 to yellow
            strip.show();
          }
          break;
        case 0xFF38C7:
          Serial.println("5");
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(59, 0, 84));  // set STRIP3 to magenta
            strip3.show();
          }
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(59, 0, 84));  // set STRIP3 to magenta
            strip.show();
          }
          break;
        case 0xFF5AA5:
          Serial.println("6");
          xmas();
          break;
        case 0xFF42BD:
          Serial.println("7");
led();
          break;
        case 0xFF4AB5:
          Serial.println("8");
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(250, 5, 229));  // set pixel 0 to pink
            strip.show();
          }
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(250, 5, 229));  // set pixel to green
            strip3.show();
          }
          break;
        case 0xFF52AD:
          Serial.println("9");
          for (int a = 0; a < 8; a++) {
            strip.setPixelColor(a, strip.Color(0, 188, 255));  // set pixel 0 to pink
            strip.show();
          }
          for (int c = 0; c < 31; c++) {
            strip3.setPixelColor(c, strip3.Color(0, 188, 255));  // set pixel to green
            strip3.show();
          }

          break;
      }
      irrecv.resume();
      delay(10);
    } else {
    }
  }
  return;
}

void led() {

  if (currentMillis - previousMillis <= ledtimer) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    strip.setPixelColor(0, strip.Color(0, 0, 255));  // set pixel 0 to blue
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(1, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(2, strip.Color(255, 0, 0));  // set pixel 0 to red
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(3, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(4, strip.Color(0, 0, 255));  // set pixel 0 to blue
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(5, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(6, strip.Color(255, 0, 0));  // set pixel 0 to red
    strip.show();
    delay(BLINK_TIME);
    strip.setPixelColor(7, strip.Color(0, 0, 255));  // set pixel 0 to blue
    strip.show();
    delay(BLINK_GAP);

    strip.setPixelColor(0, strip.Color(250, 5, 229));  // set pixel 0 to pink
    delay(BLINK_TIME);
    strip.setPixelColor(1, strip.Color(0, 0, 255));  // set pixel 0 to blue
    delay(BLINK_TIME);
    strip.setPixelColor(2, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(3, strip.Color(255, 0, 0));  // set pixel 0 to red
    strip.show();
    delay(BLINK_TIME);

    delay(BLINK_GAP);
    strip.setPixelColor(0, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(1, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(2, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(3, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(4, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(5, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(6, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.setPixelColor(7, strip.Color(250, 5, 229));  // set pixel 0 to pink
    strip.show();
    delay(BLINK_GAP);

    strip.setPixelColor(0, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(1, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(2, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(3, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(4, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(5, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(6, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    delay(BLINK_TIME);
    strip.setPixelColor(7, strip.Color(0, 255, 0, 50));  // set pixel 0 to green
    strip.show();
    delay(BLINK_GAP);
  }
  return;
}

void temp() {

  int currentMillis = millis();
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    lcd.clear();
    lcd.print(F("Error reading temperature!"));
    delay(500);
    lcd.clear();
  } else {
    lcd.setCursor(0, 2);
    lcd.print(F("Temp: "));
    lcd.print(event.temperature);
    lcd.print(F("C"));
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("??C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
      lcd.clear();
      lcd.print(F("Error reading humidity!"));
      delay(500);
      lcd.clear();
    } else {
      lcd.setCursor(0, 3);
      lcd.print(F("Humid: "));
      lcd.print(event.relative_humidity);
      lcd.print(F("%"));
    }
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
  }
  delay(10);
  return;
}

void pir() {

  val = digitalRead(PIRmotion);

  if (val == 1) {
    switch (val) {
      case 0:
        strip2.setBrightness(255);
        strip2.setPixelColor(0, strip2.Color(0, 255, 0));  // set pixel 0 to green
        strip2.setPixelColor(1, strip2.Color(0, 255, 0));
        strip2.setPixelColor(2, strip2.Color(0, 255, 0));
        strip2.setPixelColor(3, strip2.Color(0, 255, 0));
        strip2.show();
        Serial.println("pir on");
        delay(100);
        break;
      case 1:
        val = 0;
        strip2.setBrightness(0);
        strip2.show();
        break;
    }
  }
  return;
}

void piroff() {

  if (val == 0) {
    switch (val) {
      case 0:
        Serial.println("pir off");
      case 1:
        val = 1;
    }
  }
  if (i >= 0) {
    Serial.println("piroff test");
    return;
  } else {
    piroff();
  }
}

void LDR() {
  float sensorValue = analogRead(A15);

  int range = map(sensorValue, sensorMin, sensorMax, 0, 4);
  Serial.println(range);
  switch (range) {

    case 0:
      strip.setBrightness(255);
      strip3.setBrightness(255);
      strip.show();
      strip3.show();
      break;

    case 1:
      strip.setBrightness(200);
      strip3.setBrightness(200);
      strip.show();
      strip3.show();
      break;

    case 2:
      strip.setBrightness(150);
      strip3.setBrightness(150);
      strip.show();
      strip3.show();
      break;

    case 3:
      strip.setBrightness(100);
      strip.setBrightness(100);
      strip.show();
      strip.show();
      break;

    case 4:
      strip.setBrightness(50);
      strip3.setBrightness(50);
      strip.show();
      strip3.show();
      break;
  }
  return;
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
    Serial.println("test stages8");
  }
  return;
}


void police() {

  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
    strip.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(0, 0, 255));  // set STRIP to blue
    strip3.show();
    delay(100);
  }
  for (int c = 0; c < 31; c--) {
    strip3.setPixelColor(c, strip.Color(0, 0, 0));  // set STRIP to off
    strip3.show();
    delay(100);
  }
  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
    strip.show();
    delay(100);
  }

  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(0, 0, 255));  // set STRIP to blue
    strip.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(255, 0, 0));  // set STRIP to red
    strip3.show();
    delay(100);
  }
  for (int c = 0; c < 31; c--) {
    strip3.setPixelColor(c, strip.Color(0, 0, 0));  // set STRIP to off
    strip3.show();
    delay(100);
  }
keys();
}
void xmas() {

  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
    strip.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(0, 255, 0));  // set STRIP to green
    strip3.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(0, 0, 255));  // set STRIP to blue
    strip3.show();
    delay(100);
  }
  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
    strip.show();
    delay(100);
  }
  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(0, 255, 0));  // set STRIP to green
    strip.show();
    delay(100);
  }
  for (int a = 0; a < 8; a++) {
    strip.setPixelColor(a, strip.Color(0, 0, 255));  // set STRIP to blue
    strip.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(255, 0, 0));  // set STRIP to red
    strip3.show();
    delay(100);
  }
  for (int c = 0; c < 31; c++) {
    strip3.setPixelColor(c, strip.Color(0, 255, 0));  // set STRIP to green
    strip3.show();
    delay(100);
  }
keys();
}

void Clock() {

  int currentMillis = millis();

  rtc.refresh();

  lcd.setCursor(0, 0);
  lcd.print(rtc.hour());
  lcd.print(':');
  lcd.print(rtc.minute());
  lcd.print(':');
  lcd.print(rtc.second());

  lcd.setCursor(0, 1);
  lcd.print(rtc.day());
  lcd.print('/');
  lcd.print(rtc.month());
  lcd.print('/');
   lcd.print(rtc.year());

  if (currentMillis - previousMillis >= intervaltime) {
    previousMillis = currentMillis;
    Serial.print("RTC DateTime: ");

    Serial.print(rtc.year());
    Serial.print('/');
    Serial.print(rtc.month());
    Serial.print('/');
    Serial.print(rtc.day());

    Serial.print(' ');

    Serial.print(rtc.hour());
    Serial.print(':');
    Serial.print(rtc.minute());
    Serial.print(':');
    Serial.print(rtc.second());

    Serial.print(" DOW: ");
    Serial.print(rtc.dayOfWeek());

    Serial.println();
  }
}
