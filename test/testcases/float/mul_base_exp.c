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
 * Filename: mul_base_exp.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-26
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <allocator.h>

#define MP_FLOAT_negative(val)                do { (val)->size = -(val)->size; } while(0)

START_TEST(test_MP_FLOAT_1) {
  lmp_limb_t limbs[3] = {1, 2, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, 32, &STDAllocator);
  ck_assert(num.size == 2);
  ck_assert(num.exp == (32 / LMP_LIMB_BIT));
  ck_assert(limbs[0] == (1ULL << (32 % LMP_LIMB_BIT)));
  ck_assert(limbs[1] == (2ULL << (32 % LMP_LIMB_BIT)));
  ck_assert(limbs[2] == 0);
}
END_TEST

START_TEST(test_MP_FLOAT_2) {
  lmp_limb_t limbs[3] = {1, 2, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, 79, &STDAllocator);
  ck_assert(num.size == 2);
  ck_assert(num.exp == (79 / LMP_LIMB_BIT));
  ck_assert(limbs[0] == (1ULL << (79 % LMP_LIMB_BIT)));
  ck_assert(limbs[1] == (2ULL << (79 % LMP_LIMB_BIT)));
  ck_assert(limbs[2] == 0);
}
END_TEST

START_TEST(test_MP_FLOAT_3) {
  lmp_limb_t limbs[3] = {1, 2ULL << 32, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, 32, &STDAllocator);
  ck_assert(num.size == 3);
  ck_assert(num.exp == 0);
  ck_assert(limbs[0] == 1ULL << 32);
  ck_assert(limbs[1] == 0);
  ck_assert(limbs[2] == 2ULL);
}
END_TEST

START_TEST(test_MP_FLOAT_4) {
  lmp_limb_t limbs[3] = {1ULL << 32, 2, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, 32, &STDAllocator);
  ck_assert(num.size == 1);
  ck_assert(num.exp == 1);
  ck_assert(limbs[0] == (2ULL << 32) + 1);
  ck_assert(limbs[1] == 0);
  ck_assert(limbs[2] == 0);
}
END_TEST

START_TEST(test_MP_FLOAT_5) {
  lmp_limb_t limbs[3] = {1, 2, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, -32, &STDAllocator);
  ck_assert(num.size == 2);
  ck_assert(num.exp == -1);
  ck_assert(limbs[0] == 1ULL << 32);
  ck_assert(limbs[1] == 2ULL << 32);
  ck_assert(limbs[2] == 0);
}
END_TEST

START_TEST(test_MP_FLOAT_6) {
  lmp_limb_t limbs[3] = {1ULL << 32, 2, 0};
  MP_FLOAT num = {.prec = -2, .size = 2, .limbs = limbs, .exp = 0};
  MP_FLOAT_mul_base_exp(&num, -32, &STDAllocator);
  ck_assert(num.size == 1);
  ck_assert(num.exp == 0);
  ck_assert(limbs[0] == (2ULL << 32) + 1);
  ck_assert(limbs[1] == 0);
  ck_assert(limbs[2] == 0);
}
END_TEST

Suite *MP_TEST_float_mul_base_exp_suite()
{
  Suite *s = suite_create("float_mul_base_exp");
  TCase *tc = tcase_create("float_mul_base_exp");

  tcase_add_test(tc, test_MP_FLOAT_1);
  tcase_add_test(tc, test_MP_FLOAT_2);
  tcase_add_test(tc, test_MP_FLOAT_3);
  tcase_add_test(tc, test_MP_FLOAT_4);
  tcase_add_test(tc, test_MP_FLOAT_5);
  tcase_add_test(tc, test_MP_FLOAT_6);

  suite_add_tcase(s, tc);
  return s;
}
