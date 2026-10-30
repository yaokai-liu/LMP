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
 * Module Name: test
 * Filename: debug.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"
#include "parse.h"
#define lenof(str_literal) ((sizeof str_literal) - 1)
#define abs(a) ((a) > (0) ? (a) : -(a))

int main () {
  MP_NUM_TYPE num_type = {};
  lmp_limb_t limbs[8] = {};
  MP_NUMBER number = {.val.INT = {.alloc = -8, .limbs = limbs}, .repr = MP_NUM_REPR_NONE};
  const char *num_str = "-123.12345678999991234'56789123456789_123456789LL<IEEE754>";
  constexpr uint32_t str_len = lenof("-123.12345678999991234'56789123456789_123456789LL<IEEE754>");
  const uint32_t parse_len = LMP_parse(num_str, &num_type, &number, &STDAllocator);
  if (parse_len != str_len) { return -1; }
  if (num_type.type != MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT, 16)) { return -2; }
  if (number.val.FLOAT.size != -4) { return -3; }
  if (number.val.FLOAT.exp != -3) { return -4; }
  const int64_t size = abs(number.val.FLOAT.size);
  if (number.val.FLOAT.size < 0) { printf("-0x"); } else { printf("0x"); }
  for (int32_t i = 0; i < size; i++) {
    printf((size - 1 - i >= - number.val.FLOAT.exp) ? "%llX" : "%016llX", number.val.FLOAT.limbs[size - 1 - i]);
    if (size - 1 - i == - number.val.FLOAT.exp) { printf("."); }
  }
  return 0;
}
