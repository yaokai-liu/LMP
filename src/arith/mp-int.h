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
 * Filename: int.h
 * Creator: Yaokai Liu
 * Create Date: 2026-11-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_INT_H
#define LMP_INT_H
#include "LMP/lmp.h"
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
// Allow (rop = op1)
// Force set rop->size >= 0
// Always assume abs(rop->prec) large enough
_LOCAL_API void MP_INT_abs_add_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *restrict op2, int32_t offset, const Allocator *allocator);
// Allow (rop = op1)
// Force set rop->size >= 0
// Always assume abs(rop->prec) large enough
// Always assume abs(op1) >= abs(op2) * pow(LMP_LIMB_VAL, offset)
_LOCAL_API void MP_INT_abs_sub_offset(MP_INT *rop, const MP_INT *op1, const MP_INT *restrict op2, int32_t offset, const Allocator *allocator);
// dividend as remainder
// Force set dividend->size >= 0
// Force set quotient->size >= 0
_LOCAL_API void MP_INT_abs_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor, MP_INT *restrict quotient, const Allocator *allocator);

_LOCAL_API bool MP_INT_carry_check(const MP_INT *val);
_LOCAL_API int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2);
_LOCAL_API bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, int32_t offset);

#endif //LMP_INT_H
