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
 * Module Name: src/arith
 * Filename: limb.h
 * Creator: Yaokai Liu
 * Create Date: 2026-11-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_LIMB_H
#define LMP_LIMB_H
#include "LMP/lmp.h"

_LOCAL_API lmp_limb_t lmp_fast_pow(lmp_half_limb_t base, lmp_exp_t exp);
_LOCAL_API lmp_limb_t lmp_fast_quotient(lmp_double_limb_t dividend, lmp_double_limb_t dividend_follow, lmp_double_limb_t divisor);

#endif //LMP_LIMB_H
