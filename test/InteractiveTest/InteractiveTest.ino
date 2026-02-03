/*
  MicroWakeupper Interactive Test

  This sketch helps you test all MicroWakeupper functions.
  Open Serial Monitor at 115200 baud and follow the instructions.

  Test scenarios:
  1. Cold Boot (power on) -> resetedBySwitch() should be FALSE
  2. Deep Sleep -> Timer wakeup -> resetedBySwitch() should be FALSE
  3. Deep Sleep -> MicroWakeupper trigger -> resetedBySwitch() should be TRUE
  4. Hardware Reset button -> resetedBySwitch() should be FALSE

  (c) 2026 Tobias Stöger (@tstoegi)
*/

// To test installed library instead of local version, uncomment this line:
// #define USE_INSTALLED_LIBRARY

#ifdef USE_INSTALLED_LIBRARY
  #include <MicroWakeupper.h>  // Installed Arduino library
#else
  #include "MicroWakeupper.h"  // Local version from src/
#endif

MicroWakeupper mw;

#define SLEEP_SECONDS 15

// Read RTC data directly for debugging
struct DebugRTCData {
  uint32_t magic;
  bool wasInSleep;
  bool mwWasActive;
};

void printRTCDebug(const char* label) {
  DebugRTCData rtc;
  ESP.rtcUserMemoryRead(0, (uint32_t*)&rtc, sizeof(rtc));

  Serial.print("[RTC ");
  Serial.print(label);
  Serial.print("] magic=");
  Serial.print(rtc.magic == 0x4D575550 ? "VALID" : "INVALID");
  Serial.print(", wasInSleep=");
  Serial.print(rtc.wasInSleep ? "true" : "false");
  Serial.print(", mwWasActive=");
  Serial.println(rtc.mwWasActive ? "true" : "false");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("========================================");
  Serial.println("   MicroWakeupper Interactive Test");
  Serial.println("========================================");
  Serial.println();

  // Debug: Show RTC data BEFORE begin()
  printRTCDebug("BEFORE begin()");

  // Initialize MicroWakeupper
  mw.begin();

  // Debug: Show RTC data AFTER begin()
  printRTCDebug("AFTER begin()");
  Serial.println();

  // Print all debug info
  printDebugInfo();

  // Print test result
  printTestResult();

  // Print battery voltage
  Serial.println();
  Serial.print("Battery Voltage: ");
  Serial.print(mw.readVBatt());
  Serial.println(" V");

  // Instructions for next test
  printNextSteps();
}

void loop() {
  // Check if user sent command via Serial
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 's' || cmd == 'S') {
      // Start deep sleep WITH reenable
      Serial.println();
      Serial.println(">>> Preparing for Deep Sleep...");
      Serial.println(">>> Calling reenable() to save state and enable triggers");

      mw.reenable();

      Serial.print(">>> Sleeping for ");
      Serial.print(SLEEP_SECONDS);
      Serial.println(" seconds...");
      Serial.println(">>> You can now:");
      Serial.println(">>>   - Wait for timer wakeup (resetedBySwitch = FALSE)");
      Serial.println(">>>   - Trigger MicroWakeupper switch (resetedBySwitch = TRUE)");
      Serial.println(">>>   - Press hardware reset button (resetedBySwitch = FALSE)");
      Serial.println();
      Serial.flush();

      delay(100);
      ESP.deepSleep(SLEEP_SECONDS * 1000000);
    }
    else if (cmd == 'n' || cmd == 'N') {
      // Start deep sleep WITHOUT reenable (for Test 4)
      Serial.println();
      Serial.println(">>> Preparing for Deep Sleep...");
      Serial.println(">>> NOT calling reenable() - MicroWakeupper stays disabled");

      Serial.print(">>> Sleeping for ");
      Serial.print(SLEEP_SECONDS);
      Serial.println(" seconds...");
      Serial.println(">>> You can now:");
      Serial.println(">>>   - Wait for timer wakeup (resetedBySwitch = FALSE)");
      Serial.println(">>>   - Press hardware reset button (resetedBySwitch = FALSE)");
      Serial.println(">>>   - Switch trigger will NOT work (MW disabled)");
      Serial.println();
      Serial.flush();

      delay(100);
      ESP.deepSleep(SLEEP_SECONDS * 1000000);
    }
    else if (cmd == 'r' || cmd == 'R') {
      // Re-print debug info
      Serial.println();
      printDebugInfo();
      printTestResult();
    }
    else if (cmd == 'd' || cmd == 'D') {
      // Disable MicroWakeupper
      mw.disable();
      Serial.println(">>> MicroWakeupper DISABLED");
    }
    else if (cmd == 'e' || cmd == 'E') {
      // Enable MicroWakeupper (without sleep)
      mw.reenable();
      Serial.println(">>> MicroWakeupper REENABLED (state saved to RTC)");
    }
  }

  delay(100);
}

void printDebugInfo() {
  Serial.println("--- Debug Info ---");

  Serial.print("ESP Reset Reason: ");
  Serial.println(ESP.getResetReason());

  Serial.print("STA_PIN (D6) state: ");
  Serial.println(mw.isActive() ? "HIGH (active)" : "LOW (inactive)");

  Serial.print("MicroWakeupper isActive(): ");
  Serial.println(mw.isActive() ? "true" : "false");

  Serial.print("MicroWakeupper isEnabled(): ");
  Serial.println(mw.isEnabled() ? "true" : "false");

  Serial.println();
}

void printTestResult() {
  Serial.println("--- Test Result ---");

  bool result = mw.resetedBySwitch();

  Serial.print("resetedBySwitch() = ");
  Serial.println(result ? "TRUE" : "FALSE");

  Serial.println();

  if (result) {
    Serial.println(">>> DETECTED: MicroWakeupper Trigger");
    Serial.println("    The ESP was woken from deep sleep by the MicroWakeupper.");
  } else {
    String reason = ESP.getResetReason();
    if (reason == "Deep-Sleep Wake") {
      Serial.println(">>> DETECTED: Timer Wakeup");
      Serial.println("    The ESP woke from deep sleep by internal timer.");
    } else if (reason == "External System") {
      Serial.println(">>> DETECTED: Hardware Reset or Cold Boot");
      Serial.println("    The ESP was reset externally (button or power-on).");
    } else {
      Serial.print(">>> DETECTED: Other (");
      Serial.print(reason);
      Serial.println(")");
    }
  }

  Serial.println();
}

void printNextSteps() {
  Serial.println("--- Commands ---");
  Serial.println("Send via Serial Monitor:");
  Serial.println("  's' = Start Deep Sleep WITH reenable (MW enabled)");
  Serial.println("  'n' = Start Deep Sleep WITHOUT reenable (MW disabled)");
  Serial.println("  'r' = Re-print debug info");
  Serial.println("  'd' = Disable MicroWakeupper");
  Serial.println("  'e' = Enable MicroWakeupper");
  Serial.println();
  Serial.println("--- Test Scenarios ---");
  Serial.println("1. Power off and on -> should show FALSE");
  Serial.println("2. Press 's', wait for timer -> should show FALSE");
  Serial.println("3. Press 'e', then 's', trigger switch -> should show TRUE");
  Serial.println("4. Press 'n', press reset button -> should show FALSE");
  Serial.println();
  Serial.println("========================================");
  Serial.println();
}
