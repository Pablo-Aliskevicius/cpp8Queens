#pragma once

// sixteen_queens.h
// Solution for 16x16


namespace qns16
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