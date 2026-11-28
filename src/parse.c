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

#include "LMP/parse.h"
#include "LMP/spec.h"
#include "LMP/arith.h"
#include <allocator.h>

#include "arith/mp-base-number.h"

enum LMP_PARSE_MODE_MASK_TYPE : uint8_t {
  LMP_PARSE_SIGN_MASK = (uint8_t)~LMP_PARSE_NO_SIGN,
  LMP_PARSE_SEPA_MASK = (uint8_t)~LMP_PARSE_NO_SEPA,
  LMP_PARSE_REPR_MASK = (uint8_t)~LMP_PARSE_NO_REPR,
  LMP_PARSE_SIZE_MASK = (uint8_t)~LMP_PARSE_NO_SIZE,
  LMP_PARSE_PREFIX_MASK = (uint8_t)~LMP_PARSE_NO_PREFIX,
  LMP_PARSE_SUFFIX_MASK = (uint8_t)~LMP_PARSE_NO_SUFFIX,
};

struct limb_pos {
  const char * start;
  const char * end;
};

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define bitsof(w)          (sizeof(w) * CHAR_BIT)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))
#define abs(a)             ((a) > (0) ? (a) : -(a))
static void limb_digit_parse_adic16(const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic10(const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic8 (const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic2 (const struct limb_pos *pos, lmp_limb_t *value);
static uint32_t LMP_parse_num_suffix(const char *input, bool is_float, uint32_t adic_type, MP_NUM_TYPE *num_type);

#define isSign(pText)             ((*(pText) == '-') || (*(pText) == '+'))
#define isDot(pText)              ((*(pText) == '.'))
#define isHexLetter(pText)        (('a' <= (pText)[0] && (pText)[0] <= 'f') || ('A' <= (pText)[0] && (pText)[0] <= 'F'))
#define isIdentChar(pText)          (startswithLetter(pText) || isDecDigital(pText) || (*(pText) == '_') || (*(pText) == '-'))
#define startswithLetter(pText)   (('a' <= (pText)[0] && (pText)[0] <= 'z') || ('A' <= (pText)[0] && (pText)[0] <= 'Z'))
#define isSeparator(pText)        (*(pText) == '\'' || *(pText) == '_')
#define isExponer(pText)          (*(pText) == 'e' || *(pText) == 'E' || *(pText) == 'p' || *(pText) == 'P')
#define isDecmier(pText)          (*(pText) == 'd' || *(pText) == 'D')
#define isFloater(pText)          (*(pText) == 'f' || *(pText) == 'F' || *(pText) == 'g' || *(pText) == 'G')
#define isSigned(pText)           (*(pText) == 's' || *(pText) == 'S')
#define isUnsigned(pText)         (*(pText) == 'u' || *(pText) == 'U')
#define isSizeHinter(pText)       (*(pText) == 'l' || *(pText) == 'L')
#define isHyphen(pText)           (*(pText) == '-' || *(pText) == '_')
#define isHexPrefix(pText)        (*(pText) == 'x' || *(pText) == 'X')
#define isDecPrefix(pText)        (*(pText) == 'd' || *(pText) == 'D')
#define isOctPrefix(pText)        (*(pText) == 'o' || *(pText) == 'O')
#define isBinPrefix(pText)        (*(pText) == 'b' || *(pText) == 'B')
#define isLAngleBracket(pText)    (*(pText) == '<')
#define isRAngleBracket(pText)    (*(pText) == '>')

static bool isBinDigital(const char *pText) { return ('0' == pText[0] || pText[0] == '1'); }
static bool isOctDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '7'); }
static bool isDecDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '9'); }
static bool isHexDigital(const char *pText) { return isDecDigital(pText) || isHexLetter(pText); }

uint32_t stridx_o(char chr, const char * str);
static uint32_t strcmp_o(const char * str1, const char * str2);
static uint32_t try_special_number(const char *input, MP_NUM_TYPE *type, MP_BASE_NUMBER *NUMBER, bool negative);

static uint32_t LMP_parse_num_digits(const char *input, MP_NUM_TYPE *num_type, MP_BASE_NUMBER *NUMBER,
                                      bool negative, uint32_t adic_type, const Allocator *allocator);

