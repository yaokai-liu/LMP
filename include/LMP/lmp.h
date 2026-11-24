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
#include "extfloat.h"
#include "extint.h"

#ifndef USING_NTT_SIZE
#define USING_NTT_SIZE  64
#endif
#ifndef MP_ALLOC_LEN
#define MP_ALLOC_LEN 8
#endif

typedef int64_t lmp_exp_t;
typedef int32_t lmp_prec_t;
typedef lmp_prec_t lmp_size_t;

#ifndef _LMP_LIMB_WIDTH_
#define _LMP_LIMB_WIDTH_        64
typedef uint64_t  lmp_limb_t;
typedef uint32_t  lmp_half_limb_t;
typedef uint128_t lmp_double_limb_t;
typedef uint256_t lmp_quarter_limb_t;
#define HEX_LIMB_WIDTH          16
#define DEC_LIMB_WIDTH          19
#define OCT_LIMB_WIDTH          21
#define BIN_LIMB_WIDTH          64
#define HEX_LIMB_VAL            LMP_LIMB_VAL
#define DEC_LIMB_VAL            ((lmp_double_limb_t)1'0000'0000'0000'0000'000ULL)
#define OCT_LIMB_VAL            ((lmp_double_limb_t)0x8000'0000'0000'0000ULL)
#define BIN_LIMB_VAL            HEX_LIMB_VAL
#endif
#define LMP_LIMB_SIZE         (sizeof(lmp_limb_t))
#define LMP_LIMB_BIT          (LMP_LIMB_SIZE * CHAR_BIT)
#define LMP_LIMB_MAX          (~(lmp_limb_t)0)
#define LMP_HALF_LIMB_MAX     (~(lmp_half_limb_t)0)
#define LMP_DOUBLE_LIMB_MAX   (~(lmp_double_limb_t)0)
#define LMP_LIMB_VAL          (((lmp_double_limb_t)1) << LMP_LIMB_BIT)
enum MP_NUMBER_SPECIAL_VALUE_TYPE : int32_t {
  MP_NUMBER_ZERO = 0,
  MP_NUMBER_INF,
  MP_NUMBER_INS, // infinitesimal

  // For LMP itself, if special value is not in
  // values above or their negative, and is not
  // a bad value, then it will be treate as a NAN.
  // Users can write custom functions to specify more
  // special value.

  MP_NUMBER_NAN = INT32_MAX,
  MP_NUMBER_NEG_INF = - MP_NUMBER_INF,
  MP_NUMBER_NEG_INS = - MP_NUMBER_INS,
  MP_NUMBER_NEG_NAN = - MP_NUMBER_NAN,
  MP_NUMBER_BAD_VALUE = INT32_MIN, // this value should not be reached
};

// INT = sgn(size) * sum(limbs[i] * pow(LMP_LIMB_VAL, i)),
// here i from 0 to (abs(size) - 1).
// Notice:
//    All width here or in functions is respected to the 2 instead of LMP_LIMB_VAL.
typedef struct MP_INT {
  // (prec < 0) means it's a fixed capacity variable.
  // abs(prec) is how many limbs allowed to use.
  lmp_prec_t  prec;
  // (size < 0) means negative number
  lmp_size_t  size;
  // little endian
  lmp_limb_t *limbs;
} MP_INT;

// if (size != 0):
//    FLOAT = sgn(size) * sum(limbs[i] * pow(LMP_LIMB_VAL, i)) * pow(LMP_LIMB_VAL, exp),
//    i from 0 to (abs(size) - 1).
// else:
//    FLOAT = MP_FLOAT_SPECIAL_VALUE_TYPE(exp)
//    if (prec > 0) limbs will be released.
//    In this situation, the 'limbs' will not be used by the library,
//    users can use this field for other means.
//    But if this value will interactive in the library's arith part,
//    especially as result-end, the 'limbs' field would be changed.
// Promise:
//    1. The mantissa of given pointer of struct MP_FLOAT is exactly
//       the data of casting to MP_INT if is not special value.
//       For exampe, mantissa of 'MP_FLOAT *p_float' is '(MP_INT *)p_float'.
// Notice:
//    All width here or in functions is respected to the 2 instead of LMP_LIMB_VAL.
typedef struct MP_FLOAT {
  // alloc memory size for limbs
  // (prec < 0) means it's a fixed capacity variable.
  // abs(prec) is how many limbs allowed to use.
  // max limbs with out realloc
  lmp_prec_t  prec;
  // current value size, the mantissa size
  // (size < 0) means negative number
  lmp_size_t  size;
  // mantissa limbs, little endian
  lmp_limb_t *limbs;
  // exponent of 'LMP_FLOAT_VAL';
  // If (size == 0), this field will be used to represent special value.
  lmp_exp_t   exp;
} MP_FLOAT;

