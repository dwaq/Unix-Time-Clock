#include "getTime.h"
// WiFi settings:
// (refer to credentials-example.h for assistance)
#include "credentials.h"
#include <WiFi.h>
#include "time.h"

// send start and stop signals to WiFi task
QueueHandle_t WiFiStatus;
#define WIFI_WAIT 0
#define WIFI_REFRESH 1

// run the WiFi tasks on another core
void coreTask(void * pvParameters)
{
  // create a queue to send data across cores
  WiFiStatus = xQueueCreate(1, sizeof(int));

  // start out by getting time
  getTimeFromInternet();

  // then go into a wait state until told to refresh
  int currentWiFiStatus = WIFI_WAIT;

  // can't return, so keep on loopin'
  while(1)
  {
    // continue recieving the queue data
    xQueueReceive(WiFiStatus, &currentWiFiStatus, portMAX_DELAY);
    
    // when asked to refresh,
    if (currentWiFiStatus == WIFI_REFRESH)
    {
      // get the time from the internet again
      getTimeFromInternet();

      // and continue to wait
      currentWiFiStatus = WIFI_WAIT;
    }
  }

  // remove a task from management:
  //vTaskDelete(NULL);
}

const char * ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// get current time from the NTP server
void getTimeFromInternet(void) {
  //connect to WiFi
  Serial.printf("Connecting to %s ", SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  if (!getLocalTime( & timeinfo)) {
    //Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(unix_time);

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

// store the individual decimal digits of the unix time
struct Time_Digits {
   byte dig1;
   byte dig2;
   byte dig3;
   byte dig4;
   byte dig5;
   byte dig6;
   byte dig7;
   byte dig8;
   byte dig9;
   byte dig10;
   byte refresh;
}; 

struct Time_Digits display_time;

// get current time and store it in individual digits
void setTime(void)
{
  // get current time
  int unix_time = time(NULL);

  //Serial.println(unix_time);
  
  // loop through the time and set each digit:
  
  // get lowest digit
  display_time.dig10 = unix_time % 10;
  // shift over
  unix_time = unix_time / 10;
  // get next lowest digit
  display_time.dig9 = unix_time % 10;
  // shift over agin for the next round
  unix_time = unix_time / 10;

  // continue looping over the remaining digits:

  display_time.dig8 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig7 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig6 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig5 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig4 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig3 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig2 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig1 = unix_time % 10;

  // every 10,000 seconds (~2.7 hours),
  // check the time from the internet to re-sync the internal clock
  // TODO: change from dig9 to dig6
  if (display_time.refresh != display_time.dig9)
  {
    // re-sync the time from the internet
    // TODO: launch on other core so it doesn't hold up setting the time
    //getTimeFromInternet();
    int currentWiFiStatus = WIFI_REFRESH;
    xQueueSend(WiFiStatus, &currentWiFiStatus, portMAX_DELAY);

    // set to current digit catch it next time
    display_time.refresh = display_time.dig9;
  }
}

// return the individual digits at the index
byte getDigit(int digit)
{
  switch (digit) {
    case 1:
      return display_time.dig1;
      break;
    case 2:
      return display_time.dig2;
      break;
    case 3:
      return display_time.dig3;
      break;
    case 4:
      return display_time.dig4;
      break;
    case 5:
      return display_time.dig5;
      break;
    case 6:
      return display_time.dig6;
      break;
    case 7:
      return display_time.dig7;
      break;
    case 8:
      return display_time.dig8;
      break;
    case 9:
      return display_time.dig9;
      break;
    case 10:
      return display_time.dig10;
      break;
  }
}
