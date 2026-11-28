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
 * Module Name: src
 * Filename: float.h
 * Creator: Yaokai Liu
 * Create Date: 2026-11-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_FLOAT_H
#define LMP_FLOAT_H
#include "LMP/lmp.h"
#include "mp-int.h"

#define MP_FLOAT_mantissa_abs_mul_limb(dest, op, val, allocator)   MP_INT_abs_mul_limb(MP_FLOAT_mantissa(dest), (MP_INT *)(op), (val), (allocator))
#define MP_FLOAT_mantissa_abs_add_limb(dest, op, val, allocator)   MP_INT_abs_add_limb(MP_FLOAT_mantissa(dest), (MP_INT *)(op), (val), (allocator))
#define MP_FLOAT_mantissa_abs_sub_limb(dest, op, val, allocator)   MP_INT_abs_sub_limb(MP_FLOAT_mantissa(dest), (MP_INT *)(op), (val), (allocator))
// No truncating
// Allow (op1 == op2)
// Force set rop->size >= 0
// Special value may cause unexpected behavior.
// Always assume abs(op->prec) large enough. In other words,
// precision of result is sum of that of both op1 and op2.
_LOCAL_API void MP_FLOAT_normal_abs_mul(MP_FLOAT *restrict rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator);
// No truncating
// Force set rop->size >= 0
// Allow (rop = op1) or (rop = op2)
// Special value may cause unexpected behavior.
// Always assume abs(op->prec) large enough.
_LOCAL_API void MP_FLOAT_normal_abs_add(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1)
// Force set rop->size >= 0
// Special value may cause unexpected behavior.
// Always assume abs(op1) >= abs(op2) * pow(LMP_LIMB_VAL, offset)
_LOCAL_API void MP_FLOAT_normal_abs_sub(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT *restrict op2, const Allocator *allocator);
// dividend as remainder
// Force set dividend->size >= 0
// Force set quotient->size >= 0
// max precision follow the quotient->prec and not change the quotient->prec.
_LOCAL_API void MP_FLOAT_normal_abs_divmod(MP_FLOAT *restrict dividend,const MP_FLOAT *restrict divisor,
                                           MP_FLOAT *restrict quotient, const Allocator *allocator);


_LOCAL_API int32_t MP_FLOAT_abs_cmp(const MP_FLOAT *op1, const MP_FLOAT *op2);
#define MP_FLOAT_carry_check(val) MP_INT_carry_check((MP_INT *)(val))

#endif //LMP_FLOAT_H
