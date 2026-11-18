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
 * Module Name: arith
 * Filename: base-number.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-18
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "arith.h"
#include "extbit.h"

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a) > (0) ?   1 :  -1)

static lmp_limb_t fast_pow [[gnu::always_inline]] (lmp_limb_t base, lmp_exp_t exp);

inline lmp_limb_t fast_pow [[gnu::always_inline]] (lmp_limb_t base, lmp_exp_t exp) {
  lmp_limb_t result = 1;
  while (exp > 0) {
    if (exp & 1) { result *= base; }
    base *= base; exp >>= 1;
  }
  return result;
}

inline void MP_BASE_INT_mul_abs_base_exp(MP_BASE_NUMBER *mp_val, const uint32_t abs_exp) {
  if (mp_val->size == 0) { return; }
  const uint32_t offset = abs_exp / mp_val->base_limb_width;
  const uint32_t shift = abs_exp % mp_val->base_limb_width;
  const lmp_limb_t multiplier = fast_pow(mp_val->limb_base, shift);
  for (uint32_t i = abs(mp_val->size) - 1; i < (uint32_t)abs(mp_val->size); i--) {
    lmp_double_limb_t low2_limbs = MP_INT_get_double_limbs((MP_INT *)mp_val, i);
    low2_limbs = (low2_limbs >> LMP_LIMB_BIT) * mp_val->base_limb_val + (lmp_limb_t)low2_limbs;
    mp_val->limbs[i + offset + 1] = (lmp_limb_t)((lmp_quarter_limb_t)low2_limbs * multiplier / mp_val->base_limb_val);
  }
  lmp_double_limb_t low2_limbs = MP_INT_get_double_limbs((MP_INT *)mp_val, 0);
  low2_limbs = (low2_limbs >> LMP_LIMB_BIT)* mp_val->base_limb_val + (lmp_limb_t)low2_limbs;
  mp_val->limbs[offset] = (lmp_limb_t)((lmp_quarter_limb_t)low2_limbs * multiplier % mp_val->base_limb_val);
  for (uint32_t i = 0; i < offset; i++) { mp_val->limbs[i] = 0; }

  MP_INT_clamp((MP_INT*)mp_val);
}

inline void MP_BASE_INT_div_abs_base_exp(MP_BASE_NUMBER *mp_val, const uint32_t abs_exp) {
  if (mp_val->size == 0) { return; }
  const uint32_t offset = abs_exp / mp_val->base_limb_width;
  const uint32_t shift = abs_exp % mp_val->base_limb_width;
  const lmp_limb_t divisor = fast_pow(mp_val->limb_base, shift);
  for (int32_t i = 0; i < abs(mp_val->size); i++) {
    lmp_double_limb_t low2_limbs = MP_INT_get_double_limbs((MP_INT *)mp_val, i + offset);
    low2_limbs = (low2_limbs >> LMP_LIMB_BIT) * mp_val->base_limb_val + (lmp_limb_t)low2_limbs;
    mp_val->limbs[i] = (lmp_limb_t)(low2_limbs / divisor % mp_val->base_limb_val);
  }
  MP_INT_clamp((MP_INT*)mp_val);
}

