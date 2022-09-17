#include <bitset>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>

#include <immintrin.h>  // Using intel intrinsics to learn about it. Precondition: you need AVX2 at least (which you probably have).

#include "sixteen_queens.h"
#include "high_res_clock.h"
#include "write_solutions.h"

namespace qns16
{
    using flags_t = __m256i;
    using map_t = flags_t;

    inline bool operator == (const __m256i& a, const __m256i& b)
    {
        return (
            a.m256i_u64[0] == b.m256i_u64[0] &&
            a.m256i_u64[1] == b.m256i_u64[1] &&
            a.m256i_u64[2] == b.m256i_u64[2] &&
            a.m256i_u64[3] == b.m256i_u64[3]);
    }

#define MY_COMPUTER_SUPPORTS_AVX2
    inline __m256i operator & (const __m256i& a, const __m256i& b)
    {
#ifdef MY_COMPUTER_SUPPORTS_AVX2
        return _mm256_and_si256(a, b);
#else
        __m256i ret;
        ret.m256i_u64[0] = a.m256i_u64[0] & b.m256i_u64[0];
        ret.m256i_u64[1] = a.m256i_u64[1] & b.m256i_u64[1];
        ret.m256i_u64[2] = a.m256i_u64[2] & b.m256i_u64[2];
        ret.m256i_u64[3] = a.m256i_u64[3] & b.m256i_u64[3];
        return ret;
#endif
    }

    inline __m256i operator | (const __m256i& a, const __m256i& b)
    {
#ifdef MY_COMPUTER_SUPPORTS_AVX2
#ifdef _DEBUG
        auto ret = _mm256_or_si256(a, b);
        return ret;
#else
        return _mm256_or_si256(a, b);
#endif // DEBUG

#else
        __m256i ret;
        ret.m256i_u64[0] = a.m256i_u64[0] | b.m256i_u64[0];
        ret.m256i_u64[1] = a.m256i_u64[1] | b.m256i_u64[1];
        ret.m256i_u64[2] = a.m256i_u64[2] | b.m256i_u64[2];
        ret.m256i_u64[3] = a.m256i_u64[3] | b.m256i_u64[3];
        return ret;
#endif
    }

    // Note: 
    // =====
    // From https://en.wikipedia.org/wiki/Eight_queens_puzzle#Counting_solutions_for_other_sizes_n
    // There are 14,772,512 solutions for n = 16, should get half of that. We'll just count them (expected 7'386'256), not build them.

    static constexpr int maximum_allowed_board_size = 16; // memory allocations are based on this. 

    // static std::vector<std::vector<int>> solutions(14'772'512/2, std::vector<int>(16, -1)); 
    static uint_fast32_t failures_count = 0;
    static uint_fast32_t success_count = 0;
    static bool verbose = false;
    static int_fast32_t trials = -1;
    static int board_size = maximum_allowed_board_size; // Supported sizes: 4 - 16, but should use the regular version under 9.
    // We shall only show the first 20
    static std::vector<std::vector<int>> solutions(20, std::vector<int>(16, -1)); 


    // Using LibreOffice CALC to generate code.
    static constexpr flags_t row_masks[] = {
        {.m256i_u16 = {0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff }},
    };

    static_assert(sizeof(row_masks) / sizeof(row_masks[0]) == maximum_allowed_board_size,
        "Literal array of wrong size here.");

