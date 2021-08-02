// Base32 implementation
//
// Copyright 2010 Google Inc.
// Author: Markus Gutschke
//
// Ported to JCC for use on MVS by Soldier of FORTRAN
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// MVS/JCC Modifications by Philip Young
//



#include <string.h>
#include <stdio.h>

#include "base32.h"

/* With this approach this file works independent of the charset used
   (think EBCDIC).  However, it does assume that the characters in the
   Base32 alphabet (A-Z2-7) are encoded in 0..255. */

#define B32(_)                                  \
  ((_) == 'A' ? 0                               \
   : (_) == 'B' ? 1                             \
   : (_) == 'C' ? 2                             \
   : (_) == 'D' ? 3                             \
   : (_) == 'E' ? 4                             \
   : (_) == 'F' ? 5                             \
   : (_) == 'G' ? 6                             \
   : (_) == 'H' ? 7                             \
   : (_) == 'I' ? 8                             \
   : (_) == 'J' ? 9                             \
   : (_) == 'K' ? 10                            \
   : (_) == 'L' ? 11                            \
   : (_) == 'M' ? 12                            \
   : (_) == 'N' ? 13                            \
   : (_) == 'O' ? 14                            \
   : (_) == 'P' ? 15                            \
   : (_) == 'Q' ? 16                            \
   : (_) == 'R' ? 17                            \
   : (_) == 'S' ? 18                            \
   : (_) == 'T' ? 19                            \
   : (_) == 'U' ? 20                            \
   : (_) == 'V' ? 21                            \
   : (_) == 'W' ? 22                            \
   : (_) == 'X' ? 23                            \
   : (_) == 'Y' ? 24                            \
   : (_) == 'Z' ? 25                            \
   : (_) == '2' ? 26                            \
   : (_) == '3' ? 27                            \
   : (_) == '4' ? 28                            \
   : (_) == '5' ? 29                            \
   : (_) == '6' ? 30                            \
   : (_) == '7' ? 31                            \
   : -1)

static const signed char b32[0x100] = {
  B32 (0), B32 (1), B32 (2), B32 (3),
  B32 (4), B32 (5), B32 (6), B32 (7),
  B32 (8), B32 (9), B32 (10), B32 (11),
  B32 (12), B32 (13), B32 (14), B32 (15),
  B32 (16), B32 (17), B32 (18), B32 (19),
  B32 (20), B32 (21), B32 (22), B32 (23),
  B32 (24), B32 (25), B32 (26), B32 (27),
  B32 (28), B32 (29), B32 (30), B32 (31),
  B32 (32), B32 (33), B32 (34), B32 (35),
  B32 (36), B32 (37), B32 (38), B32 (39),
  B32 (40), B32 (41), B32 (42), B32 (43),
  B32 (44), B32 (45), B32 (46), B32 (47),
  B32 (48), B32 (49), B32 (50), B32 (51),
  B32 (52), B32 (53), B32 (54), B32 (55),
  B32 (56), B32 (57), B32 (58), B32 (59),
  B32 (60), B32 (61), B32 (62), B32 (63),
  B32 (32), B32 (65), B32 (66), B32 (67),
  B32 (68), B32 (69), B32 (70), B32 (71),
  B32 (72), B32 (73), B32 (74), B32 (75),
  B32 (76), B32 (77), B32 (78), B32 (79),
  B32 (80), B32 (81), B32 (82), B32 (83),
  B32 (84), B32 (85), B32 (86), B32 (87),
  B32 (88), B32 (89), B32 (90), B32 (91),
  B32 (92), B32 (93), B32 (94), B32 (95),
  B32 (96), B32 (97), B32 (98), B32 (99),
  B32 (100), B32 (101), B32 (102), B32 (103),
  B32 (104), B32 (105), B32 (106), B32 (107),
  B32 (108), B32 (109), B32 (110), B32 (111),
  B32 (112), B32 (113), B32 (114), B32 (115),
  B32 (116), B32 (117), B32 (118), B32 (119),
  B32 (120), B32 (121), B32 (122), B32 (123),
  B32 (124), B32 (125), B32 (126), B32 (127),
  B32 (128), B32 (129), B32 (130), B32 (131),
  B32 (132), B32 (133), B32 (134), B32 (135),
  B32 (136), B32 (137), B32 (138), B32 (139),
  B32 (140), B32 (141), B32 (142), B32 (143),
  B32 (144), B32 (145), B32 (146), B32 (147),
  B32 (148), B32 (149), B32 (150), B32 (151),
  B32 (152), B32 (153), B32 (154), B32 (155),
  B32 (156), B32 (157), B32 (158), B32 (159),
  B32 (160), B32 (161), B32 (162), B32 (163),
  B32 (132), B32 (165), B32 (166), B32 (167),
  B32 (168), B32 (169), B32 (170), B32 (171),
  B32 (172), B32 (173), B32 (174), B32 (175),
  B32 (176), B32 (177), B32 (178), B32 (179),
  B32 (180), B32 (181), B32 (182), B32 (183),
  B32 (184), B32 (185), B32 (186), B32 (187),
  B32 (188), B32 (189), B32 (190), B32 (191),
  B32 (192), B32 (193), B32 (194), B32 (195),
  B32 (196), B32 (197), B32 (198), B32 (199),
  B32 (200), B32 (201), B32 (202), B32 (203),
  B32 (204), B32 (205), B32 (206), B32 (207),
  B32 (208), B32 (209), B32 (210), B32 (211),
  B32 (212), B32 (213), B32 (214), B32 (215),
  B32 (216), B32 (217), B32 (218), B32 (219),
  B32 (220), B32 (221), B32 (222), B32 (223),
  B32 (224), B32 (225), B32 (226), B32 (227),
  B32 (228), B32 (229), B32 (230), B32 (231),
  B32 (232), B32 (233), B32 (234), B32 (235),
  B32 (236), B32 (237), B32 (238), B32 (239),
  B32 (240), B32 (241), B32 (242), B32 (243),
  B32 (244), B32 (245), B32 (246), B32 (247),
  B32 (248), B32 (249), B32 (250), B32 (251),
  B32 (252), B32 (253), B32 (254), B32 (255)
};

