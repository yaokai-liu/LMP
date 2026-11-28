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
 * Filename: print.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "LMP/print.h"

#define UPPER true
#define LOWER false
#define digit(n, _upper)  ((n) < 10 ? ('0' + (n)) : ((_upper) == UPPER ? ('A' + ((n) - 10)) : ('a' + ((n) - 10))))

uint32_t MP_BASE_NUMBER_abs_to_string(char *dest, const uint32_t max_len, const MP_BASE_NUMBER *mp_val, const Allocator *) {
  if (mp_val->BASE.base < 2 || mp_val->BASE.base > 36 || max_len == 0) { return 0; }
  if (mp_val->size == 0) {  *dest = '0'; return 1; }
  const uint32_t size = mp_val->size;

  if (max_len < mp_val->BASE.limb_width * size + 1) { return 0; }

  char *ptr = dest;
  for (uint32_t i = size - 1; i < size; i--) {
    lmp_limb_t limb = mp_val->limbs[i];
    for (uint32_t j = mp_val->BASE.limb_width - 1; j < mp_val->BASE.limb_width; j--) {
      const uint8_t n = limb % mp_val->BASE.base;
      ptr[j] = digit(n, UPPER);
      limb /= mp_val->BASE.base;
    }
    ptr += mp_val->BASE.limb_width;
  }

  return ptr - dest;
}

uint32_t MP_FLOAT_abs_to_base_string(char *, const uint32_t , const MP_FLOAT *, const uint32_t , const Allocator *) {
  // TODO
  return 0;
}
uint32_t MP_FLOAT_abs_to_base_exp_string(char *, const uint32_t, const MP_FLOAT *, const uint32_t, const Allocator *) {
  // TODO
  return 0;
}
