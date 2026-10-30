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
 * Filename: abs_mul.c
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
  int32_t           a_len;
  int32_t           b_len;
  int32_t           e_len;
} MulCase;

static void run_mul_case(const MulCase *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + (size_t)c->b_len <=
            sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .alloc = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B    = { .alloc = -c->b_len, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT DEST = { .alloc = -(int32_t)(sizeof(dest_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = dest_limbs };

  MP_INT_abs_mul(&DEST, &A, &B, &STDAllocator);
  MP_INT_clamp(&DEST);

  ck_assert_msg(abs(DEST.size) == c->e_len,
                "%s: result size %d != %d", c->name, DEST.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(DEST.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
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

static const MulCase abs_mul_cases[] = {
  { "mul1_simple",     MA1,  MB1,  ME1,  1, 1, 1 },
  { "mul2_small",      MA2,  MB2,  ME2,  1, 1, 1 },
  { "mul3_carry",      MA3,  MB3,  ME3,  1, 1, 2 },
  { "mul4_half_carry", MA4,  MB4,  ME4,  1, 1, 2 },
  { "mul5_multi1",     MA5,  MB5,  ME5,  2, 1, 2 },
  { "mul6_multi_carry",MA6,  MB6,  ME6,  2, 1, 2 },
  { "mul7_ff_ff",      MA7,  MB7,  ME7,  1, 1, 2 },
  { "mul8_multi_multi",MA8,  MB8,  ME8,  2, 2, 3 },
  { "mul9_zero_a",     MA9,  MB9,  ME9,  0, 2, 0 },
  { "mul10_zero_b",    MA10, MB10, ME10, 2, 0, 0 },
  { "mul11_chain",     MA11, MB11, ME11, 2, 2, 4 },
};


START_TEST(test_abs_mul)
{
  run_mul_case(&abs_mul_cases[_i]);
}
END_TEST

Suite *abs_mul_suite()
{
  Suite *s = suite_create("abs_mul");
  TCase *tc = tcase_create("abs_mul");

  constexpr int n = sizeof(abs_mul_cases)/sizeof(MulCase);
  tcase_add_loop_test(tc, test_abs_mul, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
