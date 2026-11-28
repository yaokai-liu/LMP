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
 * Module Name: src/arith
 * Filename: limb.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/
#include "limb.h"

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
  lmp_limb_t quotient = (dividend >> LMP_LIMB_BIT) == d_high ? LMP_LIMB_MAX : dividend / d_high;
  lmp_double_limb_t dividend_hat_high = quotient * d_high;
  lmp_double_limb_t dividend_hat_tail = (lmp_double_limb_t)quotient * (lmp_limb_t)divisor;
  if (dividend_hat_high + (dividend_hat_tail >> LMP_LIMB_BIT) < dividend_hat_high) {
    dividend_hat_high -= d_high;
    quotient -= 1;
  }
  dividend_hat_high += dividend_hat_tail >> LMP_LIMB_BIT;
  dividend_hat_tail = (lmp_limb_t)dividend_hat_tail;
  if (dividend_hat_high > dividend || (dividend_hat_high == dividend && dividend_hat_tail > dividend_follow)) { quotient -= 1; }
  return quotient;
}
