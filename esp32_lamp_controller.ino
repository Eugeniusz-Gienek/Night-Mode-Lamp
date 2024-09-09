#include "RTClib.h"
#include <Wire.h>
#include "wiring_private.h"
#include "wifi-credentials.h"
#include "OTA.h"

#define I2C1_SDA1 8
#define I2C1_SCL1 9
#define I2C_FREQ 400000
#define RELAY_PIN 4

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


TwoWire I2C_1 = TwoWire(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  I2C_1.begin(I2C1_SDA1, I2C1_SCL1, I2C_FREQ);
  pinMode(RELAY_PIN, INPUT | OUTPUT_OPEN_DRAIN);
  digitalWrite(RELAY_PIN, HIGH);
  if (! rtc.begin(&I2C_1)) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  setupOTA("NightModeLampController", myWifiSSID, myWifiPASSWORD);
}

void loop() {
  #ifdef defined(ESP32_RTOS) && defined(ESP32)
  #else // If you do not use FreeRTOS, you have to regulary call the handle method.
    ArduinoOTA.handle();
  #endif
  DateTime now = rtc.now();
  if((now.hour() >= 8) && (now.hour() < 22)) {
    delay(500);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    delay(500);
    digitalWrite(RELAY_PIN, HIGH);
  }
  delay(500);
    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");

    Serial.println();
    */

    /*delay(1000);
    digitalWrite(RELAY_PIN, LOW);
    delay(1000);
    digitalWrite(RELAY_PIN, HIGH);*/
}
