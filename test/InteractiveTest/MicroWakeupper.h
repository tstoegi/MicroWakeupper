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
#define RTC_MAGIC                     0x4D575550  // "MWUP" - MicroWakeupper marker

// RTC Memory structure to persist state across deep sleep
struct MicroWakeupperRTCData {
    uint32_t magic;         // Magic number to validate data
    bool     wasInSleep;    // Was in deep sleep
    bool     mwWasActive;   // MicroWakeupper was active before sleep
};

class MicroWakeupper
{
public:
    MicroWakeupper(
                   bool disableAtStartup = true, // Prevent triggering during ESP runtime
                   int staPin = STA_PIN_DEFAULT, // Override status PIN (D6)
                   int disPin = DIS_PIN_DEFAULT  // Override disable PIN (D7)
    );
    void    begin();        // Initialisation
    
    void    reenable();     // Call before deepSleep()! Reenables triggering (2-3s hardware delay) and saves state for resetedBySwitch()
    void    disable();      // Preventing new triggering/resets
    bool    isEnabled();    // Ready for new triggering/resets
    bool    isActive();     // MicroWakeupper is active (LED) - there was a trigger/reset
    bool    resetedBySwitch();  // true = MicroWakeupper trigger, false = timer/reset/cold boot (requires reenable() before deepSleep)
    
    void    setVoltageDivider(float voltageDivider);    // A0 read (ADO max 1024) - check VOLTAGEDIVIDER_DEFAULT
    float   readVBatt(); // calculated battery voltage (based on A0 divided by _voltageDivider)
    
private:
    bool _disableAtStartup;
    int _sta_pin;
    int _dis_pin;
    bool _switchReset;
    float _voltageDivider = VOLTAGEDIVIDER_DEFAULT;
    MicroWakeupperRTCData _rtcData;

    void _saveStateToRTC();
    void _loadStateFromRTC();
    
};

#endif
