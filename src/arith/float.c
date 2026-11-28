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
 * Filename: float.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-18
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include <extbit.h>

#include "limb.h"
#include "LMP/arith.h"
#include "mp-float.h"

#define max(a, b)   ((a) >  (b) ? (a) :  (b))
#define abs(a)      ((a) >  (0) ? (a) : -(a))
#define sgn(a)      ((a) == (0) ? 0 : ((a) > 0) ? 1  :  -1)


void MP_FLOAT_mul_base_exp(MP_FLOAT *op, lmp_exp_t exp, const Allocator *allocator) {
  const lmp_size_t size = abs(op->size);
  if (size == 0) { return; }
  op->exp += exp / (lmp_exp_t)LMP_LIMB_BIT;
  exp %= (lmp_exp_t)LMP_LIMB_BIT;
  lmp_limb_t low1_limb = MP_FLOAT_get_limb(op, 0);
  const lmp_exp_t log_base = extstd_trailing_zeros(low1_limb);
  if (exp > 0) {
    if (log_base + exp >= (lmp_exp_t)LMP_LIMB_BIT) {
      op->exp += 1;
      const uint32_t shift_width = LMP_LIMB_BIT - exp;
      MP_INT_right_shift((MP_INT *)op, shift_width, allocator);
    } else {
      const lmp_exp_t shift_width = exp;
      MP_INT_left_shift((MP_INT *)op, shift_width, allocator);
    }
  } else if (exp < 0) {
    if (log_base + exp < 0) {
      op->exp -= 1;
      const uint32_t shift_width = LMP_LIMB_BIT + exp;
      MP_INT_left_shift((MP_INT *)op, shift_width, allocator);
    } else {
      const lmp_exp_t shift_width = -exp;
      MP_INT_right_shift((MP_INT *)op, shift_width, allocator);
    }
  }
  MP_FLOAT_tidy(op);
}

inline void MP_FLOAT_normal_abs_mul(MP_FLOAT *restrict rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator) {
  MP_INT_abs_mul((MP_INT *)rop, (MP_INT *)op1, (MP_INT *)op2, allocator);
  rop->exp = op1->exp + op2->exp;
}

inline void MP_FLOAT_normal_abs_add(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator) {
  if (op1->exp < op2->exp) {
    MP_INT_abs_add_offset((MP_INT *)rop, (MP_INT *)op1, (MP_INT *)op2, (int32_t)(op2->exp - op1->exp), allocator);
    rop->exp = op1->exp;
  } else {
    MP_INT_abs_add_offset((MP_INT *)rop, (MP_INT *)op2, (MP_INT *)op1, (int32_t)(op1->exp - op2->exp), allocator);
    rop->exp = op2->exp;
  }
}

inline void MP_FLOAT_normal_abs_sub(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator) {
  if (op1->exp < op2->exp) {
    MP_INT_abs_sub_offset((MP_INT *)rop, (MP_INT *)op1, (MP_INT *)op2, (int32_t)(op2->exp - op1->exp), allocator);
    rop->exp = op1->exp;
  } else {
    const lmp_exp_t offset = op1->exp - op2->exp;
    if (rop != op1) {
      MP_FLOAT_realloc(rop, abs(op1->size) + (lmp_prec_t)offset, allocator);
      allocator->memcpy(rop->limbs + offset, op1->limbs, abs(op1->size) * sizeof(lmp_limb_t));
      allocator->memset(rop->limbs, 0,  offset * sizeof(lmp_limb_t));
      rop->size = op1->size + (int32_t)offset;
    } else {
      MP_INT_left_shift((MP_INT *)op1, (int32_t)(offset * LMP_LIMB_BIT), allocator);
    }
    rop->exp = op2->exp;
    MP_INT_abs_sub_offset((MP_INT *)rop, (MP_INT *)rop, (MP_INT *)op2, 0, allocator);
  }
}

