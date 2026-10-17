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
 * Filename: arith.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-14
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "arith.h"

#include <stdlib.h>

#include "array.h"
#include "mp_type.h"

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a >> ((sizeof(a) - 1) * 8 + 7)) ? -1 : 1)

inline void MP_INT_abs_mul_u64(MP_INT *dest, const MP_INT *op, const uint64_t val, const Allocator *allocator) {
  if (op->size == 0 || val == 0) { dest->size = 0; return; }

  const int32_t size = abs(op->size);
  MP_INT_realloc(dest, size + 1, allocator);

  lmp_limb_t carry = 0;
  for (int32_t i = 0; i < size; i++) {
    const lmp_double_limb_t prod = (lmp_double_limb_t) op->limbs[i] * val + carry;
    carry = (lmp_limb_t)(prod >> LMP_LIMB_BITS);
    dest->limbs[i] = (lmp_limb_t) prod;
  }
  if (carry) {
    dest->limbs[size] = carry;
    dest->size = size + 1;
  } else {
    dest->limbs[size] = 0;
    dest->size = size;
  }
}

inline void MP_INT_abs_add_u64(MP_INT *dest, const MP_INT *op, const uint64_t val, const Allocator *allocator) {
  const int32_t size = abs(op->size);
  if (size == 0) {
    dest->limbs[0] = val;
    dest->size = val ? 1 : 0;
    return;
  }
  MP_INT_realloc(dest, size + 1, allocator);
  lmp_limb_t carry = val;
  for (int32_t i = 0; i < size; i++) {
    const lmp_limb_t sum = op->limbs[i] + carry;
    carry = (sum < op->limbs[i]) ? 1 : 0;
    dest->limbs[i] = sum;
  }
  if (carry) {
    dest->limbs[size] = 1;
    dest->size = abs(size) + 1;
  } else {
    dest->limbs[size] = 0;
    dest->size = size;
  }
  MP_INT_clamp(dest);
}

void MP_INT_abs_sub_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *) {
  const int32_t size = abs(op->size);
  if (size == 0) {
    dest->limbs[0] = val;
    dest->size = val ? -1 : 0;
    return;
  }
  bool borrow = op->limbs[0] < val;
  dest->limbs[0] = op->limbs[0] - val;
  for (int32_t i = 1; i < size; i++) {
    const bool new_borrow = op->limbs[i] < borrow;
    dest->limbs[i] = op->limbs[i] - borrow;
    borrow = new_borrow;
  }
  dest->size = (dest->size >= 0) ? size : -size;
  MP_INT_clamp(dest);
}

inline void MP_INT_abs_mul(MP_INT *rop, const MP_INT *op1, const MP_INT * restrict op2, const Allocator *allocator) {
  const int32_t size1 = abs(op1->size);
  const int32_t size2 = abs(op2->size);
  if (size1 == 0 || size2 == 0) { rop->size = 0; return; }

  MP_INT_realloc(rop, size1 + size2, allocator);

  if (size1 + size2 > USING_FFT_SIZE) {
    // TODO: using FFT
  }

  lmp_limb_t *const res_limbs = rop->limbs;
  for (int i = 0; i < size1; i++) {
    lmp_limb_t carry = 0;
    for (int j = 0; j < size2; j++) {
      const lmp_double_limb_t prod = (lmp_double_limb_t)op1->limbs[i] * op2->limbs[j] + res_limbs[i + j] + carry;
      res_limbs[i + j] = (lmp_limb_t) prod;
      carry = prod >> LMP_LIMB_BITS;
    }
    res_limbs[i + size2] += carry;
  }
  rop->size = size1 + size2;
  MP_INT_clamp(rop);
}

inline void MP_INT_abs_add_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *restrict op2,
                                  const int32_t offset, const Allocator *allocator) {
  const int32_t size1 = abs(op1->size);
  const int32_t size2 = abs(op2->size);
  const int32_t max_size = max(size1, size2 + offset);
  MP_INT_realloc(rop, max_size + 1, allocator);

  lmp_limb_t carry = 0;
  for (int32_t i = 0; i < max_size; i++) {
    const lmp_limb_t val1 = (i < size1) ? op1->limbs[i] : 0;
    const lmp_limb_t val2 = (i >= offset && i - offset < size2) ? op2->limbs[i - offset] : 0;
    const lmp_limb_t sum = val1 + val2;
    const lmp_limb_t c1  = (sum < val1);
    const lmp_limb_t sum2 = sum + carry;
    const lmp_limb_t c2  = (sum2 < sum);
    rop->limbs[i] = sum2;
    carry = c1 | c2;
  }
  if (carry) {
    rop->limbs[max_size] = 1;
    rop->size = max_size + 1;
  } else {
    rop->limbs[max_size] = 0;
    rop->size = max_size;
  }
}

