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
#define _CRTDBG_MAP_ALLOC          // 必须放在最前面
#include <stdlib.h>
#include <crtdbg.h>
#include <inttypes.h>
#include <stdio.h>
#include "lmp.h"
#include "parse.h"
#define lenof(str_literal) ((sizeof str_literal) - 1)
#define abs(a) ((a) > (0) ? (a) : -(a))

int main () {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
  // MP_NUM_TYPE num_type = {};
  // lmp_limb_t limbs[8] = {};
  // MP_BASE_NUMBER number = { .prec = -8, .size = 0, .limbs = limbs };
  // const char *num_str = "-123.12345678999991234'56789123456789_123456789LL<IEEE754>";
  // constexpr uint32_t str_len = lenof("-123.12345678999991234'56789123456789_123456789LL<IEEE754>");
  // const uint32_t parse_len = LMP_parse_base_number(num_str, &num_type, &number, &STDAllocator);
  // if (parse_len != str_len) { return -1; }
  // if (num_type.type != MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT, 128)) { return -2; }
  // if (number.size != -4) { return -3; }
  // if (number.exp != -3) { return -4; }
  // const int64_t size = abs(number.size);
  // if (number.size < 0) { printf("-0x"); } else { printf("0x"); }
  // for (int32_t i = 0; i < size; i++) {
  //   printf((size - 1 - i >= - number.exp) ? "%llX" : "%016llX", number.limbs[size - 1 - i]);
  //   if (size - 1 - i == - number.exp) { printf("."); }
  // }
  // static lmp_limb_t L_long[] = { 0x437430C1913D27FF, 0x05CA0698478CF1B3, 0x1F9ADD3B84CDAD12, 0x7B };
  // static lmp_limb_t L_long1   [] = { 8900000000000000000ULL, 7891234567891234567ULL, 1234567899999123456ULL, 123ULL };
  lmp_limb_t limbs[8] = {};
  static lmp_limb_t L_long1_bin[] = {  0x37C1780B91B52434LL,  0x949B0F713E9185BLL };
  MP_FLOAT f_number = { .prec = 8, .size = -2, .limbs = L_long1_bin };
  MP_BASE_NUMBER number = {.prec = -8, .size = -2, .limbs = limbs, .exp = -2, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL } };
  MP_INT_to_base_number(&number, (MP_INT*)&f_number, 10, &STDAllocator);
  // static lmp_limb_t L_long1    [] = { 2345678912345678900LL, 1234567891234567891LL };
  // MP_FLOAT f_number = { .prec = 8, .size = 0, .limbs = limbs };
  // MP_BASE_NUMBER number = {.prec = -2, .size = -2, .limbs = L_long1, .exp = -2, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL } };
  // MP_BASE_FLOAT_to_float(&f_number, &number, &STDAllocator);
  const int64_t size = abs(f_number.size);
  if (f_number.size < 0) { printf("-0x"); } else { printf("0x"); }
  if (number.exp && size - 1 == - f_number.exp) { printf("."); }
  for (int32_t i = 0; i < size; i++) {
    printf((size - 1 - i >= - number.exp) ? "%"PRIu64 : "%"PRIu64, number.limbs[size - 1 - i]);
   if (number.exp && size - 1 - i == - number.exp) { printf("."); }
  }
  return 0;
}
