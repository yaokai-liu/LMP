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
 * Filename: lmp.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-13
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "lmp.h"
#include <stdlib.h>
#include "extbit.h"
#include <math.h>
#include "arith.h"

#define max(a, b)   ((a) >  (b) ? (a) :  (b))
#define min(a, b)   ((a) <  (b) ? (a) :  (b))
#define abs(a)      ((a) >  (0) ? (a) : -(a))
#define sgn(a)      ((a) == (0) ? 0 : ((a) > 0) ? 1  :  -1)
// ceil to times of MP_ALLOC_LEN
#define aligned(count)  ((((lmp_prec_t)(count) - 1) / MP_ALLOC_LEN + 1) * MP_ALLOC_LEN)

#ifdef __GNUC__
#define ALWAYS_INLINE __attribute((always_inline))
#elifdef _MSC_VER
#define ALWAYS_INLINE __forceinline
#endif

inline lmp_limb_t lmp_fast_pow(const lmp_half_limb_t base, lmp_exp_t exp) {
  lmp_limb_t result = 1;
  lmp_double_limb_t cache = base;
  while (exp > 0) {
    if (exp & 1) { result *= cache; }
    cache *= cache;
    exp >>= 1;
  }
  return result;
}

inline lmp_limb_t lmp_fast_quotient(const lmp_double_limb_t dividend, const lmp_double_limb_t dividend_follow, const lmp_double_limb_t divisor) {
  const lmp_double_limb_t d_high = divisor >> LMP_LIMB_BIT;
  lmp_limb_t quotient = dividend / d_high;
  const lmp_limb_t dividend_hat_high = (quotient * d_high) >> LMP_LIMB_BIT;
  const lmp_limb_t dividend_high = dividend >> LMP_LIMB_BIT;
  const lmp_double_limb_t dividend_hat_tail = ((quotient * d_high) << LMP_LIMB_BIT) + (lmp_double_limb_t)quotient * (lmp_limb_t)divisor;
  const lmp_double_limb_t dividend_tail = (dividend << LMP_LIMB_BIT) + dividend_follow;
  if (dividend_hat_high > dividend_high || (dividend_hat_high == dividend_high && dividend_hat_tail > dividend_tail)) { quotient -= 1; }
  return quotient;
}

inline void MP_INT_init(MP_INT *val, const int32_t initial_alloc, const Allocator *allocator) {
  if (val->prec == 0) {
    const lmp_prec_t prec = aligned(abs(initial_alloc));
    val->limbs = allocator->calloc(prec, LMP_LIMB_SIZE);
    val->prec = initial_alloc >= 0 ? prec : -prec;
    val->size = 0;
  }
}

inline void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator) {
  MP_INT_realloc(dest, src->size, allocator);
  allocator->memcpy(dest->limbs, src->limbs, abs(src->size) * LMP_LIMB_SIZE);
  dest->size = src->size;
}

inline void MP_INT_clear(MP_INT *val, const Allocator *allocator) {
  if (val->prec > 0 && val->limbs) {
    allocator->free(val->limbs);
    val->limbs = nullptr;
    val->prec = 0;
  }
  val->size = 0;
}

inline void MP_INT_realloc(MP_INT *val, const int32_t required, const Allocator *allocator) {
  if (val->prec < 0) { return ; }
  if (val->prec < abs(required)) {
    const lmp_prec_t prec = aligned(abs(required));
    lmp_limb_t *ptr = allocator->realloc(val->limbs, prec * LMP_LIMB_SIZE);
    if (!ptr) return;
    allocator->memset(&ptr[val->prec], 0, (prec - val->prec) * LMP_LIMB_SIZE);
    val->limbs = ptr;
    val->prec = required > 0 ? prec : -prec;
  }
}

inline void MP_INT_clamp(MP_INT *val) {
  lmp_size_t size = abs(val->size);
  while (size > 0 && val->limbs[size - 1] == 0) { size--; }
  val->size = val->size >= 0 ? size : -size;
}

inline size_t MP_INT_bitsof(const MP_INT *val) {
  lmp_size_t size = abs(val->size);
  while (size > 0 && val->limbs[size - 1] == 0) { size--; }
  return (size - 1) * LMP_LIMB_BIT + extstd_bit_width(val->limbs[size - 1]);
}

