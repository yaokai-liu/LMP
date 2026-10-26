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
 * Filename: type.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-26
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_TYPE_H
#define LMP_TYPE_H

#include <stdint.h>

enum MP_NUMBER_STANDARD_TYPE : uint8_t {
  MP_NUM_LMP_STANDARD = 0x00,
  // std C integer
  MP_NUM_INT_STANDARD = 0x01,

  MP_NUM_UNKNOWN_STANDARD = 0xFF
};
enum MP_NUMBER_LMP_REPRESENTATION_TYPE : uint8_t {
  MP_NUM_REPR_MP_NONE = 0x00,
  MP_NUM_REPR_MP_INT = 0x01,
  MP_NUM_REPR_MP_FLOAT = 0x02,
  MP_NUM_REPR_MP_DECIMAL = 0x03,
};
enum MP_NUMBER_INT_REPRESENTATION_TYPE : uint8_t {
  MP_NUM_REPR_STD_C_INT = 0x00,
  MP_NUM_REPR_STD_C_UINT = 0x01,
};

#ifndef __BYTE_ORDER__
#error "unknown byte order."
#endif
#if defined(_MSC_VER)
#  define MP_PACKED
#  pragma pack(push, 1)
#elif defined(__GNUC__)
#  define MP_PACKED __attribute__((__packed__))
#else
#  error "unknown compiler"
#endif
// Layout in case of (type >= MP_NUM_TYPE_INT_TYPE):
//     bit [31-24] (stdd) : kind (MP_NUMBER_STANDARD_TYPE)
//     bit [23-16] (repr) : representation. standard specify.
//     bit [15-0 ] (size) : target's size. standard specify.
typedef union {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint16_t  size;
    uint8_t   repr;
    uint8_t   stdd;
#else
    uint8_t   stdd;
    uint8_t   repr;
    uint16_t  size;
#endif
  }     pack MP_PACKED;
  uint32_t  type;
} MP_NUM_TYPE;
_Static_assert(sizeof(((MP_NUM_TYPE*)nullptr)->pack) == 4, "pack layout broken");

#define MP_NUM_TYPE_KIND(t)       (((t) & 0xFF'00'0000u) >> 24)
#define MP_NUM_TYPE_REPR(t)       (((t) & 0x00'FF'0000u) >> 16)
#define MP_NUM_TYPE_BITS(t)       ((t) & 0x000'0'FFFFu)
#define MP_NUM_TYPE_IS_MP_TYPE(t) (MP_NUM_TYPE_KIND(t) == MP_NUM_LMP_STANDARD)
#define MP_NUM_TYPE_REPR_IS(t, std, repr) \
  (MP_NUM_TYPE_KIND(t) == (std) && MP_NUM_TYPE_REPR(t) == (repr))
#define MP_NUM_TYPE_OF(_standard, _repr, _size) \
  ((((_standard) & 0xFFu) << 24) | (((_repr) & 0xFF) << 16) | ((_size) & 0xFFFFu))

#endif //LMP_TYPE_H
