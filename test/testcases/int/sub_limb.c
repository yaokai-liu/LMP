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
 * Filename: abs_sub_limb.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <allocator.h>

extern void MP_INT_abs_sub_limb(MP_INT *dest, const MP_INT *op, lmp_limb_t val, const Allocator *allocator);


typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *e;
  int32_t           a_len;
  int32_t           e_len;
  uint64_t          val;
} IntSubLimbCase;

static void run_sub_limb_case(const IntSubLimbCase *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .prec = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT DEST = { .prec = -(int32_t)(sizeof(dest_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = dest_limbs };

  MP_INT_sub_limb(&DEST, &A, c->val, &STDAllocator);

  ck_assert_msg(DEST.size == c->e_len,
                "%s: result size %"PRI_SIZE" != %"PRI_SIZE"\n", c->name, DEST.size, c->e_len);
  for (int32_t i = 0; i < abs(c->e_len); i++) {
    ck_assert_msg(DEST.limbs[i] == c->e[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != %"PRI_LIMB"\n", c->name, i,
                  (lmp_limb_t)DEST.limbs[i],
                  (lmp_limb_t)c->e[i]);
  }
}

static const lmp_limb_t SA1[] = { 2 };
static const lmp_limb_t SE1[] = { 1 };

static const lmp_limb_t SA2[] = { 2234 };
static const lmp_limb_t SE2[] = { 1234 };

static const lmp_limb_t SA3[] = { 0, 1 };
static const lmp_limb_t SE3[] = { LMP_LIMB_MAX };

static const lmp_limb_t SA4[] = { 3, 3 };
static const lmp_limb_t SE4[] = { 2, 3 };

static const lmp_limb_t SA5[] = { LMP_LIMB_MAX, 1 };
static const lmp_limb_t SE5[] = { LMP_LIMB_MAX - 1, 1 };

static const lmp_limb_t SA6[] = { 1 };
static const lmp_limb_t SE6[] = { 0 };

static const lmp_limb_t SA7[] = { 5, 6 };
static const lmp_limb_t SE7[] = { 5, 6 };

static const lmp_limb_t SA8[] = { 0 };
static const lmp_limb_t SE8[] = { 0 };

static const lmp_limb_t SA9[] = { 0, 0, 1 };
static const lmp_limb_t SE9[] = { LMP_LIMB_MAX,
                                  LMP_LIMB_MAX };

static const lmp_limb_t SA10[] = { LMP_LIMB_MAX,
                                   LMP_LIMB_MAX,
                                   LMP_LIMB_MAX };
static const lmp_limb_t SE10[] = { LMP_LIMB_MAX - 1,
                                   LMP_LIMB_MAX,
                                   LMP_LIMB_MAX };

static const lmp_limb_t SA11[] = { (1ULL << 63) };
static const lmp_limb_t SE11[] = { 0 };

static const lmp_limb_t SA12[] = { 0, 5 };
static const lmp_limb_t SE12[] = { LMP_LIMB_MAX - 1, 4 };

static const lmp_limb_t SA13[] = { 1000 };
static const lmp_limb_t SE13[] = { 234 };

static const lmp_limb_t SA14[] = { 0 };
static const lmp_limb_t SE14[] = { 234 };

static const IntSubLimbCase abs_sub_limb_cases[] = {
  { .name = "sub1_simple",           .a = SA1,  .e = SE1,  .a_len = 1, .e_len =  1, .val = 1 },
  { .name = "sub2_no_borrow",        .a = SA2,  .e = SE2,  .a_len = 1, .e_len =  1, .val = 1000 },
  { .name = "sub3_borrow",           .a = SA3,  .e = SE3,  .a_len = 2, .e_len =  1, .val = 1 },
  { .name = "sub4_multi",            .a = SA4,  .e = SE4,  .a_len = 2, .e_len =  2, .val = 1 },
  { .name = "sub5_hi_no_borrow",     .a = SA5,  .e = SE5,  .a_len = 2, .e_len =  2, .val = 1 },
  { .name = "sub6_zero_result",      .a = SA6,  .e = SE6,  .a_len = 2, .e_len =  0, .val = 1 },
  { .name = "sub7_zero_val",         .a = SA7,  .e = SE7,  .a_len = 2, .e_len =  2, .val = 0 },
  { .name = "sub8_zero_size",        .a = SA8,  .e = SE8,  .a_len = 0, .e_len =  0, .val = 0 },
  { .name = "sub9_borrow_chain",     .a = SA9,  .e = SE9,  .a_len = 3, .e_len =  2, .val = 1 },
  { .name = "sub10_all_ff",          .a = SA10, .e = SE10, .a_len = 3, .e_len =  3, .val = 1 },
  { .name = "sub11_to_zero",         .a = SA11, .e = SE11, .a_len = 1, .e_len =  0, .val = (1ULL << 63) },
  { .name = "sub12_offset_borrow",   .a = SA12, .e = SE12, .a_len = 2, .e_len =  2, .val = 2 },
  { .name = "sub13_one_limb_borrow", .a = SA13, .e = SE13, .a_len = 1, .e_len = -1, .val = 1234 },
  { .name = "sub13_zero_sub",        .a = SA14, .e = SE14, .a_len = 0, .e_len = -1, .val = 234 },
};


START_TEST(test_abs_sub_limb)
{
  run_sub_limb_case(&abs_sub_limb_cases[_i]);
}
END_TEST

START_TEST(test_MP_INT_abs_sub_limb_1) {
  lmp_limb_t limbs[2] = {0};
  MP_INT num1 = {.prec = -2, .size = 0, .limbs = limbs};
  MP_INT_abs_sub_limb(&num1, &num1, 1234, &STDAllocator);
  ck_assert(num1.size == -1);
  ck_assert(limbs[0] == 1234);
}
END_TEST

START_TEST(test_MP_INT_abs_sub_limb_2) {
  lmp_limb_t limbs[2] = {1000, 0};
  MP_INT num1 = {.prec = -2, .size = 1, .limbs = limbs};
  MP_INT_abs_sub_limb(&num1, &num1, 1234, &STDAllocator);
  ck_assert(num1.size == -1);
  ck_assert(limbs[0] == 234);
}
END_TEST

START_TEST(test_MP_INT_abs_sub_limb_3) {
  lmp_limb_t limbs[2] = {2234, 0};
  MP_INT num1 = {.prec = -2, .size = 1, .limbs = limbs};
  MP_INT_abs_sub_limb(&num1, &num1, 1234, &STDAllocator);
  ck_assert(num1.size == 1);
  ck_assert(limbs[0] == 1000);
  ck_assert(limbs[1] == 0);
}
END_TEST

START_TEST(test_MP_INT_abs_sub_limb_4) {
  lmp_limb_t limbs[2] = {1234, 0};
  MP_INT num1 = {.prec = -2, .size = 1, .limbs = limbs};
  MP_INT_abs_sub_limb(&num1, &num1, 1234, &STDAllocator);
  ck_assert(num1.size == 0);
}
END_TEST

Suite *MP_TEST_int_abs_sub_limb_suite()
{
  Suite *s = suite_create("abs_sub_limb");
  TCase *tc = tcase_create("abs_sub_limb");

  constexpr int n = sizeof(abs_sub_limb_cases)/sizeof(IntSubLimbCase);
  tcase_add_loop_test(tc, test_abs_sub_limb, 0, n);
  tcase_add_test(tc, test_MP_INT_abs_sub_limb_1);
  tcase_add_test(tc, test_MP_INT_abs_sub_limb_2);
  tcase_add_test(tc, test_MP_INT_abs_sub_limb_3);
  tcase_add_test(tc, test_MP_INT_abs_sub_limb_4);

  suite_add_tcase(s, tc);
  return s;
}