inline void MP_INT_abs_sub_offset(MP_INT *rop, const MP_INT *op1, const MP_INT * restrict op2,
                                  const int32_t offset, const Allocator *) {
  const int32_t size1 = abs(op1->size);
  const int32_t size2 = abs(op2->size);
  const int32_t max_size = max(size1, size2);

  bool borrow = 0;
  for (int32_t i = offset; i < max_size; i++) {
    const lmp_limb_t val1 = (i < size1) ? op1->limbs[i] : 0;
    const lmp_limb_t val2 = (i - offset < size2) ? op2->limbs[i - offset] : 0;
    rop->limbs[i] = val1 - (val2 + borrow);
    borrow = val1 < val2 || (val1 == val2 && borrow);
  }
  rop->size = max_size;
  MP_INT_clamp(rop);
}

inline void MP_INT_left_shift(MP_INT *op, const int32_t bit_count, const Allocator *allocator) {
  if (op->size == 0) { return; }
  const int32_t size = abs(op->size);
  const int32_t shift_limbs = bit_count / (int32_t) LMP_LIMB_BITS;
  const int32_t shift_bits = bit_count % (int32_t) LMP_LIMB_BITS;

  const int32_t leading_zeros = __builtin_clzll(op->limbs[size - 1]);
  const int32_t res_size = size + shift_limbs + (shift_bits > leading_zeros);
  MP_INT_realloc(op, res_size, allocator);

  for (int32_t i = res_size - 1; i >= 0; i--) {
    const lmp_limb_t current = (i >= shift_limbs && i - shift_limbs <= size - 1)
                             ? op->limbs[i - shift_limbs] : 0;
    op->limbs[i] = shift_bits ? current << shift_bits : current;
    if (i + 1 <= res_size - 1) {
      op->limbs[i + 1] |= shift_bits ? current >> (LMP_LIMB_BITS - shift_bits) : 0;
    }
  }
  op->size = op->size > 0 ? (res_size) : -res_size;
  MP_INT_clamp(op);
}

inline void MP_INT_right_shift(MP_INT *op, const int32_t bit_count, const Allocator *) {
  if (op->size == 0) { return; }
  const int32_t size = abs(op->size);
  const int32_t shift_limbs = bit_count / (int32_t) LMP_LIMB_BITS;
  const int32_t shift_bits = bit_count % (int32_t) LMP_LIMB_BITS;

  if (shift_limbs >= size) {
    for (int32_t i = 0; i < size; i ++) { op->limbs[i] = 0; }
    return MP_INT_clamp(op);
  }

  for (int32_t i = shift_limbs; i < size-1; i++) {
    const lmp_double_limb_t res = op->limbs[i] | (((lmp_double_limb_t)op->limbs[i + 1]) << LMP_LIMB_BITS);
    op->limbs[i - shift_limbs] = (lmp_limb_t) (res >> shift_bits);
  }
  op->limbs[size - 1 - shift_limbs] = op->limbs[size - 1] >> shift_bits;

  for (int32_t i = size - shift_limbs; i < size; i ++) { op->limbs[i] = 0; }
  MP_INT_clamp(op);
}

#define left_shift_load(int_num, shift_bits) (                                                      \
    ((int_num)->size > 1) ?                                                                         \
  ((((int_num)->limbs[(int_num)->size - 1]) << (shift_bits))                                        \
  | ((shift_bits) ? ((int_num)->limbs[(int_num)->size - 2] >> (LMP_LIMB_BITS - (shift_bits))) : 0)) \
  : (((int_num)->limbs[(int_num)->size - 1]) << (shift_bits))                                       \
)

#define left_shift_load_double(int_num, shift_bits) (                                                \
    ((int_num)->size > 1) ? (((int_num)->size > 2)                                                   \
  ? ((((lmp_double_limb_t) (int_num)->limbs[(int_num)->size - 1]) << (LMP_LIMB_BITS + (shift_bits))) \
  | (((lmp_double_limb_t) (int_num)->limbs[(int_num)->size - 2]) << (shift_bits))                    \
  | ((shift_bits) ? ((int_num)->limbs[(int_num)->size - 3] >> (LMP_LIMB_BITS - (shift_bits))) : 0))  \
  : ((((lmp_double_limb_t) (int_num)->limbs[(int_num)->size - 1]) << (LMP_LIMB_BITS + (shift_bits))) \
  | (((lmp_double_limb_t) (int_num)->limbs[(int_num)->size - 2]) << (shift_bits))))                  \
  : (((lmp_double_limb_t) (int_num)->limbs[(int_num)->size - 1]) << (LMP_LIMB_BITS + (shift_bits)))  \
)

