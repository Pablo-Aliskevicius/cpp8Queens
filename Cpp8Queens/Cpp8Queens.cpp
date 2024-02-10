// Cpp8Queens.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <ctype.h>
#include <iostream>
#include <map>

#include <immintrin.h>

#include "queens.h"
#include "high_res_clock.h"
#include "sixteen_queens_common.h"
#include "sixteen_queens.h"
#include "sixteen_queens_avx2.h"
#include "sixteen_queens_avx2_mt.h"

/*
Command line arguments:
-v   verbose
-t   test
-s n short(n) - try only N different solutions, showing failures

*/

using microsecs_t = double;

enum class solution_type
{
    sixty_four_standard,
    avx2_multi_threaded,
    avx2_single_threaded,
    two_fifty_six_standard
};

// Template lambdas require an argument. Old fashioned is good.
template<typename Solver, typename durations_t>
void run(durations_t& durations, int first_size, int end_size, solution_type sol_type)
{
    for (int desired_board_size = first_size; desired_board_size < end_size; ++desired_board_size)
    {
        Solver::set_board_size(desired_board_size);
        microsecs_t median_duration = Solver::solve();
        durations[desired_board_size][sol_type] = median_duration;
    }
};

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
                    qns::solver::set_short(short_trials);
                }
        } // switch
    }

    if (verbose)
    {
        qns::solver::set_verbose(true);
    }
    if (test)
    {
        qns::solver::test();
        qns16cmn::test();
        qns16::solver::test();
        qns16avx2::solver::test();
        return 0;
    }

    std::map< int, std::map<solution_type, microsecs_t> > durations;

    std::cout << "****************************** 64-bits, standard code ******************************" << std::endl;
    run<qns::solver, decltype(durations)>(durations, 4, 9, solution_type::sixty_four_standard);

    // Don't feel like adding a header just to declare two functions.
    extern void print_out_instruction_sets();
    print_out_instruction_sets();

    extern bool avx2_supported();
    if (avx2_supported())
    {
        std::cout << "****************************** 256-bits, AVX2, multi threaded ******************************" << std::endl;
        // I have four cores, no point trying under 8.
        run<qns16avx2mt::solver, decltype(durations)>(durations, 8, 17, solution_type::avx2_multi_threaded);

        std::cout << "****************************** 256-bits, AVX2, single threaded *****************************" << std::endl;
        run<qns16avx2::solver, decltype(durations)>(durations, 4, 17, solution_type::avx2_single_threaded);
    }

    // Reference: support 16 by 16 without using AVX2.
    run<qns16::solver, decltype(durations)>(durations, 4, 17, solution_type::two_fifty_six_standard);

    // Display data. C++ 20 brings some handy methods, very nice to have.  
    using std::cout;
    using std::endl;
    using std::setw;
    std::locale::global(std::locale("en_US.UTF-8")); // use comma for thousands separator. 
    auto ts = [](double d) { 
        return std::format("{:.3Lf}", d); 
    };
    cout 
        << "***************** Median durations (microseconds) ****************" << endl 
        << "Size,      64 bits,       256 bits,           AVX2, AVX2 Multithreaded" << endl
        << "---    ------------ --------------- --------------- --------------" << endl
        ;
    const char* sep = ",";
    const char* na = "N/A";
    auto either_or_na = [&ts, &na](std::map<solution_type, microsecs_t>& d_curr, solution_type st) {
        return (d_curr.contains(st) ? ts(d_curr[st]) : na);
    };
    for (int board_size = 4; board_size < 17; ++board_size)
    {
        std::map<solution_type, microsecs_t>& d_current = durations[board_size];
        cout << setw(2) << board_size << sep
            << setw(15) << either_or_na(d_current, solution_type::sixty_four_standard)  << sep
            << setw(15) << either_or_na(d_current, solution_type::two_fifty_six_standard)  << sep
            << setw(15) << either_or_na(d_current, solution_type::avx2_single_threaded) << sep
            << setw(15) << either_or_na(d_current, solution_type::avx2_multi_threaded) << endl
            ;
    }

    return 0;
}
