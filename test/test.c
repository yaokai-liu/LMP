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
 * Module Name: test
 * Filename: test.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "lmp.h"
#include <check.h>
#include "testcases/test.h"
#include <stdlib.h>

int main()
{
  SRunner *sr = srunner_create(nullptr);
  srunner_add_suite(sr, abs_add_offset_suite());
  srunner_add_suite(sr, abs_sub_offset_suite());
  srunner_add_suite(sr, left_shift_suite());
  srunner_add_suite(sr, right_shift_suite());
  srunner_add_suite(sr, abs_mul_u64_suite());
  srunner_add_suite(sr, abs_add_u64_suite());
  srunner_add_suite(sr, abs_sub_u64_suite());
  srunner_add_suite(sr, abs_mul_suite());
  srunner_add_suite(sr, mp_int_misc_suite());
  srunner_add_suite(sr, cmp_suite());
  srunner_add_suite(sr, abs_3limbs_inv_suite());

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  const int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
