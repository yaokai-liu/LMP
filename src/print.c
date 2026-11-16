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

#include "print.h"
#include "extbit.h"
#include <math.h>
#include "arith.h"

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a) > (0) ?   1 :  -1)

#define UPPER true
#define LOWER false
#define digit(n, _upper)  ((n) < 10 ? ('0' + (n)) : ((_upper) == UPPER ? ('A' + ((n) - 10)) : ('a' + ((n) - 10))))

typedef struct BaseCache {
  uint32_t    count;
  MP_INT     *mpint;
  lmp_limb_t *limbs;
} BaseCache;

static BaseCache LMP_GLOBAL_BASE_CACHE[] = {};

MP_POLYi *MP_INT_to_base_polynomial(uint32_t *p_BASE_LIMB_WIDTH, const MP_INT *mp_val, const uint32_t base, const Allocator *allocator) {
  if (base < 2) { return nullptr; }
  const lmp_size_t size = abs(mp_val->size);
  if (extstd_has_single_bit(base)) {
    /* TODO: when base is a power of 2 */
    return nullptr;
  }
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)((double)LMP_LIMB_BIT / log2(base));
  uint32_t base_limb_count = ceil(LMP_LIMB_BIT * size / log2(base) / BASE_LIMB_WIDTH);
  lmp_double_limb_t BASE_LIMB_VAL = powl(base, BASE_LIMB_WIDTH / 2);
  BASE_LIMB_VAL *= BASE_LIMB_VAL;
  if (BASE_LIMB_WIDTH % 2 == 1) { BASE_LIMB_VAL *= base; }
  const uint32_t level_count = (uint32_t)ceil(log2(base_limb_count));
  bool free_cache = false;
  BaseCache *cache = nullptr; // TODO: get global base cache
  if (!cache) { cache = allocator->calloc(1, sizeof(BaseCache)); free_cache = true; }
  if (cache->count < level_count) {
    void *ptr = nullptr;
    ptr = allocator->realloc(cache->mpint, level_count * sizeof(MP_INT));
    if (ptr) { cache->mpint = ptr; }
    ptr = allocator->realloc(cache->limbs, 1 << level_count * sizeof(lmp_limb_t));
    if (ptr) { cache->limbs = ptr; }
    if (cache->count == 0) {
      cache->limbs[0] = BASE_LIMB_VAL;
      cache->mpint[0] = (MP_INT){.alloc = -1, .size = 1, .limbs = &cache->limbs[0]};
      cache->count += 1;
    }
    for (uint32_t i = cache->count; i < level_count; i++) {
      const int32_t cache_size = 1 << i;
      const int32_t cache_limb_offset = (1 << i) - 1;
      cache->mpint[i].alloc = -cache_size;
      cache->mpint[i].size = cache_size;
      cache->mpint[i].limbs = &cache->limbs[cache_limb_offset];
      MP_INT_mul(&cache->mpint[i], &cache->mpint[i - 1], &cache->mpint[i - 1], nullptr);
    }
    cache->count = level_count;
  }
  lmp_limb_t *result_limbs = allocator->calloc(1 << level_count, sizeof(lmp_limb_t));
  MP_INT copy_val = {.alloc = -size, .size = 0, .limbs = result_limbs};
  MP_INT_copy(&copy_val, mp_val, nullptr);

  uint32_t divmod_task_count = 0;
  struct { lmp_limb_t *limbs; int32_t size; uint32_t level; } DIVMOD_TASK_QUEUE[level_count] = {};
  DIVMOD_TASK_QUEUE[divmod_task_count].limbs = copy_val.limbs;
  DIVMOD_TASK_QUEUE[divmod_task_count].size = copy_val.size;
  DIVMOD_TASK_QUEUE[divmod_task_count].level = level_count - 1;
  divmod_task_count += 1;
  while (divmod_task_count > 0) {
    auto const task = DIVMOD_TASK_QUEUE[--divmod_task_count];
    if (task.level == 0) { continue; }
    MP_INT dividend = {.alloc = -(1ULL << task.level), .size = task.size, .limbs = task.limbs };
    int32_t quotient_alloc = 1ULL << task.level;
    lmp_limb_t *quotient_limbs = task.limbs + (1ULL << task.level);
    MP_INT quotient = {.alloc = -quotient_alloc, .size = 0, .limbs = quotient_limbs};
    MP_INT_abs_divmod(&dividend, &cache->mpint[task.level], &quotient, nullptr);
    DIVMOD_TASK_QUEUE[divmod_task_count].limbs = quotient_limbs;
    DIVMOD_TASK_QUEUE[divmod_task_count].size = quotient.size;
    DIVMOD_TASK_QUEUE[divmod_task_count].level = task.level - 1;
    divmod_task_count += 1;
    DIVMOD_TASK_QUEUE[divmod_task_count].limbs = task.limbs;
    DIVMOD_TASK_QUEUE[divmod_task_count].size = dividend.size;
    DIVMOD_TASK_QUEUE[divmod_task_count].level = task.level - 1;
    divmod_task_count += 1;
  }
  while (base_limb_count && !result_limbs[base_limb_count - 1]) { base_limb_count--; }
  if (free_cache) {
    allocator->free(cache->mpint);
    allocator->free(cache->limbs);
    allocator->free(cache);
  }

  MP_POLYi *result = allocator->calloc(1, sizeof(MP_INT));
  result->alloc = -base_limb_count;
  result->size = sgn(mp_val->size) * base_limb_count;
  result->limbs = result_limbs;

  if (p_BASE_LIMB_WIDTH) { *p_BASE_LIMB_WIDTH = BASE_LIMB_WIDTH; }

  return result;
}

uint32_t MP_INT_to_base_string(char *dest, const uint32_t max_len, const MP_INT *mp_val, const uint32_t base, const Allocator *allocator) {
  if (base < 2 || base > 36 || max_len == 0) { return 0; }
  if (mp_val->size == 0) {  *dest = '0'; return 1; }
  uint32_t BASE_LIMB_WIDTH = 0;
  MP_POLYi *poly = MP_INT_to_base_polynomial(&BASE_LIMB_WIDTH, mp_val, base, allocator);
  const uint32_t size = abs(poly->size);

  if (max_len < (uint64_t)BASE_LIMB_WIDTH * size) {
    allocator->free(poly->limbs);
    allocator->free(poly);
    return 0;
  }

  char *ptr = dest;
  for (uint32_t i = size - 1; i < size; i--) {
    lmp_limb_t limb = poly->limbs[i];
    for (uint32_t j = BASE_LIMB_WIDTH - 1; j < BASE_LIMB_WIDTH; j--) {
      const uint8_t n = limb % base;
      ptr[j] = digit(n, UPPER);
      limb /= base;
    }
    ptr += BASE_LIMB_WIDTH;
  }

  allocator->free(poly->limbs);
  allocator->free(poly);
  return ptr - dest;
}

uint32_t MP_FLOAT_to_str(char *dest, uint32_t max_len, const MP_DECIMAL *mp_val, uint32_t base) {
  return 0;
}

uint32_t MP_DECIMAL_to_str(char *dest, uint32_t max_len, const MP_DECIMAL *mp_val, uint32_t base) {
  return 0;
}