typedef struct MP_BASE {
  // the real base of the number
  lmp_half_limb_t base;
  // max{i: pow(BASE.base, i) <= LMP_LIMB_VAL}
  lmp_half_limb_t limb_width;
  // pow(BASE.base, BASE.limb_width)
  lmp_limb_t      limb_val;
} MP_BASE;
// if (size != 0):
//    NUMBER = sgn(size) * sum(limbs[i] * pow(BASE.limb_val, i)) * pow(BASE.limb_val, exp),
//    i from 0 to (abs(size) - 1).
// else if (BASE.base == 0):
//    NUMBER is a polynomial.
//    NUMBER = sgn(exp) * sum(limbs[i] * pow(X,i)),
//    i from 0 to (abs(size) - 1), i.e. abs(size) = degree(NUMBER) + 1
// else:
//    NUMBER = MP_FLOAT_SPECIAL_VALUE_TYPE(exp)
//    if (prec > 0) limbs will be released.
//    In this situation, the 'limbs' will not be used by the library,
//    users can use this field for other means.
//    But if this value will interactive in the library's arith part,
//    especially as result-end, the 'limbs' field would be changed.
// if (exp == 0 && size != 0):
//    Of course it is an integer.
//    NUMBER = sgn(size) * sum(limbs[i] * pow(BASE.limb_val, i)),
//    i from 0 to (abs(size) - 1).
// Promise:
//    1. limbs[i] < BASE.limb_val (no unsigned int overflow case) < LMP_LIMB_VAL.
//    2. If and only if (BASE.base == 2), the value of given pointer of
//       struct MP_BASE_NUMBER is exactly the data of casting to MP_FLOAT.
//       For exampe, for 'MP_BASE_NUMBER *p_number', if (p_number->BASE.base == 2),
//       then 'MP_BASE_NUMBER_to_MP_FLOAT(p_number)' is exactly '(MP_FLOAT *)p_number'.
//       However, for those (BASE.base != 2) but 'BASE.limb_width' divides LMP_LIMB_BIT,
//       the 'BASE.limb_val' will be 0, which are expected behaviors(unsigned int overflow).
//       Here the mantissa of number is exactly '(MP_INT *)p_number'.
//       And We can prove that when the number is an integer, when base is transform in these
//       making 'BASE.limb_val' be 0, the number need to change nothing on limbs (or mantissa for float)
//       Users may use these behaviors to decide if do some optimizations.
// Special behaviors:
//    1. If (BASE.base == 0): then the base number should be consider a polynomial
//       that all coeffidents less than LMP_LIMB_VAL.
//    2. If (BASE.base == 1): then the base number should be consider 0 or ±INF.
// Notice:
//    1. All width here or in functions is respected to the BASE.base instead of BASE.limb_val.
//    2. Users should keep syncing BASE.limb_val and BASE.limb_width. Particalarly, library will always
//       assume (BASE.limb_val == 0) equivalents to BASE.limb_width divides LMP_LIMB_BIT.
typedef struct MP_BASE_NUMBER {
  // alloc memory size for limbs
  // (prec < 0) means it's a fixed capacity variable.
  // abs(prec) is how many limbs allowed to use.
  // max limbs with out realloc
  lmp_prec_t  prec;
  // current value size, the mantissa size
  // (size < 0) means negative number
  lmp_size_t  size;
  // mantissa limbs, little endian
  lmp_limb_t *limbs;
  // exponent of 'BASE.limb_val';
  // If (size == 0), this field will be used to represent special values.
  lmp_exp_t   exp;
  // the base struct of the number
  MP_BASE     BASE;
} MP_BASE_NUMBER;

_SHARED_API void MP_INT_init(MP_INT *val,  int32_t initial_alloc, const Allocator *allocator);
_SHARED_API void MP_INT_realloc(MP_INT *val,  int32_t required, const Allocator *allocator);
_SHARED_API void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator);

