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
 * Module Name: test/testcases/ext
 * Filename: c-int.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-31
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "ext/c-int.h"

typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t    expected_result;
  const int32_t     expected_value;
} I32Case;
typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t    expected_result;
  const int64_t     expected_value;
} I64Case;
typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t    expected_result;
  const int128_t    expected_value;
} I128Case;

typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t   expected_result;
  const uint32_t    expected_value;
} U32Case;
typedef struct  {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t    expected_result;
  const uint64_t    expected_value;
} U64Case;
typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const uint32_t    expected_result;
  const uint128_t   expected_value;
} U128Case;

static void run_i32_case(const I32Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  int32_t value = 0;
  const uint32_t result = MP_INT_to_stdc_i32(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %d != expected %d", c->name, value, c->expected_value);
}
static void run_i64_case(const I64Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  int64_t value = 0;
  const uint32_t result = MP_INT_to_stdc_i64(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %lld != expected %lld", c->name, value, c->expected_value);
}
static void run_i128_case(const I128Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  int128_t value = 0;
  const uint32_t result = MP_INT_to_stdc_i128(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %llX%llX != expected %llX%llX", c->name, (int64_t)(value >> 64), (uint64_t)value,
                (int64_t)(c->expected_value >> 64), (uint64_t)c->expected_value);
}

static void run_u32_case(const U32Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  uint32_t value = 0;
  const uint32_t result = MP_INT_to_stdc_u32(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %u != expected %u", c->name, value, c->expected_value);
}
static void run_u64_case(const U64Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  uint64_t value = 0;
  const uint32_t result = MP_INT_to_stdc_u64(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %llu != expected %llu", c->name, value, c->expected_value);
}
static void run_u128_case(const U128Case *c)
{
  const MP_INT mp_val = { .alloc = -c->mp_size, .size = c->mp_size, .limbs = (lmp_limb_t *)c->mp_limbs };

  uint128_t value = 0;
  const uint32_t result = MP_INT_to_stdc_u128(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %llX%llX != expected %llX%llX", c->name, (int64_t)(value >> 64), (uint64_t)value,
                (int64_t)(c->expected_value >> 64), (uint64_t)c->expected_value);
}

static const lmp_limb_t limbs_i32_0[]        = { 0 };
static const lmp_limb_t limbs_i32_1[]        = { 1 };
static const lmp_limb_t limbs_i32_neg1[]     = { 1 };
static const lmp_limb_t limbs_i32_max[]      = { 0x7FFFFFFF };
static const lmp_limb_t limbs_i32_min[]      = { 0x80000000 };
static const lmp_limb_t limbs_i32_over[]     = { 0x80000000 };
static const lmp_limb_t limbs_i32_under[]    = { 0x80000001 };
static const lmp_limb_t limbs_i32_big[]      = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };
static const lmp_limb_t limbs_i32_bigneg[]   = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };

static const I32Case i32_cases[] = {
  { "zero",             limbs_i32_0,      0, 0,  0 },
  { "one",              limbs_i32_1,      1, 0,  1 },
  { "minus one",        limbs_i32_neg1,  -1, 0, -1 },
  { "INT32_MAX",        limbs_i32_max,    1, 0,  INT32_MAX },
  { "INT32_MIN",        limbs_i32_min,   -1, 0,  INT32_MIN },
  { "INT32_MAX+1 over", limbs_i32_over,  1,  0,INT32_MAX },
  { "INT32_MIN-1 under",limbs_i32_under,-1,  0,INT32_MIN },
  { "big over",         limbs_i32_big,    2, 0,  INT32_MAX },
  { "big under",        limbs_i32_bigneg,-2, 0,  INT32_MIN },
};

static const lmp_limb_t limbs_i64_0[]      = { 0 };
static const lmp_limb_t limbs_i64_1[]      = { 1 };
static const lmp_limb_t limbs_i64_neg1[]   = { 1 };
static const lmp_limb_t limbs_i64_max[]    = { 0x7FFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_i64_min[]    = { 0x8000000000000000ULL };
static const lmp_limb_t limbs_i64_over[]   = { 0x8000000000000000ULL };
static const lmp_limb_t limbs_i64_under[]  = { 0x8000000000000001ULL };
static const lmp_limb_t limbs_i64_big[]    = { 0, 1 };
static const lmp_limb_t limbs_i64_bigneg[] = { 0, 1 };

static const I64Case i64_cases[] = {
  { "zero",            limbs_i64_0,      0,0,  0 },
  { "one",             limbs_i64_1,      1,0,  1 },
  { "minus one",       limbs_i64_neg1,  -1, 0,-1 },
  { "INT64_MAX",       limbs_i64_max,    1,0,  INT64_MAX },
  { "INT64_MIN",       limbs_i64_min,   -1,0,  INT64_MIN },
  { "INT64_MAX+1 over", limbs_i64_over,  1,0,  INT64_MAX },
  { "INT64_MIN-1 under", limbs_i64_under,-1,0,  INT64_MIN },
  { "2^64 over",        limbs_i64_big,   2,0,  INT64_MAX },
  { "2^64 under",      limbs_i64_bigneg,-2,0, INT64_MIN },
};

static const lmp_limb_t limbs_i128_0[]    = { 0 };
static const lmp_limb_t limbs_i128_1[]    = { 1 };
static const lmp_limb_t limbs_i128_neg1[] = { 1 };
static const lmp_limb_t limbs_i128_max[]  = { 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_i128_min[]  = { 0, 0x8000000000000000ULL };
static const lmp_limb_t limbs_i128_over[] = { 0, 0x8000000000000000ULL };
static const lmp_limb_t limbs_i128_under[]= { 1, 0x8000000000000000ULL };
static const lmp_limb_t limbs_i128_big[]  = { 0, 0, 1 };
static const lmp_limb_t limbs_i128_bigneg[]={ 0, 0, 1 };

static const I128Case i128_cases[] = {
  { "zero",            limbs_i128_0,      0,0,  0 },
  { "one",             limbs_i128_1,      1,0,  1 },
  { "minus one",       limbs_i128_neg1,  -1, 0,-1 },
  { "INT64_MAX",       limbs_i128_max,    2,0,  INT128_MAX },
  { "INT64_MIN",       limbs_i128_min,   -2,0,  INT128_MIN },
  { "INT64_MAX+1 over", limbs_i128_over,  2,0,  INT128_MAX },
  { "INT64_MIN-1 under", limbs_i128_under,-2,0,  INT128_MIN },
  { "2^64 over",        limbs_i128_big,   3,0,  INT128_MAX },
  { "2^64 under",      limbs_i128_bigneg,-3,0, INT128_MIN },
};

static const lmp_limb_t limbs_u32_0[]        = { 0 };
static const lmp_limb_t limbs_u32_1[]        = { 1 };
static const lmp_limb_t limbs_u32_neg1[]     = { 1 };
static const lmp_limb_t limbs_u32_negmax[]   = { 0xFFFFFFFF };
static const lmp_limb_t limbs_u32_max[]      = { 0xFFFFFFFF };
static const lmp_limb_t limbs_u32_over[]     = { 0x100000000ULL };
static const lmp_limb_t limbs_u32_overneg[]  = { 0x100000000ULL };
static const lmp_limb_t limbs_u32_big[]      = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u32_bigneg[]   = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u32_p1[]   = { 0x7FFFFFFF };
static const lmp_limb_t limbs_u32_p2[]   = { 0x80000000 };

static const U32Case u32_cases[] = {
  { "zero",               limbs_u32_0,       0, 0, 0 },
  { "one",                limbs_u32_1,       1, 0, 1 },
  { "minus one",          limbs_u32_neg1,   -1, 0, -1 },
  { "UINT32_MAX",         limbs_u32_max,     1, 0, UINT32_MAX },
  { "-(UINT32_MAX)",      limbs_u32_negmax, -1, 0, -UINT32_MAX },
  { "2^32 over",          limbs_u32_over,    1, 0, UINT32_MAX },
  { "2^32 over neg",      limbs_u32_overneg,-1, 0, -UINT32_MAX },
  { "128 over",           limbs_u32_big,     2, 0, UINT32_MAX },
  { "128 over neg",       limbs_u32_bigneg, -2, 0, -UINT32_MAX },
  { "promise INT32_MAX",  limbs_u32_p1, 1, 0, INT32_MAX },
  { "promise INT32_MIN",  limbs_u32_p2, -1, 0, INT32_MIN }
};

static const lmp_limb_t limbs_u64_0[]       = { 0 };
static const lmp_limb_t limbs_u64_1[]       = { 1 };
static const lmp_limb_t limbs_u64_neg1[]    = { 1 };
static const lmp_limb_t limbs_u64_max[]     = { 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u64_negmax[]  = { 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u64_over[]    = { 0, 1 };
static const lmp_limb_t limbs_u64_overneg[] = { 0, 1 };
static const lmp_limb_t limbs_u64_big[]     = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u64_bigneg[]  = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u64_p1[]      = { 0x7FFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u64_p2[]      = { 0x8000000000000000ULL };

static const U64Case u64_cases[] = {
  { "zero",             limbs_u64_0,       0, 0, 0 },
  { "one",              limbs_u64_1,       1, 0, 1 },
  { "minus one",        limbs_u64_neg1,   -1, 0, -1 },
  { "UINT64_MAX",       limbs_u64_max,     1, 0, UINT64_MAX },
  { "-(UINT64_MAX)",    limbs_u64_negmax, -1, 0, -UINT64_MAX },
  { "2^64 over",        limbs_u64_over,    2, 0, UINT64_MAX },
  { "2^64 over neg",    limbs_u64_overneg,-2, 0, -UINT64_MAX },
  { "128 over",         limbs_u64_big,     2, 0, UINT64_MAX },
  { "128 over neg",     limbs_u64_bigneg, -2, 0, -UINT64_MAX },
  { "promise INT64_MAX",limbs_u64_p1,      1, 0, INT64_MAX },
  { "promise INT64_MIN",limbs_u64_p2,     -1, 0, INT64_MIN }
};

static const lmp_limb_t limbs_u128_0[]      = { 0 };
static const lmp_limb_t limbs_u128_1[]      = { 1 };
static const lmp_limb_t limbs_u128_neg1[]   = { 1 };
static const lmp_limb_t limbs_u128_max[]    = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u128_negmax[] = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u128_over[]   = { 0, 0, 1 };
static const lmp_limb_t limbs_u128_overneg[]= { 0, 0, 1 };
static const lmp_limb_t limbs_u128_big[]    = { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x1 };
static const lmp_limb_t limbs_u128_p1[]     = { 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL };
static const lmp_limb_t limbs_u128_p2[]     = { 0, 0x8000000000000000ULL };

static const U128Case u128_cases[] = {
  { "zero",               limbs_u128_0,       0, 0, 0 },
  { "one",                limbs_u128_1,       1, 0, 1 },
  { "minus one",          limbs_u128_neg1,   -1, 0, -1 },
  { "UINT128_MAX",        limbs_u128_max,     2, 0, UINT128_MAX },
  { "-(UINT128_MAX)",     limbs_u128_negmax, -2, 0, -UINT128_MAX },
  { "2^128 over",         limbs_u128_over,    3, 0, UINT128_MAX },
  { "2^128 over neg",     limbs_u128_overneg,-3, 0, -UINT128_MAX },
  { "160 over",           limbs_u128_big,     3, 0, UINT128_MAX },
  { "promise INT128_MAX", limbs_u128_p1,      2, 0, INT128_MAX },
  { "promise INT128_MIN", limbs_u128_p2,     -2, 0, INT128_MIN },
};

START_TEST(test_i32)
{
  run_i32_case(&i32_cases[_i]);
}
END_TEST

START_TEST(test_i64)
{
  run_i64_case(&i64_cases[_i]);
}
END_TEST

START_TEST(test_i128)
{
  run_i128_case(&i128_cases[_i]);
}
END_TEST
START_TEST(test_u32)
{
  run_u32_case(&u32_cases[_i]);
}
END_TEST

START_TEST(test_u64)
{
  run_u64_case(&u64_cases[_i]);
}
END_TEST

START_TEST(test_u128)
{
  run_u128_case(&u128_cases[_i]);
}
END_TEST

Suite *c_int_suite()
{
  Suite *s = suite_create("c-int");
  TCase *tc = tcase_create("c-int");

  constexpr int n_i32 = sizeof(i32_cases)/sizeof(I32Case);
  constexpr int n_i64 = sizeof(i64_cases)/sizeof(I64Case);
  constexpr int n_i128 = sizeof(i128_cases)/sizeof(I128Case);
  constexpr int n_u32 = sizeof(u32_cases)/sizeof(U32Case);
  constexpr int n_u64 = sizeof(u64_cases)/sizeof(U64Case);
  constexpr int n_u128 = sizeof(u128_cases)/sizeof(U128Case);
  tcase_add_loop_test(tc, test_i32, 0, n_i32);
  tcase_add_loop_test(tc, test_i64, 0, n_i64);
  tcase_add_loop_test(tc, test_i128, 0, n_i128);
  tcase_add_loop_test(tc, test_u32, 0, n_u32);
  tcase_add_loop_test(tc, test_u64, 0, n_u64);
  tcase_add_loop_test(tc, test_u128, 0, n_u128);

  suite_add_tcase(s, tc);
  return s;
}
