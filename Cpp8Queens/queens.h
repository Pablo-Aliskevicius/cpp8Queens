#pragma once

// queens.h
// Solution for 8x8 board. Same as any board I've ever seen


namespace qns
{
    double solve(); // returns average microseconds
    void set_verbose(bool new_val);
    void set_short(int trials);
    void test();
    void set_board_size(int size);
    
}