_SHARED_API void MP_INT_from_c_int(MP_INT *mp_val, int256_t val, const Allocator *allocator);
_SHARED_API void MP_INT_from_c_uint(MP_INT *mp_val, uint256_t val, const Allocator *allocator);
_SHARED_API void MP_INT_clear(MP_INT *val, const Allocator *allocator);
_SHARED_API void MP_INT_clamp(MP_INT *val);
_SHARED_API size_t MP_INT_bitsof(const MP_INT *val);
_LOCAL_API bool MP_INT_carry_check(const MP_INT *val);
_LOCAL_API int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2);
_LOCAL_API bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, int32_t offset);
_SHARED_API lmp_limb_t MP_INT_get_limb(const MP_INT *restrict num, int32_t index);
_SHARED_API lmp_double_limb_t MP_INT_get_double_limbs(const MP_INT *restrict num, int32_t index);
_SHARED_API lmp_limb_t MP_INT_get_flat_limb(const MP_INT *restrict num, int32_t index);
_SHARED_API lmp_double_limb_t MP_INT_get_double_flat_limbs(const MP_INT *restrict num, int32_t index);
// Always assume 0 <= bit_count < LMP_LIMB_BIT
_SHARED_API lmp_limb_t MP_INT_left_shift_load(const MP_INT *restrict num, int32_t index, int32_t width);
// Always assume 0 <= bit_count < LMP_LIMB_BIT
_SHARED_API lmp_double_limb_t MP_INT_left_shift_load_double(const MP_INT *restrict num, int32_t index, int32_t width);

_SHARED_API void MP_FLOAT_init(MP_FLOAT *val, int32_t initial_alloc, const Allocator *allocator);
_SHARED_API void MP_FLOAT_from_ieee754_float(MP_FLOAT *mp_val, float128_t val, const Allocator *allocator);
_SHARED_API void MP_FLOAT_copy(MP_FLOAT *dest, const MP_FLOAT *src, const Allocator *allocator);
_SHARED_API void MP_FLOAT_clear(MP_FLOAT *val, const Allocator *allocator);
// Truncate mantissa in given prec limbs.
// It will not change the exp field.
// Notice: may cause precision-lose
_SHARED_API void MP_FLOAT_truncate(MP_FLOAT *val, lmp_prec_t prec);
// Remove trailing zeros of mantissa.
// It will not change the value or precision.
// It will change the exp and size field.
_SHARED_API void MP_FLOAT_tidy(MP_FLOAT *val);
_LOCAL_API int32_t MP_FLOAT_abs_cmp(const MP_FLOAT *op1, const MP_FLOAT *op2);
#define MP_FLOAT_mantissa(p_float)                                ((MP_INT *)(p_float))
#define MP_FLOAT_clamp(val)                                       MP_INT_clamp((MP_INT *)(val))
#define MP_FLOAT_carry_check(val)                                 MP_INT_carry_check((MP_INT *)(val))
#define MP_FLOAT_realloc(val, required, allocator)                MP_INT_realloc((MP_INT *)(val), (required), (allocator))
#define MP_FLOAT_get_limb(num, index)                             MP_INT_get_limb((const MP_INT *)(num), (index))
#define MP_FLOAT_get_double_limbs(num, index)                     MP_INT_get_double_limbs((MP_INT *)(num), (index))
#define MP_FLOAT_get_flat_limb(num, index)                        MP_INT_get_flat_limb((const MP_INT *)(num), (index))
#define MP_FLOAT_get_double_flat_limbs(num, index)                MP_INT_get_double_flat_limbs((MP_INT *)(num), (index))
#define MP_FLOAT_left_shift_load(num, index, bit_count)           MP_INT_left_shift_load((MP_INT *)(num), (index), (bit_count))
#define MP_FLOAT_left_shift_load_double(num, index, bit_count)    MP_INT_left_shift_load_double((MP_INT *)(num), (index), (bit_count))

