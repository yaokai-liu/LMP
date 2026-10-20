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
} MulU64Case;

static void run_mul_u64_case(const MulU64Case *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .alloc = 0, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT DEST = { .alloc = 0, .size = 0,        .limbs = dest_limbs };

  MP_INT_abs_mul_u64(&DEST, &A, c->val, &STDAllocator);
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

static const lmp_limb_t MA1[] = { 1 };
static const lmp_limb_t ME1[] = { 1 };

static const lmp_limb_t MA2[] = { 1234 };
static const lmp_limb_t ME2[] = { 1234000 };

static const lmp_limb_t MA3[] = { UINT64_MAX };
static const lmp_limb_t ME3[] = { UINT64_MAX - 1, 1 };

static const lmp_limb_t MA4[] = { 2, 3 };
static const lmp_limb_t ME4[] = { 4, 6 };

static const lmp_limb_t MA5[] = { (1ULL << 63), 1 };
static const lmp_limb_t ME5[] = { 0, 3 };

static const lmp_limb_t MA6[] = { 123, 456 };
static const lmp_limb_t ME6[] = { 0 };

static const lmp_limb_t MA7[] = { 0 };
static const lmp_limb_t ME7[] = { 0 };

static const lmp_limb_t MA8[] = { UINT64_MAX,
                                  UINT64_MAX };
static const lmp_limb_t ME8[] = { UINT64_MAX - 1,
                                  UINT64_MAX, 1 };

static const lmp_limb_t MA9[] = { 7 };
static const lmp_limb_t ME9[] = { 21 };

static const lmp_limb_t MA10[] = { 2 };
static const lmp_limb_t ME10[] = { 0, 1 };

static const lmp_limb_t MA11[] = { 2, 1 };
static const lmp_limb_t ME11[] = { UINT64_MAX - 1, 0, 1 };

static const MulU64Case abs_mul_u64_cases[] = {
  { "mul1_simple",      MA1,  ME1,  1, 1, 1 },
  { "mul2_no_carry",    MA2,  ME2,  1, 1, 1000 },
  { "mul3_carry",       MA3,  ME3,  1, 2, 2 },
  { "mul4_multi",       MA4,  ME4,  2, 2, 2 },
  { "mul5_multi_carry", MA5,  ME5,  2, 2, 2 },
  { "mul6_zero_val",    MA6,  ME6,  2, 0, 0 },
  { "mul7_zero_size",   MA7,  ME7,  0, 0, 12345 },
  { "mul8_all_ff",      MA8,  ME8,  2, 3, 2 },
  { "mul9_simple2",     MA9,  ME9,  1, 1, 3 },
  { "mul10_big_val",    MA10, ME10, 1, 2, (1ULL << 63) },
  { "mul11_carry_mid",  MA11, ME11, 2, 3, UINT64_MAX },
};

START_TEST(test_abs_mul_u64)
{
  run_mul_u64_case(&abs_mul_u64_cases[_i]);
}
END_TEST

Suite *abs_mul_u64_suite()
{
  Suite *s = suite_create("abs_mul_u64");
  TCase *tc = tcase_create("abs_mul_u64");

  constexpr int n = sizeof(abs_mul_u64_cases)/sizeof(MulU64Case);
  tcase_add_loop_test(tc, test_abs_mul_u64, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
