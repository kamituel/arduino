/**
 * Plays a melody specified in a RTTTL format
 * (https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language).
 * 
 */

#include "Notes.h"
#include "Melodies.h" 

#define DEBUG
#define SPEAKER_PIN 9
#define OCTAVE_OFFSET 0

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop(void) {
  int melody = 0;
  while (true) {
    if (melodies[melody] == 0) {
      melody = 0;
    }
    
    play_rttl(melodies[melody]);
    delay(1000);

    melody++;
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

