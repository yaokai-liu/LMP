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
 * Filename: test.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_TEST_H
#define LMP_TEST_H

#include "lmp.h"
#include "arith.h"
#include "parse.h"
#include "print.h"
#include <check.h>

Suite *abs_add_offset_suite();
Suite *abs_sub_offset_suite();
Suite *left_shift_suite();
Suite *right_shift_suite();
Suite *abs_mul_u64_suite();
Suite *abs_add_u64_suite();
Suite *abs_sub_u64_suite();
Suite *abs_mul_suite();
Suite *mp_int_misc_suite();
Suite *cmp_suite();
Suite *abs_3limbs_inv_suite();
Suite *abs_divmod_suite();
#endif //LMP_TEST_H
