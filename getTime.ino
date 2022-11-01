#include <ESP8266WiFi.h>
#include <time.h>

const char *ssid     = "Airhome";
const char *password = "Coco2020";

const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO = "CET-1CEST,M3.5.0,M10.5.0/3";

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;


void setup() {
  Serial.begin(115200);
  Serial.println("\n\nNTP Time Test\n");
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("\n\nWiFi connected\n\n");

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);

  if (getNTPtime(10)) {  // wait up to 10sec to sync
  } 
  
  else {
    Serial.println("Time not set");
    ESP.restart();
  }

  lastNTPtime = time(&now);

}

void loop() {
  Serial.print(getNTPtime(10)+3600);
  Serial.print("\n");
  delay(1000);
}


int getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
  }
  return now;
}
