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
 * Module Name: ext
 * Filename: ieee754.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-30
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "LMP/ext/ieee754.h"
#include "extbit.h"

#define max(a, b)   ((a) >  (b) ? (a) :  (b))
#define abs(a)      ((a) >  (0) ? (a) : -(a))
#define sgn(a)      ((a) == (0) ? 0 : ((a) > 0) ? 1  :  -1)

#define FLOAT(w) float##w##_t
#define UINT(w) uint##w##_t
#define INT(w) int##w##_t

static_assert(sizeof(lmp_limb_t) == sizeof(uint64_t), "'lmp_limb_t' is required has same size with 'uint64_t'");
static_assert(((lmp_limb_t)-1) > ((lmp_limb_t) 0), "'lmp_limb_t' is required an unsigned type.");

#define MP_FLOAT_to_ieee754bin(_w, _ew)                                                                       \
uint32_t MP_FLOAT_to_ieee754_f##_w(FLOAT(_w) *restrict dest, const MP_FLOAT *restrict mp_val) {               \
  if (mp_val->size == 0) {                                                                                    \
    switch (mp_val->exp) {                                                                                    \
      case  MP_NUMBER_ZERO: { *dest =  0;                return 0; }                                          \
      case  MP_NUMBER_NAN : { *dest =  NAN;              return 0; }                                          \
      case -MP_NUMBER_NAN : { *dest = -NAN;              return 0; }                                          \
      case  MP_NUMBER_INF : { *dest =  INFINITY;         return 0; }                                          \
      case -MP_NUMBER_INF : { *dest = -INFINITY;         return 0; }                                          \
      case  MP_NUMBER_INS : { *dest =  FLOAT##_w##_INS;  return 0; }                                          \
      case -MP_NUMBER_INS : { *dest = -FLOAT##_w##_INS;  return 0; }                                          \
      default:{ /*TODO: error report */ return 0; }                                                           \
    }                                                                                                         \
  }                                                                                                           \
  const int64_t leading_zeros = extstd_leading_zeros(MP_FLOAT_get_limb(mp_val, -1));                          \
  int64_t exp2 = (abs(mp_val->size) + mp_val->exp) * (int64_t)LMP_LIMB_BIT - leading_zeros - 1;               \
  uint128_t mantissa = MP_FLOAT_get_double_limbs(mp_val, -2);                                                 \
  const int64_t mant_bits = (2 * (int64_t)LMP_LIMB_BIT) - leading_zeros;                                      \
  const bool round_bit = (mantissa << (leading_zeros + FLOAT##_w##_MANT_BIT + 1)) >= (((uint128_t)1) << 127); \
  mantissa >>= mant_bits - FLOAT##_w##_MANT_BIT - 1;                                                          \
  if ((mantissa & FLOAT##_w##_MANT_MAX) == FLOAT##_w##_MANT_MAX && round_bit) {                               \
    mantissa += 1; mantissa >>= 1; exp2 += 1;                                                                 \
  } else if (round_bit) { mantissa += 1; }                                                                    \
  if (exp2 <= -(int##_ew##_t) FLOAT##_w##_EXP_BIAS) { mantissa >>= 1; }                                       \
  *dest = extstd_float##_w##_compose(mp_val->size < 0, (uint##_w##_t)mantissa, (int##_ew##_t) exp2);          \
  return 0;                                                                                                   \
}

MP_FLOAT_to_ieee754bin(32, 16)
MP_FLOAT_to_ieee754bin(64, 16)
