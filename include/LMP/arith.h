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
 * Filename: arith.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-14
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_ARITH_H
#define LMP_ARITH_H
#include "lmp.h"

// Always assume abs(op->prec) large enough
_SHARED_API void MP_INT_left_shift(MP_INT *op, int32_t width, const Allocator *allocator);
// Always assume abs(op->prec) large enough
_SHARED_API void MP_INT_right_shift(MP_INT *op, int32_t width, const Allocator *);
// Allow (dest = op)
// Force set dest->size >= 0
// Always assume abs(dest->prec) large enough
_LOCAL_API void MP_INT_abs_mul_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
// Force set dest->size >= 0
// Always assume abs(dest->prec) large enough
_LOCAL_API void MP_INT_abs_add_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
// Force set dest->size >= 0
// Always assume abs(dest->prec) large enough
_LOCAL_API void MP_INT_abs_sub_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (op1 == op2)
// Force set rop->size >= 0
// Always assume abs(rop->prec) large enough
_LOCAL_API void MP_INT_abs_mul(MP_INT *restrict rop, const MP_INT *op1, const MP_INT *op2, const Allocator *allocator);
// Force set rop->size >= 0
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
_LOCAL_API void MP_INT_abs_add_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *op2, int32_t offset, const Allocator *allocator);
// Allow (rop = op1)
// Force set rop->size >= 0
// Always assume abs(rop->prec) large enough
// Always assume abs(op1) >= abs(op2) * pow(LMP_LIMB_VAL, offset)
_LOCAL_API void MP_INT_abs_sub_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *restrict op2, int32_t offset, const Allocator *allocator);
// dividend as remainder
// Force set dividend->size >= 0
// Force set quotient->size >= 0
_LOCAL_API void MP_INT_abs_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor, MP_INT *restrict quotient, const Allocator *allocator);

// Always assume abs(op->prec) large enough
_SHARED_API void MP_FLOAT_mul_base_exp(MP_FLOAT *op, lmp_exp_t exp, const Allocator *allocator);

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

_LOCAL_API lmp_double_limb_t MP_INT_abs_3limbs_inv(lmp_double_limb_t op);

// Allow (dest = op)
_SHARED_API void MP_INT_mul_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
_SHARED_API void MP_INT_add_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
_SHARED_API void MP_INT_sub_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (op1 == op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_INT_mul(MP_INT *restrict rop, const MP_INT *op1, const MP_INT *op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_INT_add(MP_INT *rop, const MP_INT *op1, const MP_INT * op2, const Allocator *allocator);
// Allow (rop = op1)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_INT_sub(MP_INT *rop, const MP_INT *op1, const MP_INT * restrict op2, const Allocator *allocator);

// dividend as remainder
_SHARED_API void MP_INT_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator);
// No truncating
// Allow (op1 == op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_FLOAT_mul(MP_FLOAT *restrict rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_FLOAT_add(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_FLOAT_sub(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * restrict op2, const Allocator *allocator);
// Truncating
// dividend as remainder
// Always assume abs(rop->prec) large enough
// Precision follow the quotient->prec and not change the quotient->prec.
_SHARED_API void MP_FLOAT_divmod(MP_FLOAT *restrict dividend,const MP_FLOAT *restrict divisor,
                                 MP_FLOAT *restrict quotient, const Allocator *allocator);

_SHARED_API void MP_BASE_INT_left_shift(MP_BASE_NUMBER *mp_val, uint32_t width, const Allocator *allocator);
_SHARED_API void MP_BASE_INT_right_shift(MP_BASE_NUMBER *mp_val, uint32_t width, const Allocator *allocator);
// Notice: base_exp based on BASE.base instead of BASE.limb_val.
_SHARED_API void MP_BASE_FLOAT_mul_base_exp(MP_BASE_NUMBER *mp_val, lmp_exp_t exp, const Allocator *allocator);

// Allow (dest = op)
// Always assume op is an integer
// Always assume (op->BASE.base == dest->BASE.base)
// else is an unexpected behavior
// Force set dest->size >= 0
_LOCAL_API void MP_BASE_INT_abs_mul_limb(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
// Always assume op is an integer
// Always assume (op->BASE.base == dest->BASE.base)
//    else is an unexpected behavior
// Always assume (val < BASE.limb_val)
//    else is an unexpected behavior
// Force set dest->size >= 0
_LOCAL_API void MP_BASE_INT_abs_add_limb(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
// Always assume op is an integer
// Always assume (op->BASE.base == dest->BASE.base)
//    else is an unexpected behavior
// Always assume (val < BASE.limb_val)
//    else is an unexpected behavior
// Force set dest->size >= 0
_LOCAL_API void MP_BASE_INT_abs_sub_limb(MP_BASE_NUMBER *dest, const MP_BASE_NUMBER *op, lmp_limb_t val, const Allocator *allocator);

// Force set rop->size >= 0
// Always assume op1, op2 both are integer
// Always assume (op1->BASE.base == op2->BASE.base == rop->BASE.base)
// else is an unexpected behavior
// Always assume op1, op2 both are integer
_LOCAL_API void MP_BASE_INT_abs_mul(MP_BASE_NUMBER *restrict rop, const MP_BASE_NUMBER *op1,
                         const MP_BASE_NUMBER *op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Always assume op1, op2 both are integer
// Always assume (op1->BASE.base == op2->BASE.base == rop->BASE.base)
// else is an unexpected behavior
// Force set rop->size >= 0
_LOCAL_API void MP_BASE_INT_abs_add_offset(MP_BASE_NUMBER *rop, const MP_BASE_NUMBER *op1, const MP_BASE_NUMBER * op2,
                                int32_t offset, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Always assume op1, op2 both are integer
// Always assume (op1->BASE.base == op2->BASE.base == rop->BASE.base)
// else is an unexpected behavior
// Always assume abs(op1) >= abs(op2) * pow(BASE.limb_val, offset)
// Force set rop->size >= 0
_LOCAL_API void MP_BASE_INT_abs_sub_offset(MP_BASE_NUMBER *rop, const MP_BASE_NUMBER *op1, const MP_BASE_NUMBER * op2,
                                int32_t offset, const Allocator *allocator);
// dividend as remainder
// Always assume dividend, divisor both are integer
// Always assume (dividend->BASE.base == divisor->BASE.base == quotient->BASE.base)
// else is an unexpected behavior
// Force set dividend->size >= 0
// Force set quotient->size >= 0
_LOCAL_API void MP_BASE_INT_abs_divmod(MP_BASE_NUMBER *restrict dividend,const MP_BASE_NUMBER *restrict divisor,
                            MP_BASE_NUMBER *restrict quotient, const Allocator *allocator);


#endif //LMP_ARITH_H
