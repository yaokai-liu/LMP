# LMP - A Lite Multiple Precision Arithmetic Library

LMP is a lightweight, header-driven multiple-precision arithmetic library written in C. 
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
`binary/decimal` exponents, `l/L` suffixes, and encoding tags.
**Support custom float point number standard**. Users just need add custom standard short name and standard id,
the parser will automatically recognize and record the number's standard type.
* **IEEE-754 integration**: literals with an `l/L` suffix and no encoding are returned as IEEE-754 float types via `MP_NUMBER_EXTEND_IEEE754_TYPE`.
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

## Type Encoding

`MP_NUMBER_TYPE` encodes both the kind and the target bit width:

|  Bits   |          Meaning          |
|:-------:|:-------------------------:|
| [31:16] | Kind (MP_NUM_TYPE_*_TYPE) |
| [15:0]  |     Target bit width      |

```C
enum MP_NUMBER_TYPE : uint32_t {
  MP_NUM_TYPE_NONE,
  MP_NUM_TYPE_INT,
  MP_NUM_TYPE_FLOAT,
  MP_NUM_TYPE_DECIMAL,

  MP_NUM_TYPE_INT_TYPE  = 0x7000'0000,
  MP_NUM_TYPE_UINT_TYPE = 0x7001'0000,
};

enum MP_NUMBER_EXTEND_IEEE754_TYPE : uint32_t {
  MP_NUM_TYPE_EXT_IEEE754_FLOAT_TYPE   = 0x7002'0000,
  MP_NUM_TYPE_EXT_IEEE754_DECIMAL_TYPE = 0x7003'0000,
};
```

## Parsing

### Accepted Literal Formats
```regexp
[+-]?0[xX][a-fA-F0-9]+(\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?) |
[+-]?0[bB][01]+(\.[01]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?)|
[+-]?0[oO][0-7]+(\.[0-7]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?)|
[+-]?[0-9]+(\.[0-9]+([pPeE][+-]?[0-9]+)?)?(([lL]{0,2}|[lL][0-9]+)(-<encoding>)?)[dD]?
```
In summary:
* Optional leading `+/-`.
* Base prefixes: `0x/0X` (hex), `0b/0B` (binary), `0o/0O` (octal), or none (decimal).
* Optional fractional part.
* Optional exponent: `p/P` for hexadecimal exponent bases; `p/P/e/E` for other adic.
* Optional length suffix: up to two `l/L`, or `l/L` followed by a width in digits.
* Optional -<encoding> tag selecting a target numeric standard. 
If none, treat as an IEEE-754 type according to `MP_NUMBER_EXTEND_IEEE754_TYPE`.

## Parsing Memory Modes
* Dynamic mode (default): if `NUMBER->INT.alloc >= 0`, the parser allocates and grows limbs;
the caller must release them with `MP_*_clear`.
* Fixed-capacity mode: if `NUMBER->INT.alloc < 0`, the parser performs no allocation or deallocation.
The caller must supply `NUMBER->INT.limbs` with capacity at least `abs(NUMBER->INT.alloc)`.

## Special values

Via `size == 0`: for `MP_FLOAT/MP_DECIMAL`, a zero size marks the value as a special (`MP_FLOAT_SPECIAL_VAL_TYPE`), and exp carries the tag.
In that state limbs is free for user use, though the library may overwrite it if the value is used as an arithmetic result destination.

## License

See the repository for license details.
