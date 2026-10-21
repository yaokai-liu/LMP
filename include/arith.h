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

void MP_INT_left_shift(MP_INT *op, int32_t bit_count, const Allocator *allocator);

void MP_INT_right_shift(MP_INT *op, int32_t bit_count, const Allocator *);
// Allow (dest = op)
// Force set dest->size >= 0
void MP_INT_abs_mul_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);
// Allow (dest = op)
// Force set dest->size >= 0
void MP_INT_abs_add_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);
// Allow (dest = op)
// Force set dest->size >= 0
void MP_INT_abs_sub_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);

// Force set rop->size >= 0
void MP_INT_abs_mul(MP_INT *restrict rop, const MP_INT *restrict op1,
                    const MP_INT *restrict op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Force set rop->size >= 0
void MP_INT_abs_add_offset(MP_INT *rop, const MP_INT *op1, const MP_INT * op2,
                           int32_t offset, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Force set rop->size >= 0
// Assert abs(op1) >= abs(op2) * (LMP_LIMB_VAL ** offset)
void MP_INT_abs_sub_offset(MP_INT *rop, const MP_INT *op1, const MP_INT * op2,
                           int32_t offset, const Allocator *allocator);

// dividend as remainder
// Force set dividend->size >= 0
// Force set quotient->size >= 0
void MP_INT_abs_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor,
                       MP_INT *restrict quotient, const Allocator *allocator);

lmp_double_limb_t MP_INT_abs_3limbs_inv(lmp_double_limb_t op);

// Allow (dest = op)
void MP_INT_mul_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);
// Allow (dest = op)
void MP_INT_add_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);
// Allow (dest = op)
void MP_INT_sub_u64(MP_INT *dest, const MP_INT *op, uint64_t val, const Allocator *allocator);

void MP_INT_mul(MP_INT *restrict rop, const MP_INT *restrict op1,
                    const MP_INT *restrict op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
void MP_INT_add(MP_INT *rop, const MP_INT *op1, const MP_INT * op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
void MP_INT_sub(MP_INT *rop, const MP_INT *op1, const MP_INT * op2, const Allocator *allocator);

// dividend as remainder
void MP_INT_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator);


#endif //LMP_ARITH_H
