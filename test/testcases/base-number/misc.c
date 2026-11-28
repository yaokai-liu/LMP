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
 * Module Name: test/testcases/base-number
 * Filename: misc.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-02
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <allocator.h>

#define MP_BASE_NUMBER_negative(val)                do { (val)->size = -(val)->size; } while(0)

START_TEST(test_MP_BASE_NUMBER_1) {
  MP_BASE_NUMBER num = {};
  MP_BASE_NUMBER_init(&num, 0, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_BASE_NUMBER_realloc(&num, 131, &STDAllocator);
  ck_assert_uint_eq(num.prec, 136);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_BASE_NUMBER_2) {
  MP_BASE_NUMBER num = {};
  MP_BASE_NUMBER_init(&num, 1, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_BASE_NUMBER_3) {
  MP_BASE_NUMBER num = {};
  MP_BASE_NUMBER_init(&num, 0, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
  MP_BASE_NUMBER_init(&num, MP_ALLOC_LEN - 1, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
  MP_BASE_NUMBER_init(&num, MP_ALLOC_LEN, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
  MP_BASE_NUMBER_init(&num, MP_ALLOC_LEN + 1, 10, &STDAllocator);
  ck_assert_uint_eq(num.prec, 2 * MP_ALLOC_LEN);
  MP_BASE_NUMBER_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_BASE_NUMBER_4) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {3, 4}};
  MP_BASE_NUMBER num1 = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_BASE_NUMBER num2 = {.prec = 2, .size = 2, .limbs = limbs[1]};
  MP_BASE_NUMBER_copy(&num2, &num1, &STDAllocator);
  ck_assert_uint_eq(num2.size, num1.size);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_BASE_NUMBER_5) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_BASE_NUMBER num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_BASE_NUMBER_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_BASE_NUMBER_6) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_BASE_NUMBER num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_BASE_NUMBER_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_BASE_INT_7) {
  lmp_limb_t limbs[2] = {1, 2};
  MP_BASE_NUMBER num1 = {.prec = -2, .size = 2, .limbs = limbs};
  MP_BASE_NUMBER_init(&num1, 0, 10, &STDAllocator);
  const lmp_double_limb_t val1 = MP_BASE_INT_get_double_flat_limbs(&num1, 0);
  ck_assert(val1 == ((lmp_double_limb_t)2 * DEC_LIMB_VAL) + 1);
  const lmp_double_limb_t val2 = MP_BASE_INT_get_double_flat_limbs(&num1, 1);
  ck_assert(val2 == (lmp_double_limb_t)2);
  const lmp_double_limb_t val3 = MP_BASE_INT_get_double_flat_limbs(&num1, -1);
  ck_assert(val3 == ((lmp_double_limb_t)1 * DEC_LIMB_VAL));
}
END_TEST

Suite *MP_TEST_base_number_misc_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("mp_type_all");

  tcase_add_test(tc, test_MP_BASE_NUMBER_1);
  tcase_add_test(tc, test_MP_BASE_NUMBER_2);
  tcase_add_test(tc, test_MP_BASE_NUMBER_3);
  tcase_add_test(tc, test_MP_BASE_NUMBER_4);
  tcase_add_test(tc, test_MP_BASE_NUMBER_5);
  tcase_add_test(tc, test_MP_BASE_NUMBER_6);
  tcase_add_test(tc, test_MP_BASE_INT_7);

  suite_add_tcase(s, tc);
  return s;
}
