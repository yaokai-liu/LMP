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
 * Filename: mp_type.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-15
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_MP_TYPE_H
#define LMP_MP_TYPE_H
#include "lmp.h"

#ifndef MP_ALLOC_LEN
#define MP_ALLOC_LEN 8
#endif
#define aligned(count) (((count) / MP_ALLOC_LEN + 1) * MP_ALLOC_LEN)

void MP_INT_init(MP_INT *val,  int32_t initial_alloc, const Allocator *allocator);
void MP_INT_realloc(MP_INT *val,  int32_t required, const Allocator *allocator);
void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator);
void MP_INT_clear(MP_INT *val, const Allocator *allocator);
void MP_INT_clamp(MP_INT *val);

void MP_INT_negative(MP_INT *val);
int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2);

#endif //LMP_MP_TYPE_H