extern const struct MP_STANDARD MP_NUM_INT_STANDARD_LIST[MP_NUM_INT_STANDARD_COUNT];
extern const struct MP_STANDARD MP_NUM_FLOAT_STANDARD_LIST[MP_NUM_FLOAT_STANDARD_COUNT];
extern const struct MP_STANDARD_REPR MP_NUM_INT_STD_REPR__LIST[MP_NUM_INT_STANDARD_COUNT];
extern const struct MP_STANDARD_REPR MP_NUM_FLOAT_STD_REPR__LIST[MP_NUM_FLOAT_STANDARD_COUNT];
static const struct {
  MP_BASE BASE;
  void (*const limb_parser)(const struct limb_pos *, lmp_limb_t *);
  bool (*const digit_checker)(const char *);
} BASE_LIMB_DATA[] = {
  [ADIC_TYPE_16] = { .BASE = { .base = 16, .limb_width = HEX_LIMB_WIDTH, .limb_val = (lmp_limb_t)HEX_LIMB_VAL },
                        .limb_parser = limb_digit_parse_adic16, .digit_checker = isHexDigital },
  [ADIC_TYPE_10] = { .BASE = { .base = 10, .limb_width = DEC_LIMB_WIDTH, .limb_val = (lmp_limb_t)DEC_LIMB_VAL },
                        .limb_parser = limb_digit_parse_adic10, .digit_checker = isDecDigital },
  [ADIC_TYPE_8 ] = { .BASE = { .base =  8, .limb_width = OCT_LIMB_WIDTH, .limb_val = (lmp_limb_t)OCT_LIMB_VAL },
                        .limb_parser = limb_digit_parse_adic8 , .digit_checker = isOctDigital },
  [ADIC_TYPE_2 ] = { .BASE = { .base =  2, .limb_width = BIN_LIMB_WIDTH, .limb_val = (lmp_limb_t)BIN_LIMB_VAL },
                        .limb_parser = limb_digit_parse_adic2 , .digit_checker = isBinDigital },
};

// [a-fA-F0-9'_]+
inline void limb_digit_parse_adic16(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if ('0' <= *pText && *pText <= '9') {
      val = (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      val = (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      val = (*pText - 'A' + 0xA);
    } else if (isSeparator(pText) || isDot(pText)) {
      continue;
    } else { break; }
    *value = ((*value) << 4) + val;
  }
}
// [0-9'_]+
inline void limb_digit_parse_adic10(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isDecDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText) || isDot(pText)) {
      continue;
    } else { break; }
    *value = (*value) * 10 + val;
  }
}
// [0-7'_]+
inline void limb_digit_parse_adic8(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isOctDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText) || isDot(pText)) {
      continue;
    } else { break; }
    *value = ((*value) << 3) + val;
  }
}
// [01'_]+
inline void limb_digit_parse_adic2(const struct limb_pos *pos, lmp_limb_t *value) {
  for (const char *pText = pos->start; pText < pos->end; pText ++) {
    uint32_t val = 0;
    if (isBinDigital(pText)) {
      val = (*pText - '0');
    } else if (isSeparator(pText) || isDot(pText)) {
      continue;
    } else { break; }
    *value = ((*value) << 1) + val;
  }
}

