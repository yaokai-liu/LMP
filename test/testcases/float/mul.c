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

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE"\n", c->name, RES.size, c->e_len);
  ck_assert_msg(RES.exp == c->e_exp,
                "%s: result exp %"PRI_EXP" != %"PRI_EXP"\n", c->name, RES.exp, c->e_exp);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB"\n", c->name, i, RES.limbs[i], c->e[i]);
  }
}

static const lmp_limb_t MA1[] = { 1 };
static const lmp_limb_t MB1[] = { 1 };
static const lmp_limb_t ME1[] = { 1 };

static const lmp_limb_t MA2[] = { 1234 };
static const lmp_limb_t MB2[] = { 5678 };
static const lmp_limb_t ME2[] = { 7006652 };

static const lmp_limb_t MA3[] = { LMP_LIMB_MAX };
static const lmp_limb_t MB3[] = { 2 };
static const lmp_limb_t ME3[] = { LMP_LIMB_MAX - 1, 1 };

static const lmp_limb_t MA4[] = { (1ULL << 63) };
static const lmp_limb_t MB4[] = { 2 };
static const lmp_limb_t ME4[] = { 0, 1 };

static const lmp_limb_t MA5[] = { 2, 3 };
static const lmp_limb_t MB5[] = { 2 };
static const lmp_limb_t ME5[] = { 4, 6 };

static const lmp_limb_t MA6[] = { LMP_LIMB_MAX, 1 };
static const lmp_limb_t MB6[] = { 2 };
static const lmp_limb_t ME6[] = { LMP_LIMB_MAX - 1, 3 };

static const lmp_limb_t MA7[] = { LMP_LIMB_MAX };
static const lmp_limb_t MB7[] = { LMP_LIMB_MAX };
static const lmp_limb_t ME7[] = { 1, LMP_LIMB_MAX - 1 };

static const lmp_limb_t MA8[] = { 2, 1 };
static const lmp_limb_t MB8[] = { 3, 1 };
static const lmp_limb_t ME8[] = { 6, 5, 1 };

static const lmp_limb_t MA9[] = { 0 };
static const lmp_limb_t MB9[] = { 123, 456 };
static const lmp_limb_t ME9[] = { 0 };

static const lmp_limb_t MA10[] = { 123, 456 };
static const lmp_limb_t MB10[] = { 0 };
static const lmp_limb_t ME10[] = { 0 };

static const lmp_limb_t MA11[] = { LMP_LIMB_MAX, LMP_LIMB_MAX };
static const lmp_limb_t MB11[] = { LMP_LIMB_MAX, LMP_LIMB_MAX };
static const lmp_limb_t ME11[] = { 1, 0, LMP_LIMB_MAX - 1, LMP_LIMB_MAX };

static const lmp_limb_t MA12[] = { 0 };
static const lmp_limb_t MB12[] = { 0 };
static const lmp_limb_t ME12[] = { 0 };

static const lmp_limb_t MA13[] = { 1 };
static const lmp_limb_t MB13[] = { 2 };
static const lmp_limb_t ME13[] = { 0 };

static const FloatMulCase abs_mul_cases[] = {
  { .name = "mul1_simple",      .a = MA1,  .b = MB1,  .e = ME1,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 1 },
  { .name = "mul2_small",       .a = MA2,  .b = MB2,  .e = ME2,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 1 },
  { .name = "mul3_carry",       .a = MA3,  .b = MB3,  .e = ME3,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 2 },
  { .name = "mul4_half_carry",  .a = MA4,  .b = MB4,  .e = ME4,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 2 },
  { .name = "mul5_multi1",      .a = MA5,  .b = MB5,  .e = ME5,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 2, .b_len = 1, .e_len = 2 },
  { .name = "mul6_multi_carry", .a = MA6,  .b = MB6,  .e = ME6,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 2, .b_len = 1, .e_len = 2 },
  { .name = "mul7_ff_ff",       .a = MA7,  .b = MB7,  .e = ME7,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 1, .b_len = 1, .e_len = 2 },
  { .name = "mul8_multi_multi", .a = MA8,  .b = MB8,  .e = ME8,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 2, .b_len = 2, .e_len = 3 },
  { .name = "mul9_zero_a",      .a = MA9,  .b = MB9,  .e = ME9,  .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 0, .b_len = 2, .e_len = 0 },
  { .name = "mul10_zero_b",     .a = MA10, .b = MB10, .e = ME10, .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 2, .b_len = 0, .e_len = 0 },
  { .name = "mul11_chain",      .a = MA11, .b = MB11, .e = ME11, .a_exp = 0, .b_exp = 0, .e_exp = 0, .a_len = 2, .b_len = 2, .e_len = 4 },

  { .name = "mul_special1",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_NAN, .b_exp = MP_NUMBER_ZERO, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special2",    .a = MA12, .b = MB12, .e = ME12, .a_exp = MP_NUMBER_ZERO, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special3",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_NAN, .b_exp = -MP_NUMBER_INF, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special4",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_NAN, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special5",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_NAN, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special6",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special7",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special8",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INF, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special9",    .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INF, .b_exp = MP_NUMBER_ZERO, .e_exp = MP_NUMBER_ZERO, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special10",   .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_INF, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special11",   .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_INS, .e_exp =  MP_NUMBER_INS, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special12",   .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INS, .b_exp =  MP_NUMBER_NAN, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len = 0, .e_len = 0 },
  { .name = "mul_special13",   .a = MA12, .b = MB12, .e = ME12, .a_exp =  MP_NUMBER_INS, .b_exp = MP_NUMBER_ZERO, .e_exp = MP_NUMBER_ZERO, .a_len = 0, .b_len = 0, .e_len = 0 },

  { .name = "mul_special14",   .a = MA13, .b = MB13, .e = ME13, .a_exp = -MP_NUMBER_INS, .b_exp = 0, .e_exp = -MP_NUMBER_INS, .a_len = 0, .b_len =  1, .e_len = 0 },
  { .name = "mul_special15",   .a = MA13, .b = MB13, .e = ME13, .a_exp = -MP_NUMBER_INS, .b_exp = 0, .e_exp =  MP_NUMBER_INS, .a_len = 0, .b_len = -1, .e_len = 0 },
  { .name = "mul_special16",   .a = MA13, .b = MB13, .e = ME13, .a_exp = -MP_NUMBER_INF, .b_exp = 0, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len = -1, .e_len = 0 },
  { .name = "mul_special17",   .a = MA13, .b = MB13, .e = ME13, .a_exp =  MP_NUMBER_INF, .b_exp = 0, .e_exp =  MP_NUMBER_INF, .a_len = 0, .b_len =  1, .e_len = 0 },

  { .name = "mul_unknown1",   .a = MA13, .b = MB13, .e = ME13, .a_exp = -10, .b_exp =   0, .e_exp =  MP_NUMBER_NAN, .a_len = 0, .b_len =  1, .e_len = 0 },
  { .name = "mul_unknown2",   .a = MA13, .b = MB13, .e = ME13, .a_exp =   0, .b_exp =  10, .e_exp =  MP_NUMBER_NAN, .a_len = 1, .b_len =  0, .e_len = 0 },
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
