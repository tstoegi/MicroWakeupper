# About the microWakeupper board

The microWakeupper board is the perfect add-on for your long running (battery powered) projects.

The ESP supports different DeepSleep modes to reduce power consumption to a minimum.
For more information check this doc:

 ESP8266 -> https://www.espressif.com/sites/default/files/9b-esp8266-low_power_solutions_en_0.pdf

If you google for "ESP deep sleep" there already some really cool examples/projects available,
for example:

 https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep


The microWakeupper board is NOT necessary, if the ESP just has to wakeup based on
a sleep timer - don't forget to connect the pins GPIO16 and RST.

BUT if you want to wakeup (additionally) through an external event (e.g. a switch, button or PIR-sensor)
the microWakeupper makes your life much easier!

---
# How to connect the microWakupper to your ESP?
```
 ESP                microWakeupper (Rev.2)
 VCC (3.3)   <-->   VCC   (mandatory)
 GND         <-->   GND   (mandatory)
 RST         <-->   RST   (mandatory)
 GPIO16      <-->   G16   (optional)
     Not necessary if you don't use a sleep timer,
     or GPIO16 is already connect to RST.
 OUTPUT      <-->   DIS   (optional)
     Just set DIS (disable pin) to HIGH, if you want
     to disable the microWakeupper. This prevents further
     resets as long as your code is running (until going to sleep again).
 INPUT       <-->   STA   (optional)
     This pin is HIGH, if the microWakeupper was triggered
     within the last 2-3 seconds.
```
 (Check also the backside notes directly on the board ;-)

---
# What else should you know?

 + You don't have to take care about debouncing a connected button
 + Recommended voltage is 3.3V
 + You can connect buttons or switches with normally closed or normally opened logic
 + There is a default timeout of 2-3 seconds for retriggering
 + As long as you see the onboard led active, triggering is disabled
 + The jumper on the pcb has to be set to
```
     -> NC for mode "normally closed" or
     -> NO for mode "normally opened"
```

 + If you like, just connect more than one microWakeupper to your ESP (see STA pin to check which one triggered)
 + Current consumption:
 ```
     -> Mode NC     ~3uA // TODO update to Rev.2
     -> Mode NO     ~1nA // TODO update to Rev.2
     -> Active      ~1mA (feel free to remove the onboard LED ;-)
```
---
# The latest board revision is available on tindie:
https://www.tindie.com/stores/moreiolabs/
Rev.2 should be available in a few days.
Or you order the (naked) pcbs at Aisler: https://aisler.net/p/KFXIJRBH

//Todo add gerber files to repo
---
# You'll always find the latest schematic and code on GitHub:

https://github.com/tstoegi/microWakeupper

The example code "microWakupperESP" is specially for a NodeMCU ESP-board, because the (second) internal LED (on pin D4) is used. Just remove/modify the relevant lines for other ESP breakouts, e.g. the Wemos D1 mini.

---
A nice pic...
![microWakeupper in action](https://github.com/tstoegi/microWakeupper/blob/master/microWakeupperInAction.jpeg "The microWakeupper in action")

---
That's it!

Have fun and hopefully your project will run for years now ;-)

#Special thanks to @tinyledmatrix and @davedarko !

Created 13 Dec 2018 by Tobias Stöger (@tstoegi, @moreioLabs)  
