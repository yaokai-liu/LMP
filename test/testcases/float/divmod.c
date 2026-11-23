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
 * Filename: divmod.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-18
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"


typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *b;
  const lmp_limb_t *eq;
  const lmp_limb_t *er;
  lmp_exp_t         a_exp;
  lmp_exp_t         b_exp;
  lmp_exp_t         eq_exp;
  lmp_exp_t         er_exp;
  lmp_size_t        a_len;
  lmp_size_t        b_len;
  lmp_size_t        eq_len;
  lmp_size_t        er_len;
  uint32_t          q_prec;
} FloatDivmodCase;

static void run_abs_divmod_case(const FloatDivmodCase *c)
{
  lmp_limb_t a_limbs[64] = {};
  lmp_limb_t q_limbs[64] = {};
  STDAllocator.memcpy(a_limbs, c->a, LMP_LIMB_SIZE * c->a_len);

  MP_FLOAT A = { .prec = -c->a_len, .size = c->a_len, .limbs = a_limbs, .exp = c->a_exp };
  const MP_FLOAT B = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b, .exp = c->b_exp };
  MP_FLOAT Q = { .prec = -(int32_t)c->q_prec, .size = 0, .limbs = q_limbs };

  MP_FLOAT_divmod(&A, &B, &Q, &STDAllocator);

  ck_assert_msg(abs(Q.size) == c->eq_len,
                "%s: quotient size %"PRI_SIZE" != %"PRI_SIZE, c->name, Q.size, c->eq_len);
  ck_assert_msg(abs(Q.exp) == c->eq_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP, c->name, Q.exp, c->eq_exp);
  for (int32_t i = 0; i < c->eq_len; i++) {
    ck_assert_msg(Q.limbs[i] == c->eq[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB, c->name, i, Q.limbs[i], c->eq[i]);
  }
  ck_assert_msg(abs(A.size) == c->er_len,
                "%s: remainder size %"PRI_SIZE" != %"PRI_SIZE, c->name, A.size, c->er_len);
  ck_assert_msg(abs(A.exp) == c->er_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP, c->name, A.exp, c->er_exp);
  for (int32_t i = 0; i < c->er_len; i++) {
    ck_assert_msg(A.limbs[i] == c->er[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB, c->name, i, A.limbs[i], c->er[i]);
  }
}

static const FloatDivmodCase abs_divmod_cases[] = {
};

START_TEST(test_divmod)
{
  run_abs_divmod_case(&abs_divmod_cases[_i]);
}
END_TEST

Suite *MP_TEST_float_divmod_suite()
{
  Suite *s = suite_create("abs_divmod");
  TCase *tc = tcase_create("abs_divmod");

  constexpr int n = sizeof(abs_divmod_cases)/sizeof(FloatDivmodCase);
  tcase_add_loop_test(tc, test_divmod, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
