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
  lmp_exp_t         a_exp;
  lmp_exp_t         b_exp;
  lmp_exp_t         e_exp;
  lmp_size_t        a_len;
  lmp_size_t        b_len;
  lmp_size_t        e_len;
} FloatAddCase;

static void run_add_case(const FloatAddCase *c)
{

  lmp_limb_t A_limbs[64] = {};
  lmp_limb_t B_limbs[64] = {};
  lmp_limb_t RES_limbs[64] = {};
  STDAllocator.memcpy(A_limbs, c->a, LMP_LIMB_SIZE * abs(c->a_len));
  STDAllocator.memcpy(B_limbs, c->b, LMP_LIMB_SIZE * abs(c->b_len));

  MP_FLOAT A = { .prec = -c->a_len, .size = c->a_len, .limbs = A_limbs, .exp = c->a_exp };
  MP_FLOAT B = { .prec = -c->b_len, .size = c->b_len, .limbs = B_limbs, .exp = c->b_exp };
  MP_FLOAT RES = { .prec = -(lmp_prec_t)(sizeof(RES_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = RES_limbs };

  MP_FLOAT_add(&RES, &A, &B, &STDAllocator);

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE"\n", c->name, RES.size, c->e_len);
  ck_assert_msg(RES.exp == c->e_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP"\n", c->name, RES.exp, c->e_exp);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB"\n", c->name, i, RES.limbs[i], c->e[i]);
  }
}

static const lmp_limb_t A1[] = {1, 2, 3};
static const lmp_limb_t B1[] = {4, 5, 6};
static const lmp_limb_t E1[] = {5, 7, 9};

static const lmp_limb_t A2[] = {1};
static const lmp_limb_t B2[] = {2};
static const lmp_limb_t E2[] = {3};

static const lmp_limb_t A3[] = {~(lmp_limb_t)0, ~(lmp_limb_t)0};
static const lmp_limb_t B3[] = {1};
static const lmp_limb_t E3[] = {1};

static const lmp_limb_t A4[] = {1, 2, 3, 4};
static const lmp_limb_t B4[] = {5, 6};
static const lmp_limb_t E4[] = {6, 8, 3, 4};

static const lmp_limb_t A5[] = {1, 2, 3};
static const lmp_limb_t B5[] = {4, 5};
static const lmp_limb_t E5[] = {1, 6, 8};

static const lmp_limb_t A6[] = {1, 2};
static const lmp_limb_t B6[] = {3, 4};
static const lmp_limb_t E6[] = {1, 2, 0, 3, 4};

static const lmp_limb_t A7[] = {1};
static const lmp_limb_t B7[] = {2, 3};
static const lmp_limb_t E7[] = {1, 0, 2, 3};

static const lmp_limb_t A8[] = {~(lmp_limb_t)0, ~(lmp_limb_t)0};
static const lmp_limb_t B8[] = {1};
static const lmp_limb_t E8[] = {~(lmp_limb_t)0, 0, 1};

static const lmp_limb_t A9[] = {7};
static const lmp_limb_t B9[] = {8, 9};
static const lmp_limb_t E9[] = {7, 8, 9};

static const lmp_limb_t A10[] = { 0 };
static const lmp_limb_t B10[] = { 0 };
static const lmp_limb_t E10[] = { 0 };

static const lmp_limb_t A11[] = { 1 };
static const lmp_limb_t B11[] = { 2 };
static const lmp_limb_t E11[] = { 1 };

static const lmp_limb_t A12[] = {1, 0, 2, 3};
static const lmp_limb_t B12[] = {2, 3};
static const lmp_limb_t E12[] = {1};

static const lmp_limb_t A13[] = {2, 3};
static const lmp_limb_t B13[] = {1, 0, 2, 3};
static const lmp_limb_t E13[] = {1};

