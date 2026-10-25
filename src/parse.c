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

#include "parse.h"
#include "arith.h"
#include "allocator.h"

struct limb_pos {
  const char * start;
  const char * end;
};

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))
#define abs(a)             ((a) > (0) ? (a) : -(a))
static void limb_digit_parse_adic16(const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic10(const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic8 (const struct limb_pos *pos, lmp_limb_t *value);
static void limb_digit_parse_adic2 (const struct limb_pos *pos, lmp_limb_t *value);
static void scale_adic16(MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic10(MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic8 (MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic2 (MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void float_compose_adic16(MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic10(MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic8 (MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic2 (MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void decimal_compose(MP_NUMBER *NUMBER,  int32_t exp, const Allocator * allocator);

#define isSign(pText)             ((*(pText) == '-') || (*(pText) == '+'))
#define isDot(pText)              ((*(pText) == '.'))
#define isHexLetter(pText)        (('a' <= (pText)[0] && (pText)[0] <= 'f') || ('A' <= (pText)[0] && (pText)[0] <= 'F'))
#define isIdentChar(pText)          (startswithLetter(pText) || isDecDigital(pText) || (*(pText) == '_') || (*(pText) == '-'))
#define startswithLetter(pText)   (('a' <= (pText)[0] && (pText)[0] <= 'z') || ('A' <= (pText)[0] && (pText)[0] <= 'Z'))
#define isSeparator(pText)        (*(pText) == '\'' || *(pText) == '_')
#define isExponer(pText)          (*(pText) == 'e' || *(pText) == 'E' || *(pText) == 'p' || *(pText) == 'P')
#define isDecmier(pText)          (*(pText) == 'd' || *(pText) == 'D')
#define isSizeLimiter(pText)      (*(pText) == 'l' || *(pText) == 'L')

static bool isBinDigital(const char *pText) { return ('0' == pText[0] || pText[0] == '1'); }
static bool isOctDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '7'); }
static bool isDecDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '9'); }
static bool isHexDigital(const char *pText) { return isDecDigital(pText) || isHexLetter(pText); }

static uint32_t strcmp_o(const char * str1, const char * str2);
static uint32_t try_special_number(const char *input, uint32_t *type, MP_NUMBER *NUMBER, bool negative);

static uint32_t LMP_parse_adic_number(const char *input, uint32_t *num_type, MP_NUMBER *NUMBER,
                                      bool negative, uint32_t adic_type, const Allocator *allocator);

enum LMP_ADIC_TYPE : int32_t {
  ADIC_TYPE_16 = 0,
  ADIC_TYPE_10 = 1,
  ADIC_TYPE_8  = 2,
  ADIC_TYPE_2  = 3,
};

static constexpr uint32_t ADIC_BASE[] = {
  [ADIC_TYPE_16] = 16,
  [ADIC_TYPE_10] = 10,
  [ADIC_TYPE_8 ] = 8,
  [ADIC_TYPE_2 ] = 2,
};
static constexpr uint32_t MAX_DIGIT_PER_LIMB[] = {
  [ADIC_TYPE_16] = (LMP_LIMB_BITS / 4),
  [ADIC_TYPE_10] = (19 * 64 / LMP_LIMB_BITS),
  [ADIC_TYPE_8 ] = (LMP_LIMB_BITS / 3),
  [ADIC_TYPE_2 ] = LMP_LIMB_BITS,
};
static void (*const DIGITAL_FUNC_TOOLS[4])(const struct limb_pos *, lmp_limb_t *) = {
  [ADIC_TYPE_16] = limb_digit_parse_adic16,
  [ADIC_TYPE_10] = limb_digit_parse_adic10,
  [ADIC_TYPE_8 ] = limb_digit_parse_adic8 ,
  [ADIC_TYPE_2 ] = limb_digit_parse_adic2 ,
};
static void (*const SCALE_LIMB_FUNCS[])(MP_INT *, uint32_t, const Allocator *) = {
  [ADIC_TYPE_16] = scale_adic16,
  [ADIC_TYPE_10] = scale_adic10,
  [ADIC_TYPE_8 ] = scale_adic8 ,
  [ADIC_TYPE_2 ] = scale_adic2 ,
};
static bool (*const ADIC_DIGIT_CHECK_FUNCS[])(const char *) = {
  [ADIC_TYPE_16] = isHexDigital,
  [ADIC_TYPE_10] = isDecDigital,
  [ADIC_TYPE_8 ] = isOctDigital,
  [ADIC_TYPE_2 ] = isBinDigital,
};
static void (*const ADIC_FLOAT_COMPOSE_FUNCS[])(MP_NUMBER *, int32_t, const Allocator *) = {
  [ADIC_TYPE_16] = float_compose_adic16,
  [ADIC_TYPE_10] = float_compose_adic10,
  [ADIC_TYPE_8 ] = float_compose_adic8 ,
  [ADIC_TYPE_2 ] = float_compose_adic2 ,
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

inline void scale_adic16(MP_INT *op, const uint32_t n_digits, const Allocator *allocator) {
  return MP_INT_left_shift(op, (int32_t) n_digits, allocator);
}

inline void scale_adic10(MP_INT *op, const uint32_t n_digits, const Allocator *allocator) {
  lmp_limb_t scale = 1;
  for (uint32_t i = 0 ; i < n_digits; i++) { scale *= 10; }
  return MP_INT_abs_mul_u64(op, op, scale, allocator);
}

inline void scale_adic8(MP_INT *op, const uint32_t n_digits, const Allocator *allocator) {
  return MP_INT_left_shift(op, (int32_t) n_digits, allocator);
}

inline void scale_adic2(MP_INT *op, const uint32_t n_digits, const Allocator *allocator) {
  return MP_INT_left_shift(op, (int32_t) n_digits, allocator);
}

inline void float_compose_adic16(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  int32_t n_limbs = 0, remains = 0;
  if (exp >= 0) {
    const int32_t n_bits = exp * 4;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS;
    remains = n_bits % (int32_t) LMP_LIMB_BITS;
    MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  } else {
    const int32_t n_bits = exp * 4;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS - (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  MP_INT_clamp(&NUMBER->INT);
  const int32_t prec = NUMBER->INT.alloc;
  const int32_t size = NUMBER->INT.size;
  lmp_limb_t* limbs = NUMBER->INT.limbs;
  NUMBER->FLOAT.prec = prec;
  NUMBER->FLOAT.size = size;
  NUMBER->FLOAT.limbs = limbs;
  NUMBER->FLOAT.exp = n_limbs;
}

inline void float_compose_adic10(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  const int32_t limb_max_digit = (int32_t) MAX_DIGIT_PER_LIMB[ADIC_TYPE_10];
  int32_t prec = abs(NUMBER->INT.alloc);
  if (exp >= 0) {
    int32_t limb_exp = 0;
    const int32_t n_limbs = exp / limb_max_digit;
    const int32_t remains = exp % limb_max_digit;
    for (int32_t i = 0; i < n_limbs; i++) {
      scale_adic10(&NUMBER->INT, MAX_DIGIT_PER_LIMB[ADIC_TYPE_10], allocator);
      if (abs(NUMBER->INT.size) >= prec && i < n_limbs - 1) {
        MP_INT_right_shift(&NUMBER->INT, LMP_LIMB_BITS, allocator);
        limb_exp ++;
      }
    }
    if (remains > 0) {
      if (abs(NUMBER->INT.size) >= prec) {
        MP_INT_right_shift(&NUMBER->INT, LMP_LIMB_BITS, allocator);
      }
      scale_adic10(&NUMBER->INT, remains, allocator);
    }
    MP_INT_clamp(&NUMBER->INT);
    prec = NUMBER->INT.alloc;
    const int32_t size = NUMBER->INT.size;
    lmp_limb_t* limbs = NUMBER->INT.limbs;
    NUMBER->FLOAT.prec = prec;
    NUMBER->FLOAT.size = size;
    NUMBER->FLOAT.limbs = limbs;
    NUMBER->FLOAT.exp = limb_exp;
  } else {
    const int32_t n_limbs = exp / limb_max_digit;
    const int32_t remains = exp % limb_max_digit;

    MP_INT DIVISOR = {};
    MP_INT_init(&DIVISOR, (int32_t) n_limbs + 1, allocator);
    DIVISOR.size = 1; DIVISOR.limbs[0] = 1;
    for (int32_t i = 0; i < abs(n_limbs); i++) {
      scale_adic10(&DIVISOR, limb_max_digit, allocator);
    }
    if (remains != 0) {
      scale_adic10(&DIVISOR, abs(remains), allocator);
    }
    MP_INT_clamp(&DIVISOR);

    const int32_t quotient_exp_abs = abs(DIVISOR.size);
    int32_t quotient_size = abs(NUMBER->INT.size) + 1;
    MP_INT QUOTIENT = {};
    MP_INT_init(&QUOTIENT, quotient_size, allocator);
    MP_INT_left_shift(&NUMBER->INT, (int32_t) (quotient_exp_abs * LMP_LIMB_BITS), allocator);
    MP_INT_divmod(&NUMBER->INT, &DIVISOR, &QUOTIENT, allocator);
    MP_INT_clear(&DIVISOR, allocator);
    MP_INT_clear(&NUMBER->INT, allocator);
    if (QUOTIENT.size == 0) {
      MP_INT_clear(&QUOTIENT, allocator);
      NUMBER->FLOAT.exp = MP_FLOAT_ZERO;
      NUMBER->FLOAT.size = 0;
      return ;
    }
    if (NUMBER->INT.alloc < 0) {
      quotient_size = abs(QUOTIENT.size);
      if (quotient_size > abs(NUMBER->INT.alloc)) {
        const int32_t rounded_limbs = quotient_size - abs(NUMBER->INT.alloc);
        MP_INT_right_shift(&QUOTIENT, rounded_limbs * (int32_t)LMP_LIMB_BITS, allocator);
        MP_INT_clamp(&QUOTIENT);
        quotient_size = abs(NUMBER->INT.alloc);
      }
      lmp_limb_t *const limbs = NUMBER->INT.limbs;
      allocator->memcpy(limbs, QUOTIENT.limbs, quotient_size * LMP_LIMB_SIZE);
      NUMBER->FLOAT.limbs = limbs;
      NUMBER->FLOAT.prec = quotient_size;
      NUMBER->FLOAT.size = QUOTIENT.size;
      NUMBER->FLOAT.exp = - quotient_exp_abs;
      MP_INT_clear(&QUOTIENT, allocator);
    } else {
      NUMBER->FLOAT.limbs = QUOTIENT.limbs;
      NUMBER->FLOAT.prec = QUOTIENT.alloc;
      NUMBER->FLOAT.size = QUOTIENT.size;
      NUMBER->FLOAT.exp = - quotient_exp_abs;
    }
  }
}

inline void float_compose_adic8(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  int32_t n_limbs = 0, remains = 0;
  if (exp >= 0) {
    const int32_t n_bits = exp * 3;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS;
    remains = n_bits % (int32_t) LMP_LIMB_BITS;
    MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  } else {
    const int32_t n_bits = exp * 3;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS - (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  MP_INT_clamp(&NUMBER->INT);
  const int32_t prec = NUMBER->INT.alloc;
  const int32_t size = NUMBER->INT.size;
  lmp_limb_t* limbs = NUMBER->INT.limbs;
  NUMBER->FLOAT.prec = prec;
  NUMBER->FLOAT.size = size;
  NUMBER->FLOAT.limbs = limbs;
  NUMBER->FLOAT.exp = n_limbs;
}

inline void float_compose_adic2(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  int32_t n_limbs = 0, remains = 0;
  if (exp >= 0) {
    const int32_t n_bits = exp;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS;
    remains = n_bits % (int32_t) LMP_LIMB_BITS;
    MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  } else {
    const int32_t n_bits = exp;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS - (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->INT, remains, allocator);
  MP_INT_clamp(&NUMBER->INT);
  const int32_t prec = NUMBER->INT.alloc;
  const int32_t size = NUMBER->INT.size;
  lmp_limb_t* limbs = NUMBER->INT.limbs;
  NUMBER->FLOAT.prec = prec;
  NUMBER->FLOAT.size = size;
  NUMBER->FLOAT.limbs = limbs;
  NUMBER->FLOAT.exp = n_limbs;
}

inline void decimal_compose(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const) {
  const int32_t prec = NUMBER->INT.alloc;
  const int32_t size = NUMBER->INT.size;
  lmp_limb_t* limbs = NUMBER->INT.limbs;
  NUMBER->FLOAT.prec = prec;
  NUMBER->FLOAT.size = size;
  NUMBER->FLOAT.limbs = limbs;
  NUMBER->FLOAT.exp = exp;
}

inline uint32_t LMP_parse_adic_number(const char *const input, uint32_t *num_type, MP_NUMBER *const NUMBER,
                                      const bool negative, const uint32_t adic_type, const Allocator *const allocator) {
  auto const fun_check = ADIC_DIGIT_CHECK_FUNCS[adic_type];
  auto const limb_effective_len = MAX_DIGIT_PER_LIMB[adic_type];
  auto const func_scale_limb = SCALE_LIMB_FUNCS[adic_type];
  auto const func_int_limb_parse = DIGITAL_FUNC_TOOLS[adic_type];
  auto const func_float_compose = ADIC_FLOAT_COMPOSE_FUNCS[adic_type];

  /***************************************************/
  /************** NUMBER parse ***********************/
  /***************************************************/
  const char *pText = input;
  bool is_float = false;
  uint32_t effective_len = 0;
  uint32_t effective_int_len = 0;
  struct limb_pos curr_limb = {pText, nullptr};
  MP_INT_init(&NUMBER->INT, LMP_PARSE_ALLOC_SCALE * MP_ALLOC_LEN, allocator);
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
      lmp_limb_t limb_val = 0;
      func_int_limb_parse(&curr_limb, &limb_val);
      func_scale_limb(&NUMBER->INT, limb_effective_len, allocator);
      MP_INT_abs_add_u64(&NUMBER->INT, &NUMBER->INT, limb_val, allocator);
      curr_limb.start = pText;
    }
  }
  curr_limb.end = pText;
  if (curr_limb.end > curr_limb.start) {
    lmp_limb_t limb_val = 0;
    func_int_limb_parse(&curr_limb, &limb_val);
    func_scale_limb(&NUMBER->INT, curr_limb.end - curr_limb.start, allocator);
    MP_INT_abs_add_u64(&NUMBER->INT, &NUMBER->INT, limb_val, allocator);
  }
  MP_INT_clamp(&NUMBER->INT);
  NUMBER->INT.size = negative ? - NUMBER->INT.size : NUMBER->INT.size;
  int32_t literal_exp = 0;
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
      literal_exp = negative_exp ? - (int32_t) exp_val : (int32_t) exp_val;
    } else {
      pText = pText_back;
    }
  }
  bool is_decimal = false;
  if (adic_type == ADIC_TYPE_10) {
    if (is_float && isDecmier(pText)) { is_decimal = true; pText ++; }
  }
  lmp_limb_t size = 0;
  if (isSizeLimiter(pText)) {
    size += 1; pText ++;
    if (isSizeLimiter(pText)) { size += 1;  pText ++; }
    else {
      const char *pText_back = pText;
      curr_limb.start = pText;
      while (*pText) {
        if (isSeparator(pText)) {
          if (!isDecDigital(pText + 1)) { break; }
        }else if (!isDecDigital(pText)) { break; }
        pText++;
      }
      curr_limb.end = pText;
      if (curr_limb.end > curr_limb.start) {
        size = 0;
        limb_digit_parse_adic10(&curr_limb, &size);
      } else {
        pText = pText_back;
      }
    }
  }
  if (!is_decimal && adic_type == ADIC_TYPE_10) {
    if (is_float && isDecmier(pText)) { is_decimal = true; pText ++; }
  }

  /***************************************************/
  /************** NUMBER compose *********************/
  /***************************************************/
  if (!is_float) {
    *num_type = MP_NUM_TYPE_INT;
    // todo: precision (size)
    return pText - input;
  }
  literal_exp -= (int32_t) (effective_len - effective_int_len);
  if (literal_exp < - LMP_PARSE_EXP_MAX) {
    MP_INT_clear(&NUMBER->INT, allocator);
    const int32_t prec = NUMBER->INT.alloc;
    lmp_limb_t * limbs = NUMBER->INT.limbs;
    NUMBER->FLOAT.prec = prec;
    NUMBER->FLOAT.size = 0;
    NUMBER->FLOAT.exp = negative ? MP_FLOAT_NEG_INS : MP_FLOAT_INS;
    NUMBER->FLOAT.limbs = limbs;
  } else if (literal_exp > LMP_PARSE_EXP_MAX) {
    MP_INT_clear(&NUMBER->INT, allocator);
    const int32_t prec = NUMBER->INT.alloc;
    lmp_limb_t * limbs = NUMBER->INT.limbs;
    NUMBER->FLOAT.prec = prec;
    NUMBER->FLOAT.size = 0;
    NUMBER->FLOAT.exp = negative ? MP_FLOAT_NEG_INF : MP_FLOAT_INF;
    NUMBER->FLOAT.limbs = limbs;
  } else if (is_decimal) {
    decimal_compose(NUMBER, literal_exp, allocator);
    // todo: precision (size)
    *num_type = MP_NUM_TYPE_DECIMAL;
  } else {
    func_float_compose(NUMBER, literal_exp, allocator);
    // todo: precision (size)
    *num_type = MP_NUM_TYPE_FLOAT;
  }
  return pText - input;
}

static struct {
  const char * pattern;
  uint32_t pattern_len;
  int32_t        value;
} SPECIAL_NUMBER_PATTERNS[] = {
  { "inf", lenof("inf"), MP_FLOAT_INF },
  { "INF", lenof("INF"), MP_FLOAT_INF },
  { "ins", lenof("ins"), MP_FLOAT_INS },
  { "INS", lenof("INS"), MP_FLOAT_INS },
  { "nan", lenof("nan"), MP_FLOAT_NAN },
  { "NAN", lenof("NAN"), MP_FLOAT_NAN },
};

inline uint32_t try_special_number(const char *input, uint32_t *type, MP_NUMBER *NUMBER, const bool negative) {
  constexpr int32_t n_special_val = sizeof(SPECIAL_NUMBER_PATTERNS) / sizeof(SPECIAL_NUMBER_PATTERNS[0]);
  for (int32_t i = 0; i < n_special_val; i++) {
    auto const triple = SPECIAL_NUMBER_PATTERNS[i];
    if (strcmp_o(input, triple.pattern) == triple.pattern_len && !isIdentChar(&input[triple.pattern_len])) {
      NUMBER->FLOAT.size = 0;
      NUMBER->FLOAT.exp = negative ? - triple.value : triple.value;
      *type = MP_NUM_TYPE_FLOAT;
      return triple.pattern_len;
    }
  }
  return 0;
}

inline uint32_t strcmp_o(const char *const str1, const char * const str2) {
  uint32_t i = 0;
  while (str1[i] && str2[i] && (str1[i] == str2[i])) { i++; }
  return i;
}

// [+-]?0[xX][a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?) |
// [+-]?0[bB][01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?) |
// [+-]?0[oO][0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?) |
// [+-]?[0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?)[dD]?
uint32_t LMP_parse(const char * const input, uint32_t *type, MP_NUMBER *NUMBER, const Allocator * const allocator) {
  const char *pText = input;
  const bool negative = ('-' == *pText);
  if (isSign(pText)) {
    pText++;
    while (*pText == ' ' || *pText == '\t') { pText++; }
  }
  if (isDecDigital(pText)) {
    uint32_t adic_type = ADIC_TYPE_10;
    if (*pText == '0') {
      pText ++;
      switch (*pText) {
        case 'x':
        case 'X': { pText ++; adic_type = ADIC_TYPE_16; break; }
        case 'o':
        case 'O': { pText ++; adic_type = ADIC_TYPE_8 ; break; }
        case 'b':
        case 'B': { pText ++; adic_type = ADIC_TYPE_2 ; break; }
        default: {}
      }
    }
    pText += LMP_parse_adic_number(pText, type, NUMBER, negative, adic_type, allocator);
    return pText - input;
  }
  return try_special_number(pText, type, NUMBER, negative);
}
