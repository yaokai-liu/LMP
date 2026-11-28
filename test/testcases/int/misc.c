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
 * Module Name: test/testcases/int
 * Filename: misc.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <allocator.h>

#define MP_INT_negative(val)                do { (val)->size = -(val)->size; } while(0)

extern bool MP_INT_carry_check(const MP_INT *val);

START_TEST(test_MP_INT_1) {
  MP_INT num = {};
  MP_INT_init(&num, 0, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_realloc(&num, 131, &STDAllocator);
  ck_assert_uint_eq(num.prec, 136);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_2) {
  MP_INT num = {};
  MP_INT_init(&num, 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_3) {
  MP_INT num = {};
  MP_INT_init(&num, 0, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  ck_assert_uint_eq(num.size, 0);
  ck_assert_ptr_ne(num.limbs, nullptr);
  MP_INT_clear(&num, &STDAllocator);
  MP_INT_init(&num, MP_ALLOC_LEN - 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_INT_clear(&num, &STDAllocator);
  MP_INT_init(&num, MP_ALLOC_LEN, &STDAllocator);
  ck_assert_uint_eq(num.prec, MP_ALLOC_LEN);
  MP_INT_clear(&num, &STDAllocator);
  MP_INT_init(&num, MP_ALLOC_LEN + 1, &STDAllocator);
  ck_assert_uint_eq(num.prec, 2 * MP_ALLOC_LEN);
  MP_INT_clear(&num, &STDAllocator);
}
END_TEST

START_TEST(test_MP_INT_4) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {3, 4}};
  MP_INT num1 = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_INT num2 = {.prec = 2, .size = 2, .limbs = limbs[1]};
  MP_INT_copy(&num2, &num1, &STDAllocator);
  ck_assert_uint_eq(num2.size, num1.size);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_INT_5) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_INT num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_INT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_INT_6) {
  lmp_limb_t limbs[2][2] = {{1, 2}, {1, 2}};
  MP_INT num = {.prec = 2, .size = 2, .limbs = limbs[0]};
  MP_INT_negative(&num);
  ck_assert_int_eq(num.size, -2);
  ck_assert_uint_eq(limbs[1][0], limbs[0][0]);
  ck_assert_uint_eq(limbs[1][1], limbs[0][1]);
}
END_TEST

START_TEST(test_MP_INT_7) {
  lmp_limb_t limbs[2] = { 1, 1};
  MP_INT num = {.prec = 2, .size = 2, .limbs = limbs};
  const size_t bits = MP_INT_bitsof(&num);
  ck_assert_uint_eq(bits, LMP_LIMB_BIT + 1);
}
END_TEST

START_TEST(test_MP_INT_9) {
  lmp_limb_t limbs[4] = {};
  MP_INT num1 = {.prec = 4, .size = 0, .limbs = limbs};
  MP_INT_from_c_int(&num1, 10086, &STDAllocator);
  ck_assert_int_eq(num1.size, 1);
  ck_assert_uint_eq(limbs[0], 10086);
  MP_INT_from_c_int(&num1, -10086, &STDAllocator);
  ck_assert_int_eq(num1.size, -1);
  ck_assert_uint_eq(limbs[0], 10086);
  MP_INT_from_c_int(&num1, ((int128_t)0x10086110'10100100ULL << 64) | 0x012a, &STDAllocator);
  ck_assert_int_eq(num1.size, 2);
  ck_assert_uint_eq(limbs[0], 0x012aULL);
  ck_assert_uint_eq(limbs[1], 0x10086110'10100100ULL);
  MP_INT_from_c_int(&num1, ((int128_t)0x90086110'10100100ULL << 64) | 0x012a, &STDAllocator);
  ck_assert_int_eq(num1.size, -2);
  ck_assert_uint_eq(limbs[0], ~0x012aULL + 1);
  ck_assert_uint_eq(limbs[1], ~0x90086110'10100100ULL);
}
END_TEST

START_TEST(test_MP_INT_10) {
  lmp_limb_t limbs[4] = {};
  MP_INT num1 = {.prec = -4, .size = 0, .limbs = limbs};
  MP_INT_from_c_uint(&num1, 10086, &STDAllocator);
  ck_assert_int_eq(num1.size, 1);
  ck_assert_uint_eq(limbs[0], 10086);
  MP_INT_from_c_uint(&num1, -10086, &STDAllocator);
  ck_assert_int_eq(num1.size, 4);
  ck_assert_uint_eq(limbs[0], ~10086 + 1);
  ck_assert_uint_eq(limbs[1], LMP_LIMB_MAX);
  ck_assert_uint_eq(limbs[2], LMP_LIMB_MAX);
  ck_assert_uint_eq(limbs[3], LMP_LIMB_MAX);
  MP_INT_from_c_uint(&num1, ((uint128_t)0x10086110'10100100ULL << 64) | 0x012a, &STDAllocator);
  ck_assert_int_eq(num1.size, 2);
  ck_assert_uint_eq(limbs[0], 0x012aULL);
  ck_assert_uint_eq(limbs[1], 0x10086110'10100100ULL);
  MP_INT_from_c_uint(&num1, ((int128_t)0x90086110'10100100ULL << 64) | 0x012a, &STDAllocator);
  ck_assert_int_eq(num1.size, 4);
  ck_assert_uint_eq(limbs[0], 0x012aULL);
  ck_assert_uint_eq(limbs[1], 0x90086110'10100100ULL);
  ck_assert_uint_eq(limbs[2], LMP_LIMB_MAX);
  ck_assert_uint_eq(limbs[3], LMP_LIMB_MAX);
}
END_TEST

START_TEST(test_MP_INT_11) {
  lmp_limb_t limbs[2] = {1, 2};
  const MP_INT num1 = {.prec = -2, .size = 2, .limbs = limbs};
  const lmp_double_limb_t val1 = MP_INT_get_double_flat_limbs(&num1, 0);
  ck_assert(val1 == ((lmp_double_limb_t)2 << LMP_LIMB_BIT) + 1);
  const lmp_double_limb_t val2 = MP_INT_get_double_flat_limbs(&num1, 1);
  ck_assert(val2 == (lmp_double_limb_t)2);
  const lmp_double_limb_t val3 = MP_INT_get_double_flat_limbs(&num1, -1);
  ck_assert(val3 == ((lmp_double_limb_t)1 << LMP_LIMB_BIT));
}
END_TEST

Suite *MP_TEST_int_misc_suite()
{
  Suite *s = suite_create("int_misc");
  TCase *tc = tcase_create("int_misc");

  tcase_add_test(tc, test_MP_INT_1);
  tcase_add_test(tc, test_MP_INT_2);
  tcase_add_test(tc, test_MP_INT_3);
  tcase_add_test(tc, test_MP_INT_4);
  tcase_add_test(tc, test_MP_INT_5);
  tcase_add_test(tc, test_MP_INT_6);
  tcase_add_test(tc, test_MP_INT_7);

  tcase_add_test(tc, test_MP_INT_9);
  tcase_add_test(tc, test_MP_INT_10);
  tcase_add_test(tc, test_MP_INT_11);

  suite_add_tcase(s, tc);
  return s;
}