inline lmp_limb_t MP_INT_left_shift_load(const MP_INT *restrict num, const int32_t index, const int32_t width) {
  const lmp_size_t size = abs(num->size);
  if (index < 0) {
    lmp_limb_t val1 = -1 <= size + index && index <=  0 ? num->limbs[size + index - 1] << width : 0;
    lmp_limb_t val2 =  0 <= size + index && index <= -1 && width ? num->limbs[size + index] >> (LMP_LIMB_BIT - width) : 0;
    const lmp_limb_t res = val1 | val2;
    return res;
  }
  lmp_limb_t val1 = (-1 <= index && index <= size - 2) ? ((num->limbs[size + index + 1]) << width) : 0;
  lmp_limb_t val2 = ( 0 <= index && index <= size - 1 && width)
                  ? ((num->limbs[size + index]) >> (LMP_LIMB_BIT - width)) : 0;
  const lmp_limb_t res = val1 | val2;
  return res;
}

inline lmp_double_limb_t MP_INT_left_shift_load_double(const MP_INT *restrict num, const int32_t index, const int32_t width) {
  const lmp_size_t size = abs(num->size);
  if (index < 0) {
    lmp_double_limb_t val1 = ( 1 <= size + index && index <=  0)
                           ? (((lmp_double_limb_t)num->limbs[size + index - 1]) << width) : 0;
    lmp_double_limb_t val2 = ( 0 <= size + index && index <= -1)
                           ? (((lmp_double_limb_t)num->limbs[size + index    ]) << width) : 0;
    lmp_double_limb_t val3 = (-1 <= size + index && index <= -2)
                           ? (((lmp_double_limb_t)num->limbs[size + index + 1]) << width) : 0;
    val1 >>= LMP_LIMB_BIT;
    val3 <<= LMP_LIMB_BIT;
    const lmp_double_limb_t res = val1 | val2 | val3;
    return res;
  }
  lmp_double_limb_t val1 = ( 1 <= index && index <= size    ) ? (lmp_double_limb_t)num->limbs[index - 1] << width : 0;
  lmp_double_limb_t val2 = ( 0 <= index && index <= size - 1) ? (lmp_double_limb_t)num->limbs[index    ] << width : 0;
  lmp_double_limb_t val3 = (-1 <= index && index <= size - 2) ? (lmp_double_limb_t)num->limbs[index + 1] << width : 0;
  val1 >>= LMP_LIMB_BIT;
  val3 <<= LMP_LIMB_BIT;
  const lmp_double_limb_t res = val1 | val2 | val3;
  return res;
}

inline lmp_limb_t MP_INT_get_limb(const MP_INT *restrict num, const int32_t index) {
  const lmp_size_t size = abs(num->size);
  if (index < 0) { return -size <= index ? num->limbs[size + index] : 0; }
  return  index < size ? num->limbs[index] : 0;
}

inline lmp_double_limb_t MP_INT_get_double_limbs(const MP_INT *restrict num, const int32_t index) {
  const lmp_size_t size = abs(num->size);
  if (index < 0) {
    const lmp_double_limb_t val1 = size + index >= 0 ? (lmp_double_limb_t)num->limbs[size + index] : 0;
    const lmp_double_limb_t val2 = -size - 2 <= index && index <= -2 ? (lmp_double_limb_t)num->limbs[size + index + 1] : 0;
    return (val2 << LMP_LIMB_BIT) | val1;
  }
  const lmp_double_limb_t val1 = index < size ? (lmp_double_limb_t)num->limbs[index] : 0;
  const lmp_double_limb_t val2 = index + 1 < size ? (lmp_double_limb_t)num->limbs[index + 1] : 0;
  return (val2 << LMP_LIMB_BIT) | val1;
}

inline int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2) {
  const lmp_size_t size = max(abs(op1->size), abs(op2->size));
  for (int32_t i = size - 1; i >= 0; i --) {
    if (MP_INT_get_limb(op1, i) > MP_INT_get_limb(op2, i)) { return  1; }
    if (MP_INT_get_limb(op1, i) < MP_INT_get_limb(op2, i)) { return -1; }
  }
  return 0;
}