static const FloatAddCase add_cases[] = {
  { .name = "basic3x3",        .a = A1, .b = B1, .e = E1, .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 3, .b_len = 3, .e_len = 3 },
  { .name = "basic1x1",        .a = A2, .b = B2, .e = E2, .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 1 },
  { .name = "carry",           .a = A3, .b = B3, .e = E3, .a_exp = 0, .b_exp = 0, .e_exp = 2, .a_len = 2, .b_len = 1, .e_len = 1 },
  { .name = "diff_len",        .a = A4, .b = B4, .e = E4, .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 4, .b_len = 2, .e_len = 4 },
  { .name = "offset1_overlap", .a = A5, .b = B5, .e = E5, .a_exp = 0, .b_exp = 1, .e_exp = 0, .a_len = 3, .b_len = 2, .e_len = 3 },
  { .name = "offset3_gap",     .a = A6, .b = B6, .e = E6, .a_exp = 0, .b_exp = 3, .e_exp = 0, .a_len = 2, .b_len = 2, .e_len = 5 },
  { .name = "offset2_extend",  .a = A7, .b = B7, .e = E7, .a_exp = 0, .b_exp = 2, .e_exp = 0, .a_len = 1, .b_len = 2, .e_len = 4 },
  { .name = "offset1_carry",   .a = A8, .b = B8, .e = E8, .a_exp = 0, .b_exp = 1, .e_exp = 0, .a_len = 2, .b_len = 1, .e_len = 3 },
  { .name = "offset1_tail",    .a = A9, .b = B9, .e = E9, .a_exp = 0, .b_exp = 1, .e_exp = 0, .a_len = 1, .b_len = 2, .e_len = 3 },

  { .name = "add_special1",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_NAN, .b_exp = MP_NUMBER_ZERO, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special2",    .a = A10, .b = B10, .e = E10, .a_exp = MP_NUMBER_ZERO, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special3",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_NAN, .b_exp = -MP_NUMBER_INF, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special4",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_NAN, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special5",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_NAN, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },

  { .name = "add_special6",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special7",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp = -MP_NUMBER_INF, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special8",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special9",    .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special10",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp = -MP_NUMBER_INS, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special11",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INF, .b_exp = MP_NUMBER_ZERO, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special12",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special13",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp = -MP_NUMBER_INF, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special14",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special15",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp =  MP_NUMBER_INS, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special16",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp = -MP_NUMBER_INS, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special17",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INF, .b_exp = MP_NUMBER_ZERO, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },

  { .name = "add_special18",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special19",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp = -MP_NUMBER_INF, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special20",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special21",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special22",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp = -MP_NUMBER_INS, .e_exp = -MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special23",   .a = A10, .b = B10, .e = E10, .a_exp =  MP_NUMBER_INS, .b_exp = MP_NUMBER_ZERO, .e_exp =  MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special24",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special25",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp = -MP_NUMBER_INF, .e_exp = -MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special26",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special27",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special28",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp = -MP_NUMBER_INS, .e_exp = -MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "add_special29",   .a = A10, .b = B10, .e = E10, .a_exp = -MP_NUMBER_INS, .b_exp = MP_NUMBER_ZERO, .e_exp = -MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },

  { .name = "add_special31",   .a = A11, .b = B11, .e = E11, .a_exp = -MP_NUMBER_INF, .b_exp = 0, .e_exp = -MP_NUMBER_INF, .a_len =  0, .b_len =  1, .e_len = 0 },

  { .name = "add_special32",   .a = A11, .b = B11, .e = E11, .a_exp = 0, .b_exp = MP_NUMBER_ZERO, .e_exp =  0, .a_len =  1, .b_len = 0, .e_len = 1 },
  { .name = "add_special32",   .a = A11, .b = B11, .e = E11, .a_exp = 0, .b_exp = -MP_NUMBER_INS, .e_exp =  0, .a_len =  1, .b_len = 0, .e_len = 1 },
  { .name = "add_special34",   .a = A11, .b = B11, .e = E11, .a_exp = 0, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_INF, .a_len = -1, .b_len = 0, .e_len = 0 },
  { .name = "add_special34",   .a = A11, .b = B11, .e = E11, .a_exp = 0, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len =  1, .b_len = 0, .e_len = 0 },

  { .name = "add_unknown1",    .a = A11, .b = B11, .e = E11, .a_exp = -10, .b_exp =   0, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len =  1, .e_len = 0 },
  { .name = "add_unknown2",    .a = A11, .b = B11, .e = E11, .a_exp =   0, .b_exp =  10, .e_exp =  MP_NUMBER_NAN, .a_len = 1, .b_len =  0, .e_len = 0 },

  { .name = "add_neg_add2",    .a = A12, .b = B12, .e = E12, .a_exp = 0, .b_exp = 2, .e_exp = 0, .a_len =  4, .b_len = -2, .e_len =  1 },
  { .name = "add_neg_add1",    .a = A13, .b = B13, .e = E13, .a_exp = 2, .b_exp = 0, .e_exp = 0, .a_len = -2, .b_len =  4, .e_len =  1 },
};

START_TEST(test_add)
{
  run_add_case(&add_cases[_i]);
}
END_TEST

Suite *MP_TEST_float_add_suite()
{
  Suite *s = suite_create("float_add");
  TCase *tc = tcase_create("float_add");

  constexpr int n = sizeof(add_cases)/sizeof(FloatAddCase);
  tcase_add_loop_test(tc, test_add, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