// <REPR>?([lL]{0,2}|[0-9]+)<REPR>?\<standard\>
inline uint32_t LMP_parse_num_suffix(const char *const input, const bool is_float, const uint32_t adic_type,
                                     MP_NUM_TYPE *const num_type) {
  const char *pText = input;
  char repr_chr = 0;
  lmp_limb_t size = 0;
  uint8_t stdd_index = -1;
  uint8_t stdd_id = MP_NUM_UNKNOWN_STANDARD;
  auto const STD_LIST = is_float ? MP_NUM_FLOAT_STANDARD_LIST : MP_NUM_INT_STANDARD_LIST;
  auto const STD_REPR_LIST = is_float ? MP_NUM_FLOAT_STD_REPR__LIST : MP_NUM_INT_STD_REPR__LIST;
  auto const STD_COUNT = is_float ? MP_NUM_FLOAT_STANDARD_COUNT : MP_NUM_INT_STANDARD_COUNT;

  /***************************************************/
  /************** suffix parse ***********************/
  /***************************************************/
  // first <REPR>
  if (startswithLetter(pText) && !isSizeHinter(pText)) { repr_chr = *pText; pText ++; }
  // size
  if (isSizeHinter(pText)) { size += (1 << bitsof(uint16_t)); pText ++;
    if (isSizeHinter(pText)) { size += (1 << bitsof(uint16_t));  pText ++; }
  } else if (repr_chr && isDecDigital(pText)) {
    struct limb_pos curr_limb = { .start = pText, .end = nullptr };
    while (*pText) {
      if (isSeparator(pText)) {
        if (!isDecDigital(pText + 1)) { break; }
      }else if (!isDecDigital(pText)) { break; }
      pText++;
    }
    curr_limb.end = pText;
    limb_digit_parse_adic10(&curr_limb, &size);
    size &= UINT16_MAX;
  }
  // second <REPR>
  if (!repr_chr && startswithLetter(pText) && !isSizeHinter(pText)) { repr_chr = *pText; pText ++; }
  if (isDecmier(&repr_chr) && adic_type != ADIC_TYPE_10 && is_float) { repr_chr = 'F'; }
  if (!repr_chr) { repr_chr = is_float ? 'F' : 'S'; }
  // standard
  const char *pText_back = pText;
  if (isLAngleBracket(pText)) {
    pText ++;
    bool stdd_matched = false;
    for (size_t i = 0; i < STD_COUNT; i++) {
      auto const stdd = STD_LIST[i];
      if (!stdd.name) { continue; }
      if (strcmp_o(pText, stdd.name) == stdd.leng) {
        if (!isRAngleBracket(pText + stdd.leng)) { continue; }
        stdd_id = stdd.stdd;
        stdd_index = i;
        pText += stdd.leng + 1;
        stdd_matched = true;
        break;
      }
    }
    if (!stdd_matched) {
      if (startswithLetter(pText)) {
        pText ++;
        while (isIdentChar(pText)) { pText ++; }
        if (isRAngleBracket(pText)) { pText ++; }
        else { pText = pText_back; }
      } else{ pText = pText_back; }
    }
  }

  /***************************************************/
  /************** suffix compose *********************/
  /***************************************************/
  // stdd
  if (stdd_id == MP_NUM_UNKNOWN_STANDARD) {
    if (size > 0) {
      if (is_float) { stdd_id = MP_NUM_IEEE754_STANDARD; stdd_index = MP_NUM_FLOAT_STD_IEEE754; }
      else { stdd_id = MP_NUM_INT_STANDARD; stdd_index = MP_NUM_INT_STD_C; }
    } else { stdd_id = MP_NUM_LMP_STANDARD; stdd_index = MP_NUM_INT_STD_LMP; }
  }
  auto const STD_REPR = STD_REPR_LIST[stdd_index];
  // size
  if (size > UINT16_MAX) { size >>= bitsof(uint16_t); size = (size > 1) ? STD_REPR.LL : STD_REPR.L; }
  // repr
  uint8_t repr = MP_NUM_REPR_NONE;
  for (uint32_t i = 0; i < STD_REPR.count; i++) {
    auto const item = STD_REPR.items[i];
    if (repr_chr == item.chr) { repr = item.repr; break; }
  }
  if (repr == MP_NUM_REPR_NONE) {
    repr = STD_REPR.items[STD_REPR.fallback_index].repr;
  }
  num_type->type = MP_NUM_TYPE_OF(stdd_id, repr, size);

  return pText - input;
}

