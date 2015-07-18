// For light meter
#include <Wire.h>
#include "BH1750.h"

// For SD reader
#include <SPI.h>
#include <SD.h>

// For audio playback (also requires SD and SPI)
#include "TMRpcm.h"

// Chip select pin for SD reader.
#define SD_SS_PIN 10

// Pin to drive the speaker
#define SPEAKER_PIN 9

// Light level that triggers doorbell ring sound.
#define LIGHT_BULB_ON_THRESHOLD 300

BH1750 light_meter;
TMRpcm audio_player;

uint16_t read_light_level() {
  // In this mode, light meter will automatically
  // move to power down mode after measurement.
  // No need to do this manually.
  light_meter.configure(BH1750_ONE_TIME_LOW_RES_MODE);
  return light_meter.readLightLevel();
}

bool init_sd(unsigned int nof_attempts = 10) { 
  delay(1000);

  unsigned int sd_attempts = 0;
  bool sd_init_status;
  while (!(sd_init_status = SD.begin(SD_SS_PIN)) && sd_attempts++ < nof_attempts) {
    Serial.println("SD attempt failed. Let's do that again");
  }

  if (!sd_init_status) {
    Serial.println("SD card failed or not present");
  } else {
    Serial.println("SD card initialized successfuly.");
  }

  return sd_init_status;
}

void setup() {
  Serial.begin(9600);

  // Attempt to initialize SD card.
  init_sd(100);

  audio_player.speakerPin = SPEAKER_PIN;

  // Initially, light meter will be in power down mode.
  // We will power it up before each measurement in order
  // to save power.
  light_meter.begin(BH1750_POWER_DOWN);
}

void read_file(String filename) {
  File dataFile = SD.open(filename);

  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("error opening file");
  }
}

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    Serial.println(ch);

    if (ch == 'f') {
      read_file("a.txt");
    }

    if (ch == 'p') {
      audio_player.play("voice2x.wav");
    }

    if (ch == 'q') {
      audio_player.play("voice2xi.wav");
    }

    if (ch == 's') {
      analogWrite(9, 20);
      delay(200);
      analogWrite(9, 0);
    }

    if (ch == 'l') {
      uint16_t light_level = read_light_level();
      Serial.print("Light level: ");
      Serial.println(light_level);
    }
  }


  
  /*int is_playing = audio_player.isPlaying();
  Serial.print("Is playing audio? ");
  Serial.println(is_playing);

  if (is_playing == 0) {
    //uint16_t light_level = read_light_level();
    //Serial.print("Light level: ");
    //Serial.println(light_level);

    Serial.println("So let's tune in!");
    audio_player.play("voice01.wav");
    delay(1000);

    //if (light_level > LIGHT_BULB_ON_THRESHOLD) {
    //  Serial.println("Doorbell!");
    //  //read_file("a.txt");
    //  audio_player.play("voice01.wav");
    //  Serial.println("played");
    //  delay(1000);
    //}
  }*/

  /*analogWrite(9, v);
  if (v == 0) v = 40;
  else v = v - 1;

  Serial.println(v);
  delay(50);*/
}

