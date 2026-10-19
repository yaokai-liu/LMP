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
 * Module Name: 
 * Filename: parse.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-14
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "arith.h"
#include "lmp.h"
#include "array.h"

struct limb_pos {
  const char * start;
  const char * end;
};

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))
static void t_INT_LIMB_adic16(const struct limb_pos *pos, lmp_limb_t *value);
static void t_INT_LIMB_adic10(const struct limb_pos *pos, lmp_limb_t *value);
static void t_INT_LIMB_adic8 (const struct limb_pos *pos, lmp_limb_t *value);
static void t_INT_LIMB_adic2 (const struct limb_pos *pos, lmp_limb_t *value);
static void scale_limb_adic16(MP_INT *op, const Allocator *allocator);
static void scale_limb_adic10(MP_INT *op, const Allocator *allocator);
static void scale_limb_adic8 (MP_INT *op, const Allocator *allocator);
static void scale_limb_adic2 (MP_INT *op, const Allocator *allocator);

static uint32_t strcmp_o(const char * str1, const char * str2);
static uint32_t stridx_o(char chr, const char * str);

#define isSign(pText)             ((*pText == '-') || (*pText == '+'))
#define isDot(pText)              ((*pText == '.'))
#define isHexLetter(pText)        (('a' <= (pText)[0] && (pText)[0] <= 'f') || ('A' <= (pText)[0] && (pText)[0] <= 'F'))
#define isIdentChar(pText)          (startswithLetter(pText) || isDecDigital(pText) || (*pText == '_') || (*pText == '-'))
#define startswithLetter(pText)   (('a' <= (pText)[0] && (pText)[0] <= 'z') || ('A' <= (pText)[0] && (pText)[0] <= 'Z'))
#define isSeparator(pText)        (*(pText) == '\'' || *(pText) == '_')

static bool isBinDigital(const char *pText) { return ('0' == (pText)[0] || (pText)[0] == '1'); }
static bool isOctDigital(const char *pText) { return ('0' <= (pText)[0] && (pText)[0] <= '7'); }
static bool isDecDigital(const char *pText) { return ('0' <= (pText)[0] && (pText)[0] <= '9'); }
static bool isHexDigital(const char *pText) { return (isDecDigital(pText) || isHexLetter(pText)); }

enum LMP_ADIC_TYPE {
  ADIC_TYPE_16 = 0,
  ADIC_TYPE_10 = 1,
  ADIC_TYPE_8  = 2,
  ADIC_TYPE_2  = 3,
};

enum LMP_DIGIT_TYPE {
  DIGITAL_INT,
  DIGITAL_FRAC,
};

static constexpr uint32_t ADIC_BASE[] = {
  [ADIC_TYPE_16] = 16,
  [ADIC_TYPE_10] = 10,
  [ADIC_TYPE_8 ] = 8,
  [ADIC_TYPE_2 ] = 2,
};
static constexpr uint32_t MAX_DIGIT_PER_U64[] = {
  [ADIC_TYPE_16] = (LMP_LIMB_BITS / 4),
  [ADIC_TYPE_10] = (19 * 64 / LMP_LIMB_BITS),
  [ADIC_TYPE_8 ] = (LMP_LIMB_BITS / 3),
  [ADIC_TYPE_2 ] = LMP_LIMB_BITS,
};
static void (*const DIGITAL_FUNC_TOOLS[4][2])(const struct limb_pos *, lmp_limb_t *) = {
  [ADIC_TYPE_16] = { [DIGITAL_INT] = t_INT_LIMB_adic16, [DIGITAL_FRAC] = nullptr},
  [ADIC_TYPE_10] = { [DIGITAL_INT] = t_INT_LIMB_adic10, [DIGITAL_FRAC] = nullptr},
  [ADIC_TYPE_8 ] = { [DIGITAL_INT] = t_INT_LIMB_adic8 , [DIGITAL_FRAC] = nullptr},
  [ADIC_TYPE_2 ] = { [DIGITAL_INT] = t_INT_LIMB_adic2 , [DIGITAL_FRAC] = nullptr},
};
static void (*const SCALE_LIMB_FUNCS[])(MP_INT *, const Allocator *) = {
  [ADIC_TYPE_16] = scale_limb_adic16,
  [ADIC_TYPE_10] = scale_limb_adic10,
  [ADIC_TYPE_8 ] = scale_limb_adic8 ,
  [ADIC_TYPE_2 ] = scale_limb_adic2 ,
};
static  bool (*const ADIC_CHECK_FUNCS[])(const char *) = {
  [ADIC_TYPE_16] = isHexDigital,
  [ADIC_TYPE_10] = isDecDigital,
  [ADIC_TYPE_8 ] = isOctDigital,
  [ADIC_TYPE_2 ] = isBinDigital,
};

