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

