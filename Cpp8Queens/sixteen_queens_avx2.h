#pragma once

// sixteen_queens_avx2.h
// Solution for 16x16, using AVX2 to improve performance (by about 40%)

namespace qns16avx2
{
    // namespace cannot be a template argument
    struct solver
    {
        static double solve(); // returns median microseconds
        static void set_verbose(bool new_val);
        static void test();
        static void set_board_size(int size);
    };
}