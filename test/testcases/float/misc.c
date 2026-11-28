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
 * Module Name: test/testcases/float
 * Filename: misc.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-02
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <allocator.h>

#define MP_FLOAT_negative(val)                do { (val)->size = -(val)->size; } while(0)

START_TEST(test_MP_FLOAT_1) {
  MP_FLOAT num = {};
  MP_FLOAT_init(&num, 0, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_FLOAT_realloc(&num, 131, &STDAllocator);
  ck_assert_uint_eq(num.prec, 136);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_FLOAT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_FLOAT_2) {
  MP_FLOAT num = {};
  MP_FLOAT_init(&num, 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_FLOAT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_FLOAT_3) {
  MP_FLOAT num = {};
  MP_FLOAT_init(&num, 0, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_FLOAT_clear(&num, &STDAllocator);
  MP_FLOAT_init(&num, MP_ALLOC_LEN - 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_FLOAT_clear(&num, &STDAllocator);
  MP_FLOAT_init(&num, MP_ALLOC_LEN, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_FLOAT_clear(&num, &STDAllocator);
  MP_FLOAT_init(&num, MP_ALLOC_LEN + 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, 2 * MP_ALLOC_LEN);
  MP_FLOAT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_FLOAT_4) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {3, 4}};
  MP_FLOAT num1 = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_FLOAT num2 = {.prec = 2, .size = 2, .limbs = limbs[1]};
  MP_FLOAT_copy(&num2, &num1, &STDAllocator);
  ck_assert_uint_eq(num2.size, num1.size);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_FLOAT_5) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_FLOAT num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_FLOAT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_FLOAT_6) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_FLOAT num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_FLOAT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_FLOAT_7) {
  lmp_limb_t limbs[2] = {1, 2};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = -1};
  MP_FLOAT_truncate(&num, 1);
  ck_assert_int_eq(num.size, 1);
  ck_assert_uint_eq(limbs[0], 2);
  ck_assert_uint_eq(limbs[1], 0);
}
END_TEST

START_TEST(test_MP_FLOAT_8) {
  lmp_limb_t limbs[2] = {0, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = -1};
  MP_FLOAT_from_ieee754_float(&num, NAN, &STDAllocator);
  ck_assert_int_eq(num.size, 0);
  ck_assert_int_eq(num.exp, MP_NUMBER_NAN);
  MP_FLOAT_clear(&num, &STDAllocator);
  MP_FLOAT_from_ieee754_float(&num, -INFINITY, &STDAllocator);
  ck_assert_int_eq(num.size, 0);
  ck_assert_int_eq(num.exp, MP_NUMBER_NEG_INF);
  MP_FLOAT_clear(&num, &STDAllocator);
  MP_FLOAT_from_ieee754_float(&num, 0.25, &STDAllocator);
  ck_assert_int_eq(num.size, 1);
  ck_assert_int_eq(num.exp, -1);
  ck_assert_uint_eq(limbs[0], (1ULL << (62)));
  MP_FLOAT_clear(&num, &STDAllocator);
}
END_TEST

Suite *MP_TEST_float_misc_suite()
{
  Suite *s = suite_create("float_misc");
  TCase *tc = tcase_create("float_misc");

  tcase_add_test(tc, test_MP_FLOAT_1);
  tcase_add_test(tc, test_MP_FLOAT_2);
  tcase_add_test(tc, test_MP_FLOAT_3);
  tcase_add_test(tc, test_MP_FLOAT_4);
  tcase_add_test(tc, test_MP_FLOAT_5);
  tcase_add_test(tc, test_MP_FLOAT_6);
  tcase_add_test(tc, test_MP_FLOAT_7);
  tcase_add_test(tc, test_MP_FLOAT_8);

  suite_add_tcase(s, tc);
  return s;
}
