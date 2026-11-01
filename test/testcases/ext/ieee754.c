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
 * Filename: ieee754.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-30
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include <stdio.h>
#include "ext/ieee754.h"
#include "allocator.h"

typedef struct {
  const char * name;
  const uint64_t  mantissa;
  const int32_t   exponent;
  const float     expected_value;
  const bool      sign;
} Ieee754F32Case;
typedef struct  {
  const char * name;
  const uint64_t  mantissa;
  const int32_t   exponent;
  const double    expected_value;
  const bool      sign;
} Ieee754F64Case;
typedef struct {
  const char * name;
  const uint128_t   mantissa;
  const int32_t     exponent;
  const float128_t  expected_value;
  const bool        sign;
} Ieee754F128Case;

typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const int64_t     mp_exp;
  const uint32_t    expected_result;
  const float32_t   expected_value;
} F32Case;
typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const int64_t     mp_exp;
  const uint32_t    expected_result;
  const float64_t   expected_value;
} F64Case;
typedef struct {
  const char * name;
  const lmp_limb_t *mp_limbs;
  const int32_t     mp_size;
  const int64_t     mp_exp;
  const uint32_t    expected_result;
  const float128_t  expected_value;
} F128Case;

static void run_f32bin_case(const Ieee754F32Case *c) {

  const float value = ieee754bin32(c->sign, (uint32_t) c->mantissa, (int16_t)c->exponent);
  if (isnan(c->expected_value)) {
    ck_assert_msg(isnan(value), "%s: expected NaN got %f", c->name, value);
  } else {
    ck_assert_msg(value == c->expected_value,
                  "%s: value %f != expected %f", c->name, value, c->expected_value);
    // ck_assert_msg(value == c->expected_value,
    //               "%s: value %x != expected %x", c->name, *(int32_t *)&value, *(int32_t *)&c->expected_value);
  }
}
static void run_f64bin_case(const Ieee754F64Case *c) {
  const double value = ieee754bin64(c->sign, c->mantissa, (int16_t) c->exponent);
  if (isnan(c->expected_value)) {
    ck_assert_msg(isnan(value), "%s: expected NaN got %lf", c->name, value);
  } else {
    ck_assert_msg(value == c->expected_value,
                  "%s: value %lf != expected %lf", c->name, value, c->expected_value);
    // ck_assert_msg(value == c->expected_value,
    //               "%s: value %llx != expected %llx", c->name, *(int64_t *)&value, *(int64_t *)&c->expected_value);
  }
}

