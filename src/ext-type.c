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
 * Filename: convert.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-25
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "ext-type.h"
#include "ext/ieee754/float.h"
#include "ext/ieee754/decimal.h"

#define lenof(str_literal) ((sizeof str_literal) - 1)

const struct MP_STANDARD MP_EXTEND_STANDARDS[] = {
  {"MP", lenof("MP"), MP_NUM_LMP_STANDARD},
  {"C-INT", lenof("C-INT"), MP_NUM_INT_STANDARD},
  {"IEEE754", lenof("IEEE754"), MP_NUM_IEEE754_STANDARD},
};
