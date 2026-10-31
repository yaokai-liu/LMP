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
 * Filename: c-int.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-31
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "ext/c-int.h"
#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a >> ((sizeof(a) - 1) * 8 + 7)) ? -1 : 1)

static_assert(sizeof(lmp_limb_t) == sizeof(uint64_t), "'lmp_limb_t' is required has same size with 'uint64_t'");
static_assert(((lmp_limb_t)-1) > ((lmp_limb_t) 0), "'lmp_limb_t' is required an unsigned type.");

uint32_t MP_INT_to_stdc_i32 (int32_t  *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { return 0; }
  const uint64_t val = (uint64_t) MP_INT_get_limb(mp_val, 0);
  if (mp_val->size < 0 && (abs(mp_val->size) > 1 || val > (uint64_t) INT32_MAX)) { *dest = INT32_MIN; return 0; }
  if (mp_val->size > 0 && (abs(mp_val->size) > 1 || val > (uint64_t) INT32_MAX)) { *dest = INT32_MAX; return 0; }
  *dest = mp_val->size >= 0 ? (int32_t)val : (int32_t)((uint32_t)0 - val);
  return 0;
}

uint32_t MP_INT_to_stdc_i64 (int64_t  *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { return 0; }
  const uint64_t val = (uint64_t) MP_INT_get_limb(mp_val, 0);
  if (mp_val->size < 0 && (abs(mp_val->size) > 1 || val > (uint64_t) INT64_MAX)) { *dest = INT64_MIN; return 0; }
  if (mp_val->size > 0 && (abs(mp_val->size) > 1 || val > (uint64_t) INT64_MAX)) { *dest = INT64_MAX; return 0; }
  *dest = mp_val->size >= 0 ? (int64_t)val : (int64_t)((uint64_t)0 - val);
  return 0;
}

uint32_t MP_INT_to_stdc_i128(int128_t *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { return 0; }
  const uint128_t val = (uint128_t) MP_INT_get_double_limbs(mp_val, 0);
  if (mp_val->size < 0 && (abs(mp_val->size) > 2 || val > (uint128_t) INT128_MAX)) { *dest = INT128_MIN; return 0; }
  if (mp_val->size > 0 && (abs(mp_val->size) > 2 || val > (uint128_t) INT128_MAX)) { *dest = INT128_MAX; return 0; }
  *dest = mp_val->size >= 0 ? (int128_t)val : (int128_t)((uint128_t)0 - val);
  return 0;
}

uint32_t MP_INT_to_stdc_u32 (uint32_t  *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { *dest = 0; return 0; }
  const uint64_t val = MP_INT_get_limb(mp_val, 0);
  if (abs(mp_val->size) > 1 || val > UINT32_MAX) { *dest = UINT32_MAX; return 0; }
  *dest = mp_val->size >= 0 ? (uint32_t)val : (uint32_t)(0 - val);
  return 0;
}

uint32_t MP_INT_to_stdc_u64 (uint64_t  *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { *dest = 0; return 0; }
  if (abs(mp_val->size) > 1) { *dest = UINT64_MAX; return 0; }
  const uint64_t val = (uint64_t) MP_INT_get_limb(mp_val, 0);
  *dest = mp_val->size >= 0 ? val : ((uint64_t)0 - val);
  return 0;
}

uint32_t MP_INT_to_stdc_u128(uint128_t *restrict dest, const MP_INT *restrict mp_val) {
  if (mp_val->size == 0) { *dest = 0; return 0; }
  if (abs(mp_val->size) > 2) { *dest = UINT128_MAX; return 0; }
  const uint128_t val = (uint128_t) MP_INT_get_double_limbs(mp_val, 0);
  *dest = mp_val->size >= 0 ? val : ((uint128_t)0 - val);
  return 0;
}
