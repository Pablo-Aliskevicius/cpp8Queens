#include <bitset>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>

#include "queens.h"
#include "high_res_clock.h"
#include "write_solutions.h"

namespace qns
{
    using flags_t = uint_fast64_t;
    using map_t = flags_t;

    static std::vector<std::vector<int>> solutions(46, std::vector<int>(8, -1)); // use 46 if optimizing for symmetry.
    static uint_fast16_t failures_count = 0; // up to 20'160 = 4 * 7!
    static uint_fast16_t success_count = 0; // up to 20'160 = 4 * 7!
    static bool verbose = false;
    static int_fast16_t trials = -1;
    static int board_size = 8; // Supported sizes: 4 - 8.

    static constexpr int maximum_allowed_board_size = 8; // memory allocations are based on this. 


    // If you consider this is cheating, use template metaprogramming.
    static constexpr flags_t row_masks[] = {
        0xff,
        0xff00,
        0xff0000,
        0xff000000,
        0xff00000000,
        0xff0000000000,
        0xff000000000000,
        0xff00000000000000,
    };

    static_assert(sizeof(row_masks) / sizeof(row_masks[0]) == maximum_allowed_board_size,
        "Literal array of wrong size here.");

    // HEY, WE'RE LITTLE ENDIAN HERE!
    static constexpr flags_t column_masks[] = {
        0x8080808080808080,
        0x4040404040404040,
        0x2020202020202020,
        0x1010101010101010,
        0x0808080808080808,
        0x0404040404040404,
        0x0202020202020202,
        0x0101010101010101,
    };

    static_assert(sizeof(column_masks) / sizeof(column_masks[0]) == maximum_allowed_board_size,
        "Literal array of wrong size here.");


    // Visualize:
    //   b"1000 0000" // == 0x80 << 7
    //   b"0100 0000" // == 0x40 << 6
    //   b"0010 0000" // == 0x20 << 5
    // ...
    // We may have an 'endian' issue. 
    // static const flags_t main_diagonal_mask = 0x0102040810204080;

    static constexpr flags_t main_diagonal_parallels[] = {
        0x0000'0000'0000'0001, // 0
        0x0000'0000'0000'0102, // 1
        0x0000'0000'0001'0204, // 2
        0x0000'0000'0102'0408, // 3
        0x0000'0001'0204'0810, // 4
        0x0000'0102'0408'1020, // 5
        0x0001'0204'0810'2040, // 6
        0x0102'0408'1020'4080, // 7
        0x0204'0810'2040'8000, // 8
        0x0408'1020'4080'0000, // 9
        0x0810'2040'8000'0000, // a
        0x1020'4080'0000'0000, // b
        0x2040'8000'0000'0000, // c
        0x4080'0000'0000'0000, // d
        0x8000'0000'0000'0000, // e
    };