inline bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, const int32_t offset) {
  const lmp_size_t size1 = abs(op1->size);
  const lmp_size_t size2 = abs(op2->size);
  for (int32_t i = 0; i < size2; i ++) {
    if (MP_INT_get_limb(op1, size1 - 1 - i) < MP_INT_get_limb(op2, size1 - offset - 1 - i)) { return false; }
    if (MP_INT_get_limb(op1, size1 - 1 - i) > MP_INT_get_limb(op2, size1 - offset - 1 - i)) { return true;  }
  }
  return true;
}

inline void MP_INT_from_int(MP_INT *mp_val, const int256_t val, const Allocator *allocator) {
  if (val == 0) { mp_val->size = 0; return; }
  uint256_t abs_val = abs(val);
  const int32_t size = extstd_bit_width(abs_val) / LMP_LIMB_BIT + 1;
  if (-size < mp_val->prec && mp_val->prec <= 0) { return; }
  MP_INT_realloc(mp_val, size, allocator);
  for (int32_t i = 0; i < size; i++){
    mp_val->limbs[i] = (lmp_limb_t) (abs_val >> (i * LMP_LIMB_BIT));
  }
  mp_val->size = val > 0 ? size : -size;
}

inline void MP_INT_from_uint(MP_INT *mp_val, const uint256_t val, const Allocator *allocator) {
  if (val == 0) { mp_val->size = 0; return; }
  const int32_t size = extstd_bit_width(val) / LMP_LIMB_BIT + 1;
  if (-size < mp_val->prec && mp_val->prec <= 0) { return; }
  MP_INT_realloc(mp_val, size, allocator);
  for (int32_t i = 0; i < size; i++){
    mp_val->limbs[i] = (lmp_limb_t) (val >> (i * LMP_LIMB_BIT));
  }
  mp_val->size = size;
}

inline void MP_FLOAT_init(MP_FLOAT *val, const int32_t initial_alloc, const Allocator *allocator) {
  if (val->prec == 0) {
    const lmp_prec_t prec = aligned(abs(initial_alloc));
    val->limbs = allocator->calloc(prec, LMP_LIMB_SIZE);
    val->prec = initial_alloc >= 0 ? prec : -prec;
    val->exp = MP_NUMBER_ZERO;
    val->size = 0;
  }
}

inline void MP_FLOAT_copy(MP_FLOAT *dest, const MP_FLOAT *src, const Allocator *allocator) {
  MP_FLOAT_realloc(dest, src->size, allocator);
  allocator->memcpy(dest->limbs, src->limbs, abs(src->size) * LMP_LIMB_SIZE);
  dest->size = src->size;
  dest->exp = src->exp;
}

inline void MP_FLOAT_clear(MP_FLOAT *val, const Allocator *allocator) {
  if (val->prec > 0 && val->limbs) {
    allocator->free(val->limbs);
    val->limbs = nullptr;
    val->prec = 0;
  }
  val->exp = MP_NUMBER_ZERO;
  val->size = 0;
}

void MP_FLOAT_truncate(MP_FLOAT *val, const lmp_prec_t prec) {
  if (!val->size) { return; }
  lmp_size_t size = abs(val->size);
  const lmp_size_t trun = size - prec;

  for (int32_t i = trun; i < size; i ++) {
    val->limbs[i - trun] = val->limbs[i];
  }
  for (int32_t i = size - trun; i < size; i ++) { val->limbs[i] = 0; }
  val->exp += trun;

  while (size > 0 && val->limbs[size - 1] == 0) { size--; }
  val->size = val->size >= 0 ? size : -size;

  if (val->size == 0) { val->exp = MP_NUMBER_ZERO; }
}

inline void MP_FLOAT_trail(MP_FLOAT *val) {
  if (!val->size) { return; }
  lmp_size_t size = abs(val->size);

  int32_t tail_zeros = 0;
  for (; tail_zeros < size; tail_zeros ++) { if (val->limbs[tail_zeros] != 0) { break; } }
  if (tail_zeros) {
    for (int32_t i = tail_zeros; i < size; i ++) {
      val->limbs[i - tail_zeros] = val->limbs[i];
    }
    for (int32_t i = size - tail_zeros; i < size; i ++) { val->limbs[i] = 0; }
    val->exp += tail_zeros;
  }

  while (size > 0 && val->limbs[size - 1] == 0) { size--; }
  val->size = val->size >= 0 ? size : -size;

  if (val->size == 0) { val->exp = MP_NUMBER_ZERO; }
}

