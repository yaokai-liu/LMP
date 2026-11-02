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
* **IEEE-754 integration**: literals with an `l/L` suffix or `f/F` + size,
and no standard are returned as IEEE-754 float types via `MP_NUM_IEEE754_STANDARD`.
* **Allocator abstraction** for custom memory backends.
* **Fixed-capacity mode** for allocation-free parsing and arithmetic.

# Build & Usage

Build the library:
```shell
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --target LMP
```
Then you will get a `libLMP.a` in `${CMAKE_CURRENT_SOURCE_DIR}/output`.

Parse Example:
```C
#include "LMP/parse.h"
#include "LMP/print.h"

int main() {
  MP_NUM_TYPE num_type = {};
  lmp_limb_t limbs[8] = {};
  MP_NUMBER number = {.val.INT = {.alloc = -8, .limbs = limbs}, .repr = MP_NUM_REPR_NONE};
  const char *num_str = "-123.12345678999991234'56789123456789_123456789LL<IEEE754>";
  const uint32_t parse_len = LMP_parse(num_str, &num_type, &number, nullptr);
  const uint32_t print_len = LMP_printf("%{f<LMP>}", &number.val.FLOAT);
  return 0;
}
```
compile and run this code, it will print
> -123.1234567899999123456789123456789123456789

## License

See the repository for license details.
