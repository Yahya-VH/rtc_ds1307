#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
const long utcOffsetInSeconds = 5 * 3600 + 30 * 60;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

const char* ssid = "ONE";
const char* password = "12345678";

void printTwoDigits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number, DEC);
}

void printFormattedTime(DateTime now) {
  Serial.print(now.year(), DEC);
  Serial.print('-');
  printTwoDigits(now.month());
  Serial.print('-');
  printTwoDigits(now.day());
  Serial.println();
  
  int hour12 = now.hour() % 12;
  if (hour12 == 0) {
    hour12 = 12; // Midnight is 12:00 AM in 12-hour format
  }

  printTwoDigits(hour12);
  Serial.print(':');
  printTwoDigits(now.minute());
  Serial.print(':');
  printTwoDigits(now.second());
  Serial.print(" ");
  Serial.println(now.hour() < 12 ? "AM" : "PM");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  delay(3000);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  timeClient.begin();
  timeClient.update();

  if (WiFi.status() == WL_CONNECTED) {
    unsigned long epochTime = timeClient.getEpochTime();
    DateTime now(epochTime);

    rtc.adjust(now);
  }
}

void loop() {
  timeClient.update(); // Update NTPClient to get the current time
  DateTime now = rtc.now();
  
  // Print today's date and time in 12-hour format
  Serial.println("Today's Date and Time (12-Hour Format):");
  printFormattedTime(now);

  delay(1000);
}