// [a-fA-F0-9'_]+
inline void t_INT_LIMB_adic16(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if ('0' <= *pText && *pText <= '9') {
      val = (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      val = (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      val = (*pText - 'A' + 0xA);
    } else if (isSeparator(pText)) {
      pText ++; continue;
    } else { break; }
    *value = ((*value) << 4) + val;
    pText ++;
  }
}
// [0-9']+
inline void t_INT_LIMB_adic10(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isDecDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText)) {
      pText ++; continue;
    } else { break; }
    *value = (*value) * 10 + val;
    pText ++;
  }
}
// [0-7'_]+
inline void t_INT_LIMB_adic8(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isOctDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText)) {
      pText ++; continue;
    } else { break; }
    *value = ((*value) << 3) + val;
    pText ++;
  }
}
// [01'_]+
inline void t_INT_LIMB_adic2(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isBinDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText)) {
      pText ++; continue;
    } else { break; }
    *value = ((*value) << 1) + val;
    pText ++;
  }
}

inline void scale_limb_adic16(MP_INT *op, const Allocator *allocator) {
  return MP_INT_left_shift(op, LMP_LIMB_BITS, allocator);
}

inline void scale_limb_adic10(MP_INT *op, const Allocator *allocator) {
  return MP_INT_abs_mul_u64(op, op, 1'0000'0000'0000'0000'000LLU, allocator);
}

inline void scale_limb_adic8(MP_INT *op, const Allocator *allocator) {
  return MP_INT_left_shift(op, 63, allocator);
}

inline void scale_limb_adic2(MP_INT *op, const Allocator *allocator) {
  return MP_INT_left_shift(op, LMP_LIMB_BITS, allocator);
}

static uint32_t LMP_parse_digits(const char *input, MP_NUMBER *NUMBER,
                                 uint32_t adic_type, const Allocator *allocator);

inline uint32_t LMP_parse_digits(const char *const input, MP_NUMBER *const ,
                                 const uint32_t adic_type, const Allocator *const allocator) {
  auto const fun_check = ADIC_CHECK_FUNCS[adic_type];
  auto const limb_effective_len = MAX_DIGIT_PER_U64[adic_type];
  auto const func_scale_limb = SCALE_LIMB_FUNCS[adic_type];
  auto const func_int_limb_parse = DIGITAL_FUNC_TOOLS[adic_type][DIGITAL_INT];
  Array *limb_pos_array = Array_new(sizeof(struct limb_pos), 1, allocator);
  const char *pText = input;
  bool is_float = false;
  uint32_t effective_int_len = 0;
  uint32_t effective_len = 0;
  struct limb_pos curr_limb = {pText, nullptr};
  while (*pText) {
    if (isSeparator(pText)) {
      if (!fun_check(pText + 1)) { break; }
      pText ++; continue;
    }
    if (isDot(pText)) {
      effective_int_len = effective_len;
      is_float = true;
      pText ++;
      continue;
    }
    if (!fun_check(pText)) { break; }
    effective_len ++;
    pText++;

    if (effective_len % limb_effective_len == 0) {
      curr_limb.end = pText;
      Array_append(limb_pos_array, &curr_limb, 1);
      curr_limb.start = pText;
    }
  }
  curr_limb.end = pText;
  if (curr_limb.end > curr_limb.start) {
    Array_append(limb_pos_array, &curr_limb, 1);
  }

  if (Array_length(limb_pos_array) <= 2) {
    // TODO:
  }

  MP_INT INT_NUM = {};
  const uint32_t n_positions = Array_length(limb_pos_array);
  MP_INT_init(&INT_NUM, (int32_t) n_positions, allocator);
  const struct limb_pos *const positions =  Array_first_real(limb_pos_array);
  if (effective_len % limb_effective_len == 0) {
    for (uint32_t i = 0; i < n_positions; i++) {
      lmp_limb_t limb_val = 0;
      func_int_limb_parse(&positions[i], &limb_val);
      func_scale_limb(&INT_NUM, allocator);
      MP_INT_abs_add_u64(&INT_NUM, &INT_NUM, limb_val, allocator);
    }
  } else {
    for (uint32_t i = 0; i < n_positions - 1; i++) {
      lmp_limb_t limb_val = 0;
      func_int_limb_parse(&positions[i], &limb_val);
      func_scale_limb(&INT_NUM, allocator);
      MP_INT_abs_add_u64(&INT_NUM, &INT_NUM, limb_val, allocator);
    }
    lmp_limb_t scale = 1;
    lmp_limb_t limb_val = 0;
    const uint32_t len = effective_len % limb_effective_len;
    for (uint32_t i = 0 ; i < len; i++) { scale *= ADIC_BASE[adic_type]; }
    MP_INT_abs_mul_u64(&INT_NUM, &INT_NUM, scale, allocator);
    func_int_limb_parse(&positions[n_positions - 1], &limb_val);
    MP_INT_abs_add_u64(&INT_NUM, &INT_NUM, limb_val, allocator);
    MP_INT_clamp(&INT_NUM);
  }
  releasePrimeArray(limb_pos_array);

  if (is_float) {
    const uint32_t exp_divisor = effective_len - effective_int_len;
    const uint32_t n_limbs = exp_divisor / limb_effective_len;
    MP_INT DIVISOR = {};
    MP_INT_init(&DIVISOR, (int32_t) n_limbs + 1, allocator);
    DIVISOR.size = 1; DIVISOR.limbs[0] = 1;
    for (uint32_t i = 0; i < n_limbs; i++) {
      func_scale_limb(&DIVISOR, allocator);
    }
    if (exp_divisor % limb_effective_len != 0) {
      lmp_limb_t scale = 1;
      const uint32_t len = exp_divisor % limb_effective_len;
      for (uint32_t i = 0 ; i < len; i++) { scale *= ADIC_BASE[adic_type]; }
      MP_INT_abs_mul_u64(&DIVISOR, &DIVISOR, scale, allocator);
      MP_INT_clamp(&DIVISOR);
    }
    // TODO: divmod
  }

  return pText - input;
}

inline uint32_t LMP_parse_adic_number(const char *const input, uint32_t *num_type, MP_NUMBER *NUMBER,
                                      const uint32_t adic_type, const Allocator *allocator) {
  auto const fun_check = ADIC_CHECK_FUNCS[adic_type];
  auto const limb_effective_len = MAX_DIGIT_PER_U64[adic_type];
  auto const func_scale_limb = SCALE_LIMB_FUNCS[adic_type];
  auto const func_int_limb_parse = DIGITAL_FUNC_TOOLS[adic_type][DIGITAL_INT];
  const char *pText = input;
  if (!fun_check(pText)) { return 0; }
  const uint32_t length = LMP_parse_digits(input, NUMBER, adic_type, allocator);
  if (!isDot(pText + length)) {
    *num_type = MP_NUM_TYPE_INT;
    return length;
  }
  pText += length + 1;
  if (!fun_check(pText)) {
    *num_type = MP_NUM_TYPE_FLOAT;

    return length;
  }
  // TODO
  return 0;
}

/*
 * if adic
 * is ADIC_TYPE_16:    [a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2}
 * is ADIC_TYPE_10:    [01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}
 * is ADIC_TYPE_8:     [0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}
 * is ADIC_TYPE_2:     [0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}
 */


#define tokenize_adic_number(text_off, adic_type) do {          \
  uint32_t length = t_NUMBER(pText + (text_off), type, number,  \
                             negative, (adic_type), allocator); \
  length += pText - input + text_off;                           \
  return length ? length : 0;                                   \
} while(false)