static void run_f32_case(const F32Case *c)
{
  const MP_FLOAT mp_val = { .prec = -c->mp_size, .size = c->mp_size, .exp = c->mp_exp, .limbs = (lmp_limb_t *)c->mp_limbs };

  float32_t value = 0;
  const uint32_t result = MP_FLOAT_to_ieee754_f32(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  ck_assert_msg(value == c->expected_value,
                "%s: value %f != expected %f", c->name, value, c->expected_value);
  // ck_assert_msg(value == c->expected_value,
  //               "%s: value %x != expected %x", c->name, *(int32_t *)&value, *(int32_t *)&c->expected_value);
}
static void run_f64_case(const F64Case *c)
{
  const MP_FLOAT mp_val = { .prec = -c->mp_size, .size = c->mp_size, .exp = c->mp_exp, .limbs = (lmp_limb_t *)c->mp_limbs };

  float64_t value = 0;
  const uint32_t result = MP_FLOAT_to_ieee754_f64(&value, &mp_val);

  ck_assert_msg(result == c->expected_result,
                "%s: exec result %u != expected %u", c->name, result, c->expected_result);
  // ck_assert_msg(value == c->expected_value,
  //               "%s: value %lf != expected %lf", c->name, value, c->expected_value);
  ck_assert_msg(value == c->expected_value,
                "%s: value %llx != expected %llx", c->name, *(int64_t *)&value, *(int64_t *)&c->expected_value);
}


static const Ieee754F32Case f32bin_cases[] = {
  { "f32bin +0.0",            0x000000u, -127,    0.0f,   false },
  { "f32bin -0.0",            0x000000u, -127,   -0.0f,   true  },
  { "f32bin +1.0",            0x800000u, 0,  1.0f,   false },
  { "f32bin -1.0",            0x800000u, 0, -1.0f,   true  },
  { "f32bin +2.0",            0x800000u, 1,  2.0f,   false },
  { "f32bin +0.5",            0x800000u, -1,  0.5f,   false },
  { "f32bin +1.5",            0xC00000u, 0,  1.5f,   false },
  { "f32bin +3.0",            0xC00000u, 1,  3.0f,   false },
  { "f32bin +0.75",           0xC00000u, -1,  0.75f,  false },
  { "f32bin +1.25",           0xA00000u, 0,  1.25f,  false },
  { "f32bin +1.375",          0xB00000u, 0,  1.375f, false },
  { "f32bin +pi",             0xC90FDBu, 1, 3.14159274f, false },
  { "f32bin +e",              0xADF854u, 1, 2.71828175f, false },
  { "f32bin sqrt(2)",         0xB504F3u, 0, 1.41421354f, false },
  { "f32bin FLT_MIN (normal)",0x000000u, -126,     1.17549435e-38f, false },
  { "f32bin FLT_MAX",         0xFFFFFFu, 127,   3.40282347e+38f, false },
  { "f32bin denorm min",      0x000001u, -127,     1.40129846e-45f, false },
  { "f32bin -denorm min",     0x000001u, -127,    -1.40129846e-45f, true  },
  { "f32bin denorm max",      0xFFFFFFu, -127,     1.17549421e-38f, false },
  { "f32bin denorm half-min", 0x400000u, -127,     5.87747175e-39f, false },
  { "f32bin denorm 2^-128",   0x200000u, -127,     2.93873587e-39f, false },
  { "f32bin +inf",            0x000000u, 128,   (float)INFINITY, false },
  { "f32bin -inf",            0x000000u, 128,  -(float)INFINITY, true  },
};

static const Ieee754F64Case f64bin_cases[] = {
  { "f64bin +1.0",          0x10000000000000ULL, 0,  1.0,                   false },
  { "f64bin -1.0",          0x10000000000000ULL, 0, -1.0,                   true  },
  { "f64bin +2.0",          0x10000000000000ULL, 1,  2.0,                   false },
  { "f64bin +0.5",          0x10000000000000ULL, -1,  0.5,                   false },
  { "f64bin +1.5",          0x18000000000000ULL, 0,  1.5,                   false },
  { "f64bin -1.5",          0x18000000000000ULL, 0, -1.5,                   true  },
  { "f64bin +3.0",          0x18000000000000ULL, 1,  3.0,                   false },
  { "f64bin +0.75",         0x18000000000000ULL, -1,  0.75,                  false },
  { "f64bin +1.25",         0x14000000000000ULL, 0,  1.25,                  false },
  { "f64bin +1.375",        0x16000000000000ULL, 0,  1.375,                 false },
  { "f64bin +pi",           0x1921FB54442D18ULL, 1,  3.141592653589793,     false },
  { "f64bin +e",            0x15BF0A8B145769ULL, 1,  2.718281828459045,     false },
  { "f64bin sqrt(2)",       0x16A09E667F3BCDULL, 0,  1.4142135623730951,    false },
  { "f64bin DBL_MIN",       0x10000000000000ULL, -1022,   2.2250738585072014e-308, false },
  { "f64bin DBL_MAX",       0x1FFFFFFFFFFFFFULL, 1023,   1.7976931348623157e+308, false },
  { "f64bin denorm min",    0x8000000000000ULL,-1074, 4.9406564584124654e-324, false },
  { "f64bin denorm 2^-1023",0x8000000000000ULL, -1023, 1.1125369292536007e-308,false },
  { "f64bin denorm max",    0xFFFFFFFFFFFFFULL, -1023, 2.2250738585072009e-308, false },
  { "f64bin +inf",          0x0ULL,              1024, (double)INFINITY,        false },
  { "f64bin -inf",          0x0ULL,              1024,-(double)INFINITY,        true  },
};

static const lmp_limb_t f32_limbs_zero[]       = { 0 };
static const lmp_limb_t f32_limbs_one[]        = { 1 };
static const lmp_limb_t f32_limbs_two[]        = { 2 };
static const lmp_limb_t f32_limbs_four[]       = { 4 };
static const lmp_limb_t f32_limbs_neg_one[]    = { 1 };
static const lmp_limb_t f32_limbs_3[]          = { 3 };
static const lmp_limb_t f32_limbs_16777216[]   = { 16777216 };
static const lmp_limb_t f32_limbs_flt_max[]    = {0, 0xFFFFFF0000000000ULL };
static const lmp_limb_t f32_limbs_flt_true_min[] = { 0x80000000000ULL };
static const lmp_limb_t f32_limbs_two_limbs[]  = { 0x0000000000000001ULL, 0x0000000000000001ULL };
static const lmp_limb_t f32_limbs_2p63[]  = {0x8000000000000000ULL};

static const F32Case f32_cases[] = {
  { "f32_zero",           f32_limbs_zero,          0,   0,  0, 0.0f },
  { "f32_one",            f32_limbs_one,           1,   0,  0, 1.0f },
  { "f32_neg_one",        f32_limbs_neg_one,      -1,   0,  0, -1.0f },
  { "f32_three",          f32_limbs_3,             1,   0,  0, 3.0f },
  { "f32_2pow24",         f32_limbs_16777216,      1,   0,  0, 16777216.0f },
  { "f32_min_approx",     f32_limbs_one,           1,  -2,  0, 2.9387358770557188e-39f },
  { "f32_min_approx",     f32_limbs_four,          1,  -2,  0, 4*2.9387358770557188e-39f },
  { "f32_half",           f32_limbs_2p63,          1,  -1,  0, 0.5f },
  { "f32_two",            f32_limbs_two,           1,   0,  0, 2.0f },
  { "f32_near_flt_max",   f32_limbs_flt_max,       2,   0,  0, 3.4028234663852886e38f },
  { "f32_overflow_inf",   f32_limbs_flt_max,       2,   1,  0, INFINITY },
  { "f32_underflow_zero", f32_limbs_one,           1, -10,  0, 0.0f },
  { "f32_multi_limb",     f32_limbs_two_limbs,     2,   0,  0, 1.8446744073709552e19f },
  { "f32_neg_subnormal",  f32_limbs_flt_true_min, -1,  -3,  0, -1.401298464324817e-45f },
};

static const lmp_limb_t f64_limbs_zero[]       = { 0 };
static const lmp_limb_t f64_limbs_one[]        = { 1 };
static const lmp_limb_t f64_limbs_two[]        = { 2 };
static const lmp_limb_t f64_limbs_3[]          = { 3 };
static const lmp_limb_t f64_limbs_four[]       = { 4 };
static const lmp_limb_t f64_limbs_2pow53[]     = { 9007199254740992ULL };
static const lmp_limb_t f64_limbs_dbl_max[] = { 0xFFFFFFFFFFFFF800ULL, };
static const lmp_limb_t f64_limbs_two_limbs[]  = { 0x0000000000000001ULL, 0x0000000000000001ULL };
static const lmp_limb_t f64_limbs_2p63[]  = {0x8000000000000000ULL};
static const lmp_limb_t f64_limbs_flt_true_min[] = { 0x4000ULL };

static const F64Case f64_cases[] = {
  { "f64_zero",           f64_limbs_zero,         0,   0,  0, 0.0 },
  { "f64_one",            f64_limbs_one,          1,   0,  0, 1.0 },
  { "f64_neg_one",        f64_limbs_one,         -1,   0,  0,-1.0 },
  { "f64_three",          f64_limbs_3,            1,   0,  0, 3.0 },
  { "f64_2pow53",         f64_limbs_2pow53,       1,   0,  0, 9007199254740992.0 },
  { "f64_half",           f64_limbs_2p63,         1,  -1,  0, 0.5 },
  { "f64_two",            f64_limbs_two,          1,   0,  0, 2.0 },
  { "f64_dbl_max",        f64_limbs_dbl_max,      1,  15,  0, 1.7976931348623157e308 },
  { "f64_overflow_inf",   f64_limbs_dbl_max,      1,  16,  0, INFINITY },
  { "f64_underflow_zero", f64_limbs_one,          1, -20,  0, 0.0 },
  { "f64_multi_limb",     f64_limbs_two_limbs,    2,   0,  0, 1.8446744073709552e19 },
  { "f64_neg_subnormal",  f64_limbs_flt_true_min,-1, -17,  0,-4.9406564584124654e-324 },
  { "f64_min_normal",     f64_limbs_four,         1, -16,  0,  2.2250738585072014e-308 },
};

START_TEST(test_f32bin)
{
  run_f32bin_case(&f32bin_cases[_i]);
}
END_TEST
START_TEST(test_f64bin)
{
  run_f64bin_case(&f64bin_cases[_i]);
}
END_TEST

START_TEST(test_f32)
{
  run_f32_case(&f32_cases[_i]);
}
END_TEST
START_TEST(test_f64)
{
  run_f64_case(&f64_cases[_i]);
}
END_TEST

Suite *ieee754_suite()
{
  Suite *s = suite_create("ieee754");
  TCase *tc = tcase_create("ieee754");

  constexpr int n_f32bin = sizeof(f32bin_cases)/sizeof(Ieee754F32Case);
  constexpr int n_f64bin = sizeof(f64bin_cases)/sizeof(Ieee754F64Case);
  constexpr int n_f32 = sizeof(f32_cases)/sizeof(F32Case);
  constexpr int n_f64 = sizeof(f64_cases)/sizeof(F64Case);
  tcase_add_loop_test(tc, test_f32bin, 0, n_f32bin);
  tcase_add_loop_test(tc, test_f64bin, 0, n_f64bin);
  tcase_add_loop_test(tc, test_f32, 0, n_f32);
  tcase_add_loop_test(tc, test_f64, 0, n_f64);

  suite_add_tcase(s, tc);
  return s;
}
