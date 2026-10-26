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
    dest->size = sgn(dest->size) * (size + 1);
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
    dest->size = sgn(dest->size) * (size + 1);
  } else {
    dest->limbs[size] = 0;
    dest->size = size;
  }
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
  dest->size = size;
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
}

inline void MP_INT_abs_add_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *op2,
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
    rop->size = (max_size + 1);
  } else {
    rop->limbs[max_size] = 0;
    rop->size = max_size;
  }
}

inline void MP_INT_abs_sub_offset(MP_INT *rop, const MP_INT *op1, const MP_INT * op2,
                                  const int32_t offset, const Allocator *) {
  const int32_t size1 = abs(op1->size);
  const int32_t size2 = abs(op2->size);
  if (offset + size2 > size1) { return ; }

  bool borrow = 0;
  for (int32_t i = offset; i < size1; i++) {
    const lmp_limb_t val1 = op1->limbs[i];
    const lmp_limb_t val2 = (i - offset < size2) ? op2->limbs[i - offset] : 0;
    rop->limbs[i] = val1 - (val2 + borrow);
    borrow = val1 < val2 || (val1 == val2 && borrow);
  }
  rop->size = op1->limbs[size1 - 1] ? size1 : (size1 - 1);
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
  op->size = sgn(op->size) * res_size;
}

inline void MP_INT_right_shift(MP_INT *op, const int32_t bit_count, const Allocator *) {
  if (op->size == 0) { return; }
  const int32_t size = abs(op->size);
  const int32_t shift_limbs = bit_count / (int32_t) LMP_LIMB_BITS;
  const int32_t shift_bits = bit_count % (int32_t) LMP_LIMB_BITS;

  if (shift_limbs >= size) {
    for (int32_t i = 0; i < size; i ++) { op->limbs[i] = 0; }
    return ;
  }

  for (int32_t i = shift_limbs; i < size-1; i++) {
    const lmp_double_limb_t res = op->limbs[i] | (((lmp_double_limb_t)op->limbs[i + 1]) << LMP_LIMB_BITS);
    op->limbs[i - shift_limbs] = (lmp_limb_t) (res >> shift_bits);
  }
  op->limbs[size - 1 - shift_limbs] = op->limbs[size - 1] >> shift_bits;

  op->size = sgn(op->size) * (size - shift_limbs);
}

inline lmp_double_limb_t MP_INT_abs_3limbs_inv(const lmp_double_limb_t op) {
  const lmp_limb_t d_high = op >> LMP_LIMB_BITS;
  const lmp_double_limb_t n = LMP_DOUBLE_LIMB_MAX - op;
  lmp_double_limb_t d_inv = n / d_high;
  lmp_double_limb_t n_tilde_low = d_inv * (lmp_double_limb_t) (lmp_limb_t) op;
  lmp_double_limb_t n_tilde_high = d_inv * (lmp_double_limb_t) d_high + (n_tilde_low >> LMP_LIMB_BITS);
  if (n < n_tilde_high) { d_inv --; }
  return d_inv;
}

#define d_high (d >> LMP_LIMB_BITS)
inline void MP_INT_abs_divmod(MP_INT *restrict dividend, const MP_INT *restrict divisor,
                              MP_INT *restrict quotient, const Allocator *allocator) {
  if (dividend->size == 0 || MP_INT_abs_cmp(dividend, divisor) < 0) {
    quotient->size = 0;
    return;
  }
  const int32_t size_divisor  = abs(divisor->size);
  const int32_t size_dividend = abs(dividend->size);

  const int32_t d_leading_zeros = __builtin_clzll(divisor->limbs[abs(divisor->size) - 1]);
  const int32_t n_leading_zeros = __builtin_clzll(dividend->limbs[size_dividend - 1]);
  const lmp_double_limb_t d = left_shift_load_double(divisor, -2, d_leading_zeros);

  const bool overflow = n_leading_zeros < d_leading_zeros;
  const int32_t size_quotient = size_dividend - size_divisor + overflow;
  if (size_quotient == 0) {
    MP_INT_abs_sub_offset(dividend, dividend, divisor, 0, allocator);
    quotient->limbs[0] = 1;
    quotient->size = 1;
    return;
  }
  MP_INT_realloc(quotient, size_quotient, allocator);
  quotient->size = size_quotient;

  MP_INT interval = {};
  MP_INT_init(&interval, size_divisor + 1, allocator);
  if (overflow) {
    const lmp_double_limb_t n = left_shift_load_double(dividend, -1, d_leading_zeros);
    lmp_limb_t q = n / d_high;
    const lmp_limb_t n_hat_high = (q * d_high) >> LMP_LIMB_BITS;
    const lmp_limb_t n_high = n >> LMP_LIMB_BITS;
    const lmp_double_limb_t n_hat_tail = ((q * d_high) << LMP_LIMB_BITS) + (((lmp_double_limb_t)q) * (lmp_limb_t) d);
    const lmp_double_limb_t n_tail = (n << LMP_LIMB_BITS) + left_shift_load(dividend, -2, d_leading_zeros);
    if (n_hat_high > n_high || (n_hat_high == n_high && n_hat_tail > n_tail)) { q -= 1; }
    MP_INT_abs_mul_u64(&interval, divisor, q, allocator);
    if (!MP_INT_abs_offset_ge(dividend, &interval, size_quotient - 1)) {
      MP_INT_abs_sub_offset(&interval, &interval, divisor, 0, allocator);
      q --;
    }
    MP_INT_abs_sub_offset(dividend, dividend, &interval, size_quotient - 1, allocator);
    quotient->limbs[size_quotient - 1] = q;
  }
  for (int32_t i = overflow; i < size_quotient; i++) {
    const int32_t load_index = size_dividend - 2 + overflow - i;
    const int32_t curr_q_idx = size_quotient - 1 - i;
    lmp_double_limb_t n = left_shift_load_double(dividend, load_index, d_leading_zeros);
    if (n < d_high) {
      quotient->limbs[curr_q_idx] = 0;
      continue;
    }
    if (n > d) { // maybe never be reached
      MP_INT_abs_sub_offset(dividend, dividend, divisor, curr_q_idx, allocator);
      quotient->limbs[curr_q_idx + 1] += 1;
      n = left_shift_load_double(dividend, load_index, d_leading_zeros);
    }

    lmp_limb_t q = n / d_high;
    const lmp_limb_t n_hat_high = (q * d_high) >> LMP_LIMB_BITS;
    const lmp_limb_t n_high = n >> LMP_LIMB_BITS;
    const lmp_double_limb_t n_hat_tail = ((q * d_high) << LMP_LIMB_BITS) + (((lmp_double_limb_t)q) * (lmp_limb_t) d);
    const lmp_double_limb_t n_tail = (n << LMP_LIMB_BITS) + left_shift_load(dividend, -3, d_leading_zeros);
    if (n_hat_high > n_high || (n_hat_high == n_high && n_hat_tail > n_tail)) { q -= 1; }
    if (q == 0 && MP_INT_abs_offset_ge(dividend, divisor, curr_q_idx)) { q += 1; }

    MP_INT_abs_mul_u64(&interval, divisor, q, allocator);
    if (!MP_INT_abs_offset_ge(dividend, &interval, curr_q_idx)) {
      MP_INT_abs_sub_offset(&interval, &interval, divisor, 0, allocator);
      q -= 1;
    }
    MP_INT_abs_sub_offset(dividend, dividend, &interval, curr_q_idx, allocator);
    MP_INT_clamp(dividend);

    quotient->limbs[curr_q_idx] = q;
  }
  MP_INT_clear(&interval, allocator);
}

