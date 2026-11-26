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
 * Filename: print.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-17
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "LMP/print.h"
#include "allocator.h"

typedef struct {
  const char *      name;
  const char *      expected_str;
  const uint32_t    expacted_len;
  const uint32_t    base;
  const int32_t     size;
  const lmp_limb_t *limbs;
} PrintCase;

static void run_print_case(const PrintCase *c) {
  char str[1024] = {};
  const MP_BASE_NUMBER NUMBER = {.prec = -64, .size = c->size, .limbs = (lmp_limb_t *)c->limbs};
  const uint32_t length = MP_BASE_NUMBER_abs_to_string(str, 1024, &NUMBER, &STDAllocator);
  str[length] = 0;

  ck_assert_msg(length == c->expacted_len,
                "%s: print length %u != %u", c->name, length, c->expacted_len);
  ck_assert_msg(strcmp(str, c->expected_str) == 0,
                "%s: print string %s != %s", c->name, str, c->expected_str);
}

static const PrintCase print_cases[] = {

};

START_TEST(test_print)
{
  run_print_case(&print_cases[_i]);
}
END_TEST

Suite *print_suite()
{
  Suite *s = suite_create("print");
  TCase *tc = tcase_create("print");

  constexpr int n = sizeof(print_cases)/sizeof(PrintCase);
  tcase_add_loop_test(tc, test_print, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