inline int32_t MP_FLOAT_abs_cmp(const MP_FLOAT *op1, const MP_FLOAT *op2) {
  const lmp_size_t size1 = abs(op1->size);
  const lmp_size_t size2 = abs(op2->size);
  const lmp_exp_t  exp1 = op1->exp + size1;
  const lmp_exp_t  exp2 = op2->exp + size2;
  if (exp1 > exp2) { return  1; }
  if (exp1 < exp2) { return -1; }
  const lmp_size_t size = min(size1, size2);
  for (int32_t i = 0; i < size; i++) {
    if (MP_FLOAT_get_limb(op1, size1 - 1 - i) > MP_FLOAT_get_limb(op2, size2 - 1 - i)) { return  1; }
    if (MP_FLOAT_get_limb(op1, size1 - 1 - i) < MP_FLOAT_get_limb(op2, size2 - 1 - i)) { return -1; }
  }
  return sgn(size1 - size2);
}

inline void MP_BASE_NUMBER_clear(MP_BASE_NUMBER *val, const Allocator *allocator) {
  MP_FLOAT_clear((MP_FLOAT *)val, allocator);
  val->BASE.limb_width = 0;
  val->BASE.limb_val = 0;
  val->BASE.base = 0;
}

inline lmp_double_limb_t MP_BASE_INT_get_double_limbs(const MP_BASE_NUMBER *restrict num, const int32_t index) {
  const lmp_double_limb_t limbs = MP_INT_get_double_limbs((MP_INT *)num, index);
  return (limbs >> LMP_LIMB_BIT) * num->BASE.limb_val + (lmp_limb_t)limbs;
}

inline lmp_limb_t MP_BASE_NUMBER_left_shift_load(const MP_BASE_NUMBER *mp_val, const int32_t index, const int32_t width) {
  if (width == 0) { return MP_BASE_INT_get_limb(mp_val, index); }
  const lmp_limb_t mul = lmp_fast_pow(mp_val->BASE.base, width);
  const lmp_limb_t div = mp_val->BASE.limb_val / mul;
  lmp_double_limb_t limb1 = MP_BASE_INT_get_limb(mp_val, index - 1);
  lmp_double_limb_t limb2 = MP_BASE_INT_get_limb(mp_val, index    );
  return (limb2 * mul) % mp_val->BASE.limb_val + limb1 / div;
}

inline lmp_double_limb_t MP_BASE_NUMBER_left_shift_load_double(const MP_BASE_NUMBER *mp_val, const int32_t index, const int32_t width) {
  if (width == 0) { return MP_BASE_INT_get_double_limbs(mp_val, index); }
  const lmp_limb_t mul = lmp_fast_pow(mp_val->BASE.base, width);
  const lmp_limb_t div = mp_val->BASE.limb_val / mul;
  lmp_double_limb_t limb1 = MP_BASE_INT_get_limb(mp_val, index - 1);
  lmp_double_limb_t limb2 = MP_BASE_INT_get_limb(mp_val, index    );
  lmp_double_limb_t limb3 = MP_BASE_INT_get_limb(mp_val, index + 1);
  limb3 = (limb3 * mul) % mp_val->BASE.limb_val + limb2 / div;
  limb2 = (limb2 * mul) % mp_val->BASE.limb_val + limb1 / div;
  return limb3 * mp_val->BASE.limb_val + limb2;
}

void MP_BASE_INT_to_int(MP_INT *dest, const MP_BASE_NUMBER *src, const Allocator *allocator) {
  if (src->BASE.limb_val == 0) {
    MP_INT_copy(dest, (MP_INT *)src, allocator);
    MP_INT_clamp(dest);
    return;
  }
  const int32_t size = abs(src->size);
  if (size == 0) { dest->size = 0; return; }
  MP_INT_realloc(dest, src->size, allocator);
  lmp_limb_t limb = MP_BASE_INT_get_limb(src, -1);
  MP_INT_from_uint(dest, limb, allocator);
  for (int32_t i = -2; i >= -size; i--) {
    MP_INT_abs_mul_limb(dest, dest, src->BASE.limb_val, allocator);
    limb = MP_BASE_INT_get_limb(src, i);
    MP_INT_abs_add_limb(dest, dest, limb, allocator);
  }
  dest->size = sgn(src->size) * dest->size;
}

