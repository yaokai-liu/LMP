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
  int32_t num_type = 0;
  lmp_limb_t limbs[8] = {};
  MP_NUMBER number = {.INT = {.alloc = -8, .limbs = limbs}};
  const char *num_str = "-123.1234567899999123456789123456789123456789";
  const uint32_t str_len = lenof("-123.1234567899999123456789123456789123456789");
  const uint32_t parse_len = LMP_parse(num_str, &num_type, &number, &STDAllocator);
  if (parse_len != str_len) { return -1; }
  if (num_type != MP_NUM_TYPE_FLOAT) { return -2; }
  if (number.FLOAT.size != -4) { return -3; }
  if (number.FLOAT.exp != -3) { return -4; }
  const int64_t size = abs(number.FLOAT.size);
  for (int32_t i = 0; i < size; i++) {
    printf((size - 1 - i >= - number.FLOAT.exp) ? "%llX" : "%016llX", number.FLOAT.limbs[size - 1 - i]);
    if (size - 1 - i == - number.FLOAT.exp) { printf("."); }
  }
  return 0;
}
