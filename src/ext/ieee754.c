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

#include "ext/ieee754.h"
#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a >> ((sizeof(a) - 1) * 8 + 7)) ? -1 : 1)

#define first_leading_one(u)  _Generic((u),   \
  uint16_t:   first_leading_one_u16 (u),      \
  uint32_t:   first_leading_one_u32 (u),      \
  uint64_t:   first_leading_one_u64 (u),      \
  uint128_t:  first_leading_one_u128(u),      \
  uint256_t:  first_leading_one_u256(u),      \
  default:    0                               \
)

#define FLOAT(w) float##w##_t
#define UINT(w) uint##w##_t
#define INT(w) int##w##_t

#define u_to_ieee754bin(_w, _ew, _eb)                                                 \
inline FLOAT(_w) ieee754bin##_w(bool sign, UINT(_w) mantissa, INT(_ew) exponent) {    \
  const uint32_t result_bits = _w;                                                    \
  const uint32_t order_bits = _eb;                                                    \
  const UINT(_w) order_MASK = (((UINT(_w)) 1) << order_bits) - 1;                     \
  const uint32_t mant_bits = result_bits - order_bits - 1;                            \
  const UINT(_w) mant_MASK = (((UINT(_w)) 1) << mant_bits) - 1;                       \
                                                                                      \
  uint32_t final_exp = 0;                                                             \
  UINT(_w) final_mant = 0;                                                            \
                                                                                      \
  exponent += IEEE754_F##_w##_EXP_OFF;                                                \
  if (exponent == 0) {                                                                \
    final_exp  = 0;                                                                   \
    final_mant  = mantissa ? (mantissa & mant_MASK) : 0;                              \
  } else if (((UINT(_ew))exponent) >= order_MASK) {                                   \
    final_exp  = order_MASK;                                                          \
    final_mant  = mantissa ? (mantissa & mant_MASK) : 0;                              \
  } else {                                                                            \
    final_exp  = exponent & order_MASK;                                               \
    final_mant = mantissa & mant_MASK;                                                \
  }                                                                                   \
  union { UINT(_w) I; FLOAT(_w) F; } val = {};                                        \
  val.I = (((UINT(_w))sign) << (result_bits - 1))                                     \
        | ((((UINT(_w))final_exp)) << mant_bits)                                      \
        | (final_mant & mant_MASK);                                                   \
  return val.F;                                                                       \
}

u_to_ieee754bin(16, 8, IEEE754_F16_EXP_BITS)
u_to_ieee754bin(32, 8, IEEE754_F32_EXP_BITS)
u_to_ieee754bin(64, 16, IEEE754_F64_EXP_BITS)
u_to_ieee754bin(128, 16, IEEE754_F128_EXP_BITS)

static_assert(sizeof(lmp_limb_t) == sizeof(uint64_t), "'lmp_limb_t' is required has same size with 'uint64_t'");
static_assert(((lmp_limb_t)-1) > ((lmp_limb_t) 0), "'lmp_limb_t' is required an unsigned type.");

#define MP_FLOAT_to_ieee754bin(_w)                                                                                  \
uint32_t MP_FLOAT_to_ieee754_f##_w(FLOAT(_w) *restrict dest, const MP_FLOAT *restrict mp_val) {                     \
  if (mp_val->size == 0) {                                                                                          \
    switch (mp_val->exp) {                                                                                          \
      case  MP_FLOAT_ZERO: { *dest =  0;                    return 0; }                                             \
      case  MP_FLOAT_NAN : { *dest =  NAN;                  return 0; }                                             \
      case -MP_FLOAT_NAN : { *dest = -NAN;                  return 0; }                                             \
      case  MP_FLOAT_INF : { *dest =  INFINITY;             return 0; }                                             \
      case -MP_FLOAT_INF : { *dest = -INFINITY;             return 0; }                                             \
      case  MP_FLOAT_INS : { *dest =  IEEE754_F##_w##_INS;  return 0; }                                             \
      case -MP_FLOAT_INS : { *dest = -IEEE754_F##_w##_INS;  return 0; }                                             \
      default:{ /*TODO: error report */ return 0; }                                                                 \
    }                                                                                                               \
  }                                                                                                                 \
  const int64_t leading_zeros = __builtin_clzll(MP_FLOAT_get_limb(mp_val, -1));                                     \
  int64_t exp2 = (abs(mp_val->size) + mp_val->exp) * (int64_t)LMP_LIMB_BITS - leading_zeros - 1;                    \
  uint128_t mantissa = MP_FLOAT_get_double_limbs(mp_val, -2);                                                       \
  const int64_t mant_bits = (2 * (int64_t)LMP_LIMB_BITS) - leading_zeros;                                           \
  const bool round_bit = (mantissa << (leading_zeros + IEEE754_F##_w##_MANT_BITS + 1)) >= (((uint128_t)1) << 127);  \
  mantissa >>= mant_bits - IEEE754_F##_w##_MANT_BITS - 1;                                                           \
  if ((mantissa & IEEE754_F##_w##_MANT_MAX) == IEEE754_F##_w##_MANT_MAX && round_bit) {                             \
    mantissa += 1; mantissa >>= 1; exp2 += 1;                                                                       \
  } else if (round_bit) { mantissa += 1; }                                                                          \
  *dest = ieee754bin##_w(mp_val->size < 0, (uint##_w##_t)mantissa, (int8_t) exp2);                                  \
  return 0;                                                                                                         \
}

MP_FLOAT_to_ieee754bin(32)
MP_FLOAT_to_ieee754bin(64)