_LOCAL_API bool MP_BASE_INT_carry_check(const MP_BASE_NUMBER *val);
_SHARED_API void MP_BASE_NUMBER_clear(MP_BASE_NUMBER *val, const Allocator *allocator);
#define MP_BASE_NUMBER_realloc(val, required, allocator)          MP_INT_realloc((MP_INT *)(val), (required), (allocator))
#define MP_BASE_INT_init(val,  initial_alloc, allocator)          MP_INT_init((MP_INT *)(val), (initial_alloc), (allocator))
#define MP_BASE_INT_get_limb(num, index)                          MP_INT_get_limb((const MP_INT *)(num), (index))
_SHARED_API lmp_double_limb_t MP_BASE_INT_get_double_limbs(const MP_BASE_NUMBER *restrict num, int32_t index);
_SHARED_API lmp_double_limb_t MP_BASE_INT_get_double_flat_limbs(const MP_BASE_NUMBER *restrict num, int32_t index);
#define MP_BASE_INT_clamp(val)                                    MP_INT_clamp((MP_INT *)(val))
#define MP_BASE_INT_abs_cmp(op1, op2)                             MP_INT_abs_cmp((const MP_INT *)(op1), (const MP_INT *)(op2))
#define MP_BASE_INT_abs_offset_ge(op1, op2, offset)               MP_INT_abs_offset_ge((const MP_INT *)(op1), (const MP_INT *)(op2), (offset))

#define MP_BASE_FLOAT_init(val,  initial_alloc, allocator)        MP_FLOAT_init((MP_FLOAT *)(val), (initial_alloc), (allocator))
#define MP_BASE_FLOAT_negative(val)                               MP_FLOAT_negative((MP_FLOAT *)(val))
#define MP_BASE_FLOAT_get_limb(num, index)                        MP_FLOAT_get_limb((num), (index))
#define MP_BASE_FLOAT_get_double_limbs(num, index)                MP_BASE_INT_get_double_limbs((num), (index))
#define MP_BASE_FLOAT_get_flat_limb(num, index)                   MP_FLOAT_get_flat_limb((num), (index))
#define MP_BASE_FLOAT_get_double_flat_limbs(num, index)           MP_BASE_INT_get_double_flat_limbs((num), (index))
#define MP_BASE_FLOAT_carry_check(val)                            MP_BASE_INT_carry_check((val))
#define MP_BASE_FLOAT_truncate(val)                               MP_FLOAT_truncate((MP_FLOAT *)(val))
#define MP_BASE_FLOAT_clamp(val)                                  MP_FLOAT_clamp((MP_FLOAT *)(val))
#define MP_BASE_FLOAT_tidy(val)                                   MP_FLOAT_tidy((MP_FLOAT *)(val))
#define MP_BASE_FLOAT_abs_cmp(op1, op2)                           MP_FLOAT_abs_cmp((const MP_FLOAT *)(op1), (const MP_FLOAT *)(op2))

_SHARED_API void MP_BASE_NUMBER_init(MP_BASE_NUMBER *val, int32_t initial_alloc, lmp_half_limb_t base, const Allocator *allocator);
_SHARED_API void MP_BASE_NUMBER_copy(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *src, const Allocator *allocator);
// Always assume 0 <= width <= BASE.limb_width
// else is an unexpected behavior
_SHARED_API lmp_limb_t MP_BASE_NUMBER_left_shift_load(const MP_BASE_NUMBER *mp_val, int32_t index, int32_t width);
// Always assume 0 <= width <= BASE.limb_width
// else is an unexpected behavior
_SHARED_API lmp_double_limb_t MP_BASE_NUMBER_left_shift_load_double(const MP_BASE_NUMBER *mp_val, int32_t index, int32_t width);

_SHARED_API void MP_BASE_INT_to_int(MP_INT *dest, const MP_BASE_NUMBER *src, const Allocator *allocator);
_SHARED_API void MP_BASE_FLOAT_to_float(MP_FLOAT *dest, const MP_BASE_NUMBER *src, const Allocator *allocator);
_SHARED_API void MP_INT_to_base_number(MP_BASE_NUMBER *dest, const MP_INT *mp_val, uint32_t base, const Allocator *allocator);
_SHARED_API void MP_FLOAT_to_base_number(MP_BASE_NUMBER *dest, const MP_FLOAT *mp_val, uint32_t base, const Allocator *allocator);
// if (mp_val->BASE.base < 2), i.e. is a polynomial or base-INF, it will do nothing.
_SHARED_API void MP_BASE_NUMBER_change_base(MP_BASE_NUMBER *mp_val, lmp_half_limb_t base, const Allocator *allocator);

_LOCAL_API lmp_limb_t lmp_fast_pow(lmp_half_limb_t base, lmp_exp_t exp);
_LOCAL_API lmp_limb_t lmp_fast_quotient(lmp_double_limb_t dividend, lmp_double_limb_t dividend_follow, lmp_double_limb_t divisor);

#endif //LMP_H
