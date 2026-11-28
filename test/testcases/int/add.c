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
 * Filename: add.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

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
} IntAddCase;

static void run_add_case(const IntAddCase *c)
{
  lmp_limb_t RES_limbs[64] = {};

  MP_INT A = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B   = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT RES = { .prec = -(int32_t)(sizeof(RES_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = RES_limbs };

  MP_INT_add(&RES, &A, &B, &STDAllocator);

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %"PRI_SIZE" != (expected) %"PRI_SIZE"\n", c->name, RES.size, c->e_len);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != (expected) %"PRI_LIMB"\n", c->name, i,
                  RES.limbs[i], c->e[i]);
  }
}

static const lmp_limb_t aA1[] = {1, 2, 3};
static const lmp_limb_t aB1[] = {4, 5, 6};
static const lmp_limb_t aE1[] = {5, 7, 9};

static const lmp_limb_t aA2[] = {1};
static const lmp_limb_t aB2[] = {2};
static const lmp_limb_t aE2[] = {3};

static const lmp_limb_t aA3[] = {~(lmp_limb_t)0, ~(lmp_limb_t)0};
static const lmp_limb_t aB3[] = {1};
static const lmp_limb_t aE3[] = {0, 0, 1};

static const lmp_limb_t aA4[] = {1, 2, 3, 4};
static const lmp_limb_t aB4[] = {5, 6};
static const lmp_limb_t aE4[] = {6, 8, 3, 4};

static const lmp_limb_t aA5[] = {1, 2, 3};
static const lmp_limb_t aB5[] = {0, 4, 5};
static const lmp_limb_t aE5[] = {1, 6, 8};

static const lmp_limb_t aA6[] = {1, 2};
static const lmp_limb_t aB6[] = {0, 0, 0, 3, 4};
static const lmp_limb_t aE6[] = {1, 2, 0, 3, 4};

static const lmp_limb_t aA7[] = {1};
static const lmp_limb_t aB7[] = {0, 0, 2, 3};
static const lmp_limb_t aE7[] = {1, 0, 2, 3};

static const lmp_limb_t aA8[] = {~(lmp_limb_t)0, ~(lmp_limb_t)0};
static const lmp_limb_t aB8[] = {0, 1};
static const lmp_limb_t aE8[] = {~(lmp_limb_t)0, 0, 1};

static const lmp_limb_t aA9[] = {7};
static const lmp_limb_t aB9[] = {0, 8, 9};
static const lmp_limb_t aE9[] = {7, 8, 9};

static const lmp_limb_t aA10[] = {7};
static const lmp_limb_t aB10[] = {8, 9};
static const lmp_limb_t aE10[] = {1, 9};

static const lmp_limb_t aA11[] = {8, 9};
static const lmp_limb_t aB11[] = {7};
static const lmp_limb_t aE11[] = {1, 9};

static const lmp_limb_t aA12[] = {8, 9};
static const lmp_limb_t aB12[] = {8, 9};
static const lmp_limb_t aE12[] = {0};

static const IntAddCase add_cases[] = {
  { .name = "add:basic3x3",        .a = aA1,  .b = aB1,  .e = aE1,  .a_len =  3, .b_len = 3, .e_len =  3 },
  { .name = "add:basic1x1",        .a = aA2,  .b = aB2,  .e = aE2,  .a_len =  1, .b_len = 1, .e_len =  1 },
  { .name = "add:carry",           .a = aA3,  .b = aB3,  .e = aE3,  .a_len =  2, .b_len = 1, .e_len =  3 },
  { .name = "add:diff_len",        .a = aA4,  .b = aB4,  .e = aE4,  .a_len =  4, .b_len = 4, .e_len =  4 },
  { .name = "add:overlap",         .a = aA5,  .b = aB5,  .e = aE5,  .a_len =  3, .b_len = 3, .e_len =  3 },
  { .name = "add:gap",             .a = aA6,  .b = aB6,  .e = aE6,  .a_len =  2, .b_len = 5, .e_len =  5 },
  { .name = "add:extend",          .a = aA7,  .b = aB7,  .e = aE7,  .a_len =  1, .b_len = 4, .e_len =  4 },
  { .name = "add:carry",           .a = aA8,  .b = aB8,  .e = aE8,  .a_len =  2, .b_len = 2, .e_len =  3 },
  { .name = "add:tail",            .a = aA9,  .b = aB9,  .e = aE9,  .a_len =  1, .b_len = 3, .e_len =  3 },
  { .name = "add:sub1",            .a = aA10, .b = aB10, .e = aE10, .a_len = -1, .b_len = 2, .e_len =  2 },
  { .name = "add:sub2",            .a = aA11, .b = aB11, .e = aE11, .a_len = -2, .b_len = 1, .e_len = -2 },
  { .name = "add:sub3",            .a = aA12, .b = aB12, .e = aE12, .a_len = -2, .b_len = 2, .e_len =  0 },
};

START_TEST(test_add)
{
  run_add_case(&add_cases[_i]);
}
END_TEST

START_TEST(test_MP_INT_add_rop_is_op1) {
  lmp_limb_t op1_limbs[] = {7, 9};
  lmp_limb_t op2_limbs[] = {8, 9};
  lmp_limb_t e_limbs[] = {1, 0};

  MP_INT A = { .prec = -2, .size =  2, .limbs = op1_limbs };
  MP_INT B = { .prec = -2, .size = -2, .limbs = op2_limbs };
  MP_INT_add(&A, &A, &B, &STDAllocator);
  ck_assert(A.size == -1);
  ck_assert(op1_limbs[0] == e_limbs[0]);
  ck_assert(op1_limbs[1] == e_limbs[1]);
}
END_TEST


Suite *MP_TEST_int_add_suite()
{
  Suite *s = suite_create("int_add");
  TCase *tc = tcase_create("int_add");

  constexpr int n2 = sizeof(add_cases)/sizeof(IntAddCase);
  tcase_add_loop_test(tc, test_add, 0, n2);
  tcase_add_test(tc, test_MP_INT_add_rop_is_op1);

  suite_add_tcase(s, tc);
  return s;
}
