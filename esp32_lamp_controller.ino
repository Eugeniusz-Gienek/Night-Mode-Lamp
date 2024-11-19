#include "RTClib.h"
#include <Wire.h>
#include "wiring_private.h"
#include "wifi-credentials.h"
#include "OTA.h"

#define I2C1_SDA1 8
#define I2C1_SCL1 9
#define I2C_FREQ 400000
#define RELAY_PIN 4

#define START_HOUR 8
#define FINISH_HOUR 22

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


TwoWire I2C_1 = TwoWire(0);

int just_started=1;

void setup() {
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
}

void loop() {
  DateTime now = rtc.now();
  if(just_started == 1) {
    just_started = 0;
    if((now.hour() >= START_HOUR) && (now.hour() < FINISH_HOUR)) {
      delay(500);
      digitalWrite(RELAY_PIN, LOW);
    } else {
      delay(500);
      digitalWrite(RELAY_PIN, HIGH);
    }
    delay(500);
    setupOTA("NightModeLampController", myWifiSSID, myWifiPASSWORD);
  }
  else {
    #ifdef defined(ESP32_RTOS) && defined(ESP32)
    #else // If you do not use FreeRTOS, you have to regulary call the handle method.
      ArduinoOTA.handle();
    #endif
  }
  if((now.hour() >= START_HOUR) && (now.hour() < FINISH_HOUR)) {
    delay(500);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    delay(500);
    digitalWrite(RELAY_PIN, HIGH);
  }
  delay(500);
}
