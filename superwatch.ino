#include <FastLED.h>
#include <Preferences.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <time.h>
#include <RTClib.h>
#include <Update.h>
#include <WebServer.h>

#include "color.h"
#include "matrix.h"
#include "watch.h"

const int ROWS = 8;
const int COLUMNS = 8;
const int NUM_LEDS = ROWS * COLUMNS;
const int MATRIX_PIN = 18;
const int WIFI_CONNTECT_ATTEMPTS = 20;

String NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 7 * 3600; // GMT + 7
const int DAYLIGHT_OFFSET_SEC = 3600;

Preferences preferences;
WebServer server(80);
RTC_DS3231 rtc;
bool rtcExists;
CRGB leds[NUM_LEDS];
Matrix matrix(ROWS, COLUMNS, leds);
Watch watch;

void enableNetwork(const char* ssid, const char* password) {
  Serial.printf("Connecting to %s ..", ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  for (int i = 0; i < WIFI_CONNTECT_ATTEMPTS && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" connected!");
  } else {
    Serial.printf("\nTimeout when connecting to %s. Probably ssid or password is incorrect\n", ssid);
    Serial.println("Setup wifi access point");
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF);
}

void setupMdns() {
  uint64_t chipid = ESP.getEfuseMac();
  String chipId = String((uint16_t)(chipid>>32), HEX) + String((uint32_t)chipid, HEX);
  String name = String("superwatch-") + chipId;

  if (!MDNS.begin(name.c_str())) {
    while (1) {
      Serial.println("Error setting up mDNS responder");
      delay(1000);
    } 
  }

  MDNS.setInstanceName(name);
  Serial.println("mDNS responder started");
  Serial.printf("I am: %s\n", name.c_str());
  
  MDNS.addService("http", "tcp", 80);
}

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
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi connected");

    //init and get the time
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER.c_str());
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
    Serial.println("Wifi doesn't connected");

    if (rtcExists) {
      updateTimeFromRtc();
    } else {
      Serial.println("WiFi is not available, RTC is not available, time needs to be configured manually");
    }
  }
  
  Serial.println("Update time from network completed");
}

void setup() { 
  Serial.begin(115200);
  
  preferences.begin("wifi", false);
  String ssid = preferences.getString("ssid");
  String password = preferences.getString("password");
  preferences.end();
  
  enableNetwork(ssid.c_str(), password.c_str());
  setupMdns();
  
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

  preferences.begin("http", false);
  String www_username = preferences.getString("username", "admin");
  String www_password = preferences.getString("password", "superwatch");
  preferences.end();

  server.on("/", [www_username, www_password]() {
    if (!server.authenticate(www_username.c_str(), www_password.c_str())) {
      return server.requestAuthentication(
        DIGEST_AUTH, 
        "Login required", 
        "Authentication failed"
      );
    }
    server.send(200, "text/html", "Login OK <form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    if (Update.hasError()) {
      server.send(200, "text/plain", "FAIL");
    } else {
      server.send(200, "text/plain", "Superwatch restarting...");
      delay(10000);
      ESP.restart();
    }
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin()) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    } else {
      Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
    }
  });
  server.begin();
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

  server.handleClient();
}
