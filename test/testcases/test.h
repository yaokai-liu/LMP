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

#include "LMP/lmp.h"
#include "LMP/arith.h"
#include <check.h>
#include <inttypes.h>
#define lenof(str_literal) ((sizeof str_literal) - 1)
#define bitsof(w)          (sizeof(w) * CHAR_BIT)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))
#define abs(a)             ((a) > (0) ? (a) : -(a))

#define PRI_PREC        PRIi32
#define PRI_SIZE        PRIi32
#define PRI_LIMB        PRIu64
#define PRI_EXP         PRIi64
#define PRI_HALF_LIMB   PRIu32

Suite *MP_TEST_int_add_suite();
Suite *MP_TEST_int_sub_suite();
Suite *MP_TEST_int_left_shift_suite();
Suite *MP_TEST_int_right_shift_suite();
Suite *MP_TEST_int_abs_mul_limb_suite();
Suite *MP_TEST_int_abs_add_limb_suite();
Suite *MP_TEST_int_abs_sub_limb_suite();
Suite *MP_TEST_int_abs_mul_suite();
Suite *MP_TEST_int_misc_suite();
Suite *MP_TEST_int_cmp_suite();
Suite *MP_TEST_int_abs_divmod_suite();

Suite *abs_3limbs_inv_suite();

Suite *MP_TEST_float_cmp_suite();
Suite *MP_TEST_float_add_suite();
Suite *MP_TEST_float_sub_suite();
Suite *MP_TEST_float_mul_suite();
Suite *MP_TEST_float_divmod_suite();
Suite *MP_TEST_float_misc_suite();

Suite *MP_TEST_base_number_misc_suite();

Suite *parse_suite();
Suite *print_suite();

Suite *ieee754_suite();
Suite *c_int_suite();

#endif //LMP_TEST_H