    static constexpr flags_t column_masks[] = {
        {.m256i_u16 = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, } },
        {.m256i_u16 = {0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, } },
        {.m256i_u16 = {0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, } },
        {.m256i_u16 = {0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, } },
        {.m256i_u16 = {0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, } },
        {.m256i_u16 = {0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, } },
        {.m256i_u16 = {0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, } },
        {.m256i_u16 = {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, } },
        {.m256i_u16 = {0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, } },
        {.m256i_u16 = {0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, } },
        {.m256i_u16 = {0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, } },
        {.m256i_u16 = {0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, } },
        {.m256i_u16 = {0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, } },
        {.m256i_u16 = {0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, } },
        {.m256i_u16 = {0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, } },
        {.m256i_u16 = {0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, } },
    };

    static_assert(sizeof(column_masks) / sizeof(column_masks[0]) == maximum_allowed_board_size,
        "Literal array of wrong size here.");


    static constexpr flags_t main_diagonal_parallels[] = {
{.m256i_u16 = {0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, } },
{.m256i_u16 = {0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, } },
{.m256i_u16 = {0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, } },
{.m256i_u16 = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, } },
{.m256i_u16 = {0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, } },
{.m256i_u16 = {0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, } },
{.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, } },

    };

    static_assert(sizeof(main_diagonal_parallels) / sizeof(main_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");

    static constexpr flags_t second_diagonal_parallels[] = {
        {.m256i_u16 = {0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, } },
        {.m256i_u16 = {0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, } },
        {.m256i_u16 = {0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, } },
        {.m256i_u16 = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, } },
        {.m256i_u16 = {0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, } },
        {.m256i_u16 = {0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, } },
        {.m256i_u16 = {0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, } },
        {.m256i_u16 = {0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, } },
    };

    static_assert(sizeof(second_diagonal_parallels) / sizeof(second_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");


    const int sentinel = -1;
    // Save some time in allocations at run time. 
    std::vector<std::vector<int>> safe_indices{
        std::vector<int>(maximum_allowed_board_size, sentinel), //  0
        std::vector<int>(maximum_allowed_board_size, sentinel), //  1 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  2 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  3 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  4 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  5 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  6 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  7 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  8 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  9 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 10 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 11 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 12
        std::vector<int>(maximum_allowed_board_size, sentinel), // 13 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 14 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 15
    };

#ifdef _DEBUG
    namespace dbg
    {
        void show_map(const map_t& map, const std::vector<int>& solution)
        {
            using std::cout;
            using std::endl;
            for (int row = 0; row < 16; ++row)
            {
                unsigned short current_row = map.m256i_u16[row];
                for (int col = 0; col < 16; ++col)
                {
                    if (col < solution.size() && solution[col] == row)
                    {
                        cout << 'Q';
                        continue;
                    }
                    unsigned short flag = column_masks[col].m256i_u16[0];
                    if ((flag & current_row) == flag)
                    {
                        cout << '*';
                        continue;
                    }
                    if (row >= board_size || col >= board_size)
                    {
                        cout << 'X';
                        continue;
                    }
                    cout << ((row + col) % 2 ? '+' : '-');
                }
                cout << endl;
            }
        }
    }
#endif

    inline bool is_totally_under_threat(map_t map, int current_column)
    {
        const auto column_mask = column_masks[current_column];
        return ((map & column_mask) == column_mask);
    }


    const std::vector<int>& not_threatened_rows(const map_t map, int current_column)
    {
        std::vector<int>& result = safe_indices[current_column];
        const map_t mask = (map & column_masks[current_column]);
        // One 64-bit integer == 8 chars.
        const unsigned short* pRow = mask.m256i_u16;
        int j = 0;
        // Manual loop unrolling, ugly as it looks, but fast like hell.
        // To allow several board sizes (4-8), we'll use something inspired in the Duff Device.
        int_fast8_t i = -1;
        switch (board_size)
        {
        case 16:
        default:
            if (!pRow[++i]) result[j++] = i;
        case 15:
            if (!pRow[++i]) result[j++] = i;
        case 14:
            if (!pRow[++i]) result[j++] = i;
        case 13:
            if (!pRow[++i]) result[j++] = i;
        case 12:
            if (!pRow[++i]) result[j++] = i;
        case 11:
            if (!pRow[++i]) result[j++] = i;
        case 10:
            if (!pRow[++i]) result[j++] = i;
        case 9:
            if (!pRow[++i]) result[j++] = i;
        case 8:
            if (!pRow[++i]) result[j++] = i;
        case 7:
            if (!pRow[++i]) result[j++] = i;
        case 6:
            if (!pRow[++i]) result[j++] = i;
        case 5:
            if (!pRow[++i]) result[j++] = i;
        case 4:
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
        }

        if (j < result.size())
        {
            result[j] = sentinel;
        }
        return result;
    }

    // Some good old-fashioned template metaprogramming to OR masks at compile-time.
    //constexpr map_t make_threat(int row, int column)
    //{
    //    return (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column]);
    //};
    
    #define make_threat(row, column) (row_masks[row] | main_diagonal_parallels[row + 15 - column] | second_diagonal_parallels[row + column])

    class Threats {
        std::vector<map_t> m_threats;
    public:
        Threats() : m_threats{
            make_threat(0, 0), 	make_threat(0, 1), 	make_threat(0, 2), 	make_threat(0, 3), 	make_threat(0, 4), 	make_threat(0, 5), 	make_threat(0, 6), 	make_threat(0, 7), 	make_threat(0, 8), 	make_threat(0, 9), 	make_threat(0, 10), 	make_threat(0, 11), 	make_threat(0, 12), 	make_threat(0, 13), 	make_threat(0, 14), 	make_threat(0, 15),
            make_threat(1, 0), 	make_threat(1, 1), 	make_threat(1, 2), 	make_threat(1, 3), 	make_threat(1, 4), 	make_threat(1, 5), 	make_threat(1, 6), 	make_threat(1, 7), 	make_threat(1, 8), 	make_threat(1, 9), 	make_threat(1, 10), 	make_threat(1, 11), 	make_threat(1, 12), 	make_threat(1, 13), 	make_threat(1, 14), 	make_threat(1, 15),
            make_threat(2, 0), 	make_threat(2, 1), 	make_threat(2, 2), 	make_threat(2, 3), 	make_threat(2, 4), 	make_threat(2, 5), 	make_threat(2, 6), 	make_threat(2, 7), 	make_threat(2, 8), 	make_threat(2, 9), 	make_threat(2, 10), 	make_threat(2, 11), 	make_threat(2, 12), 	make_threat(2, 13), 	make_threat(2, 14), 	make_threat(2, 15),
            make_threat(3, 0), 	make_threat(3, 1), 	make_threat(3, 2), 	make_threat(3, 3), 	make_threat(3, 4), 	make_threat(3, 5), 	make_threat(3, 6), 	make_threat(3, 7), 	make_threat(3, 8), 	make_threat(3, 9), 	make_threat(3, 10), 	make_threat(3, 11), 	make_threat(3, 12), 	make_threat(3, 13), 	make_threat(3, 14), 	make_threat(3, 15),
            make_threat(4, 0), 	make_threat(4, 1), 	make_threat(4, 2), 	make_threat(4, 3), 	make_threat(4, 4), 	make_threat(4, 5), 	make_threat(4, 6), 	make_threat(4, 7), 	make_threat(4, 8), 	make_threat(4, 9), 	make_threat(4, 10), 	make_threat(4, 11), 	make_threat(4, 12), 	make_threat(4, 13), 	make_threat(4, 14), 	make_threat(4, 15),
            make_threat(5, 0), 	make_threat(5, 1), 	make_threat(5, 2), 	make_threat(5, 3), 	make_threat(5, 4), 	make_threat(5, 5), 	make_threat(5, 6), 	make_threat(5, 7), 	make_threat(5, 8), 	make_threat(5, 9), 	make_threat(5, 10), 	make_threat(5, 11), 	make_threat(5, 12), 	make_threat(5, 13), 	make_threat(5, 14), 	make_threat(5, 15),
            make_threat(6, 0), 	make_threat(6, 1), 	make_threat(6, 2), 	make_threat(6, 3), 	make_threat(6, 4), 	make_threat(6, 5), 	make_threat(6, 6), 	make_threat(6, 7), 	make_threat(6, 8), 	make_threat(6, 9), 	make_threat(6, 10), 	make_threat(6, 11), 	make_threat(6, 12), 	make_threat(6, 13), 	make_threat(6, 14), 	make_threat(6, 15),
            make_threat(7, 0), 	make_threat(7, 1), 	make_threat(7, 2), 	make_threat(7, 3), 	make_threat(7, 4), 	make_threat(7, 5), 	make_threat(7, 6), 	make_threat(7, 7), 	make_threat(7, 8), 	make_threat(7, 9), 	make_threat(7, 10), 	make_threat(7, 11), 	make_threat(7, 12), 	make_threat(7, 13), 	make_threat(7, 14), 	make_threat(7, 15),
            make_threat(8, 0), 	make_threat(8, 1), 	make_threat(8, 2), 	make_threat(8, 3), 	make_threat(8, 4), 	make_threat(8, 5), 	make_threat(8, 6), 	make_threat(8, 7), 	make_threat(8, 8), 	make_threat(8, 9), 	make_threat(8, 10), 	make_threat(8, 11), 	make_threat(8, 12), 	make_threat(8, 13), 	make_threat(8, 14), 	make_threat(8, 15),
            make_threat(9, 0), 	make_threat(9, 1), 	make_threat(9, 2), 	make_threat(9, 3), 	make_threat(9, 4), 	make_threat(9, 5), 	make_threat(9, 6), 	make_threat(9, 7), 	make_threat(9, 8), 	make_threat(9, 9), 	make_threat(9, 10), 	make_threat(9, 11), 	make_threat(9, 12), 	make_threat(9, 13), 	make_threat(9, 14), 	make_threat(9, 15),
            make_threat(10, 0), 	make_threat(10, 1), 	make_threat(10, 2), 	make_threat(10, 3), 	make_threat(10, 4), 	make_threat(10, 5), 	make_threat(10, 6), 	make_threat(10, 7), 	make_threat(10, 8), 	make_threat(10, 9), 	make_threat(10, 10), 	make_threat(10, 11), 	make_threat(10, 12), 	make_threat(10, 13), 	make_threat(10, 14), 	make_threat(10, 15),
            make_threat(11, 0), 	make_threat(11, 1), 	make_threat(11, 2), 	make_threat(11, 3), 	make_threat(11, 4), 	make_threat(11, 5), 	make_threat(11, 6), 	make_threat(11, 7), 	make_threat(11, 8), 	make_threat(11, 9), 	make_threat(11, 10), 	make_threat(11, 11), 	make_threat(11, 12), 	make_threat(11, 13), 	make_threat(11, 14), 	make_threat(11, 15),
            make_threat(12, 0), 	make_threat(12, 1), 	make_threat(12, 2), 	make_threat(12, 3), 	make_threat(12, 4), 	make_threat(12, 5), 	make_threat(12, 6), 	make_threat(12, 7), 	make_threat(12, 8), 	make_threat(12, 9), 	make_threat(12, 10), 	make_threat(12, 11), 	make_threat(12, 12), 	make_threat(12, 13), 	make_threat(12, 14), 	make_threat(12, 15),
            make_threat(13, 0), 	make_threat(13, 1), 	make_threat(13, 2), 	make_threat(13, 3), 	make_threat(13, 4), 	make_threat(13, 5), 	make_threat(13, 6), 	make_threat(13, 7), 	make_threat(13, 8), 	make_threat(13, 9), 	make_threat(13, 10), 	make_threat(13, 11), 	make_threat(13, 12), 	make_threat(13, 13), 	make_threat(13, 14), 	make_threat(13, 15),
            make_threat(14, 0), 	make_threat(14, 1), 	make_threat(14, 2), 	make_threat(14, 3), 	make_threat(14, 4), 	make_threat(14, 5), 	make_threat(14, 6), 	make_threat(14, 7), 	make_threat(14, 8), 	make_threat(14, 9), 	make_threat(14, 10), 	make_threat(14, 11), 	make_threat(14, 12), 	make_threat(14, 13), 	make_threat(14, 14), 	make_threat(14, 15),
            make_threat(15, 0), 	make_threat(15, 1), 	make_threat(15, 2), 	make_threat(15, 3), 	make_threat(15, 4), 	make_threat(15, 5), 	make_threat(15, 6), 	make_threat(15, 7), 	make_threat(15, 8), 	make_threat(15, 9), 	make_threat(15, 10), 	make_threat(15, 11), 	make_threat(15, 12), 	make_threat(15, 13), 	make_threat(15, 14), 	make_threat(15, 15),
        }
        {
            // pass
        }
        map_t Get(int row, int col) const { return m_threats[(size_t)row * 16 + col]; }
    }; 
    const Threats threats;

    // Everything by value, on purpose.
    inline map_t threaten(map_t map, int row, int column)
    {
        // Before template metaprogramming we did this:
        //  map |= (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column]);
        return map | threats.Get(row, column);
    }

    void do_solve(map_t map, std::vector<int>& solution, int current_column)
    {
        if (current_column == (board_size - 1))
        {
            // Success! Copy the solution. Don't move, we still need the buffer.
            if (success_count < solutions.size())
            {
                std::copy(solution.cbegin(), solution.cend(), solutions[success_count].begin());
            }
            ++success_count;
#ifdef _DEBUG
            if ((success_count & 0xff) == 0x7ff)
            {
                std::cout 
                    << "    (So far " << std::dec << success_count << " solutions and " 
                    << failures_count << " failures.)" << std::endl;
                std::cout.flush();
            }
#endif // _DEBUG
            return;
        }
        const map_t new_map = threaten(map, solution[current_column], current_column);
        const int next_column = 1 + current_column;

#ifdef _DEBUG
        //dbg::show_map(new_map, solution);
#endif

        if (is_totally_under_threat(new_map, next_column))
        {
            ++failures_count;
            return;
        }
        for (auto current_row : not_threatened_rows(new_map, next_column))
        {
            if (sentinel == current_row)
            {
                break;
            }
            solution[next_column] = current_row;

            // Call recursively
            do_solve(new_map, solution, next_column);
        }
        // Leave things as they were.
        solution[next_column] = -1;
    }
} // namespace qns16

#define FOR_PROFILING
#ifdef FOR_PROFILING
void qns16::solve()
{
    failures_count = 0;
    success_count = 0;
    std::vector<int> solution(board_size, -1);
    const int loops = int(pow(16 - board_size, 3)) + 1;

    const int starting_rows_to_test = (board_size / 2) + (board_size % 2);
    hi_res_timer::microsecs_t max_time = 0ULL;
    hi_res_timer::microsecs_t min_time = std::numeric_limits<hi_res_timer::microsecs_t>::max();
    hi_res_timer::microsecs_t tot_time = 0ULL;


    for (int loop = 0; loop < loops; ++loop)
    {
        failures_count = 0;
        success_count = 0;
        map_t starting_map{ 0ULL };
        for (int i = board_size; i < maximum_allowed_board_size; ++i)
        {
            starting_map = starting_map | row_masks[i];
        }
        hi_res_timer timer;
        for (int_fast8_t current_row = 0; current_row < starting_rows_to_test; ++current_row)
        {
            solution[0] = current_row;
            do_solve(starting_map, solution, 0);
        }
        timer.Stop();
        auto microseconds = timer.GetElapsedMicroseconds();
        // std::cout << "Resolving took " << microseconds << " microseconds." << std::endl;
        if (microseconds < min_time) min_time = microseconds;
        if (microseconds > max_time) max_time = microseconds;
        tot_time += microseconds;
    }
    std::cout << "The fastest run took " << min_time << " microseconds, the slowest took " << max_time
        << ", and an average of " << loops << " runs was " << double(tot_time) / loops << std::endl
        << "We had " << success_count << " solutions and "
        << failures_count << " failures for half a board of size " << board_size
        << "." << std::endl;
    do_show_results(failures_count, success_count, solutions, board_size);
    std::cout.flush();
}
#else
void qns16::solve()
{
    failures_count = 0;
    success_count = 0;
    std::vector<int> solution(board_size, -1);
    const int starting_rows_to_test = (board_size / 2) + (board_size % 2);
    map_t starting_map{ 0ULL };
    for (int i = board_size; i < maximum_allowed_board_size; ++i)
    {
        starting_map = starting_map | row_masks[i];
    }
    hi_res_timer timer;
    for (int_fast8_t current_row = 0; current_row < starting_rows_to_test; ++current_row)
    {
        solution[0] = current_row;
        do_solve(starting_map, solution, 0);
    }
    timer.Stop();
    std::cout << "Resolving took " << timer.GetElapsedMicroseconds() 
        << " microseconds, with " << success_count << " solutions and " 
        << failures_count << " failures for half a board of size " << board_size  
        << "." << std::endl;
    std::cout.flush();
    do_show_results(failures_count, success_count, solutions, board_size);
    std::cout.flush();
}
#endif // FOR_PROFILING

void qns16::set_verbose(bool new_val)
{
    std::cout << "Setting verbose to " << new_val << std::endl;
    verbose = new_val;
}

void qns16::set_short(int trials$)
{
    std::cout << "Setting trials to " << trials$ << std::endl;
    trials = trials$;
}

void qns16::test()
{
    using std::cout;
    using std::endl;

#ifdef _DEBUG
    map_t starting_map{ 0ULL };
    map_t threatened = threaten(starting_map, 2, 0);
    std::vector<int> solution(16, -1);
    solution[0] = 2;
    dbg::show_map(threatened, solution);

    /*

1>
1>Resolving took 20 microseconds, with 2 solutions and 23 failures for half a board of size 6.
1>Resolving took 10 microseconds, with 23 solutions and 87 failures for half a board of size 7.
1>Resolving took 53 microseconds, with 46 solutions and 322 failures for half a board of size 8.
1>Resolving took 314 microseconds, with 203 solutions and 1438 failures for half a board of size 9.
1>Resolving took 1022 microseconds, with 362 solutions and 6025 failures for half a board of size 10.
1>Resolving took 4855 microseconds, with 1515 solutions and 31984 failures for half a board of size 11.
1>Resolving took 12798 microseconds, with 7100 solutions and 150265 failures for half a board of size 12.
1>Resolving took 80257 microseconds, with 40891 solutions and 889144 failures for half a board of size 13.
1>Resolving took 425108 microseconds, with 182798 solutions and 4832548 failures for half a board of size 14.
1>Resolving took 2.87394e+06 microseconds, with 1248961 solutions and 32331747 failures for half a board of size 15.
1>A task was canceled.
1>A task was canceled.

1>The fastest run took 2.81394e+06 microseconds, the slowest took 2.88681e+06, and an average of 2 runs was 2.85037e+06
1>We had 1248961 solutions and 32331747 failures for half a board of size 15.
1>We had 32331747 failures, and 1248961 solutions (filled by symmetry to 40) in a board of size 15 by 15.
1>The solutions are:
1>
1>Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-    Q+-+-+-+-+-+-+-
1>+-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+    +-Q-+-+-+-+-+-+
1>-+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-    -+-+Q+-+-+-+-+-
1>+Q+-+-+-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+    +-+-+-Q-+-+-+-+
1>-+-+-+-+-Q-+-+-    -+-+-+-+Q+-+-+-    -+-+-+-+-+-Q-+-    -+-+-+-+-+-Q-+-    -+-+-+-+-+-Q-+-    -+-+-+-+-+-Q-+-    -+-+-+-+-+-+Q+-    -+-+-+-+-+-+-Q-    -+-+-+-+-+-+-Q-
1>+-+-+-+-+-+Q+-+    +-+-+-+-+-+-+Q+    +-+-+-+-+Q+-+-+    +-+-+-+-+Q+-+-+    +-+-+-+-+-+-+Q+    +-+-+-+-+-+-+Q+    +-+Q+-+-+-+-+-+    +-+-+-+-+Q+-+-+    +-+-+-+-+Q+-+-+
1>-+-+-+-+-+-+-Q-    -+-+-+-+-+-Q-+-    -+-+-+-+-+-+-+Q    -+-+-+-+-+-+-+Q    -+-+-Q-+-+-+-+-    -+-+-+-+-+Q+-+-    -+-+-+-+-+-+-Q-    -Q-+-+-+-+-+-+-    -+-+-+-+-+-+Q+-
1>+-+Q+-+-+-+-+-+    +Q+-+-+-+-+-+-+    +-+Q+-+-+-+-+-+    +-+-+-+-+-+-Q-+    +-+-+-+-+-+-Q-+    +-+Q+-+-+-+-+-+    +-+-+-+-+-+Q+-+    +-+-+-+-+-+-Q-+    +-+Q+-+-+-+-+-+
1>-+-+-+-+-+-+Q+-    -+-+-+-+-+-+-+Q    -+-+-+-+-+-+-Q-    -+-+-Q-+-+-+-+-    -+-+-+-+-+-+-+Q    -+-+-Q-+-+-+-+-    -+-+-+-+-+-+-+Q    -+-+-+-+-+-+-+Q    -+-+-Q-+-+-+-+-
1>+-+-+-+-Q-+-+-+    +-+-+-+Q+-+-+-+    +-+-+-+-Q-+-+-+    +-+Q+-+-+-+-+-+    +-+Q+-+-+-+-+-+    +-+-+-+-+-+-+-Q    +-+-+-+-Q-+-+-+    +-+-+-+Q+-+-+-+    +-+-+-+-+-+-+-Q
1>-+-+-Q-+-+-+-+-    -+-+-Q-+-+-+-+-    -Q-+-+-+-+-+-+-    -Q-+-+-+-+-+-+-    -+-+-+-Q-+-+-+-    -Q-+-+-+-+-+-+-    -+-+-Q-+-+-+-+-    -+-Q-+-+-+-+-+-    -Q-+-+-+-+-+-+-
1>+-+-+-+-+-+-+-Q    +-+Q+-+-+-+-+-+    +-+-+Q+-+-+-+-+    +-+-+-+Q+-+-+-+    +-+-+-+-+Q+-+-+    +-+-+-+-+Q+-+-+    +Q+-+-+-+-+-+-+    +-+-+-+-+-Q-+-+    +-+-+-+-+-Q-+-+
1>-+-+-+Q+-+-+-+-    -+-+-+-+-Q-+-+-    -+-+-+-Q-+-+-+-    -+-+-+-+-+Q+-+-    -Q-+-+-+-+-+-+-    -+-+-+-Q-+-+-+-    -+-+-+-+-Q-+-+-    -+-+-+-+Q+-+-+-    -+-+-+-Q-+-+-+-
1>+-+-+-+-+-Q-+-+    +-+-+-+-+-+-Q-+    +-+-+-+-+-Q-+-+    +-+-+-+-Q-+-+-+    +-+-+-+-Q-+-+-+    +-+-+-+-+-+-Q-+    +-+-+-+Q+-+-+-+    +-+-+Q+-+-+-+-+    +-+-+-+-+-+Q+-+
1>-+-+-+-Q-+-+-+-    -+-+-+-+-+Q+-+-    -+-+-+-+-+-+Q+-    -+-+-+-+-+-+-Q-    -+-+-+-+-+Q+-+-    -+-+-+-+Q+-+-+-    -+-+-+-+-+Q+-+-    -+-+-+-+-+-Q-+-    -+-+-+-+Q+-+-+-
    
    */

    solution[0] = -1;
    std::cout << "Column masks [0]" << std::endl;
    dbg::show_map(column_masks[0], solution);

    std::cout << "Column masks [1]" << std::endl;
    dbg::show_map(column_masks[1], solution);

    std::cout << "Column masks [2]" << std::endl;
    dbg::show_map(column_masks[2], solution);

    std::cout << "main_diagonal_parallels[0]" << std::endl;
    dbg::show_map(main_diagonal_parallels[0], solution);

    std::cout << "main_diagonal_parallels[1]" << std::endl;
    dbg::show_map(main_diagonal_parallels[1], solution);

    std::cout << "main_diagonal_parallels[2]" << std::endl;
    dbg::show_map(main_diagonal_parallels[2], solution);

    std::cout << "main_diagonal_parallels[7]" << std::endl;
    dbg::show_map(main_diagonal_parallels[7], solution);

    std::cout << "main_diagonal_parallels[8]" << std::endl;
    dbg::show_map(main_diagonal_parallels[8], solution);

    std::cout << "main_diagonal_parallels[15]" << std::endl;
    dbg::show_map(main_diagonal_parallels[15], solution);

    

    std::cout << "second_diagonal_parallels[0]" << std::endl;
    dbg::show_map(second_diagonal_parallels[0], solution);

    std::cout << "second_diagonal_parallels[1]" << std::endl;
    dbg::show_map(second_diagonal_parallels[1], solution);

    std::cout << "second_diagonal_parallels[2]" << std::endl;
    dbg::show_map(second_diagonal_parallels[2], solution);

    std::cout << "second_diagonal_parallels[7]" << std::endl;
    dbg::show_map(second_diagonal_parallels[7], solution);

    std::cout << "second_diagonal_parallels[8]" << std::endl;
    dbg::show_map(second_diagonal_parallels[8], solution);

    std::cout << "second_diagonal_parallels[15]" << std::endl;
    dbg::show_map(second_diagonal_parallels[15], solution);

#endif // def _DEBUG

    set_board_size(4);
    solve();

    set_board_size(8);
    solve();

    set_board_size(9);
    solve();

    set_board_size(12);
    solve();

    //set_board_size(16);
    //solve();

}

void qns16::set_board_size(int size)
{
    if (size < 4)
    {
        std::cout << "Size must be at least 4, it is " << size << ". Doing nothing.";
        return;
    }
    if (size > 16)
    {
        std::cout << "Size must be at most 16, it is " << size << ". Doing nothing.";
        return;
    }
    board_size = size;

}