#include <Wire.h>
#include "BH1750.h"

BH1750 light_meter;

uint16_t read_light_level() {
  // In this mode, light meter will automatically
  // move to power down mode after measurement.
  // No need to do this manually.
  light_meter.configure(BH1750_ONE_TIME_LOW_RES_MODE);
  return light_meter.readLightLevel();
}

void setup() {
  Serial.begin(9600);

  // Initially, light meter will be in power down mode.
  // We will power it up before each measurement in order
  // to save power.
  light_meter.begin(BH1750_POWER_DOWN);
}

void loop() {
  Serial.print("Alive!\n");

  uint16_t light_level = read_light_level();
  Serial.print("Light level: ");
  Serial.print(light_level);
  Serial.print("\n");

  delay(500);
}

