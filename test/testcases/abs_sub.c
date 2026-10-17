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
 * Filename: abs_sub.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"

typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *b;
  const lmp_limb_t *e;
  int32_t           a_len;
  int32_t           b_len;
  int32_t           e_len;
  int32_t           offset;
} SubCase;

static void run_sub_case(const SubCase *c)
{
  lmp_limb_t RES_limbs[(c->a_len > c->b_len ? c->a_len : c->b_len) + c->offset + 1] = {};

  MP_INT A   = { .alloc = 0, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B   = { .alloc = 0, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT RES = { .alloc = 0, .size = 0,        .limbs = RES_limbs };

  MP_INT_abs_sub_offset(&RES, &A, &B, c->offset, &STDAllocator);

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %d != %d", c->name, RES.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)RES.limbs[i],
                  (unsigned long long)c->e[i]);
  }
}

static const lmp_limb_t SA1[] = { 1234 };
static const lmp_limb_t SB1[] = { 1000 };
static const lmp_limb_t SE1[] = { 234 };

static const lmp_limb_t SA2[] = { UINT64_MAX, 1 };
static const lmp_limb_t SB2[] = { 1, 1 };
static const lmp_limb_t SE2[] = { UINT64_MAX - 1 };

static const lmp_limb_t SA3[] = { 0, 1 };
static const lmp_limb_t SB3[] = { 1, 0 };
static const lmp_limb_t SE3[] = { UINT64_MAX };

static const lmp_limb_t SA4[] = { 5, 3 };
static const lmp_limb_t SB4[] = { 1 };
static const lmp_limb_t SE4[] = { 0, 2 };

static const lmp_limb_t SA5[] = { 0, 0, 5 };
static const lmp_limb_t SB5[] = { 1, 2 };
static const lmp_limb_t SE5[] = { 0, UINT64_MAX, 2 };

static const lmp_limb_t SA6[] = { 16, 0, 3 };
static const lmp_limb_t SB6[] = { 2 };
static const lmp_limb_t SE6[] = { 0, 0, 1 };

static const lmp_limb_t SA7[] = { 0, 7 };
static const lmp_limb_t SB7[] = { 0, 7 };
static const lmp_limb_t SE7[] = { 0 };

static const lmp_limb_t SA8[] = { 0, 0, 2 };
static const lmp_limb_t SB8[] = { 1, 1 };
static const lmp_limb_t SE8[] = { 0, UINT64_MAX };

static const SubCase sub_cases[] = {
  { "case1_simple",         SA1, SB1, SE1, 1, 1, 1, 0 },
  { "case2_multi_limb",     SA2, SB2, SE2, 2, 2, 1, 0 },
  { "case3_borrow_chain",   SA3, SB3, SE3, 2, 2, 1, 0 },
  { "case4_offset1",        SA4, SB4, SE4, 2, 1, 2, 1 },
  { "case5_offset1_borrow", SA5, SB5, SE5, 3, 2, 3, 1 },
  { "case6_offset2",        SA6, SB6, SE6, 3, 1, 3, 2 },
  { "case7_zero_result",    SA7, SB7, SE7, 2, 2, 0, 0 },
  { "case8_offset1_edge",   SA8, SB8, SE8, 3, 2, 2, 1 },
};

START_TEST(test_abs_sub_offset)
{
  run_sub_case(&sub_cases[_i]);
}
END_TEST

Suite *abs_sub_offset_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("abs_sub_offset");

  constexpr int n = sizeof(sub_cases)/sizeof(SubCase);
  for (int i = 0; i < n; i++) {
    tcase_add_loop_test(tc, test_abs_sub_offset, 0, n);
  }

  suite_add_tcase(s, tc);
  return s;
}
