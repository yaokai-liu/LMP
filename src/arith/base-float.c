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

#include "LMP/arith.h"
#include <extbit.h>
#include "limb.h"
#include "mp-base-number.h"

#define max(a, b)   ((a) >  (b) ? (a) :  (b))
#define abs(a)      ((a) >  (0) ? (a) : -(a))
#define sgn(a)      ((a) == (0) ? 0 : ((a) > 0) ? 1  :  -1)

inline void MP_BASE_FLOAT_normal_abs_mul(MP_BASE_FLOAT *restrict rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT *op2, const Allocator *allocator) {

}

inline void MP_BASE_FLOAT_normal_abs_add(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT *op2, const Allocator *allocator) {

}

inline void MP_BASE_FLOAT_normal_abs_sub(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT *restrict op2, const Allocator *allocator) {

}

inline void MP_BASE_FLOAT_normal_abs_divmod(MP_BASE_FLOAT *restrict dividend,const MP_BASE_FLOAT *restrict divisor,
                                            MP_BASE_FLOAT *restrict quotient, const Allocator *allocator) {

}


void MP_BASE_FLOAT_mul(MP_BASE_FLOAT *restrict rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT *op2, const Allocator *allocator) {

}

void MP_BASE_FLOAT_add(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT * op2, const Allocator *allocator) {

}

void MP_BASE_FLOAT_sub(MP_BASE_FLOAT *rop, const MP_BASE_FLOAT *op1, const MP_BASE_FLOAT * restrict op2, const Allocator *allocator) {

}

void MP_BASE_FLOAT_divmod(MP_BASE_FLOAT *restrict dividend,const MP_BASE_FLOAT *restrict divisor,
                          MP_BASE_FLOAT *restrict quotient, const Allocator *allocator) {

}
