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
 * Filename: abs_3limbs_inv.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include <stdlib.h>

#include "test.h"
#include "allocator.h"

typedef struct {
  const char       *name;
  lmp_double_limb_t divisor;
  lmp_limb_t        expect;
} Inv3Case;

static void run_add_case(const Inv3Case *c)
{
  const lmp_limb_t res = MP_INT_abs_3limbs_inv(c->divisor);
  ck_assert_msg(res == c->expect, "%s: 3limbs inverse %llu != %llu", c->name, res, c->expect);
}


static const Inv3Case inv_cases[] = {
  {
    "inv3_one_limb_1",
    (((lmp_double_limb_t)10512282028032306483ULL) << LMP_LIMB_BITS | 2678291321200806564ULL),
    (((lmp_double_limb_t)0ULL) << LMP_LIMB_BITS | 13923236679616247875ULL)

  },
  {
    "inv3_one_limb_2",
    (((lmp_double_limb_t)10512282028032306483ULL) << LMP_LIMB_BITS | 2678291321200806564ULL),
    (((lmp_double_limb_t)0ULL) << LMP_LIMB_BITS | 13923236679616247875ULL)

  },
  {
    "inv3_one_limb_3",
    (((lmp_double_limb_t)11373414061963692226ULL) << LMP_LIMB_BITS | 5637707374651487514ULL),
    (((lmp_double_limb_t)0ULL) << LMP_LIMB_BITS | 11472360697033891273ULL)

  },
  {
    "inv3_one_limb_4",
    (((lmp_double_limb_t)14643696743029893023ULL) << LMP_LIMB_BITS | 14398196988406505576ULL),
    (((lmp_double_limb_t)0ULL) << LMP_LIMB_BITS | 4790719313594345477ULL)
  },
};

START_TEST(test_abs_3limbs_inv)
{
  run_add_case(&inv_cases[_i]);
}
END_TEST

Suite *abs_3limbs_inv_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("abs_3limbs_inv");

  constexpr int n = sizeof(inv_cases)/sizeof(Inv3Case);
  tcase_add_loop_test(tc, test_abs_3limbs_inv, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
