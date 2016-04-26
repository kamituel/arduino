/**
 * Plays a melody specified in a RTTTL format
 * (https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language).
 * 
 */

// czarny zielony bialy szary-ciemny szary-jasny
// niebieski niepodpiety

// For light meter
#include "BH1750.h"

// For energy efficient sleep
#include "LowPower.h"

#include "Notes.h"
#include "Melodies.h" 

//#define DEBUG
#define SPEAKER_PIN 9
#define OCTAVE_OFFSET 0

/* Light threshold in the box will be lower */
#ifdef DEBUG
#define LIGHT_THRESHOLD 80
#endif
#ifndef DEBUG
#define LIGHT_THRESHOLD 80
#endif

BH1750 light_meter;

uint16_t read_light_level() {
  // In this mode, light meter will automatically
  // move to power down mode after measurement.
  // No need to do this manually.
  light_meter.configure(BH1750_ONE_TIME_LOW_RES_MODE); 
  return light_meter.readLightLevel();
}

// See: http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/
void sleep() {
  LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
}

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  // Initially, light meter will be in power down mode.
  // We will power it up before each measurement in order
  // to save power.
  light_meter.begin(BH1750_POWER_DOWN);

  // Sleep for a bit, to avoid incorrect initial light level readout.
  delay(1000);
}

int melody = -1;
bool just_played = false;

void loop(void) {
  uint16_t light_level = read_light_level();

#ifdef DEBUG
  Serial.print("Light level: ");
  Serial.println(light_level);
#endif

  if (light_level > LIGHT_THRESHOLD) {
    melody++;

    if (melodies[melody] == 0) {
      melody = 0;
    }

#ifdef DEBUG
    Serial.println("Dzwonek dzwoni");
#endif
    play_rttl(melodies[melody]);
    delay(1000);

    // Sometimes the next readout, after the previous one, is high, even
    // though the light is already off. This causes melody to play second time.
    // To avoid this, read and discard the next light level readout.
    // TODO: This workaround seems not to work 100% of the time.
    read_light_level();
    delay(1000);
  } else {
    sleep();
  }
}

void play_rttl(int* melody) {
  int bpm = *melody++;

#ifdef DEBUG
  Serial.print("\nBPM="); Serial.print(bpm); Serial.println("");
#endif

  // Duration of one full note.
  long full_note = (60 * 1000L / bpm) * 4;
  int octave;
  int duration;
  int pitch;
  bool dotted;

  long duration_msec;
  byte note;

  while (*melody) {
      octave = DECODE_OCTAVE(*melody);
      pitch = DECODE_PITCH(*melody);
      duration = DECODE_DURATION(*melody);
      dotted = DECODE_DOTTED(*melody);

#ifdef DEBUG
      Serial.print("octave="); Serial.print(octave); Serial.print(" ");
      Serial.print("duration="); Serial.print(duration); Serial.print(" ");
      Serial.print("dotted="); Serial.print(dotted); Serial.print(" ");
      Serial.print("pitch=");
      switch (pitch) {
        case P_P:       Serial.print("p"); break;
        case P_A:       Serial.print("a"); break;
        case P_A_POUND: Serial.print("a#"); break;
        case P_B:       Serial.print("b"); break;
        case P_C:       Serial.print("c"); break;
        case P_C_POUND: Serial.print("c#"); break;
        case P_D:       Serial.print("d"); break;
        case P_D_POUND: Serial.print("d#"); break;
        case P_E:       Serial.print("e"); break;
        case P_F:       Serial.print("f"); break;
        case P_F_POUND: Serial.print("f#"); break;
        case P_G:       Serial.print("g"); break;
        case P_G_POUND: Serial.print("g#"); break;
        default:        Serial.print("?"); break;
      }
      Serial.print(" ");
      Serial.print("value="); Serial.print(*melody); Serial.println("");
#endif

      // Duration as decoded is 1, 2, 4, 8, 16 or 32.
      duration_msec = full_note / duration;

      if (dotted) {
        duration_msec += duration_msec / 2;
      }

      switch (pitch) {
        case P_P:       note = 0;  break;
        case P_A:       note = 10; break;
        case P_A_POUND: note = 11; break;
        case P_B:       note = 12; break;
        case P_C:       note = 1;  break;
        case P_C_POUND: note = 2;  break;
        case P_D:       note = 3; break;
        case P_D_POUND: note = 4; break;
        case P_E:       note = 5; break;
        case P_F:       note = 6; break;
        case P_F_POUND: note = 7; break;
        case P_G:       note = 8; break;
        case P_G_POUND: note = 9; break;
        default:        note = 0; break; // pause on unknown value
      }

      octave += OCTAVE_OFFSET;

      if (note) {
        tone(SPEAKER_PIN, notes[(octave - 4) * 12 + note], duration_msec);
        delay(duration_msec);
        noTone(SPEAKER_PIN);
      } else {
        delay(duration_msec);
      }

      melody++;
  }
}