void MP_BASE_FLOAT_to_float(MP_FLOAT *dest, const MP_BASE_NUMBER *src, const Allocator *allocator) {
  if (src->BASE.limb_val == 0) {
    MP_FLOAT_copy(dest, (MP_FLOAT *)src, allocator);
    MP_FLOAT_clamp(dest);
    return;
  }
  MP_BASE_INT_to_int((MP_INT *)dest, src, allocator);
  const lmp_exp_t exp = src->exp;
  if (extstd_has_single_bit(src->BASE.base)) {
    const int32_t base_width = extstd_bit_width(src->BASE.base - 1);
    lmp_exp_t dest_exp, shift_count;
    if (exp >= 0) {
      const lmp_exp_t n_bits = exp * base_width;
      dest_exp = n_bits / (lmp_exp_t)LMP_LIMB_BIT;
      shift_count = n_bits % (lmp_exp_t)LMP_LIMB_BIT;
      MP_INT_left_shift((MP_INT *)dest, (int32_t)shift_count, allocator);
      MP_FLOAT_clamp(dest);
    } else {
      const lmp_exp_t n_bits = exp * base_width;
      dest_exp = n_bits / (lmp_exp_t)LMP_LIMB_BIT - 1;
      shift_count = LMP_LIMB_BIT + n_bits % (int32_t) LMP_LIMB_BIT;
    }
    MP_INT_left_shift((MP_INT *)dest, (int32_t)shift_count, allocator);
    dest->exp = dest_exp;
    MP_FLOAT_clamp(dest);
  } else {
    if (exp >= 0) {
      dest->exp = 0;
      for (int32_t i = 0; i < exp; i++) {
        MP_INT_abs_mul_limb((MP_INT *)dest, (MP_INT *)dest, src->BASE.limb_val, allocator);
        if (abs(dest->size) >= dest->prec && i < exp - 1) {
          MP_INT_right_shift((MP_INT *)dest, LMP_LIMB_BIT, allocator);
          dest->exp ++;
        }
      }
      MP_FLOAT_clamp(dest);
    } else {
      MP_INT DIVISOR = {};
      MP_INT_init(&DIVISOR, (int32_t) -exp + 1, allocator);
      DIVISOR.size = 1; DIVISOR.limbs[0] = 1;
      for (int32_t i = 0; i < -exp; i++) {
        MP_INT_abs_mul_limb(&DIVISOR, &DIVISOR, src->BASE.limb_val, allocator);
      }
      MP_INT_clamp(&DIVISOR);
      MP_INT QUOTIENT = {};
      MP_INT_init(&QUOTIENT, abs(dest->prec), allocator);
      MP_INT_left_shift((MP_INT *)dest, abs(DIVISOR.size) * LMP_LIMB_BIT, allocator);
      MP_INT_divmod((MP_INT *)dest, &DIVISOR, &QUOTIENT, allocator);
      if (QUOTIENT.size == 0) {
        MP_INT_clear(&QUOTIENT, allocator);
        dest->exp = MP_NUMBER_ZERO;
        dest->size = 0;
        return ;
      }
      const lmp_size_t quotient_size = abs(QUOTIENT.size);
      allocator->memcpy(dest->limbs, QUOTIENT.limbs, quotient_size * LMP_LIMB_SIZE);
      dest->size = sgn(src->size) * QUOTIENT.size;
      dest->exp = - abs(DIVISOR.size);
      MP_INT_clear(&DIVISOR, allocator);
      MP_INT_clear(&QUOTIENT, allocator);
      MP_FLOAT_clamp(dest);
    }
  }
}

typedef struct BaseCache {
  uint32_t    count;
  MP_INT     *mpint;
  lmp_limb_t *limbs;
} BaseCache;
// static BaseCache LMP_GLOBAL_BASE_CACHE[] = {};

