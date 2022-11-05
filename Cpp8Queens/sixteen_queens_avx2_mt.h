#pragma once

// sixteen_queens_avx2_mt.h
// Solution for 16x16, using AVX2 to improve performance (by about 40%)

namespace qns16avx2mt
{
    double solve(); // returns average microseconds
    void set_verbose(bool new_val);
    void test();
    void set_board_size(int size);
}