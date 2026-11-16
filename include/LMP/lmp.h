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
 * Filename: lmp.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-13
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_H
#define LMP_H
#include <stdint.h>
#include <limits.h>
#if CHAR_BIT != 8
#error "CHAR_BIT is required with 8."
#endif
#include "allocator.h"
#include "extint.h"

#ifndef USING_NTT_SIZE
#define USING_NTT_SIZE  64
#endif
#ifndef MP_ALLOC_LEN
#define MP_ALLOC_LEN 8
#endif

typedef uint64_t lmp_limb_t;
typedef uint32_t lmp_half_limb_t;
typedef uint128_t lmp_double_limb_t;
typedef uint256_t lmp_quarter_limb_t;

typedef int64_t lmp_exp_t;
typedef int32_t lmp_prec_t;
typedef int32_t lmp_size_t;

#define LMP_LIMB_SIZE         (sizeof(lmp_limb_t))
#define LMP_LIMB_BIT          (LMP_LIMB_SIZE * CHAR_BIT)
#define LMP_LIMB_MAX          (~(lmp_limb_t)0)
#define LMP_DOUBLE_LIMB_MAX   (~(lmp_double_limb_t)0)
#define LMP_LIMB_VAL          (((lmp_double_limb_t)1) << LMP_LIMB_BIT)

typedef struct MP_INT {
  // (alloc < 0) means it's a fixed capacity variable.
  // abs(alloc) is how many limbs allowed to use.
  lmp_prec_t  alloc;
  // negative if size < 0
  lmp_size_t  size;
  // little endian
  lmp_limb_t *limbs;
} MP_INT, MP_POLYi;

enum MP_FLOAT_SPECIAL_VALUE_TYPE : int32_t {
  MP_FLOAT_ZERO = 0,
  MP_FLOAT_INF,
  MP_FLOAT_INS, // infinity small
  MP_FLOAT_NAN = INT32_MAX,

  MP_FLOAT_NEG_INF = - MP_FLOAT_INF,
  MP_FLOAT_NEG_INS = - MP_FLOAT_INS,
  MP_FLOAT_NEG_NAN = - MP_FLOAT_NAN,

  MP_FLOAT_BAD_VALUE = INT32_MIN, // this value should not be reached
};

#define LMP_FLOAT_BASE              LMP_LIMB_VAL
#define LMP_FLOAT_BASE_SQUARE       (LMP_LIMB_VAL << LMP_LIMB_BIT)
// LMP_DECIMAL_BASE == pow(10, 19)
#define LMP_DECIMAL_BASE            (0x8AC7'2304'89E8'0000ULL)
// LMP_DECIMAL_BASE_SQUARE == pow(10, 38)
#define LMP_DECIMAL_BASE_SQUARE     (((uint128_t)0x4B3B'4CA8'5A86'C47AULL) << 64 | 0x0x098A'2240'0000'0000ULL)
// if (size != 0):
//    FLOAT = limbs * pow(LMP_FLOAT_BASE, exp)
//    DECIMAL = limbs * pow(LMP_DECIMAL_BASE, exp)
// else:
//    FLOAT = MP_FLOAT_SPECIAL_VALUE_TYPE(exp)
//    if (prec > 0) limbs will be released.
//    In this situation, the 'limbs' will not be used by the library,
//    users can use this field for other means.
//    But if this value will interactive in the library's arith part,
//    especially as result-end, the 'limbs' field would be changed.
// Promise:
//    1. The mantissa of given pointer of struct MP_FLOAT 'p_float' is exactly ((MP_INT *)p_float)
//       if is not special value.
//    2. All none arithmetic functions except 'MP_FLOAT_clamp' of MP_FLOAT is sutiable for MP_DECIMAL.
//       (MP_DECIMAL should use 'MP_DECIMAL_clamp' since clamping will change the 'exp' feild
//       and depend on the base)
typedef struct MP_FLOAT {
  // alloc memory size for limbs
  // (prec <  0) means it's a fixed capacity variable.
  // abs(alloc) is how many limbs allowed to use.
  // max limbs with out realloc
  lmp_prec_t  prec;
  // current value size
  // negative if (size < 0)
  lmp_size_t  size;
  // little endian
  lmp_limb_t *limbs;
  // With 'MP_FLOAT' interpreting, this field based 'LMP_FLOAT_BASE';
  // with 'MP_DECIMAL' interpreting, this field based 'LMP_DECIMAL_BASE'.
  // No matter with 'MP_FLOAT' or 'MP_DECIMAL', if (size == 0):
  //    this field will be used to represent special value.
  lmp_exp_t   exp;
} MP_FLOAT, MP_DECIMAL;

