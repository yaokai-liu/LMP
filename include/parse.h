/**
 * License
 *
 * LMP - A Lite Multiple Precision Arithmetic Library
 * Copyright (C) 2026 Yaokai Liu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Project Name: LMP
 * Module Name: include
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-19
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_PARSE_H
#define LMP_PARSE_H
#include "lmp.h"

#ifndef LMP_PARSE_ALLOC_SCALE
#define LMP_PARSE_ALLOC_SCALE 4
#endif
#ifndef LMP_PARSE_EXP_MAX
#define LMP_PARSE_EXP_MAX 2048
#endif
#ifndef LMP_LITERAL_PREC_MAX
#define LMP_LITERAL_PREC_MAX 2048
#endif

// Layout in case of (type >= MP_NUM_TYPE_INT_TYPE):
//     bit [31-16] : kind (MP_NUM_TYPE_*_TYPE)
//     bit [15-0 ] : target's bit width
// if (type < MP_NUM_TYPE_INT_TYPE): builtin type, reserved.
enum MP_NUMBER_TYPE : uint32_t {
  MP_NUM_TYPE_NONE,
  MP_NUM_TYPE_INT,
  MP_NUM_TYPE_FLOAT,
  MP_NUM_TYPE_DECIMAL,

  MP_NUM_TYPE_INT_TYPE = 0x7000'0000,
  MP_NUM_TYPE_UINT_TYPE = 0x7001'0000,
};

#define MP_NUM_TYPE_KIND(t)   ((t) & 0xFFFF'0000u)
#define MP_NUM_TYPE_BITS(t)  ((t) & 0x0000'FFFFu)
#define MP_NUM_TYPE_IS_ENCODED(t) ((t) >= MP_NUM_TYPE_INT_TYPE)
#define MP_NUM_TYPE_OF(t, bits) ((t) | bits)

typedef union MP_NUMBER {
  MP_INT      INT;
  MP_FLOAT    FLOAT;
  MP_DECIMAL  DECIMAL;
} MP_NUMBER;

// Fixed-capacity mode (caller-provided buffer):
//   If (NUMBER->INT.alloc < 0), the parser will NOT allocate or free
//   limbs. The caller MUST set NUMBER->INT.limbs to a buffer of at
//   least abs(NUMBER->INT.alloc) limbs.
// Dynamic mode (default):
//   If (NUMBER->INT.alloc >= 0), limbs is allocated by the parser
//   (and must be freed by the caller via MP_INT_clear).
uint32_t LMP_parse(const char * input, uint32_t *type, MP_NUMBER *NUMBER, const Allocator *allocator);

#endif //LMP_PARSE_H