void MP_INT_to_base_number(MP_BASE_NUMBER *dest, const MP_INT *mp_val, const uint32_t base, const Allocator *allocator) {
  if (base < 2) {
    dest->size = 0;
    dest->BASE.base = base;
    dest->BASE.limb_width = 0;
    dest->BASE.limb_val = 0;
    if (base == 0) {
      // polynomial's exp is its size, i.e. degree + 1
      dest->exp = mp_val->size;
    } else if (base == 1) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INF;
    } else {
      dest->exp = sgn(mp_val->size) * MP_NUMBER_NAN;
    }
    return;
  }
  const double base_width = extstd_has_single_bit(base) ? extstd_bit_width(base - 1) : LMP_LIMB_BIT / log2(base);
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)base_width;
  const lmp_double_limb_t BASE_LIMB_VAL = lmp_fast_pow(base, BASE_LIMB_WIDTH);
  dest->BASE.limb_val = BASE_LIMB_VAL;
  dest->BASE.limb_width = BASE_LIMB_WIDTH;
  dest->BASE.base = base;
  if (extstd_has_single_bit(base) && LMP_LIMB_BIT % (uint32_t)base_width == 0) {
    if (dest->prec < 0) {
      if (abs(mp_val->size) > -dest->prec) { dest->size = 0; return ; }
    } else if (dest->prec >= 1) {
      MP_INT_realloc((MP_INT *)dest, abs(mp_val->size), allocator);
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)dest, abs(mp_val->size), allocator);
    }
    dest->exp = 0;
    dest->size = abs(mp_val->size);
    allocator->memcpy(dest->limbs, mp_val->limbs, dest->size * sizeof(lmp_limb_t));
    MP_BASE_INT_clamp(dest);
    return;
  }

  const lmp_size_t size = abs(mp_val->size);
  if (size == 0) {
    dest->size = 0;
    dest->BASE.limb_val = BASE_LIMB_VAL;
    dest->BASE.limb_width = BASE_LIMB_WIDTH;
    dest->BASE.base = base;
    dest->exp = 0;
    return;
  }
  int32_t dest_size = ceil(size * base_width / BASE_LIMB_WIDTH);

  if (dest_size == 1) {
    if (dest->prec < 0) {
      if (dest_size > -dest->prec) { dest->size = 0; return ; }
      dest->limbs[0] = mp_val->limbs[0];
    } else if (dest->prec >= 1) {
      dest->limbs[0] = mp_val->limbs[0];
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)dest, 1, allocator);
      dest->limbs[0] = mp_val->limbs[0];
    }
    dest->size = mp_val->size >= 0 ? 1 : -1;
    dest->BASE.limb_val = BASE_LIMB_VAL;
    dest->BASE.limb_width = BASE_LIMB_WIDTH;
    dest->BASE.base = base;
    dest->exp = 0;
    return;
  }

  const uint32_t level_count = extstd_bit_width(extstd_bit_ceil((uint32_t)dest_size) - 1);
  bool free_cache = false;
  BaseCache *cache = nullptr;
  // TODO: use global base cache
  if (!cache) { cache = allocator->calloc(1, sizeof(BaseCache)); free_cache = true; }
  if (cache->count < level_count) {
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    void *ptr = nullptr;
    ptr = allocator->realloc(cache->mpint, level_count * sizeof(MP_INT));
    if (ptr) { cache->mpint = ptr; } else if (free_cache) { allocator->free(cache->mpint); cache->mpint = nullptr; }
    const uint32_t alloc_limbs = 1 << level_count;
    ptr = allocator->realloc(cache->limbs, alloc_limbs * sizeof(lmp_limb_t));
    if (ptr) { cache->limbs = ptr; } else if (free_cache) { allocator->free(cache->mpint); cache->limbs = nullptr; }
    if (cache->count == 0) {
      cache->limbs[0] = BASE_LIMB_VAL;
      cache->mpint[0] = (MP_INT){.prec = -1, .size = 1, .limbs = &cache->limbs[0]};
      cache->count += 1;
    }
    for (uint32_t i = cache->count; i < level_count; i++) {
      const int32_t cache_size = 1 << i;
      const int32_t cache_limb_offset = (1 << i) - 1;
      cache->mpint[i].prec = -cache_size;
      cache->mpint[i].size = cache_size;
      cache->mpint[i].limbs = &cache->limbs[cache_limb_offset];
      MP_INT_mul(&cache->mpint[i], &cache->mpint[i - 1], &cache->mpint[i - 1], nullptr);
    }
    cache->count = level_count;
  }
  const lmp_prec_t result_limb_alloc = 1 << level_count;
  lmp_limb_t *result_limbs = allocator->calloc(result_limb_alloc, sizeof(lmp_limb_t));
  MP_INT copy_val = { .prec = -size, .size = 0, .limbs = result_limbs };
  MP_INT_copy(&copy_val, mp_val, allocator);

  uint32_t divmod_task_count = 0;
  struct divmod_task { lmp_limb_t *limbs; int32_t size; int32_t level; };
  struct divmod_task *DIVMOD_TASK_QUEUE = allocator->calloc(level_count + 2, sizeof(struct divmod_task));
  DIVMOD_TASK_QUEUE[divmod_task_count].limbs = copy_val.limbs;
  DIVMOD_TASK_QUEUE[divmod_task_count].size = copy_val.size;
  DIVMOD_TASK_QUEUE[divmod_task_count].level = level_count - 1;
  divmod_task_count += 1;
  while (divmod_task_count > 0) {
    auto const task = DIVMOD_TASK_QUEUE[--divmod_task_count];
    if (task.level < 0) { continue; }
    MP_INT dividend = { .prec = -(1 << task.level), .size = task.size, .limbs = task.limbs };
    const lmp_prec_t quotient_alloc = 1 << task.level;
    lmp_limb_t *quotient_limbs = task.limbs + (1ULL << task.level);
    MP_INT quotient = {.prec = -quotient_alloc, .size = 0, .limbs = quotient_limbs};
    MP_INT_abs_divmod(&dividend, &cache->mpint[task.level], &quotient, allocator);
    DIVMOD_TASK_QUEUE[divmod_task_count].limbs = quotient_limbs;
    DIVMOD_TASK_QUEUE[divmod_task_count].size = quotient.size;
    DIVMOD_TASK_QUEUE[divmod_task_count].level = task.level - 1;
    divmod_task_count += 1;
    DIVMOD_TASK_QUEUE[divmod_task_count].limbs = task.limbs;
    DIVMOD_TASK_QUEUE[divmod_task_count].size = dividend.size;
    DIVMOD_TASK_QUEUE[divmod_task_count].level = task.level - 1;
    divmod_task_count += 1;
  }
  allocator->free(DIVMOD_TASK_QUEUE);
  if (free_cache) {
    allocator->free(cache->mpint);
    allocator->free(cache->limbs);
    allocator->free(cache);
  }
  while (dest_size && !result_limbs[dest_size - 1]) { dest_size--; }

  if (dest->prec < 0) {
    if (dest_size > -dest->prec) { dest->size = 0; return ; }
    allocator->memcpy(dest->limbs, result_limbs, dest_size * sizeof(lmp_limb_t));
    allocator->free(result_limbs);
  } else {
    if (dest->limbs) { allocator->free(dest->limbs); }
    dest->prec = result_limb_alloc;
    dest->limbs = result_limbs;
  }
  dest->size = mp_val->size >= 0 ? dest_size : -dest_size;
  dest->exp = 0;
}