void MP_BASE_FLOAT_mul_base_exp(MP_BASE_NUMBER *mp_val, lmp_exp_t exp, const Allocator * allocator) {
  if (mp_val->size == 0) { return; }
  mp_val->exp += exp / (lmp_exp_t)mp_val->base_limb_width;
  exp %= mp_val->base_limb_width;
  if (mp_val->base_limb_val == 0) {
    const uint32_t scale = extstd_bit_width(mp_val->limb_base - 1);
    if (exp > 0) {
      lmp_limb_t low1_limb = MP_INT_get_limb((MP_INT *)mp_val, 0);
      const lmp_exp_t log_base = extstd_trailing_zeros(low1_limb);
      if (log_base + exp * scale >= (lmp_exp_t)LMP_LIMB_BIT) {
        mp_val->exp += 1;
        const uint32_t shift_width = LMP_LIMB_BIT - exp * scale;
        MP_INT_right_shift((MP_INT *)mp_val, shift_width, allocator);
      } else {
        const lmp_exp_t shift_width = exp * scale;
        MP_INT_realloc((MP_INT *)mp_val, abs(mp_val->size) + 1, allocator);
        MP_INT_left_shift((MP_INT *)mp_val, shift_width, allocator);
      }
    } else if (exp < 0) {
      lmp_limb_t low1_limb = MP_INT_get_limb((MP_INT *)mp_val, 0);
      const lmp_exp_t log_base = extstd_trailing_zeros(low1_limb);
      if (log_base + exp * scale < 0) {
        mp_val->exp -= 1;
        const uint32_t shift_width = LMP_LIMB_BIT + exp * scale;
        MP_INT_realloc((MP_INT *)mp_val, abs(mp_val->size) + 1, allocator);
        MP_INT_left_shift((MP_INT *)mp_val, shift_width, allocator);
      } else {
        const lmp_exp_t shift_width = -exp * scale;
        MP_INT_right_shift((MP_INT *)mp_val, shift_width, allocator);
      }
    }
  } else {
    if (exp > 0) {
      lmp_exp_t log_base = 0;
      lmp_limb_t low1_limb = MP_INT_get_limb((MP_INT *)mp_val, 0);
      if (low1_limb == 0) { log_base = mp_val->base_limb_width; }
      else while (low1_limb % mp_val->limb_base == 0) { low1_limb /= mp_val->limb_base; log_base++; }
      if (log_base + exp >= mp_val->base_limb_width) {
        mp_val->exp += 1;
        const uint32_t shift_width = mp_val->base_limb_width - exp;
        MP_BASE_INT_div_abs_base_exp(mp_val, shift_width);
      } else {
        const lmp_exp_t shift_width = exp;
        MP_INT_realloc((MP_INT *)mp_val, abs(mp_val->size) + 1, allocator);
        MP_BASE_INT_mul_abs_base_exp(mp_val, shift_width);
      }
    } else if (exp < 0) {
      lmp_exp_t log_base = 0;
      lmp_limb_t low1_limb = MP_INT_get_limb((MP_INT *)mp_val, 0);
      if (low1_limb == 0) { log_base = mp_val->base_limb_width; }
      else while (low1_limb % mp_val->limb_base == 0) { low1_limb /= mp_val->limb_base; log_base++; }
      if (log_base + exp < 0) {
        mp_val->exp -= 1;
        const uint32_t shift_width = mp_val->base_limb_width + exp;
        MP_INT_realloc((MP_INT *)mp_val, abs(mp_val->size) + 1, allocator);
        MP_BASE_INT_mul_abs_base_exp(mp_val, shift_width);
      } else {
        const lmp_exp_t shift_width = -exp;
        MP_BASE_INT_div_abs_base_exp(mp_val, shift_width);
      }
    }
  }
}

void MP_BASE_INT_abs_mul_u64(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, const uint64_t val, const Allocator *allocator) {
  if (op->size == 0 || val == 0) { dest->size = 0; return; }

  const lmp_size_t size = abs(op->size);

  lmp_limb_t carry = 0;
  for (int32_t i = 0; i < size; i++) {
    const lmp_double_limb_t prod = (lmp_double_limb_t) op->limbs[i] * val + carry;
    carry = (lmp_limb_t)(prod / dest->base_limb_val);
    dest->limbs[i] = (lmp_limb_t) prod % dest->base_limb_val;
  }
  if (carry) {
    MP_BASE_NUMBER_realloc(dest, size + 1, allocator);
    dest->limbs[size] = carry;
    dest->size = sgn(dest->size) * (size + 1);
  } else {
    dest->limbs[size] = 0;
    dest->size = size;
  }
}

void MP_BASE_INT_abs_add_limb(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, const uint64_t val, const Allocator *allocator) {
  const lmp_size_t size = abs(op->size);
  if (size == 0) {
    dest->limbs[0] = val;
    dest->size = val ? 1 : 0;
    return;
  }
  lmp_limb_t carry = val;
  for (int32_t i = 0; i < size; i++) {
    const lmp_limb_t sum = (op->limbs[i] + carry) % dest->base_limb_val;
    carry = (sum < op->limbs[i]) ? 1 : 0;
    dest->limbs[i] = sum;
  }
  if (carry) {
    MP_BASE_NUMBER_realloc(dest, size + 1, allocator);
    dest->limbs[size] = 1;
    dest->size = sgn(dest->size) * (size + 1);
  } else {
    dest->limbs[size] = 0;
    dest->size = size;
  }
}

void MP_BASE_INT_abs_sub_limb(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, const uint64_t val, const Allocator *) {
  const lmp_size_t size = abs(op->size);
  if (size == 0) {
    dest->limbs[0] = val;
    dest->size = val ? -1 : 0;
    return;
  }
  if (size == 1) {
    if (op->limbs[0] < val) {
      dest->limbs[0] = val - op->limbs[0];
      dest->size = -1;
      return;
    }
    if (op->limbs[0] > val) {
      dest->limbs[0] = op->limbs[0] - val;
      dest->size =  1;
      return;
    }
    dest->size = 0;
    return;
  }
  bool borrow = op->limbs[0] < val;
  dest->limbs[0] = op->limbs[0] - val;
  for (int32_t i = 1; i < size; i++) {
    const bool new_borrow = op->limbs[i] < borrow;
    dest->limbs[i] = op->limbs[i] - borrow;
    borrow = new_borrow;
  }
  dest->size = dest->limbs[size - 1] ? size : size - 1;
}

