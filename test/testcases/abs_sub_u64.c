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
 * Filename: abs_sub_u64.c
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
} SubU64Case;

static void run_sub_u64_case(const SubU64Case *c)
{
  lmp_limb_t dest_limbs[64] = {};
  ck_assert((size_t)c->a_len + 2 <= sizeof(dest_limbs) / sizeof(dest_limbs[0]));

  MP_INT A    = { .alloc = -c->a_len, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT DEST = { .alloc = -(int32_t)(sizeof(dest_limbs) / LMP_LIMB_SIZE) , .size = 0, .limbs = dest_limbs };

  MP_INT_abs_sub_u64(&DEST, &A, c->val, &STDAllocator);
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

static const lmp_limb_t SA1[] = { 2 };
static const lmp_limb_t SE1[] = { 1 };

static const lmp_limb_t SA2[] = { 2234 };
static const lmp_limb_t SE2[] = { 1234 };

static const lmp_limb_t SA3[] = { 0, 1 };
static const lmp_limb_t SE3[] = { UINT64_MAX };

static const lmp_limb_t SA4[] = { 3, 3 };
static const lmp_limb_t SE4[] = { 2, 3 };

static const lmp_limb_t SA5[] = { UINT64_MAX, 1 };
static const lmp_limb_t SE5[] = { UINT64_MAX - 1, 1 };

static const lmp_limb_t SA6[] = { 1 };
static const lmp_limb_t SE6[] = { 0 };

static const lmp_limb_t SA7[] = { 5, 6 };
static const lmp_limb_t SE7[] = { 5, 6 };

static const lmp_limb_t SA8[] = { 0 };
static const lmp_limb_t SE8[] = { 0 };

static const lmp_limb_t SA9[] = { 0, 0, 1 };
static const lmp_limb_t SE9[] = { UINT64_MAX,
                                  UINT64_MAX };

static const lmp_limb_t SA10[] = { UINT64_MAX,
                                   UINT64_MAX,
                                   UINT64_MAX };
static const lmp_limb_t SE10[] = { UINT64_MAX - 1,
                                   UINT64_MAX,
                                   UINT64_MAX };

static const lmp_limb_t SA11[] = { (1ULL << 63) };
static const lmp_limb_t SE11[] = { 0 };

static const lmp_limb_t SA12[] = { 0, 5 };
static const lmp_limb_t SE12[] = { UINT64_MAX - 1, 4 };

static const SubU64Case abs_sub_u64_cases[] = {
  { "sub1_simple",        SA1,  SE1,  1, 1, 1 },
  { "sub2_no_borrow",     SA2,  SE2,  1, 1, 1000 },
  { "sub3_borrow",        SA3,  SE3,  2, 1, 1 },
  { "sub4_multi",         SA4,  SE4,  2, 2, 1 },
  { "sub5_hi_no_borrow",  SA5,  SE5,  2, 2, 1 },
  { "sub6_zero_result",   SA6,  SE6,  2, 0, 1 },
  { "sub7_zero_val",      SA7,  SE7,  2, 2, 0 },
  { "sub8_zero_size",     SA8,  SE8,  0, 0, 0 },
  { "sub9_borrow_chain",  SA9,  SE9,  3, 2, 1 },
  { "sub10_all_ff",       SA10, SE10, 3, 3, 1 },
  { "sub11_to_zero",      SA11, SE11, 1, 0, (1ULL << 63) },
  { "sub12_offset_borrow",SA12, SE12, 2, 2, 2 },
};


START_TEST(test_abs_sub_u64)
{
  run_sub_u64_case(&abs_sub_u64_cases[_i]);
}
END_TEST

Suite *abs_sub_u64_suite()
{
  Suite *s = suite_create("abs_sub_u64");
  TCase *tc = tcase_create("abs_sub_u64");

  constexpr int n = sizeof(abs_sub_u64_cases)/sizeof(SubU64Case);
  tcase_add_loop_test(tc, test_abs_sub_u64, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