void MP_FLOAT_to_base_number(MP_BASE_NUMBER *dest, const MP_FLOAT *mp_val, uint32_t base, const Allocator * allocator) {
  if (base < 2) {
    dest->size = 0;
    dest->BASE.base = base;
    dest->BASE.limb_width = 0;
    dest->BASE.limb_val = 0;
    if (base == 0) {
      // polynomial's exp is its size, i.e. degree + 1
      dest->exp = mp_val->size;
    } else if (base == 1) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INF;
    } else {
      dest->exp = sgn(mp_val->size) * MP_NUMBER_NAN;
    }
    return;
  }
  const double base_width = extstd_has_single_bit(base) ? extstd_bit_width(base - 1) : LMP_LIMB_BIT / log2(base);
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)base_width;
  const lmp_double_limb_t BASE_LIMB_VAL = lmp_fast_pow(base, BASE_LIMB_WIDTH);
  dest->BASE.limb_val = (lmp_limb_t)BASE_LIMB_VAL;
  dest->BASE.limb_width = BASE_LIMB_WIDTH;
  dest->BASE.base = base;

  if (extstd_has_single_bit(base) && LMP_LIMB_BIT % (uint32_t)base_width == 0) {
    if (dest->prec < 0) {
      if (abs(mp_val->size) > -dest->prec) { dest->size = 0; return ; }
    } else if (dest->prec >= 1) {
      MP_INT_realloc((MP_INT *)dest, abs(mp_val->size), allocator);
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)dest, abs(mp_val->size), allocator);
    }
    dest->size = abs(mp_val->size);
    allocator->memcpy(dest->limbs, mp_val->limbs, dest->size * sizeof(lmp_limb_t));
    dest->exp = mp_val->exp;
    return;
  }

  const lmp_size_t size = abs(mp_val->size);
  if (size == 0) {
    dest->size = 0;
    dest->BASE.limb_val = BASE_LIMB_VAL;
    dest->BASE.limb_width = BASE_LIMB_WIDTH;
    dest->BASE.base = base;
    dest->exp = 0;
    return;
  }
  const lmp_exp_t target_exp = mp_val->exp + size;
  if (target_exp > 0) {
    lmp_limb_t *result_limbs = allocator->calloc(abs(dest->prec), sizeof(lmp_limb_t));
    MP_FLOAT DIVISOR = { .prec = -abs(dest->prec), .size = 0, .limbs = result_limbs, .exp = 0 };
    result_limbs[0] = 1;
    for (lmp_exp_t i = 0; i < target_exp; i++) {
      MP_FLOAT_mantissa_abs_mul_limb(&DIVISOR, &DIVISOR, BASE_LIMB_VAL, allocator);
    }
    // TODO
  } else {
    // TODO
  }

}

