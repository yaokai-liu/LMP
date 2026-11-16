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

// @return: integer polynomial that equals to 'mp_val' under given base, i.e.
//          sum(limbs[i] * pow(BASE_LIMB_VAL, i))
// Here BASE_LIMB_VAL = pow(base, BASE_LIMB_WIDTH) is the max power of base that not larger than LMP_LIMB_VAL
// and BASE_LIMB_WIDTH is its exponent regards to base.
// Both BASE_LIMB_VAL and BASE_LIMB_WIDTH depend on the base.
// if 'p_BASE_LIMB_WIDTH' is not nullptr, BASE_LIMB_WIDTH will be placed in it.
MP_POLYi *MP_INT_to_base_polynomial(uint32_t *p_BASE_LIMB_WIDTH, const MP_INT *mp_val, uint32_t base, const Allocator *allocator);
// Print digital string in base of the 'mp_val'.
// Always aligned to BASE_LIMB_WIDTH (by fill zero).
uint32_t MP_INT_to_base_string(char *dest, uint32_t max_len, const MP_INT *mp_val, uint32_t base, const Allocator *allocator);
uint32_t MP_FLOAT_to_str(char *dest, uint32_t max_len, const MP_FLOAT *mp_val, uint32_t base);
uint32_t MP_DECIMAL_to_str(char *dest, uint32_t max_len, const MP_DECIMAL *mp_val, uint32_t base);

#endif //LMP_PRINT_H
