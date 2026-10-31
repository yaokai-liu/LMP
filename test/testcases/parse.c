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
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-28
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "parse.h"
#include "allocator.h"

typedef struct {
  const char * name;
  const char * str;
  const int32_t     res_size;
  const uint32_t    res_type;
  const int64_t     res_exp;
  const lmp_limb_t *res_limbs;
  const int8_t      res_repr;
  const uint32_t    parsed_len;
} ParseCase;


#define abs(a)             ((a) > (0) ? (a) : -(a))
static void run_parse_case(const ParseCase *c) {
  lmp_limb_t res_limbs[64] = {};
  MP_NUM_TYPE type = {};
  MP_NUMBER NUMBER = {.val.INT = {.alloc = -64, .limbs = res_limbs}};
  const uint32_t length = LMP_parse(c->str, &type, &NUMBER, &STDAllocator);

  ck_assert_msg(length == c->parsed_len,
                "%s: parsed length %u != %u", c->name, length, c->parsed_len);
  ck_assert_msg(type.type == c->res_type,
                "%s: result type %x != %x", c->name, type.type, c->res_type);
  ck_assert_msg(NUMBER.repr == c->res_repr,
                "%s: result representation %u != %u", c->name, NUMBER.repr, c->res_repr);
  if (NUMBER.repr == MP_NUM_REPR_MP_INT) {
    ck_assert_msg(NUMBER.val.INT.size == c->res_size,
                  "%s: result size %d != %d", c->name, NUMBER.val.INT.size, c->res_size);
    for (int32_t i = 0; i < abs(c->res_size); i++) {
      ck_assert_msg(NUMBER.val.INT.limbs[i] == c->res_limbs[i],
                    "%s: limb[%d] %llu != %llu", c->name, i,
                    (lmp_limb_t)NUMBER.val.INT.limbs[i],
                    (lmp_limb_t)c->res_limbs[i]);
    }
  } else if (NUMBER.repr == MP_NUM_REPR_MP_FLOAT) {
    ck_assert_msg(NUMBER.val.FLOAT.size == c->res_size,
                  "%s: result size %d != %d", c->name, NUMBER.val.FLOAT.size, c->res_size);
    ck_assert_msg(NUMBER.val.FLOAT.exp == c->res_exp,
                  "%s: result exponet %lld != %lld", c->name, NUMBER.val.FLOAT.exp, c->res_exp);
    for (int32_t i = 0; i < abs(c->res_size); i++) {
      ck_assert_msg(NUMBER.val.FLOAT.limbs[i] == c->res_limbs[i],
                    "%s: limb[%d] %llu != %llu", c->name, i,
                    (lmp_limb_t)NUMBER.val.FLOAT.limbs[i],
                    (lmp_limb_t)c->res_limbs[i]);
    }
  } else if (NUMBER.repr == MP_NUM_REPR_MP_DECIMAL) {
    ck_assert_msg(NUMBER.val.DECIMAL.size == c->res_size,
    "%s: result type %d != %d", c->name, NUMBER.val.DECIMAL.size, c->res_size);
    ck_assert_msg(NUMBER.val.DECIMAL.exp == c->res_exp,
                  "%s: result exponet %lld != %lld", c->name, NUMBER.val.DECIMAL.exp, c->res_exp);
    for (int32_t i = 0; i < abs(c->res_size); i++) {
      ck_assert_msg(NUMBER.val.DECIMAL.limbs[i] == c->res_limbs[i],
                    "%s: limb[%d] %llu != %llu", c->name, i,
                    (lmp_limb_t)NUMBER.val.DECIMAL.limbs[i],
                    (lmp_limb_t)c->res_limbs[i]);
    }
  }
}