void MP_FLOAT_mul(MP_FLOAT *restrict rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator) {
  if (op1->size == 0) {
    if (op1->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    rop->size = 0;
    const lmp_exp_t sgn1 = sgn(op1->exp);
    switch (abs(op1->exp)) {
      case MP_NUMBER_ZERO: { rop->exp = (op2->size == 0 && op2->exp == MP_NUMBER_NAN) ? MP_NUMBER_NAN : MP_NUMBER_ZERO; return; }
      case  MP_NUMBER_INS: {
        if (op2->size == 0) {
          if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
          const lmp_exp_t sgn2 = sgn(op2->exp);
          switch (abs(op2->exp)) {
            case  MP_NUMBER_INS: { rop->exp = sgn1 * sgn2 * MP_NUMBER_INS; return; }
            case MP_NUMBER_ZERO: { rop->exp = MP_NUMBER_ZERO; return; }
            case  MP_NUMBER_INF:
            default: { rop->exp = MP_NUMBER_NAN; return; }
          }
        }
        rop->exp = sgn1 * sgn(op2->size) * MP_NUMBER_INS; return;
      }
      case  MP_NUMBER_INF: {
        if (op2->size == 0) {
          if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
          const lmp_exp_t sgn2 = sgn(op2->exp);
          switch (abs(op2->exp)) {
            case  MP_NUMBER_INF: { rop->exp = sgn1 * sgn2 * MP_NUMBER_INF; return; }
            case MP_NUMBER_ZERO: { rop->exp = MP_NUMBER_ZERO; return; }
            case  MP_NUMBER_INS:
            default: { rop->exp = MP_NUMBER_NAN; return; }
          }
        }
        rop->exp = sgn1 * sgn(op2->size) * MP_NUMBER_INF; return;
      }
      default: { rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  if (op2->size == 0) {
    if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    rop->size = 0;
    switch (abs(op2->exp)) {
      case  MP_NUMBER_INS:
      case  MP_NUMBER_INF:
      case MP_NUMBER_ZERO: { rop->exp = op2->exp; return; }
      default: { rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  MP_FLOAT_normal_abs_mul(rop, op1, op2, allocator);
  rop->size = sgn(op1->size) * sgn(op2->size) * abs(rop->size);
  MP_FLOAT_tidy(rop);
}

inline void MP_FLOAT_add(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * op2, const Allocator *allocator) {
  if (op1->size == 0) {
    if (op1->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    const lmp_exp_t sgn1 = sgn(op1->exp);
    const lmp_exp_t sgn2 = sgn(op2->exp);
    switch (abs(op1->exp)) {
      case MP_NUMBER_ZERO:
      case  MP_NUMBER_INS: {
        return op2->size == 0 && op2->exp == MP_NUMBER_ZERO
              ? MP_FLOAT_copy(rop, op1, allocator)
              : MP_FLOAT_copy(rop, op2, allocator);
      }
      case  MP_NUMBER_INF: {
        rop->size = 0;
        if (op2->size == 0) {
          if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
          if (abs(op2->exp) == MP_NUMBER_INF && sgn1 != sgn2) {
            rop->exp = MP_NUMBER_NAN;
          } else if (op2->exp == MP_NUMBER_NAN){
            rop->exp = MP_NUMBER_NAN;
          } else { rop->exp = op1->exp; }
        } else { rop->exp = op1->exp; }
        return;
      }
      default: { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  if (op2->size == 0) {
    if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    switch (abs(op2->exp)) {
      case MP_NUMBER_ZERO:
      case  MP_NUMBER_INS: { return MP_FLOAT_copy(rop, op1, allocator); }
      case  MP_NUMBER_INF: { rop->size = 0; rop->exp = op2->exp; return; }
      default: { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  const lmp_size_t sgn1 = sgn(op1->size);
  const lmp_size_t sgn2 = sgn(op2->size);
  if (sgn1 == sgn2) {
    MP_FLOAT_normal_abs_add(rop, op1, op2, allocator);
    rop->size = sgn1 * abs(rop->size);
  } else if (MP_FLOAT_abs_cmp(op1, op2) > 0) {
    MP_FLOAT_normal_abs_sub(rop, op1, op2, allocator);
    rop->size = sgn1 * abs(rop->size);
  } else {
    MP_FLOAT tmp = {};
    MP_FLOAT_init(&tmp, abs(op1->size), allocator);
    MP_FLOAT_copy(&tmp, op1, allocator);
    if (rop != op2) { MP_FLOAT_copy(rop, op2, allocator); }
    MP_FLOAT_normal_abs_sub(rop, rop, &tmp, allocator);
    rop->size = - sgn1 * abs(rop->size);
    MP_FLOAT_clear(&tmp, allocator);
  }
  MP_FLOAT_tidy(rop);
}

inline void MP_FLOAT_sub(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * restrict op2, const Allocator *allocator) {
  if (op1->size == 0) {
    if (op1->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    const lmp_exp_t sgn1 = sgn(op1->exp);
    const lmp_exp_t sgn2 = sgn(op2->exp);
    switch (abs(op1->exp)) {
      case MP_NUMBER_ZERO:
      case  MP_NUMBER_INS: {
        if (op2->size == 0) {
          rop->exp = op2->exp == MP_NUMBER_ZERO ? op1->exp : -op2->exp;
          rop->size = 0;
          return;
        }
        MP_FLOAT_copy(rop, op2, allocator);
        rop->size = -rop->size;
        return;
      }
      case  MP_NUMBER_INF: {
        rop->size = 0;
        if (op2->size == 0) {
          if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
          if (abs(op2->exp) == MP_NUMBER_INF && sgn1 == sgn2) {
            rop->exp = MP_NUMBER_NAN;
          } else if (abs(op2->exp) == MP_NUMBER_NAN){
            rop->exp = MP_NUMBER_NAN;
          } else { rop->exp = op1->exp; }
        } else { rop->exp = op1->exp; }
        return;
      }
      default: { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  if (op2->size == 0) {
    if (op2->exp == MP_NUMBER_NAN) { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    switch (abs(op2->exp)) {
      case MP_NUMBER_ZERO:
      case  MP_NUMBER_INS: { return MP_FLOAT_copy(rop, op1, allocator); }
      case  MP_NUMBER_INF: { rop->size = 0; rop->exp = -op2->exp; return; }
      default: { rop->size = 0; rop->exp = MP_NUMBER_NAN; return; }
    }
  }
  const lmp_size_t sgn1 = sgn(op1->size);
  const lmp_size_t sgn2 = sgn(op2->size);
  if (sgn1 != sgn2) {
    MP_FLOAT_normal_abs_add(rop, op1, op2, allocator);
    rop->size = sgn1 * abs(rop->size);
  } else if (MP_FLOAT_abs_cmp(op1, op2) > 0) {
    MP_FLOAT_normal_abs_sub(rop, op1, op2, allocator);
    rop->size = sgn1 * abs(rop->size);
  } else {
    MP_FLOAT tmp = {};
    MP_FLOAT_init(&tmp, abs(op1->size), allocator);
    MP_FLOAT_copy(&tmp, op1, allocator);
    if (rop != op2) { MP_FLOAT_copy(rop, op2, allocator); }
    MP_FLOAT_normal_abs_sub(rop, rop, &tmp, allocator);
    rop->size = - sgn1 * abs(rop->size);
    MP_FLOAT_clear(&tmp, allocator);
  }
  MP_FLOAT_tidy(rop);
}

inline void MP_FLOAT_normal_abs_divmod(MP_FLOAT *restrict dividend, const MP_FLOAT *restrict divisor,
                                           MP_FLOAT *restrict quotient, const Allocator *allocator) {
  const lmp_size_t size_quotient = abs(quotient->prec);
  if (size_quotient == 0) { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
  if (dividend->size == 0) { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
  const lmp_size_t size_divisor  = abs(divisor->size);
  const lmp_size_t size_dividend = abs(dividend->size);
  const int32_t d_leading_zeros = extstd_leading_zeros(divisor->limbs[abs(divisor->size) - 1]);
  const int32_t n_leading_zeros = extstd_leading_zeros(dividend->limbs[size_dividend - 1]);
  const lmp_double_limb_t d = MP_INT_left_shift_load_double((MP_INT *)divisor, -2, d_leading_zeros);
  const lmp_double_limb_t d_high = d >> LMP_LIMB_BIT;
  bool overflow = n_leading_zeros <= d_leading_zeros;

  MP_FLOAT_realloc(quotient, size_quotient, allocator);
  quotient->exp = dividend->exp - divisor->exp;
  quotient->size = size_quotient;

  MP_INT INTERVAL = {};
  MP_INT_init(&INTERVAL, size_divisor + 1, allocator);
  if (overflow) {
    const lmp_double_limb_t dividend_head = MP_INT_left_shift_load_double((MP_INT *)dividend, -1, d_leading_zeros);
    const lmp_double_limb_t dividend_foll = MP_INT_left_shift_load((MP_INT *)dividend, -2, d_leading_zeros);
    if (dividend_head < d_high) {
      overflow = false;
    } else {
      lmp_limb_t q = lmp_fast_quotient(dividend_head, dividend_foll, d);
      MP_INT_abs_mul_limb(&INTERVAL, (MP_INT *)divisor, q, allocator);
      if (!MP_INT_abs_offset_ge((MP_INT *)dividend, &INTERVAL, size_quotient - 1)) {
        MP_INT_abs_sub_offset(&INTERVAL, &INTERVAL, (MP_INT *)divisor, 0, allocator);
        q --;
      }
      MP_INT_abs_sub_offset((MP_INT *)dividend, (MP_INT *)dividend, &INTERVAL, size_quotient - 1, allocator);
      quotient->limbs[size_quotient - 1] = q;
    }
  }
  const lmp_size_t sub_offset = size_quotient - 1 - overflow;
  for (lmp_size_t i = overflow; i < size_quotient; i++) {
    const lmp_size_t load_index = size_dividend - 2;
    const lmp_size_t curr_q_idx = size_quotient - 1 - i;
    lmp_double_limb_t dividend_head = MP_INT_left_shift_load_double((MP_INT *)dividend, load_index, d_leading_zeros);
    lmp_double_limb_t dividend_foll = MP_INT_left_shift_load((MP_INT *)dividend, load_index - 1, d_leading_zeros);
    if (dividend_head < d_high) {
      quotient->limbs[curr_q_idx] = 0;
      MP_INT_left_shift((MP_INT *)dividend, LMP_LIMB_BIT, allocator);
      dividend->exp-= 1;
      continue;
    }
    if (dividend_head > d) { // maybe never be reached
      MP_INT_abs_sub_offset((MP_INT *)dividend, (MP_INT *)dividend, (MP_INT *)divisor, sub_offset, allocator);
      quotient->limbs[curr_q_idx + 1] += 1;
      dividend_head = MP_INT_left_shift_load_double((MP_INT *)dividend, load_index, d_leading_zeros);
      dividend_foll = MP_INT_left_shift_load((MP_INT *)dividend, load_index - 1, d_leading_zeros);
    }
    lmp_limb_t q = lmp_fast_quotient(dividend_head, dividend_foll, d);
    if (q == 0 && MP_INT_abs_offset_ge((MP_INT *)dividend, (MP_INT *)divisor, curr_q_idx)) { q += 1; }

    MP_INT_abs_mul_limb(&INTERVAL, (MP_INT *)divisor, q, allocator);
    if (!MP_INT_abs_offset_ge((MP_INT *)dividend, &INTERVAL, curr_q_idx)) {
      MP_INT_abs_sub_offset(&INTERVAL, &INTERVAL, (MP_INT *)divisor, 0, allocator);
      q -= 1;
    }
    MP_INT_abs_sub_offset((MP_INT *)dividend, (MP_INT *)dividend, &INTERVAL, sub_offset, allocator);
    MP_INT_clamp((MP_INT *)dividend);
    quotient->limbs[curr_q_idx] = q;

    if (dividend->size == 0) {
      for (lmp_size_t j = 0; j < curr_q_idx; j ++) {
        quotient->limbs[j] = 0;
      }
      dividend->exp = 0;
      break;
    }
    MP_INT_left_shift((MP_INT *)dividend, LMP_LIMB_BIT, allocator);
    dividend->exp-= 1;
  }
  MP_INT_clear(&INTERVAL, allocator);
  MP_FLOAT_tidy(quotient);
  MP_FLOAT_tidy(dividend);
}

_SHARED_API void MP_FLOAT_divmod(MP_FLOAT *restrict dividend,const MP_FLOAT *restrict divisor,
                                 MP_FLOAT *restrict quotient, const Allocator *allocator) {
  if (divisor->size == 0) {
    if (divisor->exp == MP_NUMBER_NAN) { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
    const lmp_exp_t sgn_dividend = sgn(dividend->exp);
    const lmp_exp_t sgn_divisor  = sgn(divisor->exp);
    const lmp_exp_t sgn_quotient = sgn_dividend * sgn_divisor;
    switch (abs(divisor->exp)) {
      case MP_NUMBER_INF : {
        if (dividend->size == 0) {
          if (dividend->exp == MP_NUMBER_NAN) { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
          switch (abs(dividend->exp)) {
            case MP_NUMBER_ZERO:
            case MP_NUMBER_INS : { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
            case MP_NUMBER_INF :
            default: { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
          }
        } else { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
      }
      case  MP_NUMBER_INS: {
        if (dividend->size == 0) {
          switch (dividend->exp) {
            case MP_NUMBER_ZERO: { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
            case MP_NUMBER_INF : {
              quotient->exp = sgn_quotient * MP_NUMBER_INF;
              dividend->exp = MP_NUMBER_ZERO;
              quotient->size = 0;
              return;
            }
            case MP_NUMBER_INS :
            default: { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
          }
        } else {
          quotient->exp = sgn_divisor * sgn(dividend->size) * MP_NUMBER_INF;
          dividend->exp = MP_NUMBER_ZERO;
          quotient->size = 0;
          dividend->size = 0;
          return;
        }
      }
      default: { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
    }
  }
  const int32_t sgn_divisor = sgn(divisor->size);
  if (dividend->size == 0) {
    const lmp_exp_t sgn_dividend = sgn(dividend->exp);
    const lmp_exp_t sgn_quotient = sgn_dividend * sgn_divisor;
    if (dividend->exp == MP_NUMBER_NAN) { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
    switch (abs(dividend->exp)) {
      case MP_NUMBER_ZERO: { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
      case MP_NUMBER_INS : { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
      case MP_NUMBER_INF :  {
        quotient->exp = sgn_quotient * MP_NUMBER_INF;
        dividend->exp = MP_NUMBER_ZERO;
        quotient->size = 0;
        return;
      }
      default: { quotient->size = 0; quotient->exp = MP_NUMBER_NAN; return; }
    }
  }
  if (quotient->prec == 0) { quotient->size = 0; quotient->exp = MP_NUMBER_ZERO; return; }
  const int32_t sgn_dividend = sgn(dividend->size);
  const int32_t sgn_quotient = sgn_dividend * sgn_divisor;

  MP_FLOAT_normal_abs_divmod(dividend, divisor, quotient, allocator);
  if (sgn_dividend * sgn_divisor < 0 && dividend->size) {
    const lmp_size_t size_quotient = abs(quotient->size);
    if (MP_FLOAT_carry_check(quotient) && size_quotient == abs(quotient->prec)) {
      for (lmp_size_t i = 1; i < size_quotient; i++){ quotient->limbs[i] = 0; }
      quotient->exp += size_quotient;
      quotient->size = sgn_quotient;
      quotient->limbs[1] = 1;
    } else {
      MP_FLOAT_mantissa_abs_add_limb(quotient, quotient, 1, allocator);
      MP_FLOAT_tidy(quotient);
    }
    dividend->size = sgn_dividend * dividend->size;
    MP_FLOAT_add(dividend, dividend, divisor, allocator);
  }
  quotient->size = sgn_quotient * abs(quotient->size);
}