inline lmp_limb_t MP_INT_abs_3limbs_inv(const lmp_double_limb_t divisor) {
  const lmp_limb_t d_high = divisor >> LMP_LIMB_BITS;
  const lmp_double_limb_t n = LMP_DOUBLE_LIMB_MAX - divisor;
  lmp_double_limb_t d_inv = n / d_high;
  lmp_double_limb_t n_tilde_low = d_inv * (lmp_double_limb_t) (lmp_limb_t) divisor;
  lmp_double_limb_t n_tilde_high = d_inv * (lmp_double_limb_t) d_high + (n_tilde_low >> LMP_LIMB_BITS);
  if (n < n_tilde_high) {
    n_tilde_high -= d_high + (n_tilde_low < (lmp_limb_t) divisor);
    n_tilde_low -= (lmp_limb_t) divisor;
    d_inv --;
  }
  return d_inv;
}

inline void MP_INT_abs_divmod(MP_INT *restrict dividend, MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator) {
  if (dividend->size == 0) {
    quotient->size = 0;
    return ;
  }
  const int32_t d_leading_zeros = __builtin_clzll(divisor->limbs[divisor->size - 1]);
  MP_INT_left_shift(divisor, d_leading_zeros, allocator);
  MP_INT_left_shift(dividend, d_leading_zeros, allocator);
  MP_INT_abs_divmod_core(dividend, divisor, quotient, allocator);
  MP_INT_right_shift(dividend, d_leading_zeros, allocator);
  MP_INT_right_shift(divisor, d_leading_zeros, allocator);
}

inline void MP_INT_abs_divmod_core(MP_INT *restrict dividend, const MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator) {
  const int32_t size_divisor = abs(divisor->size);
  if (abs(dividend->size) < size_divisor) {
    quotient->size = 0;
    return;
  }
  const int32_t size_quotient = abs(dividend->size) - size_divisor;
  MP_INT_realloc(quotient, size_quotient, allocator);

  const lmp_double_limb_t d = left_shift_load_double(divisor, 0);
  const lmp_limb_t d_inv = MP_INT_abs_3limbs_inv(d);

  MP_INT interval = {};
  MP_INT_init(&interval, size_divisor + 1, allocator);
  Array *q_limbs_array = Array_new(LMP_LIMB_SIZE, 1, allocator);
  while (true) {
    const int32_t size_dividend = abs(dividend->size);
    const lmp_limb_t n_high = dividend->limbs[size_dividend - 1];
    const lmp_limb_t n_low = size_dividend > 1 ? dividend->limbs[size_dividend - 2] : 0;
    const lmp_double_limb_t n = ((lmp_double_limb_t)n_high << (LMP_LIMB_BITS)) | n_low;
    lmp_double_limb_t q = (LMP_LIMB_VAL + d_inv) * n_high + n_low;
    lmp_limb_t q_high = (q >> LMP_LIMB_BITS) + 1;
    const lmp_double_limb_t n_tilde = q_high * d;
    if (n_tilde > n) { q_high --; }
    if (n > d) { q_high ++; }

    MP_INT_abs_mul_u64(&interval, divisor, q_high, allocator);
    const int32_t size_interval = abs(interval.size);
    MP_INT_abs_sub_offset(dividend, dividend, &interval, size_dividend - size_interval, allocator);
    Array_append(q_limbs_array, &q_high, 1);
    MP_INT_clamp(dividend);
    for (int32_t k = 0; k < size_dividend - abs(dividend->size) - 1; k++) {
      q_high = 0;
      Array_append(q_limbs_array, &q_high, 1);
    }
    if (MP_INT_abs_cmp(dividend, divisor) < 0) { break;  }
  }
  const uint32_t n_limbs = Array_length(q_limbs_array);
  const lmp_limb_t * const limbs = Array_first_real(q_limbs_array);
  for (uint32_t i = 0; i < n_limbs; i++) {
    quotient->limbs[i] = limbs[n_limbs - 1 - i];
  }
  quotient->size = quotient->size > 0 ? (int32_t) n_limbs : - (int32_t) n_limbs;
  for (int32_t i = (int32_t) n_limbs; i < size_quotient; i++) {
    quotient->limbs[i] = 0;
  }
  releasePrimeArray(q_limbs_array);
}
