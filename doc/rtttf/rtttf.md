# RTTTF ringtones

RTTTF ringtones are the ringtones that were used in the past on mobile phones.

## Format

See [an article on Wikipedia](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language).

## Naive implementation

The naive implementation of RTTTF playback involves parsing a string, and then playing
a specific tone according to the values read.

This has a drawback, though, because string consumes quite a lot of space. If you need
to play only a few of ringtones, and your Arduino project is simple, it won't be an
issue.

If your Aruino project is more complex, or you want to store multiple ringtones in the
memory, this solution is not the optimal one.

This naive solution is based on a code from [this Instructable](http://www.instructables.com/id/Aruino-Tone-RTTL-Player/?ALLSTEPS).

** Melodies.h **

```c
char *a_team       = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";

```

** rtttf.ino **

```c
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
#define IS_DIGIT(n) (n >= '0' && n <= '9')
#define AS_DIGIT(n) (n - '0')

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop(void) {
 play_rtttl(a_team);
 delay(1000);
}

// num outside, to save some memory.
int num;
void read_number(char*& str, int* out) {
  num = 0;
  while (IS_DIGIT(*str)) {
    num = (num * 10) + AS_DIGIT(*str);
    str++;
  }

  *out = num;
}

/**
 * Reades one value from a header in the form of:
 *   d=4,o=5,b=108
 * After reading, pointer points at the first character after
 * the field read.
 * 
 * str         - string pointer
 * out         - pointer to an int variable to store the output value if found
 * header_name - either 'd', 'o' or 'b'
 */
void read_header_value(char*& str, int* out, char header_name) {
  if (*str == header_name) {
    // Skip header name and '=', i.e. 'd='.
    str += 2;

    // Read digits and convert them to a number.
    int num = 0;
    read_number(str, &num);

    // Skip the character after a number, i.e. a comma.
    str++;

    if (num > 0) {
      *out = num;
    }
  }
}

void play_rtttl(char *p) {
  // Absolutely no error checking in here

  int default_dur = 4;
  int default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  read_header_value(p, &default_dur, 'd');
  read_header_value(p, &default_oct, 'o');
  read_header_value(p, &bpm, 'b');

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

 #ifdef DEBUG
  Serial.print("ddur: "); Serial.println(default_dur, 10);
  Serial.print("doct: "); Serial.println(default_oct, 10);
  Serial.print("bpm: "); Serial.println(bpm, 10);
  Serial.print("wn: "); Serial.println(wholenote, 10);
#endif 

  // Play notes.
  while(*p) {
    num = 0;
    read_number(p, &num);

   // We will need to check if we are a dotted note after.
    if (num) {
      duration = wholenote / num;
    } else {
      duration = wholenote / default_dur;
    }

    // Read a note.
    note = 0;
    switch(*p) {
      case 'c': note = 1; break;
      case 'd': note = 3; break;
      case 'e': note = 5; break;
      case 'f': note = 6; break;
      case 'g': note = 8; break;
      case 'a': note = 10; break;
      case 'b': note = 12; break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // Get optional '#' sharp
    if (*p == '#') {
      note++;
      p++;
    }

    // Get optional '.' dotted note
    if (*p == '.') {
      duration += duration / 2;
      p++;
    }
 
    // Get scale
    if (IS_DIGIT(*p)) {
      scale = AS_DIGIT(*p);
      p++;
    } else {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    // Skip comma for next note (or we may be at the end)
    if (*p == ',') {
      p++;
    }

    // Play the note
    if (note) {
#ifdef DEBUG
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(duration, 10);
#endif

      tone(9, notes[(scale - 4) * 12 + note], duration);
      delay(duration);
      noTone(9);
    } else {
#ifdef DEBUG
      Serial.print("Pausing: ");
      Serial.println(duration, 10);
#endif

      delay(duration);
    }
  }
}

```

## Binary implementation

Binary implementation requires you to convert a RTTTF string to a C array first.
It has the advantage, though, that both program and variables require less space on
Arduino. And the code is simpler.

I've prepared a simple page that aids with the conversion: [here](https://jsfiddle.net/kamituel/mb8adxrk/7/embedded/result/).

For instance, this RTTTF string:

```
A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#
```

Will be converted to:

```c
int a_team[] = {125, 1860, 552, 1858, 48, 3112, 548, 1956, 40, 2864, 560, 1864, 552, 2376, 1858, 48, 1480, 1104, 560, 3240, 546, 0};
```

** rtttf_bin.ino **

```c
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
  play_rttl(a_team);
  delay(1000);
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

```

## Notes

Both programs require a `Notes.h` header that specifies the music notes' frequencies:

** Notes.h **

```c
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int notes[] = { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

```