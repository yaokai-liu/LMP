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
 * Filename: c-int.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-31
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_EXT_C_INT_H
#define LMP_EXT_C_INT_H
#include "lmp.h"

#ifndef UINT128_MAX
#define UINT128_MAX ((((uint128_t)UINT64_MAX) << 64) | UINT64_MAX)
#endif
#ifndef INT128_MAX
#define INT128_MAX  ((((int128_t)INT64_MAX) << 64) | UINT64_MAX)
#endif
#ifndef INT128_MIN
#define INT128_MIN  ((int128_t)1 << 127)
#endif

// result range [-2^{31}, 2^{31} - 1]
// if mp_val >= INT32_MAX: result INT32_MAX
// if mp_val <= INT32_MIN: result INT32_MIN
uint32_t MP_INT_to_stdc_i32 (int32_t  *restrict dest, const MP_INT *restrict mp_val);
// result range [-2^{63}, 2^{63} - 1]
// if mp_val >= INT64_MAX: result INT64_MAX
// if mp_val <= INT64_MIN: result INT64_MIN
uint32_t MP_INT_to_stdc_i64 (int64_t  *restrict dest, const MP_INT *restrict mp_val);
// result range [-2^{127}, 2^{127} - 1]
// if mp_val >= INT128_MAX: result INT128_MAX
// if mp_val <= INT128_MIN: result INT128_MIN
uint32_t MP_INT_to_stdc_i128(int128_t *restrict dest, const MP_INT *restrict mp_val);

// result range [0, 2^{32} - 1]
// if abs(mp_val) >= UINT32_MAX: result UINT32_MAX
// promise that if @mp_val in range of 'int32_t', there is
//    (int32_t)MP_INT_to_stdc_u32(mp_val) == MP_INT_to_stdc_i32(mp_val)
uint32_t MP_INT_to_stdc_u32 (uint32_t  *restrict dest, const MP_INT *restrict mp_val);
// result range [0, 2^{64} - 1]
// if abs(mp_val) >= UINT64_MAX: result UINT64_MAX
// and promise that if @mp_val in range of 'int64_t', there is
//    (int64_t)MP_INT_to_stdc_u64(mp_val) == MP_INT_to_stdc_i64(mp_val)
uint32_t MP_INT_to_stdc_u64 (uint64_t  *restrict dest, const MP_INT *restrict mp_val);
// result range [0, 2^{128} - 1]
// if abs(mp_val) >= UINT128_MAX: result UINT128_MAX
// and promise that if @mp_val in range of 'int128_t', there is
//    (int128_t)MP_INT_to_stdc_u128(mp_val) == MP_INT_to_stdc_i128(mp_val)
uint32_t MP_INT_to_stdc_u128(uint128_t *restrict dest, const MP_INT *restrict mp_val);

#endif //LMP_EXT_C_INT_H
