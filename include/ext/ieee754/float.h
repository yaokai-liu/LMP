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
 * Module Name: ext/ieee754
 * Filename: float.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-25
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_FLOAT_H
#define LMP_FLOAT_H

#include <stdint.h>
#ifndef INT128_T_DEFINED
typedef _BitInt(128) int128_t;
typedef _BitInt(256) int256_t;
typedef unsigned _BitInt(128) uint128_t;
typedef unsigned _BitInt(256) uint256_t;
#endif
#include <float.h>
typedef float   float32_t;
typedef double  float64_t;
static_assert(sizeof(float32_t) == 4, "float32_t must be 32-bit");
static_assert(FLT_RADIX == 2, "radix must be 2");
static_assert(FLT_MANT_DIG == 24, "float must have 24-bit mantissa (binary32)");
static_assert(FLT_MAX_EXP == 128, "float exponent range must match binary32");
static_assert(FLT_MIN_EXP == -125, "float exponent range must match binary32");
static_assert(sizeof(float64_t) == 8, "float64_t must be 64-bit");
static_assert(DBL_MANT_DIG == 53, "double must have 53-bit mantissa (binary64)");
static_assert(DBL_MAX_EXP == 1024, "double exponent range must match binary64");
static_assert(DBL_MIN_EXP == -1021, "double exponent range must match binary64");

#ifdef __FLT16_MANT_DIG__
typedef _Float16        float16_t;
#else
typedef unsigned short  float16_t;
#endif
#ifdef __FLT128_MANT_DIG__
typedef __float128  float128_t;
#else
typedef uint128_t   float128_t;
#endif
static_assert(sizeof(float16_t) == 2, "float16_t must be 16-bit");
static_assert(sizeof(float128_t) == 16, "float128_t must be 128-bit");

// argument @mantissa includes leading 1
float16_t ieee754bin16(bool sign, uint16_t mantissa, uint8_t exponent);
// argument @mantissa includes leading 1
float32_t ieee754bin32(bool sign, uint32_t mantissa, uint8_t exponent);
// argument @mantissa includes leading 1
float64_t ieee754bin64(bool sign, uint64_t mantissa, uint16_t exponent);
// argument @mantissa includes leading 1
float128_t ieee754bin128(bool sign, uint128_t mantissa, uint16_t exponent);
// argument @mantissa includes leading 1
// float256_t ieee754bin256(bool sign, uint256_t  mantissa, uint32_t exponent);

#endif //LMP_FLOAT_H
