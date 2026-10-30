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
#include "type.h"
#include "arith.h"
#include "allocator.h"

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
static void scale_adic16(MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic10(MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic8 (MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void scale_adic2 (MP_INT *op, uint32_t n_digits, const Allocator *allocator);
static void float_compose_adic16(MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic10(MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic8 (MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void float_compose_adic2 (MP_NUMBER *NUMBER, int32_t exp, const Allocator *allocator);
static void decimal_compose(MP_NUMBER *NUMBER,  int32_t exp, const Allocator * allocator);
static uint32_t LMP_parse_num_suffix(const char *input, bool is_float, uint32_t adic_type, MP_NUM_TYPE *num_type, uint8_t *num_repr);

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
#define isLAngleBracket(pText)    (*(pText) == '<')
#define isRAngleBracket(pText)    (*(pText) == '>')

static bool isBinDigital(const char *pText) { return ('0' == pText[0] || pText[0] == '1'); }
static bool isOctDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '7'); }
static bool isDecDigital(const char *pText) { return ('0' <= pText[0] && pText[0] <= '9'); }
static bool isHexDigital(const char *pText) { return isDecDigital(pText) || isHexLetter(pText); }

uint32_t stridx_o(char chr, const char * str);
static uint32_t strcmp_o(const char * str1, const char * str2);
static uint32_t try_special_number(const char *input, MP_NUM_TYPE *type, MP_NUMBER *NUMBER, bool negative);

static uint32_t LMP_parse_adic_number(const char *input, MP_NUM_TYPE *num_type, MP_NUMBER *NUMBER,
                                      bool negative, uint32_t adic_type, const Allocator *allocator);

enum LMP_ADIC_TYPE : int32_t {
  ADIC_TYPE_16 = 0,
  ADIC_TYPE_10 = 1,
  ADIC_TYPE_8  = 2,
  ADIC_TYPE_2  = 3,
};
static constexpr uint32_t MAX_DIGIT_PER_LIMB[] = {
  [ADIC_TYPE_16] = (LMP_LIMB_BITS / 4),
  [ADIC_TYPE_10] = ((19 * 64) / LMP_LIMB_BITS),
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
  } else {
    const int32_t n_bits = exp * 4;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS + (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->val.INT, remains, allocator);
  MP_INT_clamp(&NUMBER->val.INT);
  const int32_t prec = NUMBER->val.INT.alloc;
  const int32_t size = NUMBER->val.INT.size;
  lmp_limb_t* limbs = NUMBER->val.INT.limbs;
  NUMBER->val.FLOAT.prec = prec;
  NUMBER->val.FLOAT.size = size;
  NUMBER->val.FLOAT.limbs = limbs;
  NUMBER->val.FLOAT.exp = n_limbs;
}

inline void float_compose_adic10(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  const int32_t limb_max_digit = (int32_t) MAX_DIGIT_PER_LIMB[ADIC_TYPE_10];
  int32_t prec = abs(NUMBER->val.INT.alloc);
  if (exp >= 0) {
    int32_t limb_exp = 0;
    const int32_t n_limbs = exp / limb_max_digit;
    const int32_t remains = exp % limb_max_digit;
    for (int32_t i = 0; i < n_limbs; i++) {
      scale_adic10(&NUMBER->val.INT, MAX_DIGIT_PER_LIMB[ADIC_TYPE_10], allocator);
      if (abs(NUMBER->val.INT.size) >= prec && i < n_limbs - 1) {
        MP_INT_right_shift(&NUMBER->val.INT, LMP_LIMB_BITS, allocator);
        limb_exp ++;
      }
    }
    if (remains > 0) {
      if (abs(NUMBER->val.INT.size) >= prec) {
        MP_INT_right_shift(&NUMBER->val.INT, LMP_LIMB_BITS, allocator);
      }
      scale_adic10(&NUMBER->val.INT, remains, allocator);
    }
    MP_INT_clamp(&NUMBER->val.INT);
    prec = NUMBER->val.INT.alloc;
    const int32_t size = NUMBER->val.INT.size;
    lmp_limb_t* limbs = NUMBER->val.INT.limbs;
    NUMBER->val.FLOAT.prec = prec;
    NUMBER->val.FLOAT.size = size;
    NUMBER->val.FLOAT.limbs = limbs;
    NUMBER->val.FLOAT.exp = limb_exp;
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
    MP_INT_left_shift(&NUMBER->val.INT, (int32_t) (quotient_exp_abs * LMP_LIMB_BITS), allocator);
    int32_t quotient_size = abs(NUMBER->val.INT.size) - abs(DIVISOR.size);
    MP_INT QUOTIENT = {};
    MP_INT_init(&QUOTIENT, quotient_size, allocator);
    MP_INT_divmod(&NUMBER->val.INT, &DIVISOR, &QUOTIENT, allocator);
    quotient_size = abs(QUOTIENT.size);
    MP_INT_clear(&DIVISOR, allocator);
    MP_INT_clear(&NUMBER->val.INT, allocator);
    if (QUOTIENT.size == 0) {
      MP_INT_clear(&QUOTIENT, allocator);
      NUMBER->val.FLOAT.exp = MP_FLOAT_ZERO;
      NUMBER->val.FLOAT.size = 0;
      return ;
    }
    if (NUMBER->val.INT.alloc < 0) {
      quotient_size = abs(QUOTIENT.size);
      if (quotient_size > abs(NUMBER->val.INT.alloc)) {
        const int32_t rounded_limbs = quotient_size - abs(NUMBER->val.INT.alloc);
        MP_INT_right_shift(&QUOTIENT, rounded_limbs * (int32_t)LMP_LIMB_BITS, allocator);
        MP_INT_clamp(&QUOTIENT);
        quotient_size = abs(NUMBER->val.INT.alloc);
      }
      lmp_limb_t *const limbs = NUMBER->val.INT.limbs;
      allocator->memcpy(limbs, QUOTIENT.limbs, quotient_size * LMP_LIMB_SIZE);
      NUMBER->val.FLOAT.limbs = limbs;
      NUMBER->val.FLOAT.prec = prec;
      NUMBER->val.FLOAT.size = QUOTIENT.size;
      NUMBER->val.FLOAT.exp = - quotient_exp_abs;
      MP_INT_clear(&QUOTIENT, allocator);
    } else {
      NUMBER->val.FLOAT.limbs = QUOTIENT.limbs;
      NUMBER->val.FLOAT.prec = QUOTIENT.alloc;
      NUMBER->val.FLOAT.size = QUOTIENT.size;
      NUMBER->val.FLOAT.exp = - quotient_exp_abs;
    }
  }
  MP_FLOAT_clamp(&NUMBER->val.FLOAT); // TODO: round method
}

inline void float_compose_adic8(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  int32_t n_limbs = 0, remains = 0;
  if (exp >= 0) {
    const int32_t n_bits = exp * 3;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS;
    remains = n_bits % (int32_t) LMP_LIMB_BITS;
    MP_INT_left_shift(&NUMBER->val.INT, remains, allocator);
  } else {
    const int32_t n_bits = exp * 3;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS + (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->val.INT, remains, allocator);
  MP_INT_clamp(&NUMBER->val.INT);
  const int32_t prec = NUMBER->val.INT.alloc;
  const int32_t size = NUMBER->val.INT.size;
  lmp_limb_t* limbs = NUMBER->val.INT.limbs;
  NUMBER->val.FLOAT.prec = prec;
  NUMBER->val.FLOAT.size = size;
  NUMBER->val.FLOAT.limbs = limbs;
  NUMBER->val.FLOAT.exp = n_limbs;
}

inline void float_compose_adic2(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const allocator) {
  int32_t n_limbs = 0, remains = 0;
  if (exp >= 0) {
    const int32_t n_bits = exp;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS;
    remains = n_bits % (int32_t) LMP_LIMB_BITS;
    MP_INT_left_shift(&NUMBER->val.INT, remains, allocator);
  } else {
    const int32_t n_bits = exp;
    n_limbs = n_bits / (int32_t) LMP_LIMB_BITS - 1;
    remains = LMP_LIMB_BITS + (n_bits % (int32_t) LMP_LIMB_BITS);
  }
  MP_INT_left_shift(&NUMBER->val.INT, remains, allocator);
  MP_INT_clamp(&NUMBER->val.INT);
  const int32_t prec = NUMBER->val.INT.alloc;
  const int32_t size = NUMBER->val.INT.size;
  lmp_limb_t* limbs = NUMBER->val.INT.limbs;
  NUMBER->val.FLOAT.prec = prec;
  NUMBER->val.FLOAT.size = size;
  NUMBER->val.FLOAT.limbs = limbs;
  NUMBER->val.FLOAT.exp = n_limbs;
}

inline void decimal_compose(MP_NUMBER *const NUMBER, const int32_t exp, const Allocator *const) {
  const int32_t prec = NUMBER->val.INT.alloc;
  const int32_t size = NUMBER->val.INT.size;
  lmp_limb_t* limbs = NUMBER->val.INT.limbs;
  NUMBER->val.FLOAT.prec = prec;
  NUMBER->val.FLOAT.size = size;
  NUMBER->val.FLOAT.limbs = limbs;
  NUMBER->val.FLOAT.exp = exp;
}

// <REPR>?([lL]{0,2}|[0-9]+)<REPR>?\<standard\>
inline uint32_t LMP_parse_num_suffix(const char *const input, const bool is_float, const uint32_t adic_type,
                                     MP_NUM_TYPE *const num_type, uint8_t *const num_repr) {
  const char *pText = input;
  char repr_chr = 0;
  lmp_limb_t size = 0;
  uint8_t stdd_id = MP_NUM_UNKNOWN_STANDARD;
  auto const STD_LIST = is_float ? MP_NUM_FLOAT_STANDARDS : MP_NUM_INT_STANDARDS;
  auto const STD_REPR_LIST = is_float ? MP_NUM_FLOAT_REPR_CHARS : MP_NUM_INT_REPR_CHARS;

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
    for (uint32_t i = 0; i < MP_NUM_STANDARD_COUNT; i++) {
      auto const stdd = STD_LIST[i];
      if (!stdd.name) { continue; }
      if (strcmp_o(pText, stdd.name) == stdd.leng) {
        if (!isRAngleBracket(pText + stdd.leng)) { continue; }
        stdd_id = stdd.stdd;
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
  if (is_float) {
    *num_repr = (adic_type == ADIC_TYPE_10 && isDecmier(&repr_chr)) ? MP_NUM_REPR_MP_DECIMAL : MP_NUM_REPR_MP_FLOAT;
  } else { *num_repr = MP_NUM_REPR_MP_INT; }
  // stdd
  if (stdd_id == MP_NUM_UNKNOWN_STANDARD) {
    if (size > 0) {
      if (is_float) { stdd_id = MP_NUM_IEEE754_STANDARD; }
      else { stdd_id = MP_NUM_INT_STANDARD; }
    } else { stdd_id = MP_NUM_LMP_STANDARD; }
  }
  auto const STD_REPR = STD_REPR_LIST[stdd_id];
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
inline uint32_t LMP_parse_adic_number(const char *const input, MP_NUM_TYPE *num_type, MP_NUMBER *const NUMBER,
                                      const bool negative, const uint32_t adic_type, const Allocator *const allocator) {
  if (!*input) { return 0; }
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
 struct limb_pos curr_limb = { .start = pText, .end = nullptr };
  MP_INT_init(&NUMBER->val.INT, LMP_PARSE_ALLOC_SCALE * MP_ALLOC_LEN, allocator);
  while (*pText && effective_len <= LMP_LITERAL_MANT_LEN_MAX) {
    if (isSeparator(pText)) {
      if (!fun_check(pText + 1)) { break; }
      pText ++; continue;
    }
    if (!is_float && (isDot(pText) && fun_check(pText + 1))) {
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
      func_scale_limb(&NUMBER->val.INT, limb_effective_len, allocator);
      MP_INT_abs_add_u64(&NUMBER->val.INT, &NUMBER->val.INT, limb_val, allocator);
      curr_limb.start = pText;
    }
  }
  if (pText == input) {
    MP_INT_clear(&NUMBER->val.INT, allocator);
    return 0;
  }
  if (!effective_int_len) { effective_int_len = effective_len; }
  curr_limb.end = pText;
  if (curr_limb.end > curr_limb.start) {
    lmp_limb_t limb_val = 0;
    func_int_limb_parse(&curr_limb, &limb_val);
    func_scale_limb(&NUMBER->val.INT, curr_limb.end - curr_limb.start, allocator);
    MP_INT_abs_add_u64(&NUMBER->val.INT, &NUMBER->val.INT, limb_val, allocator);
  }
  MP_INT_clamp(&NUMBER->val.INT);
  NUMBER->val.INT.size = negative ? - NUMBER->val.INT.size : NUMBER->val.INT.size;
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
  if ((startswithLetter(pText) && !isExponer(pText)) || isLAngleBracket(pText)) {
    pText += LMP_parse_num_suffix(pText, is_float, adic_type, num_type, &NUMBER->repr);
  } else if (is_float) {
    num_type->type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_FLOAT, 0);
    NUMBER->repr = MP_NUM_REPR_MP_FLOAT;
  } else {
    num_type->type = MP_NUM_TYPE_OF(MP_NUM_LMP_STANDARD, MP_NUM_REPR_MP_INT, 0);
    NUMBER->repr = MP_NUM_REPR_MP_INT;
  }

  /***************************************************/
  /************** NUMBER compose *********************/
  /***************************************************/
  if (NUMBER->repr == MP_NUM_REPR_MP_INT) { return pText - input; }
  literal_exp -= (int32_t) (effective_len - effective_int_len);
  if (literal_exp < - LMP_PARSE_EXP_MAX) {
    MP_INT_clear(&NUMBER->val.INT, allocator);
    const int32_t prec = NUMBER->val.INT.alloc;
    lmp_limb_t * limbs = NUMBER->val.INT.limbs;
    NUMBER->val.FLOAT.prec = prec;
    NUMBER->val.FLOAT.size = 0;
    NUMBER->val.FLOAT.exp = negative ? MP_FLOAT_NEG_INS : MP_FLOAT_INS;
    NUMBER->val.FLOAT.limbs = limbs;
  } else if (literal_exp > LMP_PARSE_EXP_MAX) {
    MP_INT_clear(&NUMBER->val.INT, allocator);
    const int32_t prec = NUMBER->val.INT.alloc;
    lmp_limb_t * limbs = NUMBER->val.INT.limbs;
    NUMBER->val.FLOAT.prec = prec;
    NUMBER->val.FLOAT.size = 0;
    NUMBER->val.FLOAT.exp = negative ? MP_FLOAT_NEG_INF : MP_FLOAT_INF;
    NUMBER->val.FLOAT.limbs = limbs;
  } else if (NUMBER->repr == MP_NUM_REPR_MP_DECIMAL) {
    decimal_compose(NUMBER, literal_exp, allocator);
  } else {
    func_float_compose(NUMBER, literal_exp, allocator);
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

inline uint32_t try_special_number(const char *input, MP_NUM_TYPE *type, MP_NUMBER *NUMBER, const bool negative) {
  constexpr int32_t n_special_val = sizeof(SPECIAL_NUMBER_PATTERNS) / sizeof(SPECIAL_NUMBER_PATTERNS[0]);
  for (int32_t i = 0; i < n_special_val; i++) {
    auto const triple = SPECIAL_NUMBER_PATTERNS[i];
    if (strcmp_o(input, triple.pattern) == triple.pattern_len && !isIdentChar(&input[triple.pattern_len])) {
      NUMBER->val.FLOAT.size = 0;
      NUMBER->val.FLOAT.exp = negative ? - triple.value : triple.value;
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
uint32_t LMP_parse(const char * const input, MP_NUM_TYPE *type, MP_NUMBER *NUMBER, const Allocator * const allocator) {
  const char *pText = input;
  const bool negative = ('-' == *pText);
  if (isSign(pText)) {
    pText++;
    while (*pText == ' ' || *pText == '\t') { pText++; }
  }
  if (isDecDigital(pText)) {
    uint32_t adic_type = ADIC_TYPE_10;
    if (*pText == '0') {
      switch (pText[1]) {
        case 'x':
        case 'X': { pText += 2; adic_type = ADIC_TYPE_16; break; }
        case 'o':
        case 'O': { pText += 2; adic_type = ADIC_TYPE_8 ; break; }
        case 'b':
        case 'B': { pText += 2; adic_type = ADIC_TYPE_2 ; break; }
        default: {  }
      }
    }
    const uint32_t length = LMP_parse_adic_number(pText, type, NUMBER, negative, adic_type, allocator);
    if (length) {  return pText + length - input; }
    return 0;
  }
  return try_special_number(pText, type, NUMBER, negative);
}
