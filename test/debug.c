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

#include "lmp.h"
#include "arith.h"
#include <stdio.h>

int main () {
  lmp_limb_t A_limbs[] = {1, 2, 3};
  lmp_limb_t B_limbs[] = {4, 5, 6};
  lmp_limb_t RES_limbs[7] = {};
  MP_INT A = {.alloc = 0, .size = 3, .limbs = A_limbs};
  MP_INT B = {.alloc = 0, .size = 3, .limbs = B_limbs};
  MP_INT RES = {.alloc = 0, .size = 0, .limbs = RES_limbs};
  MP_INT_abs_add_offset(&RES, &A, &B, 1, &STDAllocator);
  for (uint32_t i = 0; i < 6; i++) {
    printf("%lld\t", RES_limbs[i]);
  }
  printf("\n%d\n", RES.size);
  return 0;
}
