
## Core Types

See [lmp.h](include/LMP/lmp.h)

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
```BNF Target=Number
Number = SIGN? PREFIX DIGITS (DOT DIGITS)? EXP? Suffix?
Suffix = REPR SIZE STANDARD 
         | SIZE REPR STANDARD
         | SIZE STANDARD
         | REPR STANDARD
         | REPR SIZE
         | STANDARD
         | SIZE
         | REPR
```
In summary:
* `SIGN`: Optional leading `+/-`.
* Base prefixes `PREFIX`: `0x/0X` (hexadecimal), `0b/0B` (binary), `0o/0O` (octal), or none (decimal).
* Optional fractional part.
* Optional exponent `EXP`: `p/P` for hexadecimal exponent bases; `p/P/e/E` for other adic.
* Optional length suffix SIZE: up to two `l/L`, or a number that following `REPR`. Standard specify.
* Optional `REPR` tag selecting a target representation,
  `FLOAT` or `DECIMAL`, `UNSIGNED` or `SIGNED`, or other standard specified representation by single letters.
* Optional `STANDARD` tag selecting a target numeric standard, must be in angle brackets.

the first representation will cover the last representation which close to the standard name,
and them can't present in same times.
And representation can be any single letter but `l/L`.
All standard should reserve `f/F`(in some cases in 16 adic, it will be parsed as mantissa's digit), `g/G` for floats
and `d/D` for decimals (`d/D` represent decimal only under 10 adic)
and `s/S`, `u/U` for integers, if them are used in standard.

If size appears before the representation, it can't be digital.

For non 10 adic, noninteger, if `d/D` is used, it will be treated as `f/F`.
if the representation is used but not in standard, it will be parsed and fallback to default.

If no standard, but size are set,
treat `FLOAT` and `DECIMAL` in IEEE-754 standard `MP_NUM_IEEE754_STANDARD`,
treat `INT` in `MP_NUM_INT_STANDARD`, which is actually std C standard.
Default treat in LMP's basic numeric standard `MP_NUM_LMP_STANDARD`,
or preset default standard before parsing.

## Memory Modes
* Dynamic mode (default): if `NUMBER->val.INT.alloc >= 0`, the library allocates and grows limbs;
  the `MP_*_clear` will free `limbs`.
* Fixed-capacity mode: if `NUMBER->val.INT.alloc < 0`, the library performs no allocation or deallocation.
  The caller must supply `NUMBER->val.INT.limbs` with capacity at least `abs(NUMBER->val.INT.alloc)`.
  If not, it is an undefined behavior.

## Special Values

Via set `size = 0` for `MP_FLOAT/MP_DECIMAL`, a zero size marks the value as a special (`MP_FLOAT_SPECIAL_VAL_TYPE`), and exp carries the tag.
In that state limbs is free for user use, though the library may overwrite it if the value is used as an arithmetic result destination.