void MP_INT_init(MP_INT *val,  int32_t initial_alloc, const Allocator *allocator);
void MP_INT_realloc(MP_INT *val,  int32_t required, const Allocator *allocator);
void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator);

void MP_INT_from_int(MP_INT *mp_val, int256_t val, const Allocator *allocator);
void MP_INT_from_uint(MP_INT *mp_val, uint256_t val, const Allocator *allocator);
void MP_INT_clear(MP_INT *val, const Allocator *allocator);
void MP_INT_clamp(MP_INT *val);

void MP_INT_negative(MP_INT *val);
size_t MP_INT_bitsof(const MP_INT *val);
int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2);
bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, int32_t offset);
lmp_limb_t MP_INT_get_limb(const MP_INT *restrict num, int32_t index);
lmp_double_limb_t MP_INT_get_double_limbs(const MP_INT *restrict num, int32_t index);
lmp_limb_t MP_INT_left_shift_load(const MP_INT *restrict num, int32_t index, int32_t bit_count);
lmp_double_limb_t MP_INT_left_shift_load_double(const MP_INT *restrict num, int32_t index, int32_t bit_count);

void MP_FLOAT_init(MP_FLOAT *val, int32_t initial_alloc, const Allocator *allocator);
void MP_FLOAT_realloc(MP_FLOAT *val, int32_t required, const Allocator *allocator);
void MP_FLOAT_copy(MP_FLOAT *dest, const MP_FLOAT *src, const Allocator *allocator);
void MP_FLOAT_clear(MP_FLOAT *val, const Allocator *allocator);
// Not sutiable for MP_DECIMAL
void MP_FLOAT_clamp(MP_FLOAT *val);

lmp_limb_t MP_FLOAT_get_limb(const MP_FLOAT *restrict num, int32_t index);
lmp_double_limb_t MP_FLOAT_get_double_limbs(const MP_FLOAT *restrict num, int32_t index);
lmp_limb_t MP_FLOAT_left_shift_load(const MP_FLOAT *restrict num, int32_t index, int32_t bit_count);
lmp_double_limb_t MP_FLOAT_left_shift_load_double(const MP_FLOAT *restrict num, int32_t index, int32_t bit_count);

#define MP_FLOAT_mantissa(p_float)  ((MP_INT *)(p_float))
#define MP_FLOAT_negative(p_float)  do { (p_float)->size = -(p_float)->size} while(false)
#define MP_FLOAT_trivert(p_float)   do { (p_float)->exp = -(p_float)->exp; } while(false)

#define MP_DECIMAL_init       MP_FLOAT_init
#define MP_DECIMAL_realloc    MP_FLOAT_realloc
#define MP_DECIMAL_copy       MP_FLOAT_copy
#define MP_DECIMAL_clear      MP_FLOAT_clear
void MP_DECIMAL_clamp(MP_DECIMAL *val);

#define  MP_DECIMAL_get_limb                MP_FLOAT_get_limb
#define  MP_DECIMAL_get_double_limbs        MP_FLOAT_get_double_limbs
#define  MP_DECIMAL_left_shift_load         MP_FLOAT_left_shift_load
#define  MP_DECIMAL_left_shift_load_double  MP_FLOAT_left_shift_load_double

#define MP_DECIMAL_mantissa MP_FLOAT_mantissa
#define MP_DECIMAL_negative MP_FLOAT_negative
#define MP_DECIMAL_trivert  MP_FLOAT_trivert

#endif //LMP_H
