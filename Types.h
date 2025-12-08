#pragma once

// Include this file as <Types.h>
// プロジェクト内で共通利用する基本型定義ヘッダ
// 既存の External/BreadLibraryTarget/Types.h と互換性を保つことを目的とした簡易ラッパー
// 変更: このヘッダは `Bread::` 名前空間内の型エイリアスに加え、従来のグローバル型名
//       (s32, u32, 等) をそのまま使えるようグローバルエイリアスも提供します。

#include <cstddef>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
  #ifndef BREAD_TARGET_WIN
    #define BREAD_TARGET_WIN
  #endif
  // SIMD 型を使う場合に備えてインクルード（コンパイラで未サポートなら無視される）
  #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    #include <immintrin.h>
  #endif
#endif

namespace Bread
{
#if defined(BREAD_TARGET_WIN)
    using s8   = signed char;
    using u8   = unsigned char;
    using s16  = short;
    using u16  = unsigned short;
    using s32  = int;
    using u32  = unsigned int;
    using s64  = long long;
    using u64  = unsigned long long;
    using f32  = float;
    using f64  = double;
    using uintPtr = std::uintptr_t;
    using sizeT   = std::size_t;

    // プラットフォーム固有のベクトル型（プロジェクト内で使用されている場合がある）
    #if defined(__SSE__) || defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
      using vector = __m128;
    #endif

#else
    // 非 Windows プラットフォーム向けの標準型定義（可搬性を重視）
    using s8   = std::int8_t;
    using u8   = std::uint8_t;
    using s16  = std::int16_t;
    using u16  = std::uint16_t;
    using s32  = std::int32_t;
    using u32  = std::uint32_t;
    using s64  = std::int64_t;
    using u64  = std::uint64_t;
    using f32  = float;
    using f64  = double;
    using uintPtr = std::uintptr_t;
    using sizeT   = std::size_t;
#endif

} // namespace Bread

// 互換性レイヤ: 既存コードがグローバルな型名 (s32, u32, ...) を直接参照しているケースに対応
// Bread:: に加えてグローバル名前空間でも同名で使えるようにエイリアスを作成します。
using s8   = Bread::s8;
using u8   = Bread::u8;
using s16  = Bread::s16;
using u16  = Bread::u16;
using s32  = Bread::s32;
using u32  = Bread::u32;
using s64  = Bread::s64;
using u64  = Bread::u64;
using f32  = Bread::f32;
using f64  = Bread::f64;
using uintPtr = Bread::uintPtr;
using sizeT   = Bread::sizeT;

#if defined(BREAD_TARGET_WIN)
  // Windows 環境で使用される場合に備えて vector のグローバルエイリアスも提供（存在する場合のみ）
  #if defined(__SSE__) || defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
    using vector = Bread::vector;
  #endif
#endif

// 安全のため、古いコードでの大小文字混在 include 対応
#ifndef TYPES_H_INCLUDED_LEGACY_GUARD
#define TYPES_H_INCLUDED_LEGACY_GUARD
#endif