static const lmp_limb_t L_0    [] = { 0 };
static const lmp_limb_t L_1    [] = { 1 };
static const lmp_limb_t L_0x1P4    [] = { 0x1'0000 };
static const lmp_limb_t L_7    [] = { 007 };
static const lmp_limb_t L_1M    [] = { 1'000'000 };
static const lmp_limb_t L_123  [] = { 123 };
static const lmp_limb_t L_12332  [] = { 12332 };
static const lmp_limb_t L_255  [] = { 0xFF };
static const lmp_limb_t L_1e10   [] = { 10000000000 };
static const lmp_limb_t L_1010 [] = { 0b1010 };
static const lmp_limb_t L_511  [] = { 0777 };
static const lmp_limb_t L_15eLIMB  [] = {0x8000'0000'0000'0000ULL, 0x1};
static const lmp_limb_t L_15eN3  [] = { 0x0062'4DD2'F1A9'FBE7 };
static const lmp_limb_t L_N15eN3  [] = { 0x0062'4DD2'F1A9'FBE8 };
static const lmp_limb_t L_314  [] = {0x243F'6A88'85A2'1F77ULL, 0x3};
static const lmp_limb_t L_DEAD [] = { 0xDEADBEEFULL };
static const lmp_limb_t L_C    [] = { 0x1800 };
static const lmp_limb_t L_25[] = { 0x4000'0000'0000'0000ULL };
static const lmp_limb_t L_0x1PN2[] = { 0x0100'0000'0000'0000ULL };
static const lmp_limb_t L_1eN10[] = { 0x0000'0000'6DF3'7F67 };
static const lmp_limb_t L_1_2[] = { 0x3333'3333'3333'3333, 1 };

static const ParseCase parse_cases[] = {
  /* -------- decimal -------- */
  { "dec_zero",       "0",         0,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_0,    MP_NUM_REPR_MP_INT,   1 },
  { "dec_one",        "1",         1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_1,    MP_NUM_REPR_MP_INT,   1 },
  { "dec_123",        "123",       1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_123,  MP_NUM_REPR_MP_INT,   3 },
  { "dec_pos",        "+123",      1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_123,  MP_NUM_REPR_MP_INT,   4 },
  { "dec_neg",        "-123",     -1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_123,  MP_NUM_REPR_MP_INT,   4 },
  { "dec_lead0",      "007",       1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_7,    MP_NUM_REPR_MP_INT,   3 },
  { "dec_sep_quote",  "1'000'000", 1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_1M,   MP_NUM_REPR_MP_INT,   9 },
  { "dec_sep_us",     "1_000_000", 1,  MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0, L_1M,   MP_NUM_REPR_MP_INT,   9 },

  /* -------- hexadecimal -------- */
  { "hex_ff",         "0xFF",           1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_255,  MP_NUM_REPR_MP_INT,  4  },
  { "hex_ff_lower",   "0xff",           1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_255,  MP_NUM_REPR_MP_INT,  4  },
  { "hex_neg",        "-0xDeadBeef",   -1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_DEAD, MP_NUM_REPR_MP_INT, 11  },
  { "hex_sep",        "0x1'F'F",        1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_511,  MP_NUM_REPR_MP_INT,  7  },
  { "hex_us_sep",     "0xdead_beef",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_DEAD, MP_NUM_REPR_MP_INT, 11  },

  /* -------- octal/binary -------- */
  { "oct_777",        "0o777",  1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_511,  MP_NUM_REPR_MP_INT, 5 },
  { "oct_upper",      "0O777",  1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_511,  MP_NUM_REPR_MP_INT, 5 },
  { "bin_1010",       "0b1010", 1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_1010, MP_NUM_REPR_MP_INT, 6 },
  { "bin_upper",      "0B1010", 1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_1010, MP_NUM_REPR_MP_INT, 6 },

  /* -------- float -------- */
  { "flt_1_0",        "1.0",     1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,  L_1,  MP_NUM_REPR_MP_FLOAT,  3 },
  { "flt_neg_1_5",    "-1.5",   -2, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1,  L_15eLIMB,  MP_NUM_REPR_MP_FLOAT,  4 },
  { "flt_0_25",       "0.25",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1,  L_25,  MP_NUM_REPR_MP_FLOAT,  4 },
  { "flt_pi",         "3.14159265358979", 2, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1, L_314, MP_NUM_REPR_MP_FLOAT, 16 },

  /* -------- exponent -------- */
  { "exp_e10",        "1e10",     1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,  L_1e10, MP_NUM_REPR_MP_FLOAT,  4 },
  { "exp_E10",        "1E10",     1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,  L_1e10, MP_NUM_REPR_MP_FLOAT,  4 },
  { "exp_e_pos",      "1e+10",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,  L_1e10, MP_NUM_REPR_MP_FLOAT,  5 },
  { "exp_e_neg",      "1e-10",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1, L_1eN10, MP_NUM_REPR_MP_FLOAT,  5 },
  { "exp_neg_float",  "-1.5e-3", -1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1,  L_N15eN3, MP_NUM_REPR_MP_FLOAT,  7 },
  { "exp_hex_p",      "0x1p4",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,   L_0x1P4, MP_NUM_REPR_MP_FLOAT,  5 },
  { "exp_hex_frac",   "0x1.8p3",  1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), 0,   L_C, MP_NUM_REPR_MP_FLOAT,  7 },
  { "exp_hex_P_neg",  "0x1P-2",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1,  L_0x1PN2, MP_NUM_REPR_MP_FLOAT,  6 },

  /* -------- suffix <repr><size><standard> -------- */
  { "suf_sll<C-INT>",  "123sll<C-INT>", 1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT, 128), 0, L_123, MP_NUM_REPR_MP_INT,  13 },
  { "suf_ull<C-INT>",  "123ull<C-INT>", 1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,128), 0, L_123, MP_NUM_REPR_MP_INT, 13 },
  { "suf_f32<IEEE754>",  "1.5f32<IEEE754>", 2, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT, 32), -1, L_15eLIMB, MP_NUM_REPR_MP_FLOAT,   15 },
  { "suf_d64<C-INT>",  "123d64<C-INT>", 1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_123, MP_NUM_REPR_MP_INT, 13 },

  /* -------- suffix <size><repr><standard> -------- */
  { "suf_ls<C-INT>",   "123ls<C-INT>",  1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT, 64), 0, L_123, MP_NUM_REPR_STD_C_INT,  12 },
  { "suf_32f<IEEE754>",  "12332f<IEEE754>", 1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_INT, 0), 0, L_12332, MP_NUM_REPR_MP_INT,    15 },
  { "suf_hex_ull<C-INT>","0xFFull<C-INT>",1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,128),0, L_255, MP_NUM_REPR_MP_INT, 14 },

  /* -------- suffix <size><standard> -------- */
  { "suf_l<C-INT>",    "123l<C-INT>",   1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_123,  MP_NUM_REPR_MP_INT, 11 },
  { "suf_bin_l<C-INT>","0b1010l<C-INT>",1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_1010, MP_NUM_REPR_MP_INT, 14 },

  /* -------- suffix <repr><standard> -------- */
  { "suf_u<C-INT>",  "123u<C-INT>",  1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,0), 0, L_123, MP_NUM_REPR_MP_INT, 11 },
  { "suf_f<IEEE754>",  "1.5f<IEEE754>",  2, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,   0), -1, L_15eLIMB,  MP_NUM_REPR_MP_FLOAT,  13 },
  { "suf_d<C-INT>",  "123d<C-INT>",  1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_123, MP_NUM_REPR_MP_INT,11 },

  /* -------- suffix <repr><size> -------- */
  { "suf_sl",  "123sl",  1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT, 64), 0, L_123, MP_NUM_REPR_MP_INT,  5 },
  { "suf_f32", "1.5f32", 2, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD,MP_NUM_REPR_IEEE754_FLOAT,   32), -1,L_15eLIMB,  MP_NUM_REPR_MP_FLOAT,   6 },
  { "suf_d64", "123d64", 1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_123, MP_NUM_REPR_MP_INT, 6 },

  /* -------- suffix <size> -------- */
  { "suf_l",  "123l",   1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_123, MP_NUM_REPR_MP_INT, 4 },
  { "suf_L",  "123L",   1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_123, MP_NUM_REPR_MP_INT, 4 },
  { "suf_hex_l","0xFFl",1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,64), 0, L_255, MP_NUM_REPR_MP_INT, 5 },

  /* -------- suffix <repr> -------- */
  { "suf_s",  "123s",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT, 0), 0, L_123, MP_NUM_REPR_MP_INT,  4 },
  { "suf_u",  "123u",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_123, MP_NUM_REPR_MP_INT, 4 },
  { "suf_f",  "1.5f",   2, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,   0), -1,L_15eLIMB,  MP_NUM_REPR_MP_FLOAT,   4 },
  { "suf_g",  "1.5g",   2, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,   0), -1,L_15eLIMB,  MP_NUM_REPR_MP_FLOAT,   4 },
  { "suf_d",  "123d",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0, L_123, MP_NUM_REPR_MP_INT, 4 },

  /* -------- full consists -------- */
  { "full_neg_hex",   "-0xFFull<C-INT>",         -1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT, 128), 0, L_255, MP_NUM_REPR_MP_INT, 15 },
  { "full_pos_float", "+1.5e-3f32<IEEE754>",        1, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,    32), -1,L_15eN3,  MP_NUM_REPR_MP_FLOAT,   19 },
  { "full_hex_float", "0x1.8p3f32<IEEE754>",        1, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,    32), 0, L_C,   MP_NUM_REPR_MP_FLOAT,   19 },
  { "full_hex_us_L",  "-0xdead_beefL",     -1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD, MP_NUM_REPR_STD_C_INT,   64), 0, L_DEAD, MP_NUM_REPR_MP_INT,  13 },
  { "full_X_P_g<IEEE754>",    "0X1P-2g<IEEE754>",           1, MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,     0), -1,L_0x1PN2,   MP_NUM_REPR_MP_FLOAT,    16 },

  /* -------- error and fallback cases -------- */
  { "err_empty",      "",       0, 0, 0, nullptr,   0, 0 },
  { "err_sign_only",  "+",      0, 0, 0, nullptr,   0, 0 },
  { "err_hex_no_digit","0x",     0, 0, 0, nullptr,   0, 0 },
  { "err_bin_2",      "0b2",    0, 0, 0, nullptr,   0, 0 },
  { "err_oct_8",      "0o8",    0, 0, 0, nullptr,   0, 0 },
  { "err_hex_G",      "0xG",    0, 0, 0, nullptr,   0, 0 },
  { "err_two_dot",    "1.2.3",  2, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,0), -1,   L_1_2,  MP_NUM_REPR_MP_FLOAT, 3 },
  { "err_exp_no_digit","1e",     1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0,  L_1,  MP_NUM_REPR_MP_INT,   1 },
  { "err_exp_plus",   "1e+",    1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0,  L_1,  MP_NUM_REPR_MP_INT,   1 },
  { "err_double_dot", "1..2",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0), 0,  L_1,  MP_NUM_REPR_MP_INT, 1 },
  { "err_unknown_suffix", "123x",   1, MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,0),   0,  L_123,MP_NUM_REPR_MP_INT,   4 },
  { "err_dup_size",   "123lll", 1, MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,128), 0,  L_123,MP_NUM_REPR_MP_INT,  5 },
  { "err_sep_start",  "'123",   0, 0, 0, nullptr, 0, 0 },
  { "err_double_sign","--123",  0, 0, 0, nullptr, 0, 0 },
  { "err_mixed_sign", "+-123",  0, 0, 0, nullptr, 0, 0 },
  { "err_bin_prefix", "0b",     0, 0, 0, nullptr, 0, 0 },
  { "err_oct_prefix", "0o",     0, 0, 0, nullptr, 0, 0 },
  { "err_X_prefix",   "0X",     0, 0, 0, nullptr, 0, 0 },
};

START_TEST(test_parse)
{
  run_parse_case(&parse_cases[_i]);
}
END_TEST

Suite *parse_suite()
{
  Suite *s = suite_create("parse");
  TCase *tc = tcase_create("parse");

  constexpr int n = sizeof(parse_cases)/sizeof(ParseCase);
  tcase_add_loop_test(tc, test_parse, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