void MP_BASE_NUMBER_change_base(MP_BASE_NUMBER *mp_val, const lmp_half_limb_t base, const Allocator *allocator) {
  if (mp_val->BASE.base == base || mp_val->BASE.base < 2) { return; }
  if (base < 2) {
    if (base == 0) /*means convert to a polynomial */ {
      mp_val->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : mp_val->size;
    } else  {
      mp_val->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INF;
    }
    mp_val->size = 0;
    mp_val->BASE.base = base;
    mp_val->BASE.limb_width = 0;
    mp_val->BASE.limb_val = 0;
    return;
  }
  const lmp_size_t size = abs(mp_val->size);
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)((double)LMP_LIMB_BIT / log2(base));
  const lmp_double_limb_t BASE_LIMB_VAL = lmp_fast_pow(base, BASE_LIMB_WIDTH);
  const int32_t target_size = ceil(mp_val->BASE.limb_width * size / log2(base) / BASE_LIMB_WIDTH);

  if (mp_val->size == 0) {
    mp_val->size = 0;
    mp_val->BASE.base = base;
    mp_val->BASE.limb_width = BASE_LIMB_WIDTH;
    mp_val->BASE.limb_val = BASE_LIMB_VAL;
    mp_val->exp = mp_val->exp;
    return;
  }
  if (target_size == 1) {
    if (mp_val->prec < 0) {
      if (target_size > -mp_val->prec) { mp_val->size = 0; return ; }
      mp_val->limbs[0] = mp_val->limbs[0];
    } else if (mp_val->prec >= 1) {
      mp_val->limbs[0] = mp_val->limbs[0];
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)mp_val, 1, allocator);
      mp_val->limbs[0] = mp_val->limbs[0];
    }
    mp_val->size = 1;
    mp_val->BASE.limb_val = BASE_LIMB_VAL;
    mp_val->BASE.limb_width = BASE_LIMB_WIDTH;
    mp_val->BASE.base = base;
    mp_val->exp = 0;
    return;
  }
  MP_INT_realloc((MP_INT *)mp_val, target_size, allocator);

  if (mp_val->exp == 0) {
    MP_INT copy_val = {};
    MP_INT_init(&copy_val, size, allocator);
    MP_BASE_INT_to_int(&copy_val, mp_val, allocator);
    if (BASE_LIMB_VAL != 0) {
      MP_INT_to_base_number(mp_val, &copy_val, base, allocator);
    }
    mp_val->exp = 0;
  } else {
    // TODO: Float
  }
  mp_val->BASE.limb_width = BASE_LIMB_WIDTH;
  mp_val->BASE.limb_val = BASE_LIMB_VAL;
  mp_val->BASE.base = base;
}
