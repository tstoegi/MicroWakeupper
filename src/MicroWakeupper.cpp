#include "MicroWakeupper.h"

MicroWakeupper::MicroWakeupper(bool disableAtStartup, int staPin, int disPin)
:   _disableAtStartup(disableAtStartup),
_sta_pin(staPin),
_dis_pin(disPin)
{
}

void MicroWakeupper::begin()
{
    pinMode(_sta_pin, INPUT);                       // Initialize the status pin as input

    bool staIsHigh = digitalRead(_sta_pin) == HIGH;

    // Load previous state from RTC memory
    _loadStateFromRTC();

    // Determine if this was a MicroWakeupper trigger
    if (_rtcData.magic == RTC_MAGIC && _rtcData.wasInSleep) {
        // We were in deep sleep - check if MicroWakeupper triggered
        // Only true if: STA is now HIGH and was NOT active before sleep
        _switchReset = staIsHigh && !_rtcData.mwWasActive;
    } else {
        // Cold boot, power-on, or hardware reset (no valid RTC data)
        // Cannot be a MicroWakeupper wake from deep sleep
        _switchReset = false;
    }

    // Clear the RTC marker (will be set again in reenable() before next sleep)
    _rtcData.wasInSleep = false;
    _saveStateToRTC();

    pinMode(_dis_pin, OUTPUT);                      // Initialize the disable pin as output
    digitalWrite(_dis_pin, _disableAtStartup ? HIGH : LOW);
}

bool  MicroWakeupper::resetedBySwitch()
{
    return _switchReset;
}

void MicroWakeupper::reenable()
{
    // Save current state to RTC memory before sleep
    _rtcData.magic = RTC_MAGIC;
    _rtcData.wasInSleep = true;
    // After reenable() + deepSleep() + power-off, STA will be LOW (hardware reset)
    _rtcData.mwWasActive = false;
    _saveStateToRTC();

    digitalWrite(_dis_pin, LOW);
}

void MicroWakeupper::disable()
{
    digitalWrite(_dis_pin, HIGH);
}

bool MicroWakeupper::isEnabled()
{
    return !isActive();
}

bool MicroWakeupper::isActive()
{
    return digitalRead(_sta_pin) == HIGH;   // We have been triggereed (LED is on)
}

void MicroWakeupper::setVoltageDivider(float voltageDivider)
{
    _voltageDivider = voltageDivider;
}

float MicroWakeupper::readVBatt()
{
    int sensorValue = analogRead(A0);
    return sensorValue / _voltageDivider;
}

void MicroWakeupper::_saveStateToRTC()
{
    ESP.rtcUserMemoryWrite(0, (uint32_t*)&_rtcData, sizeof(_rtcData));
}

void MicroWakeupper::_loadStateFromRTC()
{
    ESP.rtcUserMemoryRead(0, (uint32_t*)&_rtcData, sizeof(_rtcData));
}
