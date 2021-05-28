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
    
    _switchReset = digitalRead(_sta_pin) == HIGH;   //If the status pin is already HIGH, it was _no_ internal ESP timer reset
    
    pinMode(_dis_pin, OUTPUT);                      // Initialize the disable pin as output
    digitalWrite(_dis_pin, _disableAtStartup ? HIGH : LOW);
}

bool  MicroWakeupper::resetedBySwitch()
{
    return _switchReset;
}

void MicroWakeupper::reenable()
{
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
