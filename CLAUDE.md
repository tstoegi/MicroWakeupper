# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MicroWakeupper is an Arduino library for ESP8266 (Wemos D1 Mini) that enables battery-powered devices to wake from deep sleep or power-off states when triggered by external events (switches, buttons, PIR sensors). The library interfaces with a custom hardware shield that sits on top of the Wemos D1 Mini.

## Build and Development

This is an Arduino library. To use:
1. Place the library folder in Arduino's `libraries/` directory or install via Library Manager
2. Include with `#include <MicroWakeupper.h>`
3. Compile for ESP8266 target (Wemos D1 Mini board in Arduino IDE)

No tests or linting infrastructure exists - this is a minimal embedded library.

## Architecture

### Core Files
- `src/MicroWakeupper.h` - Class definition with configurable pins (STA_PIN=D6, DIS_PIN=D7)
- `src/MicroWakeupper.cpp` - Implementation (~55 lines): GPIO operations and ADC voltage reading
- `examples/MicroWakeupperDemo/MicroWakeupperDemo.ino` - Reference implementation showing typical workflow

### API Pattern
```cpp
MicroWakeupper mw;           // Create instance (disables triggering at startup by default)
mw.begin();                  // Initialize GPIO
mw.resetedBySwitch();        // Returns true if woken by external trigger vs internal timer
mw.readVBatt();              // Read battery voltage via A0 with voltage divider
mw.reenable();               // Re-arm for new triggers (has 2-3 second hardware delay)
ESP.deepSleep(microseconds); // Enter deep sleep until next trigger
```

### Hardware Integration
- Default pins: STA_PIN (D6), DIS_PIN (D7), battery voltage on A0
- Voltage divider default: 187 (for 220K-A0-220K-ADC-100K-GND circuit)
- Supports 2.5V-6V input (4xAA eneloop or 3.7V LiPo)

### Supporting Files
- `library.properties` - Arduino library metadata (name, version, architecture)
- `keywords.txt` - Arduino IDE syntax highlighting
- `pcb/` - Hardware schematics and Gerber files for the shield
