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
 * Filename: abs_add_limb.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"


typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *e;
  int32_t           a_len;
  int32_t           e_len;
  uint64_t          val;
} IntAddLimbCase;

static void run_add_limb_case(const IntAddLimbCase *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT DEST = { .prec = -(int32_t)(sizeof(dest_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = dest_limbs };

  MP_INT_add_limb(&DEST, &A, c->val, &STDAllocator);
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

static const lmp_limb_t AA1[] = { 1 };
static const lmp_limb_t AE1[] = { 2 };

static const lmp_limb_t AA2[] = { 1234 };
static const lmp_limb_t AE2[] = { 2234 };

static const lmp_limb_t AA3[] = { LMP_LIMB_MAX };
static const lmp_limb_t AE3[] = { 0, 1 };

static const lmp_limb_t AA4[] = { 2, 3 };
static const lmp_limb_t AE4[] = { 3, 3 };

static const lmp_limb_t AA5[] = { LMP_LIMB_MAX, 1 };
static const lmp_limb_t AE5[] = { 0, 2 };

static const lmp_limb_t AA6[] = { 0 };
static const lmp_limb_t AE6[] = { 12345 };

static const lmp_limb_t AA7[] = { 5, 6 };
static const lmp_limb_t AE7[] = { 5, 6 };

static const lmp_limb_t AA8[] = { LMP_LIMB_MAX,
                                  LMP_LIMB_MAX };
static const lmp_limb_t AE8[] = { 0, 0, 1 };

static const lmp_limb_t AA9[] = { 0, 0 };
static const lmp_limb_t AE9[] = { 7 };

static const lmp_limb_t AA10[] = { (1ULL << 63) };
static const lmp_limb_t AE10[] = { 0, 1 };

static const lmp_limb_t AA11[] = { LMP_LIMB_MAX,
                                   LMP_LIMB_MAX,
                                   LMP_LIMB_MAX };
static const lmp_limb_t AE11[] = { 0, 0, 0, 1 };

static const lmp_limb_t AA12[] = { 2234 };
static const lmp_limb_t AE12[] = { 1234 };

static const lmp_limb_t AA13[] = { 2234 };
static const lmp_limb_t AE13[] = { 0 };

static const lmp_limb_t AA14[] = {  234 };
static const lmp_limb_t AE14[] = { 1000 };

static const lmp_limb_t AA15[] = { 1234, 1 };
static const lmp_limb_t AE15[] = { LMP_LIMB_MAX - 1000 + 1 };

static const IntAddLimbCase abs_add_limb_cases[] = {
  { .name = "add1_simple",      .a = AA1,  .e = AE1,  .a_len =  1, .e_len =  1, .val = 1 },
  { .name = "add2_no_carry",    .a = AA2,  .e = AE2,  .a_len =  1, .e_len =  1, .val = 1000 },
  { .name = "add3_carry",       .a = AA3,  .e = AE3,  .a_len =  1, .e_len =  2, .val = 1 },
  { .name = "add4_multi",       .a = AA4,  .e = AE4,  .a_len =  2, .e_len =  2, .val = 1 },
  { .name = "add5_mid_carry",   .a = AA5,  .e = AE5,  .a_len =  2, .e_len =  2, .val = 1 },
  { .name = "add6_zero_size",   .a = AA6,  .e = AE6,  .a_len =  0, .e_len =  1, .val = 12345 },
  { .name = "add7_zero_val",    .a = AA7,  .e = AE7,  .a_len =  2, .e_len =  2, .val = 0 },
  { .name = "add8_all_ff",      .a = AA8,  .e = AE8,  .a_len =  2, .e_len =  3, .val = 1 },
  { .name = "add9_high_zero",   .a = AA9,  .e = AE9,  .a_len =  2, .e_len =  1, .val = 7 },
  { .name = "add10_half",       .a = AA10, .e = AE10, .a_len =  1, .e_len =  2, .val = (1ULL << 63) },
  { .name = "add11_triple",     .a = AA11, .e = AE11, .a_len =  3, .e_len =  4, .val = 1 },
  { .name = "add12_neg",        .a = AA12, .e = AE12, .a_len = -1, .e_len = -1, .val = 1000 },
  { .name = "add13_res_zero",   .a = AA13, .e = AE13, .a_len = -1, .e_len =  0, .val = 2234 },
  { .name = "add14_neg_pos",    .a = AA14, .e = AE14, .a_len = -1, .e_len =  1, .val = 1234 },
  { .name = "add15_neg_limbs",  .a = AA15, .e = AE15, .a_len = -2, .e_len = -1, .val = 2234 },
};

START_TEST(test_abs_add_limb)
{
  run_add_limb_case(&abs_add_limb_cases[_i]);
}
END_TEST

Suite *MP_TEST_int_abs_add_limb_suite()
{
  Suite *s = suite_create("abs_add_limb");
  TCase *tc = tcase_create("abs_add_limb");

  constexpr int n = sizeof(abs_add_limb_cases)/sizeof(IntAddLimbCase);
  tcase_add_loop_test(tc, test_abs_add_limb, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
