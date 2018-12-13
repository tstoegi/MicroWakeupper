/*
   microWakeupper

   Example code for "How to integrate the microWakeupper board into your own projects?"
   This example is specially for connecting a NodeMCU(ESP8266) (basically the ESP32 should work too).

   Please check the README for further information.

   created 13 Dec 2018
   by Tobias Stöger (@tstoegi, @moreioLabs)

   Special thanks to @tinyledmatrix and @davedarko!
*/

// Remove comment, if you use wifi in your project
// #include <ESP8266WiFi.h>


#define NODEMCU_ESP12_LED D4  // Do NOT use LED_BUILTIN because usually it's connected to GPIO16 (D0)
#define STA               D6  // D6 connected to STA pin
#define DIS               D7  // D7 connected to DIS pin
#define secondsToSleep    42  // Example time in seconds to sleep

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");

  pinMode(NODEMCU_ESP12_LED, OUTPUT);       // Initialize the LED_BUILTIN pin as an output
  
  pinMode(STA, INPUT);                // Initialize the pin for STA as an input

  pinMode(DIS, OUTPUT);               // Initialize the pin for DIS as an output
  digitalWrite(DIS, HIGH);            // Disable the microWakeupper to prevent retriggering
  
}

void loop() {
  Serial.println("Hi there!");

  // This is how you check what happened (during your sleep)
  if (digitalRead(STA) == HIGH) {     // If HIGH the microWakeupper was triggered (by the connected switch)
    Serial.println("ESP was reseted/wakeup by microWakeupper");
  } else {                            // If LOW it points to the sleep timer (or reset button, or restart). If you need further details check system_get_rst_info()
    Serial.println("Looks like ESP was reseted/wakeup by internal timer");
  }

  // Place you code here (whatever you will do, e.g. send something to IFTTT) >...
  digitalWrite(NODEMCU_ESP12_LED, HIGH); // Turn on internal LED
  delay(23*1000);                        // Just wait 23 seconds (during this time you can test your switch - nothing should happen if the microWakeupper is disabled)
  digitalWrite(NODEMCU_ESP12_LED, LOW); // Turn off internal LED
  // ...<

  deepSleep(secondsToSleep);            // Sleep for n-seconds (during sleep you can test your switch - a wakeup should happen)
}

void deepSleep(long seconds)
{
  // Remove the comments, if you use wifi in your project
  //  WiFi.disconnect();
  //  WiFi.mode(WIFI_OFF);
  //  delay(1);
  //  WiFi.forceSleepBegin();

  Serial.println("Going into deepSleep now");
  ESP.deepSleep(seconds * 1000000);   // ESP.deepSleep expects microseconds
  delay(200);                         // Seems recommended after calling deepSleep
}

