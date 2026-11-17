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

// static BaseCache LMP_GLOBAL_BASE_CACHE[] = {};

void MP_INT_abs_to_base_number(MP_BASE_NUMBER *dest, const MP_INT *mp_val, const uint32_t base, const Allocator *allocator) {
  if (base < 2 || base > LMP_BASE_MAX) {
    dest->size = 0;
    dest->limb_base = base;
    dest->base_limb_width = 0;
    dest->base_limb_val = 0;
    if (base > LMP_BASE_MAX) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INS;
    } else if (base == 1) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INF;
    } else {
      dest->exp = sgn(mp_val->size) * MP_NUMBER_NAN;
    }
    return;
  }
  const lmp_size_t size = abs(mp_val->size);
  if (extstd_has_single_bit(base)) {
    const uint32_t base_width = extstd_bit_width(base - 1);
    if (LMP_LIMB_BIT % base_width == 0) {
      if (dest->prec < 0) {
        if (abs(mp_val->size) > -dest->prec) { dest->size = 0; return ; }
      } else if (dest->prec >= 1) {
        MP_INT_realloc((MP_INT *)dest, abs(mp_val->size), allocator);
      } else /* (dest->alloc == 0) means not init yet. */ {
        MP_INT_init((MP_INT *)dest, abs(mp_val->size), allocator);
      }
      dest->size = abs(mp_val->size);
      allocator->memcpy(dest->limbs, mp_val->limbs, dest->size * sizeof(lmp_limb_t));
      dest->base_limb_val = (lmp_limb_t) LMP_LIMB_VAL;
      dest->base_limb_width = LMP_LIMB_BIT / base_width;
      dest->limb_base = base;
      dest->exp = 0;
    }
    /* TOOD: other base of power of 2 */
    return;
  }
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)((double)LMP_LIMB_BIT / log2(base));
  int32_t base_limb_size = ceil(LMP_LIMB_BIT * size / log2(base) / BASE_LIMB_WIDTH);
  lmp_double_limb_t BASE_LIMB_VAL = powl(base, BASE_LIMB_WIDTH / 2);
  BASE_LIMB_VAL *= BASE_LIMB_VAL;
  if (BASE_LIMB_WIDTH % 2 == 1) { BASE_LIMB_VAL *= base; }

  if (base_limb_size == 1) {
    if (dest->prec < 0) {
      if (base_limb_size > -dest->prec) { dest->size = 0; return ; }
      dest->limbs[0] = mp_val->limbs[0];
    } else if (dest->prec >= 1) {
      dest->limbs[0] = mp_val->limbs[0];
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)dest, 1, allocator);
      dest->limbs[0] = mp_val->limbs[0];
    }
    dest->size = 1;
    dest->base_limb_val = BASE_LIMB_VAL;
    dest->base_limb_width = BASE_LIMB_WIDTH;
    dest->limb_base = base;
    dest->exp = 0;
    return;
  }

  const uint32_t level_count = (uint32_t)ceil(log2(base_limb_size));
  bool free_cache = false;
  BaseCache *cache = nullptr; // TODO: using global base cache
  if (!cache) { cache = allocator->calloc(1, sizeof(BaseCache)); free_cache = true; }
  if (cache->count < level_count) {
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    void *ptr = nullptr;
    ptr = allocator->realloc(cache->mpint, level_count * sizeof(MP_INT));
    if (ptr) { cache->mpint = ptr; }
    const uint32_t alloc_limbs = 1 << level_count;
    ptr = allocator->realloc(cache->limbs, alloc_limbs * sizeof(lmp_limb_t));
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
  const uint32_t result_limb_alloc = 1 << level_count;
  lmp_limb_t *result_limbs = allocator->calloc(result_limb_alloc, sizeof(lmp_limb_t));
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
  if (free_cache) {
    allocator->free(cache->mpint);
    allocator->free(cache->limbs);
    allocator->free(cache);
  }

  while (base_limb_size && !result_limbs[base_limb_size - 1]) { base_limb_size--; }

  if (dest->prec < 0) {
    if (base_limb_size > -dest->prec) { dest->size = 0; return ; }
    allocator->memcpy(dest->limbs, result_limbs, base_limb_size * sizeof(lmp_limb_t));
    allocator->free(result_limbs);
  } else {
    if (dest->limbs) { allocator->free(dest->limbs); }
    dest->prec = result_limb_alloc;
    dest->limbs = result_limbs;
  }
  dest->size = base_limb_size;
  dest->base_limb_val = BASE_LIMB_VAL;
  dest->base_limb_width = BASE_LIMB_WIDTH;
  dest->limb_base = base;
  dest->exp = 0;
}

