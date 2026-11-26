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
#include "LMP/parse.h"
#include "allocator.h"

typedef struct {
  const char * name;
  const char * str;
  const int32_t     expected_size;
  const uint32_t    expected_type;
  const int64_t     expected_exp;
  const lmp_limb_t *expected_limbs;
  const uint32_t    parsed_len;
  const MP_BASE     BASE;
} ParseCase;

#define abs(a)             ((a) > (0) ? (a) : -(a))
static void run_parse_case(const ParseCase *c) {
  lmp_limb_t res_limbs[64] = {};
  MP_NUM_TYPE type = {};
  MP_BASE_NUMBER NUMBER = {.prec = -64, .limbs = res_limbs};
  const uint32_t length = LMP_parse_base_number(c->str, &type, &NUMBER, &STDAllocator);

  ck_assert_msg(length == c->parsed_len,
                "%s: parsed length %u != (expected) %u", c->name, length, c->parsed_len);
  ck_assert_msg(type.type == c->expected_type,
                "%s: result type %x != (expected) %x", c->name, type.type, c->expected_type);
  ck_assert_msg(NUMBER.BASE.base == c->BASE.base,
                "%s: result base %u != (expected) %u", c->name, NUMBER.BASE.base, c->BASE.base);
  ck_assert_msg(NUMBER.BASE.limb_width == c->BASE.limb_width,
                "%s: result base %u != (expected) %u", c->name, NUMBER.BASE.limb_width, c->BASE.limb_width);
  ck_assert_msg(NUMBER.BASE.limb_val == c->BASE.limb_val,
                "%s: result base %"PRI_LIMB" != (expected) %"PRI_LIMB, c->name, NUMBER.BASE.limb_val, c->BASE.limb_val);
  ck_assert_msg(NUMBER.size == c->expected_size,
                "%s: result size %"PRI_SIZE" != (expected) %"PRI_SIZE, c->name, NUMBER.size, c->expected_size);
  ck_assert_msg(NUMBER.exp == c->expected_exp,
                "%s: result exponet %"PRI_EXP" != (expected) %"PRI_EXP"", c->name, NUMBER.exp, c->expected_exp);
  for (int32_t i = 0; i < abs(c->expected_size); i++) {
    ck_assert_msg(NUMBER.limbs[i] == c->expected_limbs[i],
                  "%s: limb[%"PRI_SIZE"] %"PRI_LIMB" != (expected) %"PRI_LIMB, c->name, i, NUMBER.limbs[i], c->expected_limbs[i]);
  }
}

// static const lmp_limb_t L_0    [] = { 0 };
// static const lmp_limb_t L_1    [] = { 1 };
// static const lmp_limb_t L_0x1P4 [] = { 0x1'0000 };
// static const lmp_limb_t L_7    [] = { 007 };
// static const lmp_limb_t L_1M    [] = { 1'000'000 };
// static const lmp_limb_t L_123  [] = { 123 };
// static const lmp_limb_t L_12332  [] = { 12332 };
// static const lmp_limb_t L_255  [] = { 0xFF };
// static const lmp_limb_t L_1e10   [] = { 10000000000 };
// static const lmp_limb_t L_1010 [] = { 0b1010 };
// static const lmp_limb_t L_511  [] = { 0777 };
// static const lmp_limb_t L_15eLIMB  [] = {0x8000'0000'0000'0000ULL, 0x1};
// static const lmp_limb_t L_15eN3  [] = { 0x0062'4DD2'F1A9'FBE7 };
// static const lmp_limb_t L_N15eN3  [] = { 0x0062'4DD2'F1A9'FBE8 };
// static const lmp_limb_t L_314  [] = {0x243F'6A88'85A2'1F77ULL, 0x3};
// static const lmp_limb_t L_DEAD [] = { 0xDEADBEEFULL };
// static const lmp_limb_t L_C    [] = { 0x1800 };
// static const lmp_limb_t L_25[] = { 0x4000'0000'0000'0000ULL };
// static const lmp_limb_t L_0x1PN2[] = { 0x0100'0000'0000'0000ULL };
// static const lmp_limb_t L_1eN10[] = { 0x0000'0000'6DF3'7F67 };
// static const lmp_limb_t L_1_2[] = { 0x3333'3333'3333'3333, 1 };
// static const lmp_limb_t L_long[] = { 0x437430C1913D27FF, 0x05CA0698478CF1B3, 0x1F9ADD3B84CDAD12, 0x7B };