#define try_special_number(text_off, pattern, _type) do {                   \
  if (strcmp_o(pText, &(pattern[text_off])) == (lenof(pattern) - text_off)  \
       && !isIdentChar(pText + lenof(pattern))) {                           \
    *type = _type;                                                          \
    return lenof(pattern);                                                  \
  }                                                                         \
} while (false)
inline uint32_t stridx_o(const char chr, const char * const str) {
  if (!str) { return -1; }
  int len = 0;
  while (chr != str[len] && str[len]) { len++; }
  return len;
}
inline uint32_t strcmp_o(const char *const str1, const char * const str2) {
  uint32_t i = 0;
  while (str1[i] && str2[i] && (str1[i] == str2[i])) { i++; }
  return i;
}

// [+-]?0[xX][a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2} |
// [+-]?0[bB][01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2} |
// [+-]?0[oO][0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2} |
// [+-]?[0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}
uint32_t LMP_parse(const char * const input, uint32_t *type, MP_NUMBER *, const Allocator * const ) {
  const char *pText = input;
  const bool negative = ('-' == *pText);
  if (isSign(pText)) {
    pText++;
    while (*pText == ' ' || *pText == '\t') { pText++; }
  }
  switch (*pText) {
    case 'i': {
      try_special_number(0, "INF", MP_NUM_TYPE_INF);
      return 0;
    }
    case 'I': {
      try_special_number(0, "INF", MP_NUM_TYPE_INF);
      return 0;
    }
    case 'n': {
      try_special_number(0, "nan", MP_NUM_TYPE_NAN);
      return 0;
    }
    case 'N': {
      try_special_number(0, "NAN", MP_NUM_TYPE_NAN);
      return 0;
    }
    case '0': {
      switch (pText[1]) {
        case 'x':
        case 'X': {
          //TODO
        }
        case 'o':
        case 'O': {
          //TODO
        }
        case 'b':
        case 'B': {
          //TODO
        }
        default: {}
      }
    }
    default:;
          //TODO
  }
  // TODO
  return 0;
}
