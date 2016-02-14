// *************************
// ****  IMPORTANT  *******
// *************************
// Use this page to convert RTTTF string to an array:
// https://jsfiddle.net/kamituel/mb8adxrk/8/embedded/result/

/** 
 * How to save RTTTL in low memory? 
 * - duration:      6 values:  3 bit:  1, 2, 4, 8, 16, 32
 * - pitch:         13 values: 4 bits: P, A, A#, B, C, C#, D, D#, E, F, F#, G, G#
 * - octave:        4 values:  2 bits: 4, 5, 6, 7
 * - dotted rhythm: 1 value:   1 bit
 * 
 * Let's agree on the following format:
 *   1) Duration saved on bits 1-6 (no mapping, values stored directly, because we won't fit in one byte anyway)
 *   2) Octave saved on bits 7-8
 *      4 -> 0
 *      5 -> 1
 *      6 -> 2
 *      7 -> 3
 *   3) Dotted rhythm saved on bit 9
 *      not dotted -> 0
 *      dotted     -> 1 
 *   4) Pitch saved on bits 10-13
 *      P  -> 0
 *      A  -> 1
 *      A# -> 2
 *      B  -> 3
 *      C  -> 4
 *      C# -> 5
 *      D  -> 6
 *      D# -> 7
 *      E  -> 8
 *      F  -> 9
 *      F# -> 10
 *      G  -> 11
 *      G# -> 12
 *   
 *   So we need two bytes. 'int' on Arduino has 2 bytes.
 */

#define DECODE_DURATION(n) (n & 63)
#define D_1  1
#define D_2  2
#define D_4  4
#define D_8  8
#define D_16 16
#define D_32 32

#define OCTAVE(o) (o << 6)
#define DECODE_OCTAVE(n) (((n >> 6) & 3) + 4)
#define O_4 OCTAVE(0)
#define O_5 OCTAVE(1)
#define O_6 OCTAVE(2)
#define O_7 OCTAVE(3)

#define PITCH(p) (p << 9)
#define DECODE_PITCH(n) (n & (0xFF << 9))
#define P_P       PITCH(0)
#define P_A       PITCH(1)
#define P_A_POUND PITCH(2)
#define P_B       PITCH(3)
#define P_C       PITCH(4)
#define P_C_POUND PITCH(5)
#define P_D       PITCH(6)
#define P_D_POUND PITCH(7)
#define P_E       PITCH(8)
#define P_F       PITCH(9)
#define P_F_POUND PITCH(10)
#define P_G       PITCH(11)
#define P_G_POUND PITCH(12)

#define DECODE_DOTTED(n) (n & (1 << 8))
#define DOTTED    (1 << 8)

// "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
/*int a_team[] = {125,
                (D_4  | P_D_POUND | O_6),
                (D_8  | P_A_POUND | O_5),
                (D_2  | P_D_POUND | O_6),
                (D_16 | P_P       | O_5),
                (D_8  | P_G_POUND | O_5),
                (D_4  | P_A_POUND | O_5),
                (D_4  | P_D_POUND | O_5 | DOTTED),
                (D_8  | P_P       | O_5),
                (D_16 | P_G       | O_5),
                (D_16 | P_A_POUND | O_5),
                (D_8  | P_D_POUND | O_6),
                (D_8  | P_A_POUND | O_5),
                (D_8  | P_F       | O_6),
                (D_2  | P_D_POUND | O_6),
                (D_16 | P_P       | O_5),
                (D_8  | P_C_POUND | O_6 | DOTTED),
                (D_16 | P_C       | O_6),
                (D_16 | P_A_POUND | O_5),
                (D_8  | P_G_POUND | O_5 | DOTTED),
                (D_2  | P_A_POUND | O_5),
                0};*/

int bond[] = {80, 96, 2704, 3744, 3744, 3728, 3720, 2704, 2704, 2704, 2704, 4256, 4256, 4240, 4232, 3728, 3728, 3728, 2704, 3744, 3744, 3728, 3720, 2704, 2704, 2704, 2704, 4256, 4256, 4240, 4232, 3728, 3216, 2704, 2768, 2500, 6288, 5264, 6532, 0};
int starwars[] = {45, 96, 5216, 5216, 5216, 1864, 5512, 4256, 3744, 2720, 1928, 5520, 4256, 3744, 2720, 1928, 5520, 4256, 3744, 4256, 2952, 5216, 5216, 5216, 1864, 5512, 4256, 3744, 2720, 1928, 5520, 4256, 3744, 2720, 1928, 5520, 4256, 3744, 4256, 2696, 0};
int a_team[] = {125, 3716, 1096, 3714, 80, 6216, 1092, 3908, 72, 5712, 1104, 3720, 1096, 4744, 3714, 80, 2952, 2192, 1104, 6472, 1090, 0};
int smurfs[] = {200, 2692, 80, 5252, 96, 2704, 96, 3720, 96, 1608, 96, 6212, 80, 2692, 96, 1104, 96, 5192, 96, 1096, 96, 6212, 68, 6240, 96, 1120, 96, 1632, 96, 2208, 96, 2692, 80, 5252, 96, 2704, 96, 3720, 96, 1608, 96, 6212, 80, 2692, 96, 1104, 96, 1608, 96, 4680, 96, 5188, 0};
int axelf[] = {160, 5188, 840, 5192, 5200, 1096, 5192, 4168, 5188, 2440, 5192, 5200, 3208, 2696, 584, 5192, 2696, 5256, 5200, 4168, 4176, 2632, 6216, 5444, 0};

int lacucarac[] = {127, 2116, 2116, 2116, 4680, 584, 132, 2116, 2116, 2116, 4680, 584, 132, 4676, 4676, 4164, 4164, 3140, 3140, 2120, 132, 132, 2116, 2116, 2116, 4168, 5704, 132, 2116, 2116, 2116, 4168, 5704, 132, 2180, 3204, 2180, 836, 580, 5700, 4680, 0};

int *melodies[] = {/*lacucarac, */bond, starwars, a_team, smurfs, axelf, 0};

