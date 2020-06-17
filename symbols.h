#pragma once
#include "square.cpp"
#include "piece.cpp"
#include "color.cpp"
#include <assert.h>

using ull = unsigned long long;

constexpr ull FILE_A = (0x101010101010101ull);
constexpr ull FILE_B = (0x101010101010101ull << 1ull);
constexpr ull FILE_C = (0x101010101010101ull << 2ull);
constexpr ull FILE_D = (0x101010101010101ull << 3ull);
constexpr ull FILE_E = (0x101010101010101ull << 4ull);
constexpr ull FILE_F = (0x101010101010101ull << 5ull);
constexpr ull FILE_G = (0x101010101010101ull << 6ull);
constexpr ull FILE_H = (0x101010101010101ull << 7ull);

constexpr ull RANK_1 = (0xFFull);
constexpr ull RANK_2 = (0xFFull << 8ull);
constexpr ull RANK_3 = (0xFFull << 16ull);
constexpr ull RANK_4 = (0xFFull << 24ull);
constexpr ull RANK_5 = (0xFFull << 32ull);
constexpr ull RANK_6 = (0xFFull << 40ull);
constexpr ull RANK_7 = (0xFFull << 48ull);
constexpr ull RANK_8 = (0xFFull << 56ull);