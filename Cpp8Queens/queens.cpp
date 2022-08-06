#include <bitset>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>

#include "queens.h"
#include "high_res_clock.h"
#include "write_solutions.h"

using flags_t = uint_fast64_t;
using map_t = flags_t;

static std::vector<std::vector<int>> solutions(46, std::vector<int>(8, -1));
static uint_fast16_t failures_count = 0; // up to 20'160 = 4 * 7!
static uint_fast16_t success_count = 0; // up to 20'160 = 4 * 7!
static bool verbose = false;
static int_fast16_t trials = -1;


// If you consider this is cheating, use template metaprogramming.
static const flags_t row_masks[] = {
    0xff,
    0xff00, 
    0xff0000, 
    0xff000000, 
    0xff00000000, 
    0xff0000000000, 
    0xff000000000000, 
    0xff00000000000000, 
}; 


// HEY, WE'RE LITTLE ENDIAN HERE!
static const flags_t column_masks[] = {
    0x8080808080808080,
    0x4040404040404040,
    0x2020202020202020,
    0x1010101010101010,
    0x0808080808080808,
    0x0404040404040404,
    0x0202020202020202,
    0x0101010101010101,
};


// Visualize:
//   b"1000 0000" // == 0x80 << 7
//   b"0100 0000" // == 0x40 << 6
//   b"0010 0000" // == 0x20 << 5
// ...
// We may have an 'endian' issue. 
// static const flags_t main_diagonal_mask = 0x0102040810204080;

static const flags_t main_diagonal_parallels[] = {
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

// static const flags_t secondary_diagonal_mask = 0x8040201008040201;
static const flags_t second_diagonal_parallels[] = {
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

const int sentinel = -1;
// Save some time in allocations at run time. 
std::vector<std::vector<int>> safe_indices{
    std::vector<int>(8, sentinel),  // 0
    std::vector<int>(8, sentinel),
    std::vector<int>(8, sentinel), 
    std::vector<int>(8, sentinel),  // 3
    std::vector<int>(8, sentinel),
    std::vector<int>(8, sentinel),
    std::vector<int>(8, sentinel),  // 6
    std::vector<int>(8, sentinel),  // 7
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
    const auto mask = (map & column_masks[current_column]);
    // One 64-bit integer == 8 chars.
    const char* pi = reinterpret_cast<const char*>(&mask);
    int j = 0;
    for (int_fast8_t i = 0; i < 8; ++i)
    {
        if (!pi[i])
        {
            result[j++] = i;
        }
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

    unsigned char *bytes = (unsigned char *) &map;
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
        std::cout << out << " " << byte_too << " " << std::hex << (int) byte << std::endl;
    }
    std::cout << std::endl;
}

// Everything by value, on purpose.
inline map_t threaten(map_t map, int row, int column)
{
    map |= (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column]);
#ifdef _DEBUG
    if (verbose)
    {
        std::cout << "Just threatened cell [" << row << ", " << column << "]." << std::endl;
        show_map(map);
    }
#endif // def _DEBUG
    return map;
}

#ifdef _DEBUG
void do_show_failure(const map_t map, const std::vector<int>& result)
{
    using std::cout;
    using std::endl;


    std::vector<std::string> display = { "--------", "--------", "--------", "--------", "--------", "--------", "--------", "--------" };
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
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
        cout << line << endl;
    cout << endl;
}
#endif // def _DEBUG

void do_solve(map_t map, std::vector<int> &solution, int current_column)
{
    if (current_column == (solution.size() - 1))
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
    for (auto current_row: not_threatened_rows(new_map, next_column))
    {
        if (sentinel == current_row)
        {
            break;
        }
        solution[next_column] = current_row;
#ifdef _DEBUG
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

#define FOR_PROFILING
#ifdef FOR_PROFILING
void qns::solve()
{
    // Solution that works for an 8x8 chess board only (not generalized to n by n).
    // On the other hand, chess boards have 64 squares.
    // solutions.reserve(46); // Cheating? Nope, just using prior knowledge.
    std::vector<int> solution(8ULL, -1);
    const int loops = 1000;
    unsigned long long max_time = 0ULL;
    unsigned long long min_time = ~0ULL;
    unsigned long long tot_time = 0ULL;

    for (int loop = 0; loop < loops; ++loop)
    {
        failures_count = 0;
        success_count = 0;
        hi_res_timer timer;
        for (int_fast8_t current_row = 0; current_row < 4; ++current_row)
        {
            solution[0] = current_row;
            do_solve(map_t{ 0ULL }, solution, 0);
        }
        timer.Stop();
        auto microseconds = timer.GetElapsedMicroseconds();
        std::cout << "Resolving took " << microseconds << " microseconds." << std::endl;
        if (microseconds < min_time) min_time = microseconds;
        if (microseconds > max_time) max_time = microseconds;
        tot_time += microseconds;
    }
    std::cout << "The fastest run took " << min_time << " microseconds, the slowest took " << max_time
        << ", and an average of " << loops << " runs was " << double(tot_time) / loops << std::endl;
    do_show_results(failures_count, success_count, solutions);
}
#else
void qns::solve()
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
    do_show_results(failures_count, success_count, solutions);
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