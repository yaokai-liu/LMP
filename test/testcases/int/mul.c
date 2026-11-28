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
 * Filename: mul.c
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
} IntMulCase;

static void run_mul_case(const IntMulCase *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)abs(c->a_len) + (size_t)abs(c->b_len) <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B    = { .prec = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT DEST = { .prec = -(int32_t)(sizeof(dest_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = dest_limbs };

  MP_INT_mul(&DEST, &A, &B, &STDAllocator);
  MP_INT_clamp(&DEST);

  ck_assert_msg(DEST.size == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE"\n", c->name, DEST.size, c->e_len);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(DEST.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB"\n", c->name, i,
                  (lmp_limb_t)DEST.limbs[i],
                  (lmp_limb_t)c->e[i]);
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

static const IntMulCase abs_mul_cases[] = {
  { .name = "mul1_simple",     .a = MA1,  .b = MB1,  .e = ME1,  .a_len =  1, .b_len =  1, .e_len =  1 },
  { .name = "mul2_small",      .a = MA2,  .b = MB2,  .e = ME2,  .a_len = -1, .b_len =  1, .e_len = -1 },
  { .name = "mul3_carry",      .a = MA3,  .b = MB3,  .e = ME3,  .a_len =  1, .b_len =  1, .e_len =  2 },
  { .name = "mul4_half_carry", .a = MA4,  .b = MB4,  .e = ME4,  .a_len = -1, .b_len = -1, .e_len =  2 },
  { .name = "mul5_multi1",     .a = MA5,  .b = MB5,  .e = ME5,  .a_len =  2, .b_len =  1, .e_len =  2 },
  { .name = "mul6_multi_carry",.a = MA6,  .b = MB6,  .e = ME6,  .a_len =  2, .b_len = -1, .e_len = -2 },
  { .name = "mul7_ff_ff",      .a = MA7,  .b = MB7,  .e = ME7,  .a_len =  1, .b_len =  1, .e_len =  2 },
  { .name = "mul8_multi_multi",.a = MA8,  .b = MB8,  .e = ME8,  .a_len =  2, .b_len =  2, .e_len =  3 },
  { .name = "mul9_zero_a",     .a = MA9,  .b = MB9,  .e = ME9,  .a_len =  0, .b_len =  2, .e_len =  0 },
  { .name = "mul10_zero_b",    .a = MA10, .b = MB10, .e = ME10, .a_len =  2, .b_len =  0, .e_len =  0 },
  { .name = "mul11_chain",     .a = MA11, .b = MB11, .e = ME11, .a_len =  2, .b_len =  2, .e_len =  4 },
};


START_TEST(test_abs_mul)
{
  run_mul_case(&abs_mul_cases[_i]);
}
END_TEST

Suite *MP_TEST_int_abs_mul_suite()
{
  Suite *s = suite_create("abs_mul");
  TCase *tc = tcase_create("abs_mul");

  constexpr int n1 = sizeof(abs_mul_cases)/sizeof(IntMulCase);
  tcase_add_loop_test(tc, test_abs_mul, 0, n1);

  suite_add_tcase(s, tc);
  return s;
}
