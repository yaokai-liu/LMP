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
 * Module Name:
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-19
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_PARSE_H
#define LMP_PARSE_H
#include "lmp.h"
#include "type.h"

#ifndef LMP_PARSE_ALLOC_SCALE
#define LMP_PARSE_ALLOC_SCALE 4
#endif
#ifndef LMP_PARSE_EXP_MAX
#define LMP_PARSE_EXP_MAX 2048
#endif
#ifndef LMP_LITERAL_PREC_MAX
#define LMP_LITERAL_PREC_MAX 2048
#endif

typedef struct MP_NUMBER {
  union {
    MP_INT      INT;
    MP_FLOAT    FLOAT;
    MP_DECIMAL  DECIMAL;
  }         val;
  uint8_t   repr;
}MP_NUMBER;

// Fixed-capacity mode (caller-provided buffer):
//   If (NUMBER->val.INT.alloc < 0), the parser will NOT allocate or free
//   limbs. The caller MUST set NUMBER->val.INT.limbs to a buffer of at
//   least abs(NUMBER->val.INT.alloc) limbs.
// Dynamic mode (default):
//   If (NUMBER->val.INT.alloc >= 0), limbs is allocated by the parser
//   (and must be freed by the caller via MP_INT_clear).
// Parse patterns:
//    [+-]?0[xX][a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<standard>)?) |
//    [+-]?0[bB][01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<standard>)?) |
//    [+-]?0[oO][0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<standard>)?) |
//    [+-]?[0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)[dD]?(-<standard>)?)
uint32_t LMP_parse(const char * input, MP_NUM_TYPE *type, MP_NUMBER *NUMBER, const Allocator *allocator);

#endif //LMP_PARSE_H
