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
 * Filename: arith.h
 * Creator: Yaokai Liu
 * Create Date: 2026-11-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_BASE_NUMBER_H
#define LMP_BASE_NUMBER_H
#include "mp-int.h"
#include "mp-float.h"
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

_LOCAL_API bool MP_BASE_INT_carry_check(const MP_BASE_NUMBER *val);
#define MP_BASE_INT_abs_cmp(op1, op2)                             MP_INT_abs_cmp((const MP_INT *)(op1), (const MP_INT *)(op2))
#define MP_BASE_INT_abs_offset_ge(op1, op2, offset)               MP_INT_abs_offset_ge((const MP_INT *)(op1), (const MP_INT *)(op2), (offset))
#define MP_BASE_FLOAT_abs_cmp(op1, op2)                           MP_FLOAT_abs_cmp((const MP_FLOAT *)(op1), (const MP_FLOAT *)(op2))
#define MP_BASE_FLOAT_carry_check(val)                            MP_BASE_INT_carry_check((val))
#endif //LMP_BASE_NUMBER_H
