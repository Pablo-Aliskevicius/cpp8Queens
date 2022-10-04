// Cpp8Queens.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctype.h>

#include <immintrin.h>

#include "queens.h"
#include "sixteen_queens_common.h"
#include "sixteen_queens.h"
#include "sixteen_queens_avx2.h"

/*
Command line arguments:
-v   verbose
-t   test
-s n short(n) - try only N different solutions, showing failures

*/

int main(int argc, const char** argv)
{
    bool verbose = false;
    bool test = false;

    for (int i = 1; i < argc; ++i)
    {
        const char* arg = argv[i];
        if (arg[0] != '-' && arg[0] != '/')
            continue; // for

        switch (tolower(arg[1]))
        {
            case 'v':
                verbose = true;
                break;
            case 't':
                test = true;
                break;
            case 's':
                int short_trials = atoi(argv[++i]);
                if (0 < short_trials)
                {
                    qns::set_short(short_trials);
                }
        } // switch
    }

    if (verbose)
    {
        qns::set_verbose(true);
    }
    if (test)
    {
        qns::test();
        qns16cmn::test();
        qns16::test();
        qns16avx2::test();
        return 0;
    }
    for (int desired_board_size = 4; desired_board_size < 9; ++desired_board_size)
    {
        qns::set_board_size(desired_board_size);
        qns::solve();
    }

    for (int desired_board_size = 4; desired_board_size < 17; ++desired_board_size)
    {
        qns16::set_board_size(desired_board_size);
        qns16::solve();
    }

    // Don't feel like adding a header just to declare two functions.
    extern bool avx2_supported();
    extern void print_out_instruction_sets();

    print_out_instruction_sets();

    if (avx2_supported())
    {
        for (int desired_board_size = 4; desired_board_size < 17; ++desired_board_size)
        {
            qns16avx2::set_board_size(desired_board_size);
            qns16avx2::solve();
        }
    }

    return 0;
}
