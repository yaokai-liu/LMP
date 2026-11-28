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
 * Filename: print.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_PRINT_H
#define LMP_PRINT_H
#include "lmp.h"

typedef struct BaseCache BaseCache;

// Ignore sign of the 'mp_val'.
// Print digital string in base of the 'mp_val'.
// Always aligned to BASE_LIMB_WIDTH (by fill zero) + 1 char for sign.
// Notice: (1 < base < 37) is required
_LOCAL_API uint32_t MP_BASE_NUMBER_abs_to_string(char *dest, uint32_t max_len, const MP_BASE_NUMBER *mp_val, const Allocator *allocator);
// Ignore sign of the 'mp_val'.
// Print digital string in base of the 'mp_val'.
// result form: INT.FRAC
// Both INT and FRAC parts will always align to BASE_LIMB_WIDTH (by fill zero)
// Both INT and FRAC parts both are at least BASE_LIMB_WIDTH digits (by fill zero).
// Notice: (1 < base < 37) is required.
_LOCAL_API uint32_t MP_FLOAT_abs_to_base_string(char *dest, uint32_t max_len, const MP_FLOAT *mp_val, uint32_t base, const Allocator *allocator);
// Ignore sign of the 'mp_val'.
// Print digital string in base of the 'mp_val'.
// result form: INT@EXP
// INT parts will always align to BASE_LIMB_WIDTH (by fill zero)
// EXP part will be always signed(±) decimal integer
// Notice: EXP is based on BASE.base instead of BASE.limb_val.
// Notice: (1 < base < 37) is required.
_LOCAL_API uint32_t MP_FLOAT_abs_to_base_exp_string(char *dest, uint32_t max_len, const MP_FLOAT *mp_val, uint32_t base, const Allocator *allocator);

#endif //LMP_PRINT_H
