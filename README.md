# LMP - A Lite Multiple Precision Arithmetic Library

LMP is a lightweight, multiple-precision arithmetic library written in C. 
It provides arbitrary-precision integer, floating-point, 
and decimal arithmetic built on top of compiler-supported wide integer types (_BitInt), 
with a parser that understands C-style numeric literals across binary, octal, decimal, and hexadecimal bases.

The library is designed to be small, portable, and embeddable: 
it does not rely on GMP at the API level, but its core integer struct is laid out to be ABI-compatible with GMP's mpz, 
making interoperation straightforward. Memory management is caller-controlled through an allocator interface, 
and a fixed-capacity mode allows fully allocation-free operation for embedded or real-time use.

## Features

* **Arbitrary-precision integers**
* **Arbitrary-precision floating-point and decimal**
* **Special values for floating-point**: zero, infinity, infinitesimal, NaN, and their negatives.
* **Flexible parser**: supporting binary, octal, decimal, and hexadecimal literals with optional fractional parts,
`binary/decimal` exponents, `l/L` suffixes, and standard tags.
**Support custom float point number standard**. Users just need add custom standard short name and standard id,
the parser will automatically recognize and record the number's standard type.
* **IEEE-754 integration**: literals with an `l/L` suffix and no standard are returned as IEEE-754 float types via `MP_NUM_IEEE754_STANDARD`.
* **Allocator abstraction** for custom memory backends.
* **Fixed-capacity mode** for allocation-free parsing and arithmetic.

## Core Types
```C
typedef _BitInt(128)          int128_t;
typedef _BitInt(256)          int256_t;
typedef unsigned _BitInt(128) uint128_t;
typedef unsigned _BitInt(256) uint256_t;

typedef uint64_t  lmp_limb_t;
typedef uint32_t  lmp_half_limb_t;
typedef uint128_t lmp_double_limb_t;
typedef uint256_t lmp_quarter_limb_t;
```
The fundamental integer type is aligned to GNU's mpz:
```C
typedef struct MP_INT {
  int32_t     alloc;   // abs(alloc) = limb capacity; negative = fixed capacity
  int32_t     size;    // signed number of limbs in use
  lmp_limb_t *limbs;   // little-endian limb array
} MP_INT;
```
The floating-point / decimal type is:
```C
typedef struct MP_FLOAT {
  int32_t     prec;    // limb capacity; negative = fixed capacity
  int32_t     size;    // signed magnitude size; 0 = special value
  int64_t     exp;     // exponent (base LMP_LIMB_VAL for FLOAT, base 10 for DECIMAL)
  lmp_limb_t *limbs;   // little-endian limb array
} MP_FLOAT, MP_DECIMAL;
```

## Type and Standard

`MP_NUMBER_TYPE` encodes both the kind and the target bit width:

|  Bits   |          Meaning          |
|:-------:|:-------------------------:|
| [31-24] | Kind (MP_NUM_TYPE_*_TYPE) |
| [23-16] |      Target encoding      |
| [15-0 ] |     Target bit width      |

```C
enum MP_NUMBER_STANDARD_TYPE : uint8_t {
  MP_NUM_LMP_STANDARD = 0x00,
  // std C integer
  MP_NUM_INT_STANDARD = 0x01,
  MP_NUM_IEEE754_STANDARD   = 0x02,

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
enum MP_NUMBER_IEEE754_REPRESENTATION_TYPE : uint8_t {
  MP_NUM_REPR_IEEE754_FLOAT = 0x01,
  MP_NUM_REPR_IEEE754_DECIMAL = 0x02,
};

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
  }     pack __attribute__((__packed__));
  uint32_t  type;
} MP_NUM_TYPE;
```

## Parsing

### Accepted Literal Formats
```BNF
NUMBER = <sign>? <prefix> <digits> (<dot> <digits>)? <exp>? <suffix>?
<suffix> = <repr> <size> <standard> 
         | <size> <repr> <standard>
         | <size> <standard>
         | <repr> <standard>
         | <repr> <size>
         | <size>
         | <repr>
```
In summary:
* `<sign>`: Optional leading `+/-`.
* Base prefixes `<prefix>`: `0x/0X` (hexadecimal), `0b/0B` (binary), `0o/0O` (octal), or none (decimal).
* Optional fractional part.
* Optional exponent `<exp>`: `p/P` for hexadecimal exponent bases; `p/P/e/E` for other adic.
* Optional length suffix <size>: up to two `l/L`, or a number that following `<repr>`. Standard specify.
* Optional `<repr>` tag selecting a target representation,
`FLOAT` or `DECIMAL`, `UNSIGNED` or `SIGNED`, or other standard specified representation by single letters.
* Optional `<standard>` tag selecting a target numeric standard, must be in angle brackets.

## Memory Modes
* Dynamic mode (default): if `NUMBER->val.INT.alloc >= 0`, the library allocates and grows limbs;
the `MP_*_clear` will free `limbs`.
* Fixed-capacity mode: if `NUMBER->val.INT.alloc < 0`, the library performs no allocation or deallocation.
The caller must supply `NUMBER->val.INT.limbs` with capacity at least `abs(NUMBER->val.INT.alloc)`.
If not, it is an undefined behavior.

## Special Values

Via set `size = 0` for `MP_FLOAT/MP_DECIMAL`, a zero size marks the value as a special (`MP_FLOAT_SPECIAL_VAL_TYPE`), and exp carries the tag.
In that state limbs is free for user use, though the library may overwrite it if the value is used as an arithmetic result destination.

## License

See the repository for license details.