    static_assert(sizeof(main_diagonal_parallels) / sizeof(main_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");


    // static const flags_t secondary_diagonal_mask = 0x8040201008040201;
    static constexpr flags_t second_diagonal_parallels[] = {
        0x0000'0000'0000'0080, // 0
        0x0000'0000'0000'8040, // 1
        0x0000'0000'0080'4020, // 2
        0x0000'0000'8040'2010, // 3
        0x0000'0080'4020'1008, // 4
        0x0000'8040'2010'0804, // 5
        0x0080'4020'1008'0402, // 6
        0x8040'2010'0804'0201, // 7
        0x4020'1008'0402'0100, // 8
        0x2010'0804'0201'0000, // 9
        0x1008'0402'0100'0000, // a
        0x0804'0201'0000'0000, // b
        0x0402'0100'0000'0000, // c
        0x0201'0000'0000'0000, // d
        0x0100'0000'0000'0000, // e
    };

    static_assert(sizeof(second_diagonal_parallels) / sizeof(second_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");


    const int sentinel = -1;
    // Save some time in allocations at run time. 
    std::vector<std::vector<int>> safe_indices{
        std::vector<int>(maximum_allowed_board_size, sentinel),  // 0
        std::vector<int>(maximum_allowed_board_size, sentinel),
        std::vector<int>(maximum_allowed_board_size, sentinel),
        std::vector<int>(maximum_allowed_board_size, sentinel),  // 3
        std::vector<int>(maximum_allowed_board_size, sentinel),
        std::vector<int>(maximum_allowed_board_size, sentinel),
        std::vector<int>(maximum_allowed_board_size, sentinel),  // 6
        std::vector<int>(maximum_allowed_board_size, sentinel),  // 7
    };


    inline bool is_totally_under_threat(map_t map, int current_column)
    {
        const auto column_mask = column_masks[current_column];
#ifdef _DEBUG
        const bool ret = ((map & column_mask) == column_mask);
        if (verbose && ret)
        {
            std::cout << "Column " << current_column << " is totally under threat, "
                << std::hex
                << map << " includes " << column_mask
                << std::endl;
        }
        return ret;
#else
        return ((map & column_mask) == column_mask);
#endif // def _DEBUG

    }


    const std::vector<int>& not_threatened_rows(const map_t map, int current_column)
    {
        std::vector<int>& result = safe_indices[current_column];
        const map_t mask = (map & column_masks[current_column]);
        // One 64-bit integer == 8 chars.
        const char* pRow = reinterpret_cast<const char*>(&mask);
        int j = 0;
        // Manual loop unrolling, ugly as it looks, but fast like hell.
        // To allow several board sizes (4-8), we'll use something inspired in the Duff Device.
        int_fast8_t i = -1;
        switch (board_size)
        {
        case 8:
        default:
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
#ifdef _DEBUG
        if (verbose)
        {
            std::cout << "Found " << j << " non-threatened rows at column "
                << current_column
                << " and they are ";
            for (auto val : result)
            {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
        }
#endif // def _DEBUG
        if (j < 8)
        {
            result[j] = sentinel;
        }
        return result;
    }

    void show_map(map_t map)
    {
        static_assert(sizeof(unsigned char) * 8 == sizeof(map_t));

        std::cout << std::hex << (unsigned long long) map << std::endl;

        unsigned char* bytes = (unsigned char*)&map;
        for (int i = 0; i < 8; ++i)
        {
            const unsigned char byte = bytes[i];
            std::string out(8ULL, '.');
            unsigned char flag = 0x80;
            for (int col = 0; flag > 0 && col < 8; ++col, flag >>= 1)
            {
                if (flag & byte)
                    out[col] = '*';
            }
            std::bitset<8> byte_too(byte);
            std::cout << out << " " << byte_too << " " << std::hex << (int)byte << std::endl;
        }
        std::cout << std::endl;
    }


    // Some good old-fashioned template metaprogramming to OR masks at compile-time.
    template<int row, int column>
    struct threat_mask {
        enum struct _b : map_t {
            value = (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column])
        };
    };

    static const std::vector<map_t> threats{
        (map_t)threat_mask<0, 0>::_b::value, (map_t)threat_mask<0, 1>::_b::value, (map_t)threat_mask<0, 2>::_b::value, (map_t)threat_mask<0, 3>::_b::value,
        (map_t)threat_mask<0, 4>::_b::value, (map_t)threat_mask<0, 5>::_b::value, (map_t)threat_mask<0, 6>::_b::value, (map_t)threat_mask<0, 7>::_b::value,

        (map_t)threat_mask<1, 0>::_b::value, (map_t)threat_mask<1, 1>::_b::value, (map_t)threat_mask<1, 2>::_b::value, (map_t)threat_mask<1, 3>::_b::value,
        (map_t)threat_mask<1, 4>::_b::value, (map_t)threat_mask<1, 5>::_b::value, (map_t)threat_mask<1, 6>::_b::value, (map_t)threat_mask<1, 7>::_b::value,

        (map_t)threat_mask<2, 0>::_b::value, (map_t)threat_mask<2, 1>::_b::value, (map_t)threat_mask<2, 2>::_b::value, (map_t)threat_mask<2, 3>::_b::value,
        (map_t)threat_mask<2, 4>::_b::value, (map_t)threat_mask<2, 5>::_b::value, (map_t)threat_mask<2, 6>::_b::value, (map_t)threat_mask<2, 7>::_b::value,

        (map_t)threat_mask<3, 0>::_b::value, (map_t)threat_mask<3, 1>::_b::value, (map_t)threat_mask<3, 2>::_b::value, (map_t)threat_mask<3, 3>::_b::value,
        (map_t)threat_mask<3, 4>::_b::value, (map_t)threat_mask<3, 5>::_b::value, (map_t)threat_mask<3, 6>::_b::value, (map_t)threat_mask<3, 7>::_b::value,

        (map_t)threat_mask<4, 0>::_b::value, (map_t)threat_mask<4, 1>::_b::value, (map_t)threat_mask<4, 2>::_b::value, (map_t)threat_mask<4, 3>::_b::value,
        (map_t)threat_mask<4, 4>::_b::value, (map_t)threat_mask<4, 5>::_b::value, (map_t)threat_mask<4, 6>::_b::value, (map_t)threat_mask<4, 7>::_b::value,

        (map_t)threat_mask<5, 0>::_b::value, (map_t)threat_mask<5, 1>::_b::value, (map_t)threat_mask<5, 2>::_b::value, (map_t)threat_mask<5, 3>::_b::value,
        (map_t)threat_mask<5, 4>::_b::value, (map_t)threat_mask<5, 5>::_b::value, (map_t)threat_mask<5, 6>::_b::value, (map_t)threat_mask<5, 7>::_b::value,

        (map_t)threat_mask<6, 0>::_b::value, (map_t)threat_mask<6, 1>::_b::value, (map_t)threat_mask<6, 2>::_b::value, (map_t)threat_mask<6, 3>::_b::value,
        (map_t)threat_mask<6, 4>::_b::value, (map_t)threat_mask<6, 5>::_b::value, (map_t)threat_mask<6, 6>::_b::value, (map_t)threat_mask<6, 7>::_b::value,

        (map_t)threat_mask<7, 0>::_b::value, (map_t)threat_mask<7, 1>::_b::value, (map_t)threat_mask<7, 2>::_b::value, (map_t)threat_mask<7, 3>::_b::value,
        (map_t)threat_mask<7, 4>::_b::value, (map_t)threat_mask<7, 5>::_b::value, (map_t)threat_mask<7, 6>::_b::value, (map_t)threat_mask<7, 7>::_b::value
    };


    // Everything by value, on purpose.
    inline map_t threaten(map_t map, int row, int column)
    {
        // Before template metaprogramming we did this:
        //  map |= (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column]);
#ifdef _DEBUG
        map |= threats[row * 8 + column];
        if (verbose)
        {
            std::cout << "Just threatened cell [" << row << ", " << column << "]." << std::endl;
            show_map(map);
        }
        return map;
#endif // def _DEBUG
        return map | threats[row << 3 | column];
    }

#ifdef _DEBUG
    void do_show_failure(const map_t map, const std::vector<int>& result)
    {
        using std::cout;
        using std::endl;


        std::vector<std::string> display = { "--------", "--------", "--------", "--------", "--------", "--------", "--------", "--------" };
        for (int row = 0; row < board_size; ++row)
        {
            for (int col = 0; col < board_size; ++col)
            {
                if (map & row_masks[row] & column_masks[col])
                {
                    display[row][col] = 'x';
                }
            }
        }
        for (int col = 0; col < result.size(); ++col)
        {
            int row = result[col];
            if (row < 0)
            {
                cout << "--- Failure at column " << col << " ---" << endl;
                break; // for
            }
            display[row][col] = 'Q';
        }
        for (const auto& line : display)
            cout << line.substr(0, board_size) << endl;
        cout << endl;
    }
#endif // def _DEBUG

    void do_solve(map_t map, std::vector<int>& solution, int current_column)
    {
        if (current_column == (board_size - 1))
        {
            // Success! Copy the solution. Don't move, we still need the buffer.
            std::copy(solution.cbegin(), solution.cend(), solutions[success_count++].begin());
            return;
        }
#ifdef _DEBUG
        if (trials > 0 && failures_count >= static_cast<decltype(failures_count)>(trials))
        {
            return;
        }
#endif // def _DEBUG
        const map_t new_map = threaten(map, solution[current_column], current_column);
        const int next_column = 1 + current_column;

        if (is_totally_under_threat(new_map, next_column))
        {
            ++failures_count;
#ifdef _DEBUG
            if (trials > 0)
            {
                do_show_failure(new_map, solution);
            }
#endif // def _DEBUG
            return;
        }
        for (auto current_row : not_threatened_rows(new_map, next_column))
        {
            if (sentinel == current_row)
            {
                break;
            }
            solution[next_column] = current_row;
#ifdef _DEBUG
            _ASSERT_EXPR(current_row < board_size, "Bug found: row outside the board.");
            if (verbose)
            {
                std::cout << "Testing [" << std::dec << current_row << ", " << next_column << "]." << std::endl;
            }
#endif // def _DEBUG
            // Call recursively
            do_solve(new_map, solution, next_column);
        }
        // Leave things as they were.
        solution[next_column] = -1;
    }
} // namespace qns 

#define FOR_PROFILING
#ifdef FOR_PROFILING
double qns::solve()
{
    // Solution that works for an 8x8 chess board only (not generalized to n by n).
    // On the other hand, chess boards have 64 squares.
    // solutions.reserve(46); // Cheating? Nope, just using prior knowledge.
    std::vector<int> solution(maximum_allowed_board_size, -1);
#ifdef DEBUG
    const int loops = 1;
#else
    const int loops = 1000;
#endif // DEBUG

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
            starting_map |= row_masks[i]; // Threaten all rows outside the board.
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
        << ", and an average of " << loops << " runs was " << double(tot_time) / loops << std::endl;
    do_show_results(failures_count, success_count, solutions, board_size);
    if (success_count < solutions.size())
    {
        solutions[success_count][0] = sentinel;
    }
    std::cout.flush();
    return double(tot_time) / loops;
}
#else
double qns::solve()
{
    // Solution that works for an 8x8 chess board only (not generalized to n by n).
    // On the other hand, chess boards have 64 squares.
    // solutions.reserve(46); // Cheating? Nope, just using prior knowledge.
    std::vector<int> solution(8ULL, -1);
    hi_res_timer timer;
    for (int_fast8_t current_row = 0; current_row < 4; ++current_row)
    {
        solution[0] = current_row;
        do_solve(map_t{ 0ULL }, solution, 0);
    }
    timer.Stop();
    std::cout << "Resolving took " << timer.GetElapsedMicroseconds() << " microseconds." << std::endl;
    do_show_results(failures_count, success_count, solutions, board_size);
    return double(timer.GetElapsedMicroseconds());
}
#endif // FOR_PROFILING

void qns::set_verbose(bool new_val)
{
    std::cout << "Setting verbose to " << new_val << std::endl;
    verbose = new_val;
}

void qns::set_short(int trials$)
{
    std::cout << "Setting trials to " << trials$ << std::endl;
    trials = trials$;
}

void qns::test()
{
    using std::cout;
    using std::endl;

    cout << "=== Row masks ===" << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << "=== " << i << " ===" << endl;
        show_map((map_t)row_masks[i]);
    }

    cout << "=== Column masks ===" << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << "=== " << i << " ===" << endl;
        show_map((map_t)column_masks[i]);
    }

    cout << "=== Main diagonal ===" << endl;
    //show_map((map_t)main_diagonal_mask);

    for (int i = 0; i < sizeof(main_diagonal_parallels) / sizeof(main_diagonal_parallels[0]); ++i)
    {
        cout << "=== " << std::dec << i << " parallel ===" << endl;
        show_map((map_t)(main_diagonal_parallels[i]));
    }

    cout << "=== Second diagonal ===" << endl;
    //show_map((map_t)secondary_diagonal_mask);

    for (int i = 0; i < sizeof(second_diagonal_parallels) / sizeof(second_diagonal_parallels[0]); ++i)
    {
        cout << "=== " << std::dec << i << " second parallel ===" << endl;
        show_map((map_t)(second_diagonal_parallels[i]));
    }

    cout << "========" << endl;
    map_t map = 0ULL;
    map = threaten(map, 1, 0);
    show_map(map);

    cout << "========" << endl;
    map = threaten(map, 4, 1);
    show_map(map);

    cout << "========" << endl;
    map = threaten(map, 7, 2);
    show_map(map);

    /*map_t map = 0ULL;
    map = threaten(map, 4, 2);
    show_map(map);*/
}

void qns::set_board_size(int size)
{
    if (size < 4)
    {
        std::cout << "Size must be at least 4, it is " << size << ". Doing nothing.";
        return;
    }
    if (size > 8)
    {
        std::cout << "Size must be at most 8, it is " << size << ". Doing nothing.";
        return;
    }
    board_size = size;

}