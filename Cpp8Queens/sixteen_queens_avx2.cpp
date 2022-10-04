#define _CRT_SECURE_NO_WARNINGS  // We do NOT support Microsoft's War on Standards.

#include <bitset>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>

#include <immintrin.h>  // Using intel intrinsics to learn about it. Precondition: you need AVX2 at least (which you probably have).

#include "sixteen_queens_common.h"
#include "sixteen_queens_avx2.h"
#include "high_res_clock.h"
#include "write_solutions.h"

using namespace qns16cmn;

namespace qns16avx2
{
    using m256i = ::__m256i;

    // should test at run time and update a few function pointers. This is just an exercise, and AVX2 is OLD.
    // See documentation for int _may_i_use_cpu_feature (unsigned __int64 a); there are equivalent options for other compilers.
    // Should also look at the examples in https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX
    // And the movie: https://www.youtube.com/watch?v=AT5nuQQO96o 

    // Bitwise equality. 
    inline bool operator == (const m256i& a, const m256i& b)
    {
        return (0xffff'ffff == _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b)));
    }

    // Bitwise and.
    inline m256i operator & (const m256i& a, const m256i& b)
    {
        return _mm256_and_si256(a, b);
    }

    // Bitwise or.
    inline m256i operator | (const m256i& a, const m256i& b)
    {
        return _mm256_or_si256(a, b);
    }

    // Note: 
    // =====
    // From https://en.wikipedia.org/wiki/Eight_queens_puzzle#Counting_solutions_for_other_sizes_n
    // There are 14,772,512 solutions for n = 16, should get half of that. We'll just count them (expected 7'386'256), not build them.

    static uint_fast32_t failures_count = 0;
    static uint_fast32_t success_count = 0;
    static bool verbose = false;
    static int board_size = maximum_allowed_board_size; // Supported sizes: 4 - 16

    inline bool is_totally_under_threat(const map_t& map, int current_column)
    {
        const auto & column_mask = column_masks[current_column];
        return ((map & column_mask) == column_mask);
    }


#ifdef MY_COMPUTER_SUPPORTS_AVX2_AND_I_HAVE_TIME
    // Performance wise, this give us nothing for 16x16, and we lose for smaller board sizes.
    // But the code is shorter, and we learn a neat AVX2 trick. 
    // _p is for packed.
    static const alignas(64) m256i indices { .m256i_i16{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15 } };
    m256i not_threatened_rows_p(const map_t& map, int current_column)
    {
        return indices | _mm256_cmpeq_epi16(map & column_masks[current_column], column_masks[current_column]); // Sentinel where threatened.
    }
#endif // MY_COMPUTER_SUPPORTS_AVX2_AND_I_HAVE_TIME

    // Intel Intrinsics are not constexpr. Bummer.
    #define make_threat(row, column) (row_masks[row] | main_diagonal_parallels[row + 15 - column] | second_diagonal_parallels[row + column] )

    class Threats {
        std::vector<map_t> alignas(64) m_threats;
    public:
        Threats() : m_threats{
            make_threat(0, 0), 	make_threat(0, 1),  make_threat(0, 2), 	make_threat(0, 3), 	make_threat(0, 4), 	make_threat(0, 5), 	make_threat(0, 6), 	make_threat(0, 7), 	make_threat(0, 8), 	make_threat(0, 9), 	make_threat(0, 10), make_threat(0, 11),   make_threat(0, 12),  make_threat(0, 13), 	make_threat(0, 14),  make_threat(0, 15),
            make_threat(1, 0), 	make_threat(1, 1),  make_threat(1, 2), 	make_threat(1, 3), 	make_threat(1, 4), 	make_threat(1, 5), 	make_threat(1, 6), 	make_threat(1, 7), 	make_threat(1, 8), 	make_threat(1, 9), 	make_threat(1, 10), make_threat(1, 11),   make_threat(1, 12),  make_threat(1, 13), 	make_threat(1, 14),  make_threat(1, 15),
            make_threat(2, 0), 	make_threat(2, 1),  make_threat(2, 2), 	make_threat(2, 3), 	make_threat(2, 4), 	make_threat(2, 5), 	make_threat(2, 6), 	make_threat(2, 7), 	make_threat(2, 8), 	make_threat(2, 9), 	make_threat(2, 10), make_threat(2, 11),   make_threat(2, 12),  make_threat(2, 13), 	make_threat(2, 14),  make_threat(2, 15),
            make_threat(3, 0), 	make_threat(3, 1),  make_threat(3, 2), 	make_threat(3, 3), 	make_threat(3, 4), 	make_threat(3, 5), 	make_threat(3, 6), 	make_threat(3, 7), 	make_threat(3, 8), 	make_threat(3, 9), 	make_threat(3, 10), make_threat(3, 11),   make_threat(3, 12),  make_threat(3, 13), 	make_threat(3, 14),  make_threat(3, 15),
            make_threat(4, 0), 	make_threat(4, 1),  make_threat(4, 2), 	make_threat(4, 3), 	make_threat(4, 4), 	make_threat(4, 5), 	make_threat(4, 6), 	make_threat(4, 7), 	make_threat(4, 8), 	make_threat(4, 9), 	make_threat(4, 10), make_threat(4, 11),   make_threat(4, 12),  make_threat(4, 13), 	make_threat(4, 14),  make_threat(4, 15),
            make_threat(5, 0), 	make_threat(5, 1),  make_threat(5, 2), 	make_threat(5, 3), 	make_threat(5, 4), 	make_threat(5, 5), 	make_threat(5, 6), 	make_threat(5, 7), 	make_threat(5, 8), 	make_threat(5, 9), 	make_threat(5, 10), make_threat(5, 11),   make_threat(5, 12),  make_threat(5, 13), 	make_threat(5, 14),  make_threat(5, 15),
            make_threat(6, 0), 	make_threat(6, 1),  make_threat(6, 2), 	make_threat(6, 3), 	make_threat(6, 4), 	make_threat(6, 5), 	make_threat(6, 6), 	make_threat(6, 7), 	make_threat(6, 8), 	make_threat(6, 9), 	make_threat(6, 10), make_threat(6, 11),   make_threat(6, 12),  make_threat(6, 13), 	make_threat(6, 14),  make_threat(6, 15),
            make_threat(7, 0), 	make_threat(7, 1),  make_threat(7, 2), 	make_threat(7, 3), 	make_threat(7, 4), 	make_threat(7, 5), 	make_threat(7, 6), 	make_threat(7, 7), 	make_threat(7, 8), 	make_threat(7, 9), 	make_threat(7, 10), make_threat(7, 11),   make_threat(7, 12),  make_threat(7, 13), 	make_threat(7, 14),  make_threat(7, 15),
            make_threat(8, 0), 	make_threat(8, 1),  make_threat(8, 2), 	make_threat(8, 3), 	make_threat(8, 4), 	make_threat(8, 5), 	make_threat(8, 6), 	make_threat(8, 7), 	make_threat(8, 8), 	make_threat(8, 9), 	make_threat(8, 10), make_threat(8, 11),   make_threat(8, 12),  make_threat(8, 13), 	make_threat(8, 14),  make_threat(8, 15),
            make_threat(9, 0), 	make_threat(9, 1),  make_threat(9, 2), 	make_threat(9, 3), 	make_threat(9, 4), 	make_threat(9, 5), 	make_threat(9, 6), 	make_threat(9, 7), 	make_threat(9, 8), 	make_threat(9, 9), 	make_threat(9, 10), make_threat(9, 11),   make_threat(9, 12),  make_threat(9, 13), 	make_threat(9, 14),  make_threat(9, 15),
            make_threat(10, 0), make_threat(10, 1), make_threat(10, 2), make_threat(10, 3), make_threat(10, 4), make_threat(10, 5), make_threat(10, 6), make_threat(10, 7), make_threat(10, 8), make_threat(10, 9), make_threat(10, 10), make_threat(10, 11), make_threat(10, 12), make_threat(10, 13), make_threat(10, 14), make_threat(10, 15),
            make_threat(11, 0), make_threat(11, 1), make_threat(11, 2), make_threat(11, 3), make_threat(11, 4), make_threat(11, 5), make_threat(11, 6), make_threat(11, 7), make_threat(11, 8), make_threat(11, 9), make_threat(11, 10), make_threat(11, 11), make_threat(11, 12), make_threat(11, 13), make_threat(11, 14), make_threat(11, 15),
            make_threat(12, 0), make_threat(12, 1), make_threat(12, 2), make_threat(12, 3), make_threat(12, 4), make_threat(12, 5), make_threat(12, 6), make_threat(12, 7), make_threat(12, 8), make_threat(12, 9), make_threat(12, 10), make_threat(12, 11), make_threat(12, 12), make_threat(12, 13), make_threat(12, 14), make_threat(12, 15),
            make_threat(13, 0), make_threat(13, 1), make_threat(13, 2), make_threat(13, 3), make_threat(13, 4), make_threat(13, 5), make_threat(13, 6), make_threat(13, 7), make_threat(13, 8), make_threat(13, 9), make_threat(13, 10), make_threat(13, 11), make_threat(13, 12), make_threat(13, 13), make_threat(13, 14), make_threat(13, 15),
            make_threat(14, 0), make_threat(14, 1), make_threat(14, 2), make_threat(14, 3), make_threat(14, 4), make_threat(14, 5), make_threat(14, 6), make_threat(14, 7), make_threat(14, 8), make_threat(14, 9), make_threat(14, 10), make_threat(14, 11), make_threat(14, 12), make_threat(14, 13), make_threat(14, 14), make_threat(14, 15),
            make_threat(15, 0), make_threat(15, 1), make_threat(15, 2), make_threat(15, 3), make_threat(15, 4), make_threat(15, 5), make_threat(15, 6), make_threat(15, 7), make_threat(15, 8), make_threat(15, 9), make_threat(15, 10), make_threat(15, 11), make_threat(15, 12), make_threat(15, 13), make_threat(15, 14), make_threat(15, 15),
        }
        {
            // pass
        }
        inline const map_t&& Threaten(const map_t& map, int row, int col) const
        { 
            // Equivalent to this:
            // return (map | row_masks[row] | main_diagonal_parallels[row + 15 - column] | second_diagonal_parallels[row + column]);
            return std::move(map | m_threats[(size_t)(row * maximum_allowed_board_size + col)]);
        }
    }; 
    static const Threats threats;

    // map by value, because it't not const. 
    void do_solve(const map_t& map, std::vector<int>& solution, int current_column)
    {
        const int next_column = 1 + current_column;
        if (next_column == board_size)
        {
            // Success! Copy the solution. Don't move, we still need the buffer.
            if (success_count < solutions.size())
            {
                // TODO: USE AVX2 OR MEMCPY TO COPY THE DATA. 
                // INVARIANT: The destination has 16 integers, and the source has board_size.
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

        const map_t new_map = threats.Threaten(map, solution[current_column], current_column);
        if (is_totally_under_threat(new_map, next_column))
        {
            ++failures_count;
            return;
        }

#ifdef MY_COMPUTER_SUPPORTS_AVX2_AND_I_HAVE_TIME
        m256i not_threatened = not_threatened_rows_p(new_map, next_column);
        for (auto current_row : not_threatened.m256i_i16)
        {
            if (sentinel == current_row)
            {
                continue;
            }
            solution[next_column] = current_row;

            // Call recursively
            do_solve(new_map, solution, next_column);
        }
#else
        for (auto current_row : not_threatened_rows(new_map& column_masks[next_column], board_size, next_column))
        {
            if (sentinel == current_row)
            {
                break;
            }
            solution[next_column] = current_row;

            // Call recursively
            do_solve(new_map, solution, next_column);
        }

#endif // MY_COMPUTER_SUPPORTS_AVX2

        // Leave things as they were.
        solution[next_column] = -1;
    } // void do_solve(map_t map, std::vector<int>& solution, int current_column)

    void solve()
    {
        failures_count = 0ULL;
        success_count = 0ULL;
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

            map_t starting_map{ .m256i_u64 { 0ULL, 0ULL, 0ULL, 0ULL }  };
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

        std::time_t now = time(nullptr);

        const double average_time = double(tot_time) / double(loops);

        if (average_time < 1'000)
        {
            // Microseconds
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << min_time << " microseconds, the slowest took " << max_time
                << ", and an average of " << loops << " runs was " << average_time << "." << std::endl;
        }
        else if (average_time < 1'000'000)
        {
            // Milliseconds
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << double(min_time) / 1e3 << " milliseconds, the slowest took " << double(max_time) / 1e3
                << ", and an average of " << loops << " runs was " << average_time / 1e3 << "." << std::endl;
        }
        else
        {
            // Seconds. 16x16 takes above 17 seconds. 
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << double(min_time) / 1e6 << " seconds, the slowest took " << double(max_time) / 1e6
                << ", and an average of " << loops << " runs was " << average_time / 1e6 << "." << std::endl;
        }
        do_show_results(failures_count, success_count, solutions, board_size);
        std::cout.flush();
    }

    void solve_once()
    {
        failures_count = 0;
        success_count = 0;
        std::vector<int> solution(board_size, -1);
        const int starting_rows_to_test = (board_size / 2) + (board_size % 2);
        map_t starting_map{ .m256i_u64 { 0ULL, 0ULL, 0ULL, 0ULL } };
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

    void set_verbose(bool new_val)
    {
        std::cout << "Setting verbose to " << new_val << std::endl;
        verbose = new_val;
    }

    void test()
    {
        using std::cout;
        using std::endl;

    #ifdef _DEBUG
        map_t starting_map{ .m256i_u64 { 0ULL, 0ULL, 0ULL, 0ULL } };
        map_t threatened = threats.Threaten(starting_map, 2, 0);
        std::vector<int> solution(16, -1);
        solution[0] = 2;
        dbg::show_map(threatened, solution, board_size);
    #endif // def _DEBUG

        set_board_size(4);
        solve();

        set_board_size(8);
        solve();

        set_board_size(9);
        solve();

        set_board_size(12);
        solve();

        set_board_size(16);
        solve();

    }

    void set_board_size(int size)
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

} // namespace qns16avx2
