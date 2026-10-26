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
 * Filename: ext-type.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-25
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_EXT_TYPE_H
#define LMP_EXT_TYPE_H
#include "type.h"

// follow interpreting of enum MP_NUMBER_STANDARD_TYPE.
enum MP_NUMBER_STANDARD_IEEE754 : uint8_t {
  MP_NUM_IEEE754_STANDARD   = 0x02,
};
enum MP_NUMBER_IEEE754_REPRESENTATION_TYPE : uint8_t {
  MP_NUM_REPR_IEEE754_FLOAT = 0x01,
  MP_NUM_REPR_IEEE754_DECIMAL = 0x02,
};

#define MP_EXTEND_STANDARD_COUNT 3

extern const struct MP_STANDARD  {
  const char * const name;
  const uint16_t     leng;
  const uint16_t     stdd;
} MP_EXTEND_STANDARDS[MP_EXTEND_STANDARD_COUNT];

#endif //LMP_EXT_TYPE_H
