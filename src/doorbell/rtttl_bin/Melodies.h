// *************************
// ****  IMPORTANT  *******
// *************************
// Use this page to convert RTTTF string to an array:
// https://jsfiddle.net/kamituel/mb8adxrk/6/embedded/result/

/** 
 * How to save RTTTL in low memory? 
 * - duration:      6 values:  3 bit:  1, 2, 4, 8, 16, 32
 * - pitch:         13 values: 4 bits: P, A, A#, B, C, C#, D, D#, E, F, F#, G, G#
 * - octave:        4 values:  2 bits: 4, 5, 6, 7
 * - dotted rhythm: 1 value:   1 bit
 * 
 * Let's agree on the following format:
 *   1) Duration saved on bits 1-5 (no mapping, values stored directly, because we won't fit in one byte anyway)
 *   2) Octave saved on bits 6-7
 *      4 -> 0
 *      5 -> 1
 *      6 -> 2
 *      7 -> 3
 *   3) Dotted rhythm saved on bit 8
 *      not dotted -> 0
 *      dotted     -> 1 
 *   4) Pitch saved on bits 9-12
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

#define DECODE_DURATION(n) (n & 31)
#define D_1  1
#define D_2  2
#define D_4  4
#define D_8  8
#define D_16 16
#define D_32 32

#define OCTAVE(o) (o << 5)
#define DECODE_OCTAVE(n) (((n >> 5) & 3) + 4)
#define O_4 OCTAVE(0)
#define O_5 OCTAVE(1)
#define O_6 OCTAVE(2)
#define O_7 OCTAVE(3)

#define PITCH(p) (p << 8)
#define DECODE_PITCH(n) (n & (0xFF << 8))
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

#define DECODE_DOTTED(n) (n & (1 << 7))
#define DOTTED    (1 << 7)

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

int a_team[] = {125, 1860, 552, 1858, 48, 3112, 548, 1956, 40, 2864, 560, 1864, 552, 2376, 1858, 48, 1480, 1104, 560, 3240, 546, 0};

