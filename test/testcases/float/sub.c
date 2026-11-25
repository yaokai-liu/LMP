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
 * Filename: sub.c
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
} FloatSubCase;

static void run_sub_case(const FloatSubCase *c)
{
  lmp_limb_t A_limbs[64] = {};
  lmp_limb_t RES_limbs[64] = {};
  STDAllocator.memcpy(A_limbs, c->a, LMP_LIMB_SIZE * abs(c->a_len));

  MP_FLOAT A = { .prec = -c->a_len, .size = c->a_len, .limbs = A_limbs, .exp = c->a_exp };
  const MP_FLOAT B   = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b, .exp = c->b_exp };
  MP_FLOAT RES = { .prec = -(lmp_prec_t)(sizeof(RES_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = RES_limbs };

  MP_FLOAT_sub(&RES, &A, &B, &STDAllocator);

  ck_assert_msg(abs(RES.size) == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE, c->name, RES.size, c->e_len);
  ck_assert_msg(abs(RES.exp) == c->e_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP, c->name, RES.exp, c->e_exp);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB, c->name, i,
                  RES.limbs[i], c->e[i]);
  }
}
static const lmp_limb_t SA1[] = { 1234 };
static const lmp_limb_t SB1[] = { 1000 };
static const lmp_limb_t SE1[] = { 234 };

static const lmp_limb_t SA2[] = { LMP_LIMB_MAX, 1 };
static const lmp_limb_t SB2[] = { 1, 1 };
static const lmp_limb_t SE2[] = { LMP_LIMB_MAX - 1 };

static const lmp_limb_t SA3[] = { 0, 1 };
static const lmp_limb_t SB3[] = { 1, 0 };
static const lmp_limb_t SE3[] = { LMP_LIMB_MAX };

static const lmp_limb_t SA4[] = { 5, 3 };
static const lmp_limb_t SB4[] = { 1 };
static const lmp_limb_t SE4[] = { 5, 2 };

static const lmp_limb_t SA5[] = { 0, 0, 5 };
static const lmp_limb_t SB5[] = { 1, 2 };
static const lmp_limb_t SE5[] = { 0, LMP_LIMB_MAX, 2 };

static const lmp_limb_t SA6[] = { 16, 0, 3 };
static const lmp_limb_t SB6[] = { 2 };
static const lmp_limb_t SE6[] = { 16, 0, 1 };

static const lmp_limb_t SA7[] = { 0, 7 };
static const lmp_limb_t SB7[] = { 0, 7 };
static const lmp_limb_t SE7[] = { 0 };

static const lmp_limb_t SA8[] = { 0, 0, 2 };
static const lmp_limb_t SB8[] = { 1, 1 };
static const lmp_limb_t SE8[] = { 0, LMP_LIMB_MAX };

static const FloatSubCase sub_cases[] = {
  { .name = "case1_simple",         .a = SA1, .b = SB1, .e = SE1, .a_exp = 0, .b_exp = 0, .e_exp = 0,.a_len = 1, .b_len = 1, .e_len = 1 },
  { .name = "case2_multi_limb",     .a = SA2, .b = SB2, .e = SE2, .a_exp = 0, .b_exp = 0, .e_exp = 0,.a_len = 2, .b_len = 2, .e_len = 1 },
  { .name = "case3_borrow_chain",   .a = SA3, .b = SB3, .e = SE3, .a_exp = 0, .b_exp = 0, .e_exp = 0,.a_len = 2, .b_len = 2, .e_len = 1 },
  { .name = "case4_offset1",        .a = SA4, .b = SB4, .e = SE4, .a_exp = 0, .b_exp = 1, .e_exp = 0,.a_len = 2, .b_len = 1, .e_len = 2 },
  { .name = "case5_offset1_borrow", .a = SA5, .b = SB5, .e = SE5, .a_exp = 0, .b_exp = 1, .e_exp = 0,.a_len = 3, .b_len = 2, .e_len = 3 },
  { .name = "case6_offset2",        .a = SA6, .b = SB6, .e = SE6, .a_exp = 0, .b_exp = 2, .e_exp = 0,.a_len = 3, .b_len = 1, .e_len = 3 },
  { .name = "case7_zero_result",    .a = SA7, .b = SB7, .e = SE7, .a_exp = 0, .b_exp = 0, .e_exp = 0,.a_len = 2, .b_len = 2, .e_len = 0 },
  { .name = "case8_offset1_edge",   .a = SA8, .b = SB8, .e = SE8, .a_exp = 0, .b_exp = 1, .e_exp = 0,.a_len = 3, .b_len = 2, .e_len = 2 },
};

START_TEST(test_sub)
{
  run_sub_case(&sub_cases[_i]);
}
END_TEST

Suite *MP_TEST_float_sub_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("abs_sub_offset");

  constexpr int n = sizeof(sub_cases)/sizeof(FloatSubCase);
  tcase_add_loop_test(tc, test_sub, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