//
// switch @adic_type:
//    case ADIC_TYPE_16:
//        [a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?)
//    case ADIC_TYPE_2 :
//        [01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?)
//    case ADIC_TYPE_8 :
//        [0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?)
//    case ADIC_TYPE_10:
//        [0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?)
inline uint32_t LMP_parse_num_digits(const char *const input, MP_NUM_TYPE *num_type, MP_BASE_NUMBER *const NUMBER,
                                     const bool negative, const uint32_t adic_type, const Allocator *const allocator) {
  if (!*input) { return 0; }
  auto const fun_check = BASE_LIMB_DATA[adic_type].digit_checker;
  auto const limb_width = BASE_LIMB_DATA[adic_type].BASE.limb_width;
  auto const func_limb_parse = BASE_LIMB_DATA[adic_type].limb_parser;

  /***************************************************/
  /************** NUMBER parse ***********************/
  /***************************************************/

  const char *pText = input;
  bool is_float = false;
  uint32_t mantissa_len = 0;
  uint32_t int_part_len = 0;
 struct limb_pos curr_limb = { .start = pText, .end = nullptr };
  MP_BASE_NUMBER_init(NUMBER, LMP_PARSE_ALLOC_SCALE * MP_ALLOC_LEN, BASE_LIMB_DATA[adic_type].BASE.base, allocator);
  while (*pText && NUMBER->size <= LMP_PARSE_PREC_MAX) {
    if (isSeparator(pText)) {
      if (!fun_check(pText + 1)) { break; }
      pText ++; continue;
    }
    if (!is_float && (isDot(pText) && fun_check(pText + 1))) {
      int_part_len = mantissa_len;
      is_float = true;
      pText ++;
      continue;
    }
    if (!fun_check(pText)) { break; }
    mantissa_len ++;
    pText++;

    if (mantissa_len % limb_width == 0) {
      curr_limb.end = pText;
      lmp_limb_t limb_val = 0;
      func_limb_parse(&curr_limb, &limb_val);
      if (NUMBER->prec < NUMBER->size + 1) {
        MP_INT_realloc((MP_INT *)NUMBER, NUMBER->size + 1, allocator);
      }
      NUMBER->limbs[NUMBER->size] = limb_val;
      NUMBER->size++;
      curr_limb.start = pText;
    }
  }
  if (pText == input) {
    MP_INT_clear((MP_INT *)NUMBER, allocator);
    return 0;
  }
  for (int32_t i = 0; i < NUMBER->size / 2; i++) {
    const lmp_limb_t tmp = NUMBER->limbs[i];
    NUMBER->limbs[i] =  NUMBER->limbs[NUMBER->size - 1 - i];
    NUMBER->limbs[NUMBER->size - 1 - i] = tmp;
  }
  if (!int_part_len) { int_part_len = mantissa_len; }
  curr_limb.end = pText;
  if (curr_limb.end > curr_limb.start) {
    lmp_limb_t limb_val = 0;
    func_limb_parse(&curr_limb, &limb_val);
    if (NUMBER->prec < NUMBER->size + 1) {
      MP_INT_realloc((MP_INT *)NUMBER, NUMBER->size + 1, allocator);
    }
    const uint32_t remain_width = mantissa_len % NUMBER->BASE.limb_width;
    MP_BASE_INT_left_shift(NUMBER, remain_width, allocator);
    MP_BASE_INT_abs_add_limb(NUMBER, NUMBER, limb_val, allocator);
  }
  NUMBER->size = negative ? - NUMBER->size : NUMBER->size;
  MP_INT_clamp((MP_INT *)NUMBER);

  lmp_exp_t literal_exp = 0;
  if (isExponer(pText)) {
    const char *pText_back = pText;
    pText ++;
    const bool negative_exp = (*pText == '-');
    if (isSign(pText)) { pText++; }
    curr_limb.start = pText;
    while (*pText) {
      if (isSeparator(pText)) {
        if (!isDecDigital(pText + 1)) { break; }
      }else if (!isDecDigital(pText)) { break; }
      pText++;
    }
    curr_limb.end = pText;
    if (curr_limb.end > curr_limb.start) {
      is_float = true;
      lmp_limb_t exp_val = 0;
      limb_digit_parse_adic10(&curr_limb, &exp_val);
      literal_exp = negative_exp ? - (lmp_exp_t) exp_val : (lmp_exp_t) exp_val;
    } else {
      pText = pText_back;
    }
  }

  if ((startswithLetter(pText) && !isExponer(pText)) || isLAngleBracket(pText)) {
    pText += LMP_parse_num_suffix(pText, is_float, adic_type, num_type);
  } else if (is_float) {
    num_type->type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_FLOAT, 0);
  } else {
    num_type->type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_INT, 0);
  }

  /***************************************************/
  /************** NUMBER compose *********************/
  /***************************************************/

  literal_exp -= (lmp_exp_t) (mantissa_len - int_part_len);
  if (literal_exp < - LMP_PARSE_EXP_MAX) {
    MP_INT_clear((MP_INT *)NUMBER, allocator);
    const lmp_prec_t prec = NUMBER->prec;
    lmp_limb_t * limbs = NUMBER->limbs;
    NUMBER->prec = prec;
    NUMBER->size = 0;
    NUMBER->exp = negative ? MP_NUMBER_NEG_INS : MP_NUMBER_INS;
    NUMBER->limbs = limbs;
  } else if (literal_exp > LMP_PARSE_EXP_MAX) {
    MP_INT_clear((MP_INT *)NUMBER, allocator);
    const lmp_prec_t prec = NUMBER->prec;
    lmp_limb_t * limbs = NUMBER->limbs;
    NUMBER->prec = prec;
    NUMBER->size = 0;
    NUMBER->exp = negative ? MP_NUMBER_NEG_INF : MP_NUMBER_INF;
    NUMBER->limbs = limbs;
  } else {
    MP_BASE_FLOAT_mul_base_exp(NUMBER, literal_exp, allocator);
    MP_BASE_FLOAT_tidy(NUMBER);
  }
  return pText - input;
}

