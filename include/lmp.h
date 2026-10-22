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
 * Filename: lmp.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-13
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_H
#define LMP_H
#include <stdint.h>
#include "allocator.h"

#ifndef USING_FFT_SIZE
#define USING_FFT_SIZE  64
#endif
#ifndef MP_ALLOC_LEN
#define MP_ALLOC_LEN 8
#endif
#define aligned(count) (((count) / MP_ALLOC_LEN + 1) * MP_ALLOC_LEN)

typedef _BitInt(128) int128_t;
typedef _BitInt(256) int256_t;
typedef unsigned _BitInt(128) uint128_t;
typedef unsigned _BitInt(256) uint256_t;

typedef uint64_t lmp_limb_t;
typedef uint32_t lmp_half_limb_t;
typedef uint128_t lmp_double_limb_t;
typedef uint256_t lmp_quarter_limb_t;

#define LMP_LIMB_SIZE (sizeof(lmp_limb_t))
#define LMP_LIMB_BITS (LMP_LIMB_SIZE * 8)
#define LMP_LIMB_MAX  (~(lmp_limb_t)0)
#define LMP_DOUBLE_LIMB_MAX  (~(lmp_double_limb_t)0)
#define LMP_LIMB_VAL  (((lmp_double_limb_t)1) << LMP_LIMB_BITS)

// This struct is aligned to GMP's mpz
typedef struct MP_INT {
  // (alloc < 0) means it's a fixed limb count variable.
  int32_t    alloc;
  // negative if size < 0
  int32_t     size;
  // little endian
  lmp_limb_t *limbs;
} MP_INT;

// if size != 0:
// FLOAT = limbs * power(LMP_LIMB_VAL, exp)
// else
// FLOAT = MP_FLOAT_SPECIAL_VAL_TYPE(exp)
typedef struct MP_FLOAT {
  // alloc memory size for limbs
  // (prec <  0) means it's a fixed limb count variable.
  // max limbs with out realloc
  int32_t     prec;
  int32_t     exp;
  // current value size
  // negative if size < 0
  int64_t     size;
  // little endian
  lmp_limb_t *limbs;
} MP_FLOAT;

enum MP_NUMBER_TYPE : int32_t {
  MP_NUM_TYPE_NONE,
  MP_NUM_TYPE_INT,
  MP_NUM_TYPE_FLOAT,
};

enum MP_FLOAT_SPECIAL_VAL_TYPE : int32_t {
  MP_FLOAT_ZERO,
  MP_FLOAT_INF,
  MP_FLOAT_NAN,
};

typedef union MP_NUMBER {
  MP_INT      INT;
  MP_FLOAT    FLOAT;
} MP_NUMBER;

void MP_INT_init(MP_INT *val,  int32_t initial_alloc, const Allocator *allocator);
void MP_INT_realloc(MP_INT *val,  int32_t required, const Allocator *allocator);
void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator);
void MP_INT_clear(MP_INT *val, const Allocator *allocator);
void MP_INT_clamp(MP_INT *val);

void MP_INT_negative(MP_INT *val);
int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2);
bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, int32_t offset);
lmp_limb_t offset_load(const MP_INT *restrict num, int32_t index);
lmp_double_limb_t offset_load_double(const MP_INT *restrict num, int32_t index);
lmp_limb_t left_shift_load(const MP_INT *restrict num, int32_t index, int32_t bit_count);
lmp_double_limb_t left_shift_load_double(const MP_INT *restrict num, int32_t index, int32_t bit_count);

#endif //LMP_H
