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
// #include <limits.h>
// #if CHAR_BIT != 8
// #error "CHAR_BIT is required with 8."
// #endif
// #  if __has_include(<stdbit.h>)
// #    include <stdbit.h>
// #    define HAVE_STDBIT_H 1
// #  endif
//
// static unsigned int first_leading_one_u16  (uint16_t x);
// static unsigned int first_leading_one_u32  (uint32_t x);
// static unsigned int first_leading_one_u64  (uint64_t x);
// static unsigned int first_leading_one_u128 (uint128_t x);
// static unsigned int first_leading_one_u256 (uint256_t x);
//
//
// #define bitsof(w)   (sizeof(w) * CHAR_BIT)
// #define abs(a)      ((a) > (0) ? (a) : -(a))
//
// inline unsigned int first_leading_one_u16  (uint16_t x) {
// #ifdef HAVE_STDBIT_H
//   return stdc_first_leading_one((uint32_t)x);
// #else
//   if (x == 0) return 0;
//   return bitsof(uint32_t) - __builtin_clz((uint32_t)x);
// #endif
// }
//
// inline unsigned int first_leading_one_u32  (uint32_t x) {
// #ifdef HAVE_STDBIT_H
//   return stdc_first_leading_one(x);
// #else
//   if (x == 0) return 0;
//   return bitsof(uint32_t) - __builtin_clz(x);
// #endif
// }
//
// inline unsigned int first_leading_one_u64  (uint64_t x) {
// #ifdef HAVE_STDBIT_H
//   return stdc_first_leading_one(x);
// #else
//   if (x == 0) return 0;
//   return bitsof(uint64_t) - __builtin_clzll(x);
// #endif
// }
//
// inline unsigned int first_leading_one_u128 (uint128_t x) {
//   uint64_t p;
//   if ((p = x >> 64)) { return first_leading_one_u64(p) + 64u; }
//   if ((p = x)) { return first_leading_one_u64(p); }
//   return 0;
// }
//
// inline unsigned int first_leading_one_u256 (uint256_t x) {
//   uint64_t p;
//   if ((p = x >> 192)) { return first_leading_one_u64(p) + 192u; }
//   if ((p = x >> 128)) { return first_leading_one_u64(p) + 128u; }
//   if ((p = x >> 64)) { return first_leading_one_u64(p) + 64u; }
//   if ((p = x)) { return first_leading_one_u64(p); }
//   return 0;
// }

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
        | (((UINT(_w))final_exp) << mant_bits)                                        \
        | (final_mant & mant_MASK);                                                   \
  return val.F;                                                                       \
}

u_to_ieee754bin(16, 8, 5)
u_to_ieee754bin(32, 8, 8)
u_to_ieee754bin(64, 16, 11)
u_to_ieee754bin(128, 16, 15)

