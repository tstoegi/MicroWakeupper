/*
   microWakeupper

   Example code for "How to integrate the microWakeupper board into your own projects?"
   This example is specially for connecting to a Wemos D1 mini.

   Please check the README for further information.

   created 13 Dec 2018
   updated 17 March 2021
   by Tobias Stöger (@tstoegi)

   Special thanks to @tinyledmatrix and @davedarko!
*/

// Remove comment, if you use wifi in your project
#include <ESP8266WiFi.h>
#include <Credentials.h>

/*
  Utility functions to help debugging running code.
*/
#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H
#define Log(str)        \
  Serial.print(__LINE__);     \
  Serial.print(' ');          \
  Serial.print(millis());     \
  Serial.print(": ");         \
  Serial.print(__PRETTY_FUNCTION__); \
  Serial.print(' ');          \
  Serial.println(str);
#endif

// If uploading new code does not work, disconnect GPIO16 (temporarily) !!!

#define STA               D6  // D6 connected to STA pin
#define DIS               D7  // D7 connected to DIS pin
#define secondsToSleep    0  // Timeperiod in seconds sleeping
#define secondsAwake      5  // Time in seconds to be awake

int sta;
bool executeOnce = true;
bool startDeepSleep = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");

  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

  pinMode(STA, INPUT);                // Initialize the pin for STA as an input
  sta = digitalRead(STA);             // If we use disable (DIS), we have to save the state
  pinMode(DIS, OUTPUT);               // Initialize the pin for DIS as an output
  digitalWrite(DIS, HIGH);            // Disable the microWakeupper to prevent retriggering
  // After calling DIS(HIGH) the STA pin is always HIGH

  // This is how you check what happened (during your sleep)
  if (sta == HIGH) {   // If HIGH the microWakeupper was triggered (by the connected switch)
    Serial.println("ESP was reseted/wakeup by microWakeupper");
  } else {                            // If LOW it points to the sleep timer (or reset button, or restart). If you need further details check system_get_rst_info()
    Serial.println("Looks like ESP was reseted/wakeup by internal timer");
  }

  enableWifi();

}

void loop() {

  logVBatt();   // log VBatt to serial

  
  // Place you code here (whatever you will do, e.g. send something to IFTTT)
  // >...
  if (executeOnce) {
    Serial.println("Hello microWakeupper!");
    digitalWrite(LED_BUILTIN, LOW);         // Turn on internal LED
    delay(secondsAwake * 1000);             // Just wait (during this time you can test your switch - nothing should happen if the microWakeupper is disabled)
    digitalWrite(LED_BUILTIN, HIGH);        // Turn off internal LED
    executeOnce = false;
  }
  // ...<

  // Optional: If you want to check the current state of your external switch
  // >...
  digitalWrite(DIS, LOW);                 // Enable the microWakeupper again (after it's internal timer of about 2 seconds)
  if (digitalRead(STA) == HIGH) {
    Serial.println("Will not go into deepSleep as long as the microWakeupper is active.");
    digitalWrite(LED_BUILTIN, LOW);         // Turn on internal LED
    delay(200);                 
    digitalWrite(LED_BUILTIN, HIGH);        // Turn off internal LED
  } else {
    startDeepSleep = true;
  }
  // ...<

  if (startDeepSleep) {
    deepSleep(secondsToSleep);            // Sleep for n-seconds (during sleep you can test your switch - a wakeup should happen)
  }
    
  delay(1000);
}

void deepSleep(long seconds)
{
  // Remove the comments, if you use wifi in your project
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(1);

  WiFi.forceSleepBegin();
  delay(1);

  Serial.println("Going into deepSleep now");
  Serial.end();
  ESP.deepSleep(seconds * 1000000);   // ESP.deepSleep expects microseconds
  delay(200);                         // Seems recommended after calling deepSleep
}

bool enableWifi()
{
  
  WiFi.forceSleepBegin(); //Hack to reenable Wifi after deepsleep
  delay(1);
  WiFi.forceSleepWake();
  delay(1);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA); // <<< Station
  WiFi.setAutoConnect (false);
  WiFi.setAutoReconnect (false);
  delay(300);
  // Wait for connection

  logMacAddress();
  
  int retries = 5;
  while (WiFi.status() != WL_CONNECTED && retries > 0)
  {
    Log("WiFi.status()=");
    Log(WiFi.status());
    pinMode(LED_BUILTIN, OUTPUT);  // initialize onboard LED as output
    ESP.eraseConfig();             // to prevent random exception(2) cashes
    WiFi.begin(mySSID, myPASSWORD);
    delay(100);

    Log(retries);
    int secondsTimeout = 60;
    while (WiFi.status() != WL_CONNECTED && secondsTimeout > 0)
    {
      digitalWrite(LED_BUILTIN, LOW);  // turn on LED with voltage HIGH
      delay(500);                      // wait one second
      digitalWrite(LED_BUILTIN, HIGH);   // turn off LED with voltage LOW
      delay(500);
      Serial.print(".");
      secondsTimeout--;
    }
    retries--;
    Log();
  }

  if (retries == 0) {
    Log(mySSID);
    Log("Connection failed!");
    ESP.restart();
    return false;
  }

  Log();

  Log("Connected to ");
  Log(mySSID);
  Log("IP address: ");
  Log(WiFi.localIP());

  return true;
}

void logMacAddress()
{
  byte mac[6];         // the MAC address of your Wemos
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);
}

void logVBatt()
{
  int sensorValue = analogRead(A0);
  float voltage = sensorValue / 196.202; // VBatt-220K-A0-220K-ADC-100K-GND
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" | ");
  Serial.print("Sensor: ");
  Serial.println(sensorValue);
}
