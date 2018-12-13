/*
 * microWakeupperESP
 * 
 * Example code for "How to integrate the microWakeupper board into your own projects?"
 * This example is specially for the ESP8266 (basically the ESP32 should work too).
 * 
 * The ESP supports different DeepSleep modes to reduce power consumption to a minimum.
 * For more information check this doc: 
 * 
 *  https://www.espressif.com/sites/default/files/9b-esp8266-low_power_solutions_en_0.pdf
 *  
 * If you google for "ESP deep sleep" there alredy some really cool examples/projects available, 
 * for example: 
 *  https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep
 *  
 * The microWakeupper board is NOT necessary, if the ESP just has to wakeup based on 
 * a sleep timer - don't forget to connect the pins GPIO16 and RST.
 * BUT if you want to wakeup (additionally) through an external event (e.g. a switch, button or PIR-sensor)
 * the microWakeupper makes your life much easier!
 * 
 * 
 * How to connect the microWakupper to your ESP?
 *  (Check also the backside notes directly on the board ;-)
 *  
 *  :: ESP :::        ::: microWakeupper :::
 *  VCC (3.3)   <-->  VCC   (mandatory)
 *  GND         <-->  GND   (mandatory)
 *  RST         <-->  RST   (mandatroy)
 *  GPIO16      <-->  G16   (optional)
 *      Not necessary if you don't use a sleep timer, or GPIO16 is already connect to RST.
 *  OUTPUT      <-->  DIS   (optional)
 *      Just set DIS (disable pin) to HIGH, if you want to disable the microWakeupper. 
 *      This prevents further resets as long as your code is running (until going to sleep again).
 *  INPUT       <-->  STA   (optional)
 *      This pin is HIGH, if the microWakeupper was triggered within the last 2-3 seconds.
 * 
 *  
 * 
 * 
 *
 * 
 * 
 * This example code is in the public domain.
 * 
 * created 13 Dec 2018
 * by Tobias Stöger (@tstoegi, @moreioLabs)
 * 
 * 
 * Special thanks to @tinyledmatrix and @davedarko!
 * 
 */

#include <ESP8266WiFi.h>
#include "Ticker.h"

/*
  Log function to help debugging running code.
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

Ticker blinker;
 
#define LED D5  //On board LED

void ICACHE_RAM_ATTR onTimerISR() {
  digitalWrite(LED, !(digitalRead(LED))); //Toggle LED Pin
  timer1_write(600000);//12us
}
void setup() {
  // put your setup code here, to run once:
  // Serial
  Serial.begin(115200);
  Log("Serial begin")
  pinMode(D6, INPUT); //STA
  pinMode(LED, OUTPUT); //LED

  //Initialize Ticker every 0.5s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(600000); //120000 us

}

void loop() {
  // put your main code here, to run repeatedly:
  testDeepSleep();
}

void deepSleep(long seconds)
{
  // Sleep
  Log("ESP8266 in sleep mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(1);
  WiFi.forceSleepBegin();
  Log("Going into deepSleep");
  ESP.deepSleep(seconds * 1000000); //Microseconds
  delay(200); //Muss angeblich nach deepSleep gemacht werden
}

void testDeepSleep() {

  Log("testDeepSleep");

  if (digitalRead(D6) == HIGH) {
    Log("ESP was reseted by microWakeupper")
  } else {
    Log("ESP was reseted by internal timer [ESP.deepSleep(MicroSenconds)]")
  }
  Log("Will go back into DeepSleep in 6 seconds...")

  pinMode(D7, OUTPUT); //DIS
  digitalWrite(D7, HIGH);
  delay(6000);

  Log("...gn8 (I will sleep for 10 seconds)");
  deepSleep(10);
}
