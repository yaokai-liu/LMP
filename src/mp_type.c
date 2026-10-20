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
 * Filename: mp_type.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-15
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "lmp.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(a) ((a) > (0) ? (a) : -(a))

inline void MP_INT_init(MP_INT *val, const int32_t initial_alloc, const Allocator *allocator) {
  val->alloc = aligned(initial_alloc);
  val->limbs = allocator->calloc(val->alloc, LMP_LIMB_SIZE);
  val->size = 0;
}

void MP_INT_copy(MP_INT *dest, const MP_INT *src, const Allocator *allocator) {
  MP_INT_realloc(dest, src->size, allocator);
  allocator->memcpy(dest->limbs, src->limbs, abs(src->size) * LMP_LIMB_SIZE);
  dest->size = src->size;
}

inline void MP_INT_clear(MP_INT *val, const Allocator *allocator) {
  if (val->alloc && val->limbs) {
    allocator->free(val->limbs);
    val->limbs = nullptr;
  }
  val->alloc = 0;
  val->size = 0;
}

inline void MP_INT_realloc(MP_INT *val, const int32_t required, const Allocator *allocator) {
  if (val->alloc == 0) { return ; }
  if (val->alloc < required) {
    const int32_t new_alloc = aligned(required);
    lmp_limb_t *p = allocator->realloc(val->limbs, new_alloc * LMP_LIMB_SIZE);
    if (!p) return;
    allocator->memset(&p[val->alloc], 0, (new_alloc - val->alloc) * LMP_LIMB_SIZE);
    val->limbs = p;
    val->alloc = new_alloc;
  }
}


inline void MP_INT_clamp(MP_INT *val) {
  int32_t s = abs(val->size);
  while (s > 0 && val->limbs[s - 1] == 0) { s--; }
  val->size = (val->size >= 0) ? s : -s;
}

inline void MP_INT_negative(MP_INT *val) {
  val->size = -val->size;
}

inline int32_t MP_INT_abs_cmp(const MP_INT *op1, const MP_INT *op2) {
  if (abs(op1->size) > abs(op2->size)) { return  1; }
  if (abs(op1->size) < abs(op2->size)) { return -1; }
  for (int32_t i = abs(op1->size) - 1; i >= 0; i --) {
    if (op1->limbs[i] > op2->limbs[i]) { return  1; }
    if (op1->limbs[i] < op2->limbs[i]) { return -1; }
  }
  return 0;
}

inline bool MP_INT_abs_offset_ge(const MP_INT *op1, const MP_INT *op2, const int32_t offset) {
  const int32_t size1 = abs(op1->size);
  const int32_t size2 = abs(op2->size);
  for (int32_t i = 0; i < size2; i ++) {
    if (op1->limbs[size1 - 1 - i] < op2->limbs[size1 - offset - 1 - i]) { return false; }
    if (op1->limbs[size1 - 1 - i] > op2->limbs[size1 - offset - 1 - i]) { return true;  }
  }
  return true;
}