void MP_FLOAT_abs_to_base_number(MP_BASE_NUMBER *dest, const MP_FLOAT *mp_val, uint32_t base, const Allocator * allocator) {
  if (base < 2 || base > LMP_BASE_MAX) {
    dest->size = 0;
    dest->limb_base = base;
    dest->base_limb_width = 0;
    dest->base_limb_val = 0;
    if (base > LMP_BASE_MAX) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INS;
    } else if (base == 1) {
      dest->exp = mp_val->size == 0 ? MP_NUMBER_ZERO : sgn(mp_val->size) * MP_NUMBER_INF;
    } else {
      dest->exp = sgn(mp_val->size) * MP_NUMBER_NAN;
    }
    return;
  }
  const lmp_size_t size = abs(mp_val->size);
  const uint32_t BASE_LIMB_WIDTH = (uint32_t)((double)LMP_LIMB_BIT / log2(base));
  int32_t base_limb_size = ceil(LMP_LIMB_BIT * size / log2(base) / BASE_LIMB_WIDTH);
  lmp_double_limb_t BASE_LIMB_VAL = powl(base, BASE_LIMB_WIDTH / 2);
  BASE_LIMB_VAL *= BASE_LIMB_VAL;
  if (BASE_LIMB_WIDTH % 2 == 1) { BASE_LIMB_VAL *= base; }

  if (mp_val->size == 0) {
    dest->size = 0;
    dest->limb_base = base;
    dest->base_limb_width = BASE_LIMB_WIDTH;
    dest->base_limb_val = BASE_LIMB_VAL;
    dest->exp = mp_val->exp;
    return;
  }
  if (base_limb_size == 1) {
    if (dest->prec < 0) {
      if (base_limb_size > -dest->prec) { dest->size = 0; return ; }
      dest->limbs[0] = mp_val->limbs[0];
    } else if (dest->prec >= 1) {
      dest->limbs[0] = mp_val->limbs[0];
    } else /* (dest->alloc == 0) means not init yet. */ {
      MP_INT_init((MP_INT *)dest, 1, allocator);
      dest->limbs[0] = mp_val->limbs[0];
    }
    dest->size = 1;
    dest->base_limb_val = BASE_LIMB_VAL;
    dest->base_limb_width = BASE_LIMB_WIDTH;
    dest->limb_base = base;
    dest->exp = 0;
    return;
  }
}

uint32_t MP_BASE_NUMBER_abs_to_string(char *dest, const uint32_t max_len, const MP_BASE_NUMBER *mp_val, const Allocator *allocator) {
  if (mp_val->limb_base < 2 || mp_val->limb_base > 36 || max_len == 0) { return 0; }
  if (mp_val->size == 0) {  *dest = '0'; return 1; }
  const uint32_t size = mp_val->size;

  if (max_len < mp_val->base_limb_width * size + 1) {
    MP_INT_clear((MP_INT *)&mp_val, allocator);
    return 0;
  }

  char *ptr = dest;
  for (uint32_t i = size - 1; i < size; i--) {
    lmp_limb_t limb = mp_val->limbs[i];
    for (uint32_t j = mp_val->base_limb_width - 1; j < mp_val->base_limb_width; j--) {
      const uint8_t n = limb % mp_val->limb_base;
      ptr[j] = digit(n, UPPER);
      limb /= mp_val->limb_base;
    }
    ptr += mp_val->base_limb_width;
  }

  MP_INT_clear((MP_INT *)&mp_val, allocator);
  return ptr - dest;
}

uint32_t MP_FLOAT_abs_to_base_string(char *, const uint32_t , const MP_FLOAT *, const uint32_t , const Allocator *) {
  return 0;
}
