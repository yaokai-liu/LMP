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
 * Module Name: ext/ieee754
 * Filename: convert.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-25
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "type.h"

#define lenof(str_literal) ((sizeof str_literal) - 1)

const struct MP_STANDARD MP_NUM_INT_STANDARDS[MP_NUM_STANDARD_COUNT] = {
  [MP_NUM_LMP_STANDARD] = { .name = "LMP", .leng = lenof("LMP"), .stdd = MP_NUM_LMP_STANDARD },
  [MP_NUM_INT_STANDARD] = { .name = "C-INT", .leng = lenof("C-INT"), .stdd = MP_NUM_INT_STANDARD },
};

const struct MP_STANDARD MP_NUM_FLOAT_STANDARDS[MP_NUM_STANDARD_COUNT] = {
  [MP_NUM_LMP_STANDARD] = { .name = "LMP", .leng = lenof("LMP"), .stdd = MP_NUM_LMP_STANDARD },
  [MP_NUM_IEEE754_STANDARD] = { .name = "IEEE754", .leng = lenof("IEEE754"), .stdd = MP_NUM_IEEE754_STANDARD },
};

const struct MP_STANDARD_REPR MP_NUM_INT_REPR_CHARS[MP_NUM_STANDARD_COUNT] = {
  [MP_NUM_LMP_STANDARD] =  {.count = 0, .L = 0, .LL = 0, .fallback_index = 0, .items = {
      {.chr = '\0', .repr = MP_NUM_REPR_MP_INT},
    }},
  [MP_NUM_INT_STANDARD] =  {.count = 4, .L = 8, .LL = 16, .fallback_index = 0, .items = {
    {.chr = 's', .repr = MP_NUM_REPR_STD_C_INT},
    {.chr = 'S', .repr = MP_NUM_REPR_STD_C_INT},
    {.chr = 'u', .repr = MP_NUM_REPR_STD_C_UINT},
    {.chr = 'U', .repr = MP_NUM_REPR_STD_C_UINT},
  }},
};

const struct MP_STANDARD_REPR MP_NUM_FLOAT_REPR_CHARS[MP_NUM_STANDARD_COUNT] = {
  [MP_NUM_LMP_STANDARD] =  {.count = 6,.L = 0, .LL = 0, .fallback_index = 0, .items = {
    {.chr = 'f', .repr = MP_NUM_REPR_MP_FLOAT},
    {.chr = 'F', .repr = MP_NUM_REPR_MP_FLOAT},
    {.chr = 'g', .repr = MP_NUM_REPR_MP_FLOAT},
    {.chr = 'G', .repr = MP_NUM_REPR_MP_FLOAT},
    {.chr = 'd', .repr = MP_NUM_REPR_MP_DECIMAL},
    {.chr = 'D', .repr = MP_NUM_REPR_MP_DECIMAL},
  }},
  [MP_NUM_IEEE754_STANDARD] =  {.count = 6, .L = 8, .LL = 16, .fallback_index = 0, .items = {
    {.chr = 'f', .repr = MP_NUM_REPR_IEEE754_FLOAT},
    {.chr = 'F', .repr = MP_NUM_REPR_IEEE754_FLOAT},
    {.chr = 'g', .repr = MP_NUM_REPR_IEEE754_FLOAT},
    {.chr = 'G', .repr = MP_NUM_REPR_IEEE754_FLOAT},
    {.chr = 'd', .repr = MP_NUM_REPR_IEEE754_DECIMAL},
    {.chr = 'D', .repr = MP_NUM_REPR_IEEE754_DECIMAL},
  }},
};
