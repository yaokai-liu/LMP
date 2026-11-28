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
// Always assume abs(op->prec) large enough
_SHARED_API void MP_FLOAT_mul_base_exp(MP_FLOAT *op, lmp_exp_t exp, const Allocator *allocator);

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
// Always assume abs(quotient->prec) large enough
_SHARED_API void MP_INT_divmod(MP_INT *restrict dividend,const MP_INT *restrict divisor,
                   MP_INT *restrict quotient, const Allocator *allocator);

// No truncating
// Allow (op1 == op2)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_FLOAT_mul(MP_FLOAT *restrict rop, const MP_FLOAT *op1, const MP_FLOAT *op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_FLOAT_add(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_FLOAT_sub(MP_FLOAT *rop, const MP_FLOAT *op1, const MP_FLOAT * restrict op2, const Allocator *allocator);
// Truncating
// dividend as remainder
// Always assume divisor is tidied
// Always assume abs(quotient->prec) large enough
// Precision follow the quotient->prec and not change the quotient->prec.
// If divisor is MP_NUMBER_ZERO or MP_NUMBER_NAN, then quotient will be MP_NUMBER_NAN.
// If quotient is MP_NUMBER_NAN, the remainder is exactly dividend(no change).
// NAN / (@) = NAN ~ NAN; (*) / NAN = NAN ~ (*);
// ZERO / (@) = ZERO ~ ZERO; (*) / ZERO = NAN ~ (*);
// INF / INF = NAN ~ INF; INS / INS = NAN ~ INS;
// INF / (@) = INF ~ ZERO; INS / (@) = ZERO ~ INS;
// (@) / INF = ZERO ~ (@); (@) / INS = INF ~ ZERO;
// Here (*) means any number (special or normal),
// (@) means normal number or INF/INS (but not the special number it self).
// if quotient is NAN, then remainder's sign follows the dividend.
// Promise that result remainder and quotient are tidied.
_SHARED_API void MP_FLOAT_divmod(MP_FLOAT *restrict dividend,const MP_FLOAT *restrict divisor,
                                 MP_FLOAT *restrict quotient, const Allocator *allocator);

// Always assume abs(op->prec) large enough
_SHARED_API void MP_BASE_INT_left_shift(MP_BASE_NUMBER *mp_val, uint32_t width, const Allocator *allocator);
// Always assume abs(op->prec) large enough
_SHARED_API void MP_BASE_INT_right_shift(MP_BASE_NUMBER *mp_val, uint32_t width, const Allocator *allocator);
// Always assume abs(op->prec) large enough
// Notice: base_exp based on BASE.base instead of BASE.limb_val.
_SHARED_API void MP_BASE_FLOAT_mul_base_exp(MP_BASE_NUMBER *mp_val, lmp_exp_t exp, const Allocator *allocator);

// Allow (dest = op)
_SHARED_API void MP_BASE_INT_mul_limb(MP_BASE_INT *dest, const MP_BASE_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
_SHARED_API void MP_BASE_INT_add_limb(MP_BASE_INT *dest, const MP_BASE_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (dest = op)
_SHARED_API void MP_BASE_INT_sub_limb(MP_BASE_INT *dest, const MP_BASE_INT *op, lmp_limb_t val, const Allocator *allocator);
// Allow (op1 == op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_BASE_INT_mul(MP_BASE_INT *restrict rop, const MP_BASE_INT *op1, const MP_BASE_INT *op2, const Allocator *allocator);
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_BASE_INT_add(MP_BASE_INT *rop, const MP_BASE_INT *op1, const MP_BASE_INT * op2, const Allocator *allocator);
// Allow (rop = op1)
// Always assume abs(rop->prec) large enough
_SHARED_API void MP_BASE_INT_sub(MP_BASE_INT *rop, const MP_BASE_INT *op1, const MP_BASE_INT * restrict op2, const Allocator *allocator);
// dividend as remainder
// Always assume abs(quotient->prec) large enough
_SHARED_API void MP_BASE_INT_divmod(MP_BASE_INT *restrict dividend,const MP_BASE_INT *restrict divisor,
                                    MP_BASE_INT *restrict quotient, const Allocator *allocator);

// No truncating
// Allow (op1 == op2)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_BASE_FLOAT_mul(MP_BASE_FLOAT *restrict rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT *op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1) or (rop = op2)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_BASE_FLOAT_add(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT * op2, const Allocator *allocator);
// No truncating
// Allow (rop = op1)
// Always assume abs(rop->prec) large enough
// Promise that result rop is tidied
_SHARED_API void MP_BASE_FLOAT_sub(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT * restrict op2, const Allocator *allocator);
// Truncating
// dividend as remainder
// Always assume divisor is tidied
// Always assume abs(quotient->prec) large enough
// Precision follow the quotient->prec and not change the quotient->prec.
// If divisor is MP_NUMBER_ZERO or MP_NUMBER_NAN, then quotient will be MP_NUMBER_NAN.
// If quotient is MP_NUMBER_NAN, the remainder is exactly dividend(no change).
// NAN / (@) = NAN ~ NAN; (*) / NAN = NAN ~ (*);
// ZERO / (@) = ZERO ~ ZERO; (*) / ZERO = NAN ~ (*);
// INF / INF = NAN ~ INF; INS / INS = NAN ~ INS;
// INF / (@) = INF ~ ZERO; INS / (@) = ZERO ~ INS;
// (@) / INF = ZERO ~ (@); (@) / INS = INF ~ ZERO;
// Here (*) means any number (special or normal),
// (@) means normal number or INF/INS (but not the special number it self).
// if quotient is NAN, then remainder's sign follows the dividend.
// Promise that result remainder and quotient are tidied.
_SHARED_API void MP_BASE_FLOAT_divmod(MP_BASE_FLOAT *restrict dividend,const MP_BASE_FLOAT *restrict divisor,
                                      MP_BASE_FLOAT *restrict quotient, const Allocator *allocator);

#endif //LMP_ARITH_H