inline void MP_BASE_INT_abs_mul(MP_BASE_NUMBER *rop, const MP_BASE_NUMBER *op1,
                                const MP_BASE_NUMBER * restrict op2, const Allocator *allocator) {
  const lmp_size_t size1 = abs(op1->size);
  const lmp_size_t size2 = abs(op2->size);
  if (size1 == 0 || size2 == 0) { rop->size = 0; return; }

  MP_BASE_NUMBER_realloc(rop, size1 + size2, allocator);

  if (size1 + size2 > USING_NTT_SIZE) {
    // TODO: using NTT
  }

  lmp_limb_t *const res_limbs = rop->limbs;
  for (int i = 0; i < size1; i++) {
    lmp_limb_t carry = 0;
    for (int j = 0; j < size2; j++) {
      const lmp_double_limb_t prod = (lmp_double_limb_t)op1->limbs[i] * op2->limbs[j] + res_limbs[i + j] + carry;
      res_limbs[i + j] = prod % rop->base_limb_val;
      carry = prod / rop->base_limb_val;
    }
    res_limbs[i + size2] += carry;
  }
  rop->size = size1 + size2;
}

void MP_BASE_INT_abs_add_offset(MP_BASE_NUMBER *rop, const MP_BASE_NUMBER *op1, const MP_BASE_NUMBER * op2,
                           const int32_t offset, const Allocator *allocator) {
  const lmp_size_t size1 = abs(op1->size);
  const lmp_size_t size2 = abs(op2->size);
  const int32_t max_size = max(size1, size2 + offset);
  MP_BASE_NUMBER_realloc(rop, max_size, allocator);

  lmp_limb_t carry = 0;
  for (int32_t i = 0; i < max_size; i++) {
    const lmp_limb_t val1 = (i < size1) ? op1->limbs[i] : 0;
    const lmp_limb_t val2 = (i >= offset && i - offset < size2) ? op2->limbs[i - offset] : 0;
    const lmp_limb_t sum = ((lmp_double_limb_t)val1 + val2) % rop->base_limb_val;
    const lmp_limb_t c1  = sum < val1;
    const lmp_limb_t sum2 = (sum + carry) % rop->base_limb_val;
    const lmp_limb_t c2  = sum2 < sum;
    rop->limbs[i] = sum2;
    carry = c1 | c2;
  }
  if (carry) {
    MP_BASE_NUMBER_realloc(rop, max_size + 1, allocator);
    rop->limbs[max_size] = 1;
    rop->size = (max_size + 1);
  } else {
    rop->limbs[max_size] = 0;
    rop->size = max_size;
  }
}

void MP_BASE_INT_abs_sub_offset(MP_BASE_NUMBER *rop, const MP_BASE_NUMBER *op1, const MP_BASE_NUMBER * op2,
                           const int32_t offset, const Allocator *) {
  const lmp_size_t size1 = abs(op1->size);
  const lmp_size_t size2 = abs(op2->size);
  if (offset + size2 > size1) { return ; }

  bool borrow = 0;
  for (int32_t i = offset; i < size1; i++) {
    const lmp_limb_t val1 = op1->limbs[i];
    const lmp_limb_t val2 = (i - offset < size2) ? op2->limbs[i - offset] : 0;
    rop->limbs[i] = val1 > val2 + borrow ? val1 - (val2 + borrow) : rop->base_limb_val - val2 + borrow + val1;
    borrow = val1 < val2 || (val1 == val2 && borrow);
  }
  rop->size = op1->limbs[size1 - 1] ? size1 : (size1 - 1);
}

void MP_BASE_INT_abs_divmod(MP_BASE_NUMBER *restrict dividend,const MP_BASE_NUMBER *restrict divisor,
                            MP_BASE_NUMBER *restrict quotient, const Allocator *allocator) {
  if (dividend->size == 0 || MP_BASE_INT_abs_cmp(dividend, divisor) < 0) {
    quotient->size = 0;
    return;
  }
  const lmp_size_t size_divisor  = abs(divisor->size);
  const lmp_size_t size_dividend = abs(dividend->size);
  // TODO
}