static const lmp_limb_t L_0       [] = { 0 };
static const lmp_limb_t L_1       [] = { 1 };
static const lmp_limb_t L_0x1P4   [] = { 0x1'0000 };
static const lmp_limb_t L_7       [] = { 007 };
static const lmp_limb_t L_170     [] = { 170 };
static const lmp_limb_t L_1M      [] = { 1'000'000 };
static const lmp_limb_t L_123     [] = { 123 };
static const lmp_limb_t L_12332   [] = { 12332 };
static const lmp_limb_t L_255     [] = { 0xFF };
static const lmp_limb_t L_1e10    [] = { 10000000000 };
static const lmp_limb_t L_1010    [] = { 0b1010 };
static const lmp_limb_t L_511     [] = { 0777 };
static const lmp_limb_t L_15eLIMB [] = {5000000000000000000ULL, 1ULL};
static const lmp_limb_t L_15eN3   [] = { 15000000000000000 };
static const lmp_limb_t L_N15eN3  [] = { 15000000000000000 };
static const lmp_limb_t L_314     [] = {1415926535897900000ULL, 0x3};
static const lmp_limb_t L_DEAD    [] = { 0xDEADBEEFULL };
static const lmp_limb_t L_C       [] = { 0x1800 };
static const lmp_limb_t L_25      [] = { 2500000000000000000ULL };
static const lmp_limb_t L_0x1PN2  [] = { 0x0100'0000'0000'0000ULL };
static const lmp_limb_t L_1eN10   [] = { 1000000000 };
static const lmp_limb_t L_1_2     [] = { 2000000000000000000, 1 };
static const lmp_limb_t L_long1   [] = { 8900000000000000000ULL, 7891234567891234567ULL, 1234567899999123456ULL, 123ULL };
static const lmp_limb_t L_long2   [] = { 2345678912345678900LL, 1234567891234567891LL };

static const ParseCase parse_cases[] = {
  { .name = "dec_long_value",       .str = "-123.12345678999991234'56789123456789_123456789LL<IEEE754>",
                                                                     .expected_size = -4, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,    128), .expected_exp = -3, .expected_limbs = L_long1,   .parsed_len = 58, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_long_trail",       .str = "-0.123456789'123456789_123456789'123456789'0000000000000000000000000000000000000LL<IEEE754>",
                                                                     .expected_size = -2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,    128), .expected_exp = -2, .expected_limbs = L_long2,   .parsed_len = 91, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_zero",             .str = "0",                   .expected_size =  0, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_0,       .parsed_len =  1, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_one",              .str = "1",                   .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_1,       .parsed_len =  1, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_123",              .str = "123",                 .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  3, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_pos",              .str = "+123",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_neg",              .str = "-123",                .expected_size = -1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_lead0",            .str = "007",                 .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_7,       .parsed_len =  3, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_sep_quote",        .str = "1'000'000",           .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_1M,      .parsed_len =  9, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "dec_sep_us",           .str = "1_000_000",           .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_1M,      .parsed_len =  9, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "hex_ff",               .str = "0xFF",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_255,     .parsed_len =  4, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "hex_ff_lower",         .str = "0xff",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_255,     .parsed_len =  4, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "hex_neg",              .str = "-0xDeadBeef",         .expected_size = -1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_DEAD,    .parsed_len = 11, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "hex_sep",              .str = "0x1'F'F",             .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_511,     .parsed_len =  7, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "hex_us_sep",           .str = "0xdead_beef",         .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_DEAD,    .parsed_len = 11, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "oct_777",              .str = "0o777",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_511,     .parsed_len =  5, .BASE = {.base =  8, .limb_width = 21, .limb_val = (lmp_limb_t)OCT_LIMB_VAL }},
  { .name = "oct_upper",            .str = "0O777",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_511,     .parsed_len =  5, .BASE = {.base =  8, .limb_width = 21, .limb_val = (lmp_limb_t)OCT_LIMB_VAL }},
  { .name = "bin_1010",             .str = "0b1010",              .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_1010,    .parsed_len =  6, .BASE = { .base = 2, .limb_width = 64, .limb_val = (lmp_limb_t)BIN_LIMB_VAL }},
  { .name = "bin_upper",            .str = "0B1010",              .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_1010,    .parsed_len =  6, .BASE = { .base = 2, .limb_width = 64, .limb_val = (lmp_limb_t)BIN_LIMB_VAL }},
  { .name = "flt_1_0",              .str = "1.0",                 .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_1,       .parsed_len =  3, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "flt_neg_1_5",          .str = "-1.5",                .expected_size = -2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "flt_0_25",             .str = "0.25",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_25,      .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "flt_tail0",            .str = "0.17000e3",           .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_170,     .parsed_len =  9, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "flt_pi",               .str = "3.14159265358979",    .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_314,     .parsed_len = 16, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_e10",              .str = "1e10",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_1e10,    .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_E10",              .str = "1E10",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_1e10,    .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_e_pos",            .str = "1e+10",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_1e10,    .parsed_len =  5, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_e_neg",            .str = "1e-10",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_1eN10,   .parsed_len =  5, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_neg_float",        .str = "-1.5e-3",             .expected_size = -1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_N15eN3,  .parsed_len =  7, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "exp_hex_p",            .str = "0x1p4",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_0x1P4,   .parsed_len =  5, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "exp_hex_frac",         .str = "0x1.8p3",             .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp =  0, .expected_limbs = L_C,       .parsed_len =  7, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "exp_hex_P_neg",        .str = "0x1P-2",              .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_0x1PN2,  .parsed_len =  6, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "suf_sll<C-INT>",       .str = "123sll<C-INT>",       .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,             128), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 13, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_ull<C-INT>",       .str = "123ull<C-INT>",       .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,            128), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 13, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_f32<IEEE754>",     .str = "1.5f32<IEEE754>",     .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,     32), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len = 15, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_d64<C-INT>",       .str = "123d64<C-INT>",       .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 13, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_ls<C-INT>",        .str = "123ls<C-INT>",        .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 12, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_32f<IEEE754>",     .str = "12332f<IEEE754>",     .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_INT,                 0), .expected_exp =  0, .expected_limbs = L_12332,   .parsed_len = 15, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_hex_ull<C-INT>",   .str = "0xFFull<C-INT>",      .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,            128), .expected_exp =  0, .expected_limbs = L_255,     .parsed_len = 14, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "suf_l<C-INT>",         .str = "123l<C-INT>",         .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 11, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_bin_l<C-INT>",     .str = "0b1010l<C-INT>",      .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_1010,    .parsed_len = 14, .BASE = { .base = 2, .limb_width = 64, .limb_val = (lmp_limb_t)BIN_LIMB_VAL }},
  { .name = "suf_u<C-INT>",         .str = "123u<C-INT>",         .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,              0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 11, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_f<IEEE754>",       .str = "1.5f<IEEE754>",       .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,      0), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len = 13, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_d<C-INT>",         .str = "123d<C-INT>",         .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len = 11, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_sl",               .str = "123sl",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  5, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_f32",              .str = "1.5f32",              .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD,MP_NUM_REPR_IEEE754_FLOAT,      32), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len =  6, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_d64",              .str = "123d64",              .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  6, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_l",                .str = "123l",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_L",                .str = "123L",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_hex_l",            .str = "0xFFl",               .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,              64), .expected_exp =  0, .expected_limbs = L_255,     .parsed_len =  5, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "suf_s",                .str = "123s",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_u",                .str = "123u",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_f",                .str = "1.5f",                .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_g",                .str = "1.5g",                .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,                0), .expected_exp = -1, .expected_limbs = L_15eLIMB, .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "suf_d",                .str = "123d",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                  0), .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "full_neg_hex",         .str = "-0xFFull<C-INT>",     .expected_size = -1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_UINT,            128), .expected_exp =  0, .expected_limbs = L_255,     .parsed_len = 15, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "full_pos_float",       .str = "+1.5e-3f32<IEEE754>", .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,     32), .expected_exp = -1, .expected_limbs = L_15eN3,   .parsed_len = 19, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "full_hex_float",       .str = "0x1.8p3f32<IEEE754>", .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,     32), .expected_exp =  0, .expected_limbs = L_C,       .parsed_len = 19, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "full_hex_us_L",        .str = "-0xdead_beefL",       .expected_size = -1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD, MP_NUM_REPR_STD_C_INT,             64), .expected_exp =  0, .expected_limbs = L_DEAD,    .parsed_len = 13, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "full_X_P_g<IEEE754>",  .str = "0X1P-2g<IEEE754>",    .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_IEEE754_STANDARD, MP_NUM_REPR_IEEE754_FLOAT,      0), .expected_exp = -1, .expected_limbs = L_0x1PN2,  .parsed_len = 16, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "err_empty",            .str = " ",                   .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  0, .limb_width =  0, .limb_val = (lmp_limb_t)0            }},
  { .name = "err_sign_only",        .str = "+",                   .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  0, .limb_width =  0, .limb_val = (lmp_limb_t)0            }},
  { .name = "err_hex_no_digit",     .str = "0x",                  .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "err_bin_2",            .str = "0b2",                 .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  2, .limb_width = 64, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "err_oct_8",            .str = "0o8",                 .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  8, .limb_width = 21, .limb_val = (lmp_limb_t)OCT_LIMB_VAL }},
  { .name = "err_hex_G",            .str = "0xG",                 .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "err_two_dot",          .str = "1.2.3",               .expected_size =  2, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_FLOAT,               0),  .expected_exp = -1, .expected_limbs = L_1_2,     .parsed_len =  3, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_exp_no_digit",     .str = "1e",                  .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                 0),  .expected_exp =  0, .expected_limbs = L_1,       .parsed_len =  1, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_exp_plus",         .str = "1e+",                 .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                 0),  .expected_exp =  0, .expected_limbs = L_1,       .parsed_len =  1, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_double_dot",       .str = "1..2",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                 0),  .expected_exp =  0, .expected_limbs = L_1,       .parsed_len =  1, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_unknown_suffix",   .str = "123x",                .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD,MP_NUM_REPR_MP_INT,                 0),  .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  4, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_dup_size",         .str = "123lll",              .expected_size =  1, .expected_type = MP_NUM_TYPE_OF(MP_NUM_INT_STANDARD,MP_NUM_REPR_STD_C_INT,            128),  .expected_exp =  0, .expected_limbs = L_123,     .parsed_len =  5, .BASE = {.base = 10, .limb_width = 19, .limb_val = (lmp_limb_t)DEC_LIMB_VAL }},
  { .name = "err_sep_start",        .str = "'123",                .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  0, .limb_width =  0, .limb_val = (lmp_limb_t)0            }},
  { .name = "err_double_sign",      .str = "--123",               .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  0, .limb_width =  0, .limb_val = (lmp_limb_t)0            }},
  { .name = "err_mixed_sign",       .str = "+-123",               .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  0, .limb_width =  0, .limb_val = (lmp_limb_t)0            }},
  { .name = "err_bin_prefix",       .str = "0b",                  .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  2, .limb_width = 64, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
  { .name = "err_oct_prefix",       .str = "0o",                  .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base =  8, .limb_width = 21, .limb_val = (lmp_limb_t)OCT_LIMB_VAL }},
  { .name = "err_X_prefix",         .str = "0X",                  .expected_size =  0, .expected_type = 0,                                                                          .expected_exp =  0, .expected_limbs = nullptr,   .parsed_len =  0, .BASE = {.base = 16, .limb_width = 16, .limb_val = (lmp_limb_t)LMP_LIMB_VAL }},
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
