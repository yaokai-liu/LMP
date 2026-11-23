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
 * Filename: mul.c
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
  lmp_exp_t         a_exp;
  lmp_exp_t         b_exp;
  lmp_exp_t         e_exp;
  lmp_size_t        a_len;
  lmp_size_t        b_len;
  lmp_size_t        e_len;
} FloatMulCase;

static void run_mul_case(const FloatMulCase *c)
{
  lmp_limb_t RES_limbs[64] = {};

  const MP_FLOAT A = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a, .exp = c->a_exp };
  const MP_FLOAT B   = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b, .exp = c->b_exp };
  MP_FLOAT RES = { .prec = -(lmp_prec_t)(sizeof(RES_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = RES_limbs };

  MP_FLOAT_mul(&RES, &A, &B, &STDAllocator);

  ck_assert_msg(abs(RES.size) == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE, c->name, RES.size, c->e_len);
  ck_assert_msg(abs(RES.exp) == c->e_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP, c->name, RES.exp, c->e_exp);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB, c->name, i,
                  RES.limbs[i], c->e[i]);
  }
}

static const FloatMulCase abs_mul_cases[] = {
};


START_TEST(test_mul)
{
  run_mul_case(&abs_mul_cases[_i]);
}
END_TEST

Suite *MP_TEST_float_mul_suite()
{
  Suite *s = suite_create("abs_mul");
  TCase *tc = tcase_create("abs_mul");

  constexpr int n = sizeof(abs_mul_cases)/sizeof(FloatMulCase);
  tcase_add_loop_test(tc, test_mul, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
