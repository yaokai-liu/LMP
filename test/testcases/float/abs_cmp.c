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
 * Module Name: test/testcases/float
 * Filename: abs_cmp.c
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
  lmp_exp_t         a_exp;
  lmp_exp_t         b_exp;
  lmp_size_t        a_len;
  lmp_size_t        b_len;
  int32_t           expected;
} FloatCmpCase;

static void run_cmp_case(const FloatCmpCase *c)
{
  const MP_FLOAT A = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a, .exp = c->a_exp };
  const MP_FLOAT B   = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b, .exp = c->b_exp };

  const int32_t got = MP_FLOAT_abs_cmp(&A, &B);

  ck_assert_msg(got == c->expected,
                "%s: MP_INT_abs_cmp returned %"PRIi32", expected %"PRIi32,
                c->name, got, c->expected);
}

static const FloatCmpCase cmp_cases[] = {
};

START_TEST(test_cmp)
{
  run_cmp_case(&cmp_cases[_i]);
}
END_TEST

Suite *MP_TEST_float_cmp_suite()
{
  Suite *s = suite_create("cmp");
  TCase *tc = tcase_create("cmp");

  constexpr int n = sizeof(cmp_cases)/sizeof(FloatCmpCase);
  tcase_add_loop_test(tc, test_cmp, 0, n);

  suite_add_tcase(s, tc);
  return s;
}

