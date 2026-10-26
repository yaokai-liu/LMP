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
 * Filename: mp_int_misc.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"


START_TEST(test_MP_INT_1) {
  MP_INT num = {};
  MP_INT_init(&num, 0, &STDAllocator);
  ck_assert_uint_eq(num.alloc, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_realloc(&num, 131, &STDAllocator);
  ck_assert_uint_eq(num.alloc, 136);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_2) {
  MP_INT num = {};
  MP_INT_init(&num, 1, &STDAllocator);
  ck_assert_uint_eq(num.alloc, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_3) {
  MP_INT num = {};
  MP_INT_init(&num, MP_ALLOC_LEN, &STDAllocator);
  ck_assert_uint_eq(num.alloc, MP_ALLOC_LEN + MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_4) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {3, 4}};
  MP_INT num1 = {.alloc = 2, .size = 2, .limbs = limbs[0]};
  MP_INT num2 = {.alloc = 2, .size = 2, .limbs = limbs[1]};
  MP_INT_copy(&num2, &num1, &STDAllocator);
  ck_assert_uint_eq(num2.size, num1.size);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_INT_5) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_INT num = {.alloc = 2, .size = 2, .limbs = limbs[0]};
  MP_INT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_INT_6) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_INT num = {.alloc = 2, .size = 2, .limbs = limbs[0]};
  MP_INT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST


Suite *mp_int_misc_suite()
{
  Suite *s = suite_create("abs_sub_offset");
  TCase *tc = tcase_create("mp_type_all");

  tcase_add_test(tc, test_MP_INT_1);
  tcase_add_test(tc, test_MP_INT_2);
  tcase_add_test(tc, test_MP_INT_3);
  tcase_add_test(tc, test_MP_INT_4);
  tcase_add_test(tc, test_MP_INT_5);
  tcase_add_test(tc, test_MP_INT_6);

  suite_add_tcase(s, tc);
  return s;
}
