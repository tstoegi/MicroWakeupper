#ifndef MicroWakeupper_h
#define MicroWakeupper_h

#include <Arduino.h>

/* MicroWakeupper Library for Arduino (ESP8266, Wemos D1 Mini Shield)
 *
 * (C) 2021 Tobias Stöger (@tstoegi)
 *
 */

#define STA_PIN_DEFAULT               D6  // D6 connected to STA pin (Wemos D1 Mini Shield)
#define DIS_PIN_DEFAULT               D7  // D7 connected to DIS pin (Wemos D1 Mini Shield)
#define VOLTAGEDIVIDER_DEFAULT        187 // VBatt-220K-A0-220K-ADC-100K-GND (see setVoltageDivider)

class MicroWakeupper
{
public:
    MicroWakeupper(
                   bool disableAtStartup = true, // Prevent triggering during ESP runtime
                   int staPin = STA_PIN_DEFAULT, // Override status PIN (D6)
                   int disPin = DIS_PIN_DEFAULT  // Override disable PIN (D7)
    );
    void    begin();        // Initialisation
    
    void    reenable();     // Reenabling with a 2-3 seconds delay (hardware based)
    void    disable();      // Preventing new triggering/resets
    bool    isEnabled();    // Ready for new triggering/resets
    bool    isActive();     // MicroWakeupper is active (LED) - there was a trigger/reset
    bool    resetedBySwitch();  // false -> it was a internal timer reset
    
    void    setVoltageDivider(float voltageDivider);    // A0 read (ADO max 1024) - check VOLTAGEDIVIDER_DEFAULT
    float   readVBatt(); // calculated battery voltage (based on A0 divided by _voltageDivider)
    
private:
    bool _disableAtStartup;
    int _sta_pin;
    int _dis_pin;
    int _switchReset;
    float _voltageDivider = VOLTAGEDIVIDER_DEFAULT;
    
};

#endif
