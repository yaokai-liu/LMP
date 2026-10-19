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
 * Filename: right_shift.c
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
  int32_t           shift;
} ShiftCase;


static void run_right_shift_case(const ShiftCase *c)
{
  lmp_limb_t buf[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(buf) / sizeof(buf[0]));
  memcpy(buf, c->a, LMP_LIMB_SIZE * (size_t)c->a_len);

  MP_INT A = { .alloc = 0, .size = c->a_len, .limbs = buf };

  MP_INT_right_shift(&A, c->shift, &STDAllocator);
  MP_INT_clamp(&A);

  ck_assert_msg(abs(A.size) == c->e_len,
                "%s: result size %d != %d", c->name, A.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(A.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)A.limbs[i],
                  (unsigned long long)c->e[i]);
  }
}

static const lmp_limb_t SRA1[] = { 2 };
static const lmp_limb_t SRE1[] = { 1 };

static const lmp_limb_t SRA2[] = { 0, 1 };
static const lmp_limb_t SRE2[] = { (1ULL << 63) };

static const lmp_limb_t SRA3[] = { 3, 1 };
static const lmp_limb_t SRE3[] = { 1 };


static const lmp_limb_t SRA4[] = { 0, 2 };
static const lmp_limb_t SRE4[] = { 2 };

static const lmp_limb_t SRA5[] = { 1, UINT64_MAX };
static const lmp_limb_t SRE5[] = { (1ULL << 63),
                                   (1ULL << 63) - 1 };

static const lmp_limb_t SRA6[] = { 5 };
static const lmp_limb_t SRE6[] = { 0 };

static const ShiftCase right_cases[] = {
  { "right1_bit",        SRA1, SRE1, 1, 1, 1 },
  { "right2_carry",      SRA2, SRE2, 2, 1, 1 },
  { "right3_limb_align", SRA3, SRE3, 2, 1, 64 },
  { "right4_limb_align2",SRA4, SRE4, 2, 1, 64 },
  { "right5_cross",      SRA5, SRE5, 2, 2, 1 },
  { "right6_all_out",    SRA6, SRE6, 1, 0, 100 },
};

START_TEST(test_right_shift)
{
  run_right_shift_case(&right_cases[_i]);
}
END_TEST

Suite *right_shift_suite()
{
  Suite *s = suite_create("right_shift");
  TCase *tc = tcase_create("right_shift");

  constexpr int n = sizeof(right_cases)/sizeof(ShiftCase);
  tcase_add_loop_test(tc, test_right_shift, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
