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
 * Module Name: test/testcases
 * Filename: left_shift.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"

typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *e;
  int32_t           a_len;
  int32_t           e_len;
  int32_t           shift;
} ShiftCase;

static void run_left_shift_case(const ShiftCase *c)
{

  lmp_limb_t buf[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(buf) / sizeof(buf[0]));
  memcpy(buf, c->a, sizeof(lmp_limb_t) * (size_t)c->a_len);

  MP_INT A = { .alloc = 0, .size = c->a_len, .limbs = buf };

  MP_INT_left_shift(&A, c->shift, &STDAllocator);

  ck_assert_msg(A.size == c->e_len,
                "%s: result size %d != %d", c->name, A.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(A.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)A.limbs[i],
                  (unsigned long long)c->e[i]);
  }
}

static const lmp_limb_t SLA1[] = { 1 };
static const lmp_limb_t SLE1[] = { 2 };

static const lmp_limb_t SLA2[] = { (1ULL << 63) };
static const lmp_limb_t SLE2[] = { 0, 1 };

static const lmp_limb_t SLA3[] = { 0 };
static const lmp_limb_t SLE3[] = { 0 };

static const lmp_limb_t SLA4[] = { 3, 1 };
static const lmp_limb_t SLE4[] = { 0, 3, 1 };

static const lmp_limb_t SLA5[] = { UINT64_MAX };
static const lmp_limb_t SLE5[] = { (UINT64_MAX << 4), 15 };

static const lmp_limb_t SLA6[] = { 1, 2, 3 };
static const lmp_limb_t SLE6[] = { (1ULL << 63), 0, (1ULL << 63) + 1, 1};

static const ShiftCase left_cases[] = {
  { "left1_bit",        SLA1, SLE1, 1, 1, 1 },
  { "left2_carry",      SLA2, SLE2, 1, 2, 1 },
  { "left3_zero",       SLA3, SLE3, 0, 0, 100 },
  { "left4_limb_align", SLA4, SLE4, 2, 3, 64 },
  { "left5_overflow",   SLA5, SLE5, 1, 2, 4 },
  { "left6_multi",      SLA6, SLE6, 3, 4, 63 },
};

START_TEST(test_left_shift)
{
  run_left_shift_case(&left_cases[_i]);
}
END_TEST

Suite *left_shift_suite()
{
  Suite *s = suite_create("left_shift");
  TCase *tc = tcase_create("left_shift");

  constexpr int n = sizeof(left_cases)/sizeof(ShiftCase);
  for (int i = 0; i < n; i++) {
    tcase_add_loop_test(tc, test_left_shift, 0, n);
  }

  suite_add_tcase(s, tc);
  return s;
}
