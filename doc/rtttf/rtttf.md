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

Sample code is in [src/doorbell/rtttf]().

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

Sample code is in [src/doorbell/rtttf_bin]().

### RTTTF string to C array conversion in Javascript

For the reference, the page source:

```javascript
var map_binary = function (duration, pitch, dotted, octave) {
    var pitch_bin;
    switch (pitch) {
        case "p":
            pitch_bin = 0;
            break;
        case "a":
            pitch_bin = 1;
            break;
        case "a#":
            pitch_bin = 2;
            break;
        case "b":
            pitch_bin = 3;
            break;
        case "c":
            pitch_bin = 4;
            break;
        case "c#":
            pitch_bin = 5;
            break;
        case "d":
            pitch_bin = 6;
            break;
        case "d#":
            pitch_bin = 7;
            break;
        case "e":
            pitch_bin = 8;
            break;
        case "f":
            pitch_bin = 9;
            break;
        case "f#":
            pitch_bin = 10;
            break;
        case "g":
            pitch_bin = 11;
            break;
        case "g#":
            pitch_bin = 12;
            break;
        default:
            throw Error('Unknown pitch: ' + pitch);
    };

    var dotted_bin = dotted ? 1 : 0;
    var octave_bin = octave - 4;

    return duration | (octave_bin << 5) | (dotted_bin << 7) | (pitch_bin << 8);
};

var map_array = function (duration, pitch, dotted, octave) {
    return [duration, pitch, dotted, octave];
};

var note_to_binary = function (default_duration, default_octave, map_fn) {
    return function (note_str) {
        var note_parts = note_str.match(/^(\d{1,2})?([abcdefgp]\#?)(\.)?([4567])?$/);

        if (!note_parts) throw Error('Invalid note: ' + note_str);

        var duration = note_parts[1] || default_duration,
            pitch = note_parts[2],
            dotted = note_parts[3] ? true : false,
            octave = note_parts[4] || default_octave;

        return map_fn(Number(duration), pitch, dotted, Number(octave), note_str);
    };
};

document.querySelector('#rtttf').onkeyup = function (evt) {
    var out = document.querySelector('#binary');
    
    try {
        var melody = evt.target.value.trim(),
            default_duration = melody.match(/d=(\d+)/)[1],
            default_octave = melody.match(/o=(\d+)/)[1],
            bpm = melody.match(/b=(\d+)/)[1],
            name = melody.match(/^(.*?):.*/)[1]
                         .replace(/[^a-zA-Z0-9-_]/, '')
                         .replace('-', '_')
                         .toLowerCase(),
            notes = melody.split(':')[2].split(',')
                .map(note_to_binary(default_duration,
            default_octave,
            map_binary));

        out.value = 'int ' + name + '[] = {' + bpm + ', ' + notes.join(', ') + ', ' + 0 +  '};';
    } catch (e) {
        out.value = 'Invalid format: ' + e.message;
    }
};
```


## Notes

Both programs require a [`Notes.h`]() header that specifies the music notes' frequencies.
