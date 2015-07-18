# WAV audio playback from SD card

## Prerequisities

The TMRpcm library reads audio files from an SD card. You need to make sure that
your SD card reader and SD card itself is working properly before attempting
to play audio files from it.

Micro-SD reader setup [is covered here](../sd-slot-pololu/pololu-sd.md).

## Converting audio files

```bash
ffmpeg -i voice01.m4a \              # input file
       -ac 1 \                       # mono
       -ar 8000 \                    # 8 kHz sample rate (TMRpcm supports 8-32 kHz)
       -acodec pcm_s16le             # 16 bit quantization
       voice01.wav                   # output file
```

[Supported PCM quantization options](https://trac.ffmpeg.org/wiki/audio%20types):

```bash
$ ffmpeg -formats | grep PCM
ffmpeg version N-44123-g5d55830 Copyright (c) 2000-2012 the FFmpeg developers
 DE alaw            PCM A-law
 DE f32be           PCM 32-bit floating-point big-endian
 DE f32le           PCM 32-bit floating-point little-endian
 DE f64be           PCM 64-bit floating-point big-endian
 DE f64le           PCM 64-bit floating-point little-endian
 DE mulaw           PCM mu-law
 DE s16be           PCM signed 16-bit big-endian
 DE s16le           PCM signed 16-bit little-endian
 DE s24be           PCM signed 24-bit big-endian
 DE s24le           PCM signed 24-bit little-endian
 DE s32be           PCM signed 32-bit big-endian
 DE s32le           PCM signed 32-bit little-endian
 DE s8              PCM signed 8-bit
 DE u16be           PCM unsigned 16-bit big-endian
 DE u16le           PCM unsigned 16-bit little-endian
 DE u24be           PCM unsigned 24-bit big-endian
 DE u24le           PCM unsigned 24-bit little-endian
 DE u32be           PCM unsigned 32-bit big-endian
 DE u32le           PCM unsigned 32-bit little-endian
 DE u8              PCM unsigned 8-bit
```
Not all of those are supported for WAV though.

## Connecting speaker

TODO

## Code sample

TODO
