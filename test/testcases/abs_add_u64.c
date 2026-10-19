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
 * Filename: abs_mul_u64.c
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
} AddU64Case;

static void run_add_u64_case(const AddU64Case *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .alloc = 0, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT DEST = { .alloc = 0, .size = 0,        .limbs = dest_limbs };

  MP_INT_abs_add_u64(&DEST, &A, c->val, &STDAllocator);
  MP_INT_clamp(&DEST);

  ck_assert_msg(abs(DEST.size) == c->e_len,
                "%s: result size %d != %d", c->name, DEST.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(DEST.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)DEST.limbs[i],
                  (unsigned long long)c->e[i]);
  }
}

static const lmp_limb_t AA1[] = { 1 };
static const lmp_limb_t AE1[] = { 2 };

static const lmp_limb_t AA2[] = { 1234 };
static const lmp_limb_t AE2[] = { 2234 };

static const lmp_limb_t AA3[] = { UINT64_MAX };
static const lmp_limb_t AE3[] = { 0, 1 };

static const lmp_limb_t AA4[] = { 2, 3 };
static const lmp_limb_t AE4[] = { 3, 3 };

static const lmp_limb_t AA5[] = { UINT64_MAX, 1 };
static const lmp_limb_t AE5[] = { 0, 2 };

static const lmp_limb_t AA6[] = { 0 };
static const lmp_limb_t AE6[] = { 12345 };

static const lmp_limb_t AA7[] = { 5, 6 };
static const lmp_limb_t AE7[] = { 5, 6 };

static const lmp_limb_t AA8[] = { UINT64_MAX,
                                  UINT64_MAX };
static const lmp_limb_t AE8[] = { 0, 0, 1 };

static const lmp_limb_t AA9[] = { 0, 0 };
static const lmp_limb_t AE9[] = { 7 };

static const lmp_limb_t AA10[] = { (1ULL << 63) };
static const lmp_limb_t AE10[] = { 0, 1 };

static const lmp_limb_t AA11[] = { UINT64_MAX,
                                   UINT64_MAX,
                                   UINT64_MAX };
static const lmp_limb_t AE11[] = { 0, 0, 0, 1 };

static const AddU64Case abs_add_u64_cases[] = {
  { "add1_simple",      AA1,  AE1,  1, 1, 1 },
  { "add2_no_carry",    AA2,  AE2,  1, 1, 1000 },
  { "add3_carry",       AA3,  AE3,  1, 2, 1 },
  { "add4_multi",       AA4,  AE4,  2, 2, 1 },
  { "add5_mid_carry",   AA5,  AE5,  2, 2, 1 },
  { "add6_zero_size",   AA6,  AE6,  0, 1, 12345 },
  { "add7_zero_val",    AA7,  AE7,  2, 2, 0 },
  { "add8_all_ff",      AA8,  AE8,  2, 3, 1 },
  { "add9_high_zero",   AA9,  AE9,  2, 1, 7 },
  { "add10_half",       AA10, AE10, 1, 2, (1ULL << 63) },
  { "add11_triple",     AA11, AE11, 3, 4, 1 },
};

START_TEST(test_abs_add_u64)
{
  run_add_u64_case(&abs_add_u64_cases[_i]);
}
END_TEST

Suite *abs_add_u64_suite()
{
  Suite *s = suite_create("abs_add_u64");
  TCase *tc = tcase_create("abs_add_u64");

  constexpr int n = sizeof(abs_add_u64_cases)/sizeof(AddU64Case);
  tcase_add_loop_test(tc, test_abs_add_u64, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