static struct {
  const char * pattern;
  lmp_exp_t    value;
  uint32_t pattern_len;
} SPECIAL_NUMBER_PATTERNS[] = {
  { .pattern = "inf", .pattern_len = lenof("inf"), .value = MP_NUMBER_INF },
  { .pattern = "INF", .pattern_len = lenof("INF"), .value = MP_NUMBER_INF },
  { .pattern = "ins", .pattern_len = lenof("ins"), .value = MP_NUMBER_INS },
  { .pattern = "INS", .pattern_len = lenof("INS"), .value = MP_NUMBER_INS },
  { .pattern = "nan", .pattern_len = lenof("nan"), .value = MP_NUMBER_NAN },
  { .pattern = "NAN", .pattern_len = lenof("NAN"), .value = MP_NUMBER_NAN },
};

inline uint32_t try_special_number(const char *input, MP_NUM_TYPE *type, MP_BASE_NUMBER *NUMBER, const bool negative) {
  constexpr int32_t n_special_val = sizeof(SPECIAL_NUMBER_PATTERNS) / sizeof(SPECIAL_NUMBER_PATTERNS[0]);
  for (int32_t i = 0; i < n_special_val; i++) {
    auto const triple = SPECIAL_NUMBER_PATTERNS[i];
    if (strcmp_o(input, triple.pattern) == triple.pattern_len && !isIdentChar(&input[triple.pattern_len])) {
      NUMBER->size = 0;
      NUMBER->exp = triple.value == MP_NUMBER_NAN ? MP_NUMBER_NAN : negative ? - triple.value : triple.value;
      type->type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_FLOAT, 0);
      return triple.pattern_len;
    }
  }
  return 0;
}

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

// [+-]?0[xX][a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?) |
// [+-]?0[bB][01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?) |
// [+-]?0[oO][0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?) |
// [+-]?[0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?((REPR)?([lL]{0,2}|[0-9]+)(REPR)?<standard>?)
uint32_t LMP_parse_base_number(const char * const input, MP_NUM_TYPE *type,
                               MP_BASE_NUMBER *NUMBER, const Allocator * const allocator) {
  const char *pText = input;
  const bool negative = ('-' == *pText);
  if (isSign(pText)) {
    pText++;
    while (*pText == ' ' || *pText == '\t') { pText++; }
  }
  uint32_t adic_type = ADIC_TYPE_10;
  if (isDecDigital(pText)) {
    if (*pText == '0') {
      switch (pText[1]) {
        case 'x': case 'X': { pText += 2; adic_type = ADIC_TYPE_16; break; }
        case 'o': case 'O': { pText += 2; adic_type = ADIC_TYPE_8 ; break; }
        case 'b': case 'B': { pText += 2; adic_type = ADIC_TYPE_2 ; break; }
        default: {  }
      }
    }
    allocator->memcpy(&NUMBER->BASE, &BASE_LIMB_DATA[adic_type].BASE, sizeof(MP_BASE));
    const uint32_t length = LMP_parse_num_digits(pText, type, NUMBER, negative, adic_type, allocator);
    if (!length) { return 0; }
    return pText + length - input;
  }
  const uint32_t length = try_special_number(pText, type, NUMBER, negative);
  if (length == 0) { return 0; }
  allocator->memcpy(&NUMBER->BASE, &BASE_LIMB_DATA[adic_type].BASE, sizeof(MP_BASE));
  return length;
}
