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
 * Module Name: test/testcases/int
 * Filename: sub.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include <stdio.h>
#include "test.h"
#include <allocator.h>

typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *b;
  const lmp_limb_t *e;
  int32_t           a_len;
  int32_t           b_len;
  int32_t           e_len;
  int32_t           offset;
} IntSubCase;

static void run_sub_case(const IntSubCase *c)
{
  lmp_limb_t RES_limbs[64] = {};

  MP_INT A   = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B   = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT RES = { .prec = -(int32_t)(sizeof(RES_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = RES_limbs };

  MP_INT_sub(&RES, &A, &B, &STDAllocator);

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %"PRI_SIZE" != (expected) %"PRI_SIZE"\n", c->name, RES.size, c->e_len);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != (expected) %"PRI_LIMB"\n", c->name, i, RES.limbs[i], c->e[i]);
  }
}

static const lmp_limb_t sA1[] = { 1234 };
static const lmp_limb_t sB1[] = { 1000 };
static const lmp_limb_t sE1[] = { 234 };

static const lmp_limb_t sA2[] = { LMP_LIMB_MAX, 1 };
static const lmp_limb_t sB2[] = { 1, 1 };
static const lmp_limb_t sE2[] = { LMP_LIMB_MAX - 1 };

static const lmp_limb_t sA3[] = { 0, 1 };
static const lmp_limb_t sB3[] = { 1, 0 };
static const lmp_limb_t sE3[] = { LMP_LIMB_MAX };

static const lmp_limb_t sA4[] = { 0, 0, 5 };
static const lmp_limb_t sB4[] = { 0, 1, 2 };
static const lmp_limb_t sE4[] = { 0, LMP_LIMB_MAX, 2 };

static const lmp_limb_t sA5[] = { 0, 7 };
static const lmp_limb_t sB5[] = { 0, 7 };
static const lmp_limb_t sE5[] = { 0 };

static const lmp_limb_t sA6[] = { 0, 0, 2 };
static const lmp_limb_t sB6[] = { 0, 1, 1 };
static const lmp_limb_t sE6[] = { 0, LMP_LIMB_MAX };

static const lmp_limb_t sA7[] = { 0, 0, 2 };
static const lmp_limb_t sB7[] = { 0, 1, 1 };
static const lmp_limb_t sE7[] = { 0, 1, 3 };

static const lmp_limb_t sA8[] = { 0, 1, 1 };
static const lmp_limb_t sB8[] = { 0, 0, 2 };
static const lmp_limb_t sE8[] = { 0, LMP_LIMB_MAX };

static const IntSubCase sub_cases[] = {
  { .name = "sub:simple",         .a = sA1, .b = sB1, .e = sE1, .a_len =  1, .b_len =  1, .e_len =  1 },
  { .name = "sub:multi_limb",     .a = sA2, .b = sB2, .e = sE2, .a_len =  2, .b_len =  2, .e_len =  1 },
  { .name = "sub:borrow_chain",   .a = sA3, .b = sB3, .e = sE3, .a_len =  2, .b_len =  2, .e_len =  1 },
  { .name = "sub:borrow",         .a = sA4, .b = sB4, .e = sE4, .a_len =  3, .b_len =  3, .e_len =  3 },
  { .name = "sub:zero_result",    .a = sA5, .b = sB5, .e = sE5, .a_len =  2, .b_len =  2, .e_len =  0 },
  { .name = "sub:edge",           .a = sA6, .b = sB6, .e = sE6, .a_len =  3, .b_len =  3, .e_len =  2 },
  { .name = "sub:add",            .a = sA7, .b = sB7, .e = sE7, .a_len =  3, .b_len = -3, .e_len =  3 },
  { .name = "sub:neg1",           .a = sA8, .b = sB8, .e = sE8, .a_len = -3, .b_len = -3, .e_len =  2 },
  { .name = "sub:neg2",           .a = sA8, .b = sB8, .e = sE8, .a_len =  3, .b_len =  3, .e_len = -2 },
};

START_TEST(test_sub)
{
  run_sub_case(&sub_cases[_i]);
}
END_TEST

START_TEST(test_MP_INT_sub_rop_is_op1) {
  lmp_limb_t op1_limbs[] = {7, 9};
  lmp_limb_t op2_limbs[] = {8, 9};
  lmp_limb_t e_limbs[] = {1, 0};

  MP_INT A = { .prec = -2, .size =  2, .limbs = op1_limbs };
  MP_INT B = { .prec = -2, .size =  2, .limbs = op2_limbs };
  MP_INT_sub(&A, &A, &B, &STDAllocator);
  ck_assert(A.size == -1);
  ck_assert(op1_limbs[0] == e_limbs[0]);
  ck_assert(op1_limbs[1] == e_limbs[1]);
}
END_TEST

Suite *MP_TEST_int_sub_suite()
{
  Suite *s = suite_create("int_sub");
  TCase *tc = tcase_create("int_sub");

  constexpr int n2 = sizeof(sub_cases)/sizeof(IntSubCase);
  tcase_add_loop_test(tc, test_sub, 0, n2);
  tcase_add_test(tc, test_MP_INT_sub_rop_is_op1);

  suite_add_tcase(s, tc);
  return s;
}