void MP_INT_mul_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator) {
  MP_INT_abs_mul_u64(dest, op, val, allocator);
  dest->size = sgn(op->size) * dest->size;
}

void MP_INT_add_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator) {
  if (op->size < 0) {
    MP_INT_abs_sub_u64(dest, op, val, allocator);
    dest->size = - abs(dest->size);
  } else {
    MP_INT_abs_add_u64(dest, op, val, allocator);
    dest->size = abs(dest->size);
  }
}

void MP_INT_sub_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator) {
  if (op->size > 0) {
    MP_INT_abs_sub_u64(dest, op, val, allocator);
    dest->size = abs(dest->size);
  } else {
    MP_INT_abs_add_u64(dest, op, val, allocator);
    dest->size = - abs(dest->size);
  }
}

void MP_INT_mul(MP_INT *restrict rop, const MP_INT *restrict op1,
                    const MP_INT *restrict op2, const Allocator *allocator) {
  MP_INT_abs_mul(rop, op1, op2, allocator);
  rop->size = sgn(op1->size) * sgn(op2->size) * abs(rop->size);
}
// Allow rop == op1
void MP_INT_add(MP_INT *rop, const MP_INT *op1, const MP_INT * op2, const Allocator *allocator) {
  if (sgn(op1->size) * sgn(op2->size) > 0) {
    MP_INT_abs_add_offset(rop, op1, op2, 0, allocator);
    rop->size = sgn(op1->size) * rop->size;
  } else if (MP_INT_abs_cmp(op1, op2) > 0) {
    MP_INT_abs_sub_offset(rop, op1, op2, 0, allocator);
    rop->size = sgn(op1->size) * rop->size;
  } else if (MP_INT_abs_cmp(op1, op2) < 0) {
    MP_INT_abs_sub_offset(rop, op2, op1, 0, allocator);
    rop->size = sgn(op2->size) * rop->size;
  } else {
    rop->size = 0;
  }
}

void MP_INT_sub(MP_INT *rop, const MP_INT *op1, const MP_INT * op2, const Allocator *allocator) {
  if (sgn(op1->size) * sgn(op2->size) < 0) {
    MP_INT_abs_add_offset(rop, op1, op2, 0, allocator);
    rop->size = - rop->size;
  } else if (MP_INT_abs_cmp(op1, op2) > 0) {
    MP_INT_abs_sub_offset(rop, op1, op2, 0, allocator);
    rop->size = sgn(op1->size) * rop->size;
  } else if (MP_INT_abs_cmp(op1, op2) < 0) {
    MP_INT_abs_sub_offset(rop, op2, op1, 0, allocator);
    rop->size = - sgn(op1->size) * rop->size;
  } else {
    rop->size = 0;
  }
}

// dividend as remainder
void MP_INT_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator) {
  int32_t sgn_dividend = sgn(dividend->size);
  MP_INT_abs_divmod(dividend, divisor, quotient, allocator);
  if (sgn_dividend * sgn(divisor->size) < 0 && dividend->size) {
    MP_INT_abs_add_u64(quotient, quotient, 1, allocator);
    quotient->size = - quotient->size;
    MP_INT_abs_sub_offset(dividend, divisor, dividend, 0, allocator);
  }
  dividend->size = sgn(divisor->size) * dividend->size;
}
