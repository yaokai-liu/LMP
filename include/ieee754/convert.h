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
 * Module Name: include/ieee754
 * Filename: convert.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-25
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_CONVERT_H
#define LMP_CONVERT_H
#include "parse.h"
#include "ieee754/float.h"
#include "ieee754/decimal.h"

// follow interpreting of enum MP_NUMBER_TYPE.
enum MP_NUMBER_EXTEND_IEEE754_TYPE : uint32_t {
  MP_NUM_TYPE_EXT_IEEE754_FLOAT_TYPE = 0x7002'0000,
  MP_NUM_TYPE_EXT_IEEE754_DECIMAL_TYPE = 0x7003'0000,
};

#endif //LMP_CONVERT_H
