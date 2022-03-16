#ifndef GBEML_TYPES_H_
#define GBEML_TYPES_H_

#include <cstdint>

namespace gbeml {

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

u16 concat(u8 h, u8 l);
u16 interleave(u8 h, u8 l);

}  // namespace gbeml

#endif  // GBEML_TYPES_H_
