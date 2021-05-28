/*
  Example programm for the MicroWakeupper Wemos D1 Mini Shield

  Place the shield on your Wemos D1 Mini (Battery Connector and USB Connector on one side - Battery Connector on top side and USB at the bottom side)
  If you connect a battery take care about Plus (+ left) and Minus (- right). Do not use a unprotected Lipo - the board has no discharge protection.

  Important: Press FLASH (on the MicroWakeupper board) for uploading new sketches! ('cause RST and D0 are connected by default for DeepSleep support)

  (c)2021 by @tstoegi

*/

#include <MicroWakeupper.h>

//MicroWakeupper instance (only one is supported!)
MicroWakeupper microWakeupper;

#define secondsToSleep    10  // Timeperiod in seconds sleeping  - max 4294967295 (around 71 minutes)

void setup() {
  Serial.begin(115200);

  microWakeupper.begin();     // For correct initialisation

  if (microWakeupper.resetedBySwitch()) {
    Serial.println("Looks like ESP was reseted by reset/switch");
  } else {
    Serial.println("Looks like ESP woke up by internal timer");
  }
}

void loop()
{
  Serial.print("Current Battery Voltage:");
  Serial.println(microWakeupper.readVBatt());

  microWakeupper.reenable();                    // Re-enable MicroWakeupper for new triggering
  
  ESP.deepSleep(secondsToSleep * 1000000);      // ESP.deepSleep expects microseconds

  delay(200);
}
