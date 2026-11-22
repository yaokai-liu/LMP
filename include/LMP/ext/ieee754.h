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
 * Module Name: ext
 * Filename: ieee754.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-30
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_IEEE754_H
#define LMP_IEEE754_H
#include "extfloat.h"
#include "lmp.h"

#define FLOAT32_MAX         FLT_MAX
#define FLOAT32_INS         FLT_TRUE_MIN
#define FLOAT64_MAX         DBL_MAX
#define FLOAT64_INS         DBL_TRUE_MIN

// if abs(mp_val) > abs(IEEE754_F32_MAX): result INFs
// if abs(mp_val) < abs(IEEE754_F32_INS): result IEEE754_F32_INS/0
// (round-to-nearest, ties-away-from-zero) for normal range
_SHARED_API uint32_t MP_FLOAT_to_ieee754_f32(float32_t *restrict dest, const MP_FLOAT *restrict mp_val);
// if abs(mp_val) > abs(IEEE754_F64_MAX): result INFs
// if abs(mp_val) < abs(IEEE754_F64_INS): result IEEE754_F64_INS/0
// (round-to-nearest, ties-away-from-zero) for normal range
_SHARED_API uint32_t MP_FLOAT_to_ieee754_f64(float64_t *restrict dest, const MP_FLOAT *restrict mp_val);

#endif //LMP_IEEE754_H
