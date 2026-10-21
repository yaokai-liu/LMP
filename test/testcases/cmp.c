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
 * Filename: cmp.c
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
  int32_t           a_len;
  int32_t           b_len;
  int32_t           expect;
} CmpCase;

static void run_cmp_case(const CmpCase *c)
{
  MP_INT A = { .alloc = 0, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B = { .alloc = 0, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };

  int32_t got = MP_INT_abs_cmp(&A, &B);

  ck_assert_msg(got == c->expect,
                "%s: MP_INT_abs_cmp returned %d, expected %d",
                c->name, got, c->expect);
}

static const lmp_limb_t CA1[] = { 1 };
static const lmp_limb_t CB1[] = { 2 };

static const lmp_limb_t CA2[] = { 2 };
static const lmp_limb_t CB2[] = { 1 };

static const lmp_limb_t CA3[] = { 7 };
static const lmp_limb_t CB3[] = { 7 };

static const lmp_limb_t CA5[] = { UINT64_MAX };
static const lmp_limb_t CB5[] = { 1, 0 };

static const lmp_limb_t CA4[] = { 1, 0 };
static const lmp_limb_t CB4[] = { UINT64_MAX };

static const lmp_limb_t CA6[] = { 5, 7 };
static const lmp_limb_t CB6[] = { 9, 6 };

static const lmp_limb_t CA7[] = { 9, 6 };
static const lmp_limb_t CB7[] = { 5, 7 };

static const lmp_limb_t CA8[] = { 100, 7 };
static const lmp_limb_t CB8[] = { 99,  7 };

static const lmp_limb_t CA9[] = { 99,  7 };
static const lmp_limb_t CB9[] = { 100, 7 };

static const lmp_limb_t CA10[] = { 42, 7, 3 };
static const lmp_limb_t CB10[] = { 42, 7, 3 };

static const lmp_limb_t CA11[] = { 10, 20, 30 };
static const lmp_limb_t CB11[] = { 10, 19, 30 };

static const lmp_limb_t CA12[] = { 0 };
static const lmp_limb_t CB12[] = { 0 };

static const lmp_limb_t CA13[] = { 0 };
static const lmp_limb_t CB13[] = { 1 };

static const lmp_limb_t CA14[] = { 1 };
static const lmp_limb_t CB14[] = { 0 };

static const lmp_limb_t CA15[] = { 5, 9, 9 };
static const lmp_limb_t CB15[] = { 4, 9, 9 };

static const lmp_limb_t CA16[] = { 4, 9, 9 };
static const lmp_limb_t CB16[] = { 5, 9, 9 };

static const CmpCase cmp_cases[] = {
  { "cmp_1_lt_2",        CA1,  CB1,  1, 1, -1 },
  { "cmp_2_gt_1",        CA2,  CB2,  1, 1,  1 },
  { "cmp_7_eq_7",        CA3,  CB3,  1, 1,  0 },
  { "cmp_len_a_longer",  CA4,  CB4,  2, 1, -1 },
  { "cmp_len_a_shorter", CA5,  CB5,  1, 2,  1 },
  { "cmp_hi_a_bigger",   CA6,  CB6,  2, 2,  1 },
  { "cmp_hi_a_smaller",  CA7,  CB7,  2, 2, -1 },
  { "cmp_lo_a_bigger",   CA8,  CB8,  2, 2,  1 },
  { "cmp_lo_a_smaller",  CA9,  CB9,  2, 2, -1 },
  { "cmp_three_eq",      CA10, CB10, 3, 3,  0 },
  { "cmp_mid_limb",      CA11, CB11, 3, 3,  1 },
  { "cmp_both_zero",     CA12, CB12, 0, 0,  0 },
  { "cmp_zero_vs_one",   CA13, CB13, 0, 1, -1 },
  { "cmp_one_vs_zero",   CA14, CB14, 1, 0,  1 },
  { "cmp_lowest_bigger", CA15, CB15, 3, 3,  1 },
  { "cmp_lowest_smaller",CA16, CB16, 3, 3, -1 },
};

START_TEST(test_cmp)
{
  run_cmp_case(&cmp_cases[_i]);
}
END_TEST

Suite *cmp_suite()
{
  Suite *s = suite_create("cmp");
  TCase *tc = tcase_create("cmp");

  constexpr int n = sizeof(cmp_cases)/sizeof(CmpCase);
  tcase_add_loop_test(tc, test_cmp, 0, n);

  suite_add_tcase(s, tc);
  return s;
}

