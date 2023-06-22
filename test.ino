#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <WiFiUdp.h>

const char *ssid = "Airhome";
const char *password = "Coco2020";

const char kHostname[] = "https://api.sl.se";
const char kPath[] = "/api2/realtimedeparturesV4.xml?key=938f37c5783949a68179014a1315976d&siteid=9201&timewindow=30&Metro=false";
WiFiClient c;
HttpClient http(c, kHostname);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

// Parses out the time in from the json and returns it as number of seconds until bus arrival
int toTimes(char *charTime)
{
    int buff, time = 0;
    char *hour = strtok(NULL, ":");
    char *min = strtok(NULL, ":");
    char *sec = strtok(NULL, "\"");
    sscanf(hour, "%d", &buff);
    time += buff * 3600;
    sscanf(min, "%d", &buff);
    time += buff * 60;
    sscanf(sec, "%d", &buff);
    time += buff;

    return (time);
}

// Finds the expected time of bus arrival as a char pointer, parsed from big json file
int arrivalTimeParse(String jsonFile)
{
    char *time;
    int index = jsonFile.indexOf("Jakobsbergs station");

    if (index == -1)
    {
        Serial.println("Could not find");
        return -1;
    }

    String firstCutoff = jsonFile.substring(index);
    index = firstCutoff.indexOf("ExpectedDateTime");
    String secondCutoff = firstCutoff.substring(index);
    char buff[secondCutoff.length() + 1];
    secondCutoff.toCharArray(buff, 1000);
    time = strtok(buff, ",");
    time = strstr(time, "20");
    int arrivalTime = toTimes(time);
    return arrivalTime;
}

// Finds the urgency level of the arrival. Less time until the bus means higher urgency, ranked 0-3
int getUrgency(int hours, int minutes, int seconds, String jsonFile)
{
    int urgency;
    int busTime = arrivalTimeParse(jsonFile);
    if (busTime == -1)
        return -1;
    int currTime = (hours * 3600 + minutes * 60 + seconds);
    int timeDiff = busTime - currTime;

    if (timeDiff < 6 * 60 && timeDiff > 3 * 60)
    {
        urgency = 1;
    }
    if (timeDiff < 10 * 60 && timeDiff > 6 * 60)
    {
        urgency = 2;
    }
    if (timeDiff > 10 * 60)
    {
        urgency = 3;
    }
    else
    {
        urgency = 0;
    }
    return urgency;
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Wifi Connected");
    timeClient.begin();
}

void loop()
{
    String jsonFile;
    int err = 0;

    timeClient.update();
    int bodyLen = http.contentLength();
    while (client.connected())
    {
        String line = client.readStringUntil('\r');
        response += line;
    }
    client.stop();

    while (1)
        ;
}