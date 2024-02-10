#pragma once

// queens.h
// Solution for 8x8 board. Same as any board I've ever seen


namespace qns
{
    // namespace cannot be a template argument
    struct solver
    {
        static double solve(); // returns median microseconds
        static void set_verbose(bool new_val);
        static void set_short(int trials);
        static void test();
        static void set_board_size(int size);
    };
}