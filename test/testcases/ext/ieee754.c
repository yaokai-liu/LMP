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

typedef struct {
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

static void run_f32_case(const Ieee754F32Case *c) {

  const float value = ieee754bin32(c->sign, (uint32_t) c->mantissa, (int8_t)c->exponent);
  if (isnan(c->expected_value)) {
    ck_assert_msg(isnan(value), "%s: expected NaN got %f", c->name, value);
  } else {
    ck_assert_msg(value == c->expected_value,
                  "%s: value %f != expected %f", c->name, value, c->expected_value);
  }
}

static void run_f64_case(const Ieee754F64Case *c) {
  const double value = ieee754bin64(c->sign, c->mantissa, (int16_t) c->exponent);
  if (isnan(c->expected_value)) {
    ck_assert_msg(isnan(value), "%s: expected NaN got %lf", c->name, value);
  } else {
    ck_assert_msg(value == c->expected_value,
                  "%s: value %lf != expected %lf", c->name, value, c->expected_value);
  }
}

static const Ieee754F32Case f32_cases[] = {
  { "f32 +0.0",   0x000000u, -127,    0.0f,   false },
  { "f32 -0.0",   0x000000u, -127,   -0.0f,   true  },

  { "f32 +1.0",   0x800000u, 0,  1.0f,   false },
  { "f32 -1.0",   0x800000u, 0, -1.0f,   true  },
  { "f32 +2.0",   0x800000u, 1,  2.0f,   false },
  { "f32 +0.5",   0x800000u, -1,  0.5f,   false },
  { "f32 +1.5",   0xC00000u, 0,  1.5f,   false },
  { "f32 +3.0",   0xC00000u, 1,  3.0f,   false },
  { "f32 +0.75",  0xC00000u, -1,  0.75f,  false },
  { "f32 +1.25",  0xA00000u, 0,  1.25f,  false },
  { "f32 +1.375", 0xB00000u, 0,  1.375f, false },

  { "f32 +pi",     0xC90FDBu, 1, 3.14159274f, false },
  { "f32 +e",      0xADF854u, 1, 2.71828175f, false },
  { "f32 sqrt(2)", 0xB504F3u, 0, 1.41421354f, false },

  { "f32 FLT_MIN (normal)",  0x800000u, -126,     1.17549435e-38f, false },
  { "f32 FLT_MAX",           0xFFFFFFu, 127,   3.40282347e+38f, false },

  { "f32 denorm min",        0x000001u, -127,     1.40129846e-45f, false },
  { "f32 -denorm min",       0x000001u, -127,    -1.40129846e-45f, true  },

  { "f32 denorm max",        0x7FFFFFu, -127,     1.17549421e-38f, false },

  { "f32 denorm half-min",   0x400000u, -127,     5.87747175e-39f, false },

  { "f32 denorm 2^-128",     0x200000u, -127,     2.93873588e-39f, false },

  { "f32 +inf",              0x000000u, 128,   (float)INFINITY, false },
  { "f32 -inf",              0x000000u, 128,  -(float)INFINITY, true  },

  { "f32 qnan (quiet)",      0x400000u, 128,   (float)NAN,      false },
  { "f32 snan",              0x000001u, 128,   (float)NAN,      false },
};

static const Ieee754F64Case f64_cases[] = {
  { "f64 +1.0",    0x10000000000000ULL, 0,  1.0,                   false },
  { "f64 -1.0",    0x10000000000000ULL, 0, -1.0,                   true  },
  { "f64 +2.0",    0x10000000000000ULL, 1,  2.0,                   false },
  { "f64 +0.5",    0x10000000000000ULL, -1,  0.5,                   false },
  { "f64 +1.5",    0x18000000000000ULL, 0,  1.5,                   false },
  { "f64 -1.5",    0x18000000000000ULL, 0, -1.5,                   true  },
  { "f64 +3.0",    0x18000000000000ULL, 1,  3.0,                   false },
  { "f64 +0.75",   0x18000000000000ULL, -1,  0.75,                  false },
  { "f64 +1.25",   0x14000000000000ULL, 0,  1.25,                  false },
  { "f64 +1.375",  0x16000000000000ULL, 0,  1.375,                 false },

{ "f64 +pi",     0x1921FB54442D18ULL, 1,  3.141592653589793,     false },
{ "f64 +e",      0x15BF0A8B145769ULL, 1,  2.718281828459045,     false },
{ "f64 sqrt(2)", 0x16A09E667F3BCDULL, 0,  1.4142135623730951,    false },

{ "f64 DBL_MIN", 0x10000000000000ULL, -1022,   2.2250738585072014e-308, false },
{ "f64 DBL_MAX", 0x1FFFFFFFFFFFFFULL, 1023,   1.7976931348623157e+308, false },

{ "f64 denorm min",   0x1ULL,            -1023, 4.9406564584124654e-324, false },
{ "f64 denorm 2^-1022",0x8000000000000ULL,-1023, 1.1125369292536007e-308,false }, /* 0.5*DBL_MIN */
{ "f64 denorm max",   0xFFFFFFFFFFFFFULL,-1023, 2.2250738585072009e-308, false },

{ "f64 +inf",    0x0ULL,              1024, (double)INFINITY,        false },
{ "f64 -inf",    0x0ULL,              1024,-(double)INFINITY,        true  },

{ "f64 qnan",    0x8000000000000ULL,  1024, (double)NAN,             false },
};

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

  constexpr int n_f32 = sizeof(f32_cases)/sizeof(Ieee754F32Case);
  constexpr int n_f64 = sizeof(f64_cases)/sizeof(Ieee754F64Case);
  tcase_add_loop_test(tc, test_f32, 0, n_f32);
  tcase_add_loop_test(tc, test_f64, 0, n_f64);

  suite_add_tcase(s, tc);
  return s;
}
