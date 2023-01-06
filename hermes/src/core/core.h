#ifndef HM_CORE_H
#define HM_CORE_H

#include "core/types.h"
#include "core/log.h"

#define HM_TRUE 1
#define HM_FALSE 0

#define BIT(x) (1 << x)

#if defined(__clang__) || defined(__GNUC__)
#define HM_STATIC_ASSERT(condition, message) _Static_assert(condition, message)
#else
#define HM_STATIC_ASSERT(condition, message) static_assert(condition, message)
#endif

#ifdef HM_ENABLE_ASSERTS
#ifdef _MSC_VER
#include <intrin.h>
#define HM_DEBUGBREAK() __debugbreak()
#else
#define HM_DEBUGBREAK() __builtin_trap()
#endif
#define HM_ASSERT(x) if(!(x)) { HM_FATAL("Assertion failed at %s:%d: \"%s\"", __FILE__, __LINE__, #x); HM_DEBUGBREAK(); }
#else
#define HM_ASSERT(x)
#endif

HM_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to have a size of 4 bytes!");
HM_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to have a size of 8 bytes!");

HM_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to have a size of 1 byte!");
HM_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to have a size of 2 bytes!");
HM_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to have a size of 4 bytes!");
HM_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to have a size of 8 bytes!");

HM_STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to have a size of 1 byte!");
HM_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to have a size of 2 bytes!");
HM_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to have a size of 4 bytes!");
HM_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to have a size of 8 bytes!");

HM_STATIC_ASSERT(sizeof(b8) == 1, "Expected f32 to have a size of 1 byte!");
HM_STATIC_ASSERT(sizeof(b32) == 4, "Expected f64 to have a size of 4 bytes!");

#endif