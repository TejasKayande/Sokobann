#if !defined(BASE_H)
#define BASE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define KB(x)   ((unsigned long long)(x) * 1024ULL)
#define MB(x)   (KB(x) * 1024ULL)
#define GB(x)   (MB(x) * 1024ULL)

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(*(x)))

typedef int8_t    i8;
typedef uint8_t   u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

#endif // BASE_H