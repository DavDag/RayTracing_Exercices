#pragma once

#include <cstdint>
#include <cfloat>

using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;

using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;

using f32  = double;
using f64  = double;

using c8   = char;

static_assert(sizeof(i8  ) == 1);
static_assert(sizeof(i16 ) == 2);
static_assert(sizeof(i32 ) == 4);
static_assert(sizeof(i64 ) == 8);

static_assert(sizeof(u8  ) == 1);
static_assert(sizeof(u16 ) == 2);
static_assert(sizeof(u32 ) == 4);
static_assert(sizeof(u64 ) == 8);

static_assert(sizeof(f32 ) == 8);
static_assert(sizeof(f64 ) == 8);

static_assert(sizeof(c8  ) == 1);

constexpr i8  i8max  = INT8_MAX;
constexpr i16 i16max = INT16_MAX;
constexpr i32 i32max = INT32_MAX;
constexpr i64 i64max = INT64_MAX;

constexpr u8  u8max  = UINT8_MAX;
constexpr u16 u16max = UINT16_MAX;
constexpr u32 u32max = UINT32_MAX;
constexpr u64 u64max = UINT64_MAX;

constexpr f32 f32max = FLT_MAX;
constexpr f64 f64max = DBL_MAX;

constexpr i8  i8min  = INT8_MIN;
constexpr i16 i16min = INT16_MIN;
constexpr i32 i32min = INT32_MIN;
constexpr i64 i64min = INT64_MIN;

constexpr u8  u8min  = 0;
constexpr u16 u16min = 0;
constexpr u32 u32min = 0;
constexpr u64 u64min = 0;

constexpr f32 f32min = FLT_MIN;
constexpr f64 f64min = DBL_MIN;

