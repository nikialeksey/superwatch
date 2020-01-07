#include <FastLED.h>
#include <WiFi.h>
#include "time.h"
#include "RTClib.h"

#include "color.h"
#include "matrix.h"
#include "watch.h"

const int ROWS = 8;
const int COLUMNS = 8;
const int NUM_LEDS = ROWS * COLUMNS;
const int MATRIX_PIN = 18;
const int WIFI_CONNTECT_ATTEMPTS = 20;

const char* SSID = "****";
const char* PASSWORD = "****";

const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 7 * 3600; // GMT + 7
const int DAYLIGHT_OFFSET_SEC = 3600;

RTC_DS3231 rtc;
bool rtcExists;
CRGB leds[NUM_LEDS];
Matrix matrix(ROWS, COLUMNS, leds);
Watch watch;

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void IRAM_ATTR updateTimeFromRtc() {
  Serial.println("Update time from RTC started");

  if (rtcExists && rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the script time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  if (rtcExists) {
    DateTime nowRtc = rtc.now();
    struct tm tm;
    tm.tm_year = nowRtc.year() - 1900;
    tm.tm_mon = nowRtc.month();
    tm.tm_mday = nowRtc.day();
    tm.tm_hour = nowRtc.hour();
    tm.tm_min = nowRtc.minute();
    tm.tm_sec = nowRtc.second();

    printf("Year from RTC: %d\n", nowRtc.year());
    time_t t = mktime(&tm);
    printf("Setting time: %s", asctime(&tm));
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
  } else {
    Serial.println("RTC is not exists, will not be updated");
  }
  Serial.println("Update time from RTC completed");
}

void IRAM_ATTR updateTimeFromNetwork() {
  //connect to WiFi
  Serial.println("Update time from network started");
  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORD);
  for (int i = 0; i < WIFI_CONNTECT_ATTEMPTS && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" CONNECTED");

    //init and get the time
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
    } else {
      Serial.println(&timeinfo, "Obtained time from ntp: %Y-%m-%d %H:%M:%S");
    
      if (rtcExists) {
        Serial.println("Adjust RTC");
        rtc.adjust(
          DateTime(
            timeinfo.tm_year + 1900, 
            timeinfo.tm_mon, 
            timeinfo.tm_mday, 
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
          )
        );
        DateTime now = rtc.now();
        Serial.print("RTC adjusted: ");
        char format[] = "YYYY-MM-DD hh:mm:ss";
        Serial.println(now.toString(format));
      } else {
        Serial.println("RTC not found");
      }
    }
  } else {
    Serial.println(" TIMEOUT");

    if (rtcExists) {
      updateTimeFromRtc();
    } else {
      Serial.println("WiFi is not available, RTC is not available, time needs to be configured manually");
    }
  }
  
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Update time from network completed");
}

void setup() { 
  Serial.begin(115200);

  rtcExists = rtc.begin();
  if (rtcExists) {
    Serial.println("RTC has been found");
  } else {
    Serial.println("Couldn't find RTC");
  }

  updateTimeFromNetwork();
  //updateTimeFromRtc();

  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
}

bool moveForward = true;
int blinkPosition = 0;
void loop() { 
  fadeToBlackBy(leds, NUM_LEDS, 2);
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  watch.update(timeinfo.tm_hour, timeinfo.tm_min);
  watch.draw(matrix);

  if (moveForward) {
    EVERY_N_MILLISECONDS(1000 / COLUMNS) {
      leds[blinkPosition] = CRGB(50, 50, 50);
      if (blinkPosition < COLUMNS - 1) {
        blinkPosition++;
      } else {
        moveForward = false;
      }
    }
  } else {
    EVERY_N_MILLISECONDS(1000 / COLUMNS) {
      leds[blinkPosition] = CRGB(50, 50, 50);
      if (blinkPosition > 0) {
        blinkPosition--;
      } else {
        moveForward = true;
      }
    }
  }
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / 120); 
}
