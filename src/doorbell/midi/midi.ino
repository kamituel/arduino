/**
 *  Plays the Super Mario theme using speaker.
 *  Based on the code found on the internet.
 *  
 *  To play, send any character over serial port.
 */

#include "Notes.h"

// Pin to drive the speaker
#define SPEAKER_PIN 9

int mario_melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

int mario_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    Serial.read();

    int size = sizeof(mario_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      // To calculate the note duration, take one second divided by the note type.
      //  e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / mario_tempo[thisNote] * 1.3;  // * 1.3 because I run it on 8 Mhz Arduino, not 16 Mhz
      tone(SPEAKER_PIN, mario_melody[thisNote] / 4, noteDuration);
      // buzz(SPEAKER_PIN, melody[thisNote] / 3, noteDuration);
 
      // To distinguish the notes, set a minimum time between them.
      // The note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // Stop the tone playing:
      // buzz(SPEAKER_PIN, 0, noteDuration);
      noTone(SPEAKER_PIN);
    }
  }
}

// I don't use it anymore, because it's easier  to just use tone() and noTone() provided in the Arduino library.
/*
void buzz(int targetPin, long frequency, long length) {
    //digitalWrite(targetPin, HIGH);
    long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
    long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
    //// multiply frequency, which is really cycles per second, by the number of seconds to
    //// get the total number of cycles to produce
    for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
      digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
      delayMicroseconds(delayValue); // wait for the calculated delay value
      digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
      delayMicroseconds(delayValue); // wait again or the calculated delay value
    }
    //digitalWrite(targetPin, LOW);
}
*/
