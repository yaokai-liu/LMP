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
 * Filename: arith.c
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
  int32_t           offset;
} AddCase;

static void run_add_case(const AddCase *c)
{
  lmp_limb_t RES_limbs[(c->a_len > c->b_len ? c->a_len : c->b_len) + c->offset + 1] = {};

  const MP_INT A = { .alloc = 0, .size = c->a_len, .limbs = (lmp_limb_t *)c->a };
  MP_INT B   = { .alloc = 0, .size = c->b_len, .limbs = (lmp_limb_t *)c->b };
  MP_INT RES = { .alloc = 0, .size = 0,        .limbs = RES_limbs };

  MP_INT_abs_add_offset(&RES, &A, &B, c->offset, &STDAllocator);

  ck_assert_msg(RES.size == c->e_len,
                "%s: result size %d != %d", c->name, RES.size, c->e_len);
  for (int32_t i = 0; i < c->e_len; i++) {
    ck_assert_msg(RES.limbs[i] == c->e[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  RES.limbs[i], c->e[i]);
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
static const lmp_limb_t E3[] = {0, 0, 1};

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

static const AddCase add_cases[] = {
  {"basic3x3",          A1, B1, E1, 3, 3, 3, 0},
  {"basic1x1",          A2, B2, E2, 1, 1, 1, 0},
  {"carry",             A3, B3, E3, 2, 1, 3, 0},
  {"diff_len",          A4, B4, E4, 4, 2, 4, 0},
  {"offset1_overlap",   A5, B5, E5, 3, 2, 3, 1},
  {"offset3_gap",       A6, B6, E6, 2, 2, 5, 3},
  {"offset2_extend",    A7, B7, E7, 1, 2, 4, 2},
  {"offset1_carry",     A8, B8, E8, 2, 1, 3, 1},
  {"offset1_tail",      A9, B9, E9, 1, 2, 3, 1},
};

START_TEST(test_abs_sub_offset)
{
  run_add_case(&add_cases[_i]);
}
END_TEST

Suite *abs_add_offset_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("abs_add_offset");

  constexpr int n = sizeof(add_cases)/sizeof(AddCase);
  for (int i = 0; i < n; i++) {
    tcase_add_loop_test(tc, test_abs_sub_offset, 0, n);
  }

  suite_add_tcase(s, tc);
  return s;
}
