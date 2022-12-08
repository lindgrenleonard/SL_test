#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>

const char *ssid     = "Airhome";
const char *password = "Coco2020";


HTTPClient http;
const char* serverName = "http://api.sl.se/api2/realtimedeparturesV4.jsonl?key=938f37c5783949a68179014a1315976d&siteid=9201&timewindow=30&Metro=false";


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);


//Parses out the time in from the json and returns it as number of seconds until bus arrival
int toTimes(char *charTime) {
    int buff, time = 0;
    char *hour = strtok(NULL, ":");
    char *min = strtok(NULL, ":");
    char *sec = strtok(NULL, "\"");
    sscanf(hour, "%d", &buff);
    time += buff*3600;
    sscanf(min, "%d", &buff);
    time += buff*60;
    sscanf(sec, "%d", &buff);
    time += buff;
    
    return(time);
}

//Finds the expected time of bus arrival as a char pointer, parsed from big json file
int arrivalTimeParse(String jsonFile) {
    char *time;
    int index = jsonFile.indexOf("Jakobsbergs station");

    if(index == -1) {
      Serial.println("Could not find");
      return -1;
    }

    String firstCutoff = jsonFile.substring(index);
    index = firstCutoff.indexOf("ExpectedDateTime");
    String secondCutoff = firstCutoff.substring(index);
    char buff[secondCutoff.length()+1];
    secondCutoff.toCharArray(buff, 1000);
    time = strtok(buff, ",");
    time = strstr(time, "20");
    int arrivalTime = toTimes(time);
    return arrivalTime;
}

//Reads the json file we want to parse


//Finds the urgency level of the arrival. Less time until the bus means higher urgency, ranked 0-3
int getUrgency(int hours, int minutes, int seconds, String jsonFile) {
    int urgency;
    int busTime = arrivalTimeParse(jsonFile);
    if(busTime = -1) return;
    int currTime = (hours*3600 + minutes*60 + seconds);
    int timeDiff = busTime-currTime;
    
    if(timeDiff < 6*60 && timeDiff > 3*60) {urgency = 1;}
    if(timeDiff < 10*60 && timeDiff > 6*60) {urgency = 2;}
    if(timeDiff > 10*60) {urgency = 3;}
    else {urgency = 0;}
    return urgency;
}


void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("Wifi Connected");
  timeClient.begin();
  WiFiClient client;
  http.begin(client, serverName);
}

void loop() {
  int urgency;
  timeClient.update();
  Serial.println(timeClient.getHours());
  Serial.println(timeClient.getMinutes());
  Serial.println(timeClient.getSeconds());
  
  
  int httpCode = http.GET();
  String jsonFile;
  // Check the jsonFile code
  if (httpCode == HTTP_CODE_OK) {
    // Read the jsonFile from the server
    jsonFile += http.getString();
    Serial.println("Http jsonFile: " + jsonFile);
  } else {
    // Handle error cases
    Serial.println("Error: " + String(httpCode));
  }

  urgency = getUrgency(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds(), jsonFile);
  Serial.println("Urgency: " + urgency);
  //pinMode(LED_PIN, OUTPUT);
  //analogWrite(LED_PIN, 1);

  delay(100000);
}