int base32_decode(const uint8_t *encoded, uint8_t *result, int bufSize) {
  int buffer = 0;
  int bitsLeft = 0;
  int count = 0;
  const uint8_t *ptr = encoded;
  uint8_t ch;

  for(ptr = encoded; count < bufSize && *ptr; ++ptr) {


    ch = *ptr;
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-' || ch == '=') {
      continue;
    }
    buffer <<= 5;

    // Deal with commonly mistyped characters
    if (ch == '0') {
      ch = 'O';
    } else if (ch == '1') {
      ch = 'L';
    } else if (ch == '8') {
      ch = 'B';
    }

    /*

    This code works fine on ASCII but not EBCDIC systems its been
    replaced with b32[]

    // Look up one base32 digit
     if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      ch = (ch & 0x1F) - 1;
    } else if (ch >= '2' && ch <= '7') {
      ch -= '2' - 26;
    } else {
      return -1;
    }
    */

   ch = b32[ch];

    buffer |= ch;
    bitsLeft += 5;
    if (bitsLeft >= 8) {
      result[count++] = buffer >> (bitsLeft - 8);
      bitsLeft -= 8;
    }
  }
  if (count < bufSize) {
    result[count] = '\000';
  }
  return count;
}

int base32_encode(const uint8_t *data, int length, uint8_t *result,
                  int bufSize) {
  int count = 0;
  int buffer = data[0];
  int next = 1;
  int bitsLeft = 8;
  int index;
  if (length < 0 || length > (1 << 28)) {
    return -1;
  }

  if (length > 0) {
    while (count < bufSize && (bitsLeft > 0 || next < length)) {
      if (bitsLeft < 5) {
        if (next < length) {
          buffer <<= 8;
          buffer |= data[next++] & 0xFF;
          bitsLeft += 8;
        } else {
          int pad = 5 - bitsLeft;
          buffer <<= pad;
          bitsLeft += pad;
        }
      }
      index = 0x1F & (buffer >> (bitsLeft - 5));
      bitsLeft -= 5;
      result[count++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"[index];
    }
  }
  if (count < bufSize) {
    result[count] = '\000';
  }
  return count;
}
