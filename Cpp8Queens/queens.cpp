#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>

#include "queens.h"
#include <bitset>

using flags_t = uint_fast64_t;
using map_t = flags_t;

static std::vector<std::vector<int>> solutions;
static uint_fast64_t failures_count = 0;
static bool verbose = false;

// HEY, WE'RE LITTLE ENDIAN HERE!
const flags_t row_masks[] = {
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
const flags_t column_masks[] = {
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
static const flags_t main_diagonal_mask = 0x0102040810204080;

const flags_t main_diagonal_parallels[] = {
    0x0000'0000'0000'0001,
    0x0000'0000'0000'0102,
    0x0000'0000'0001'0204,
    0x0000'0000'0102'0408,
    0x0000'0001'0204'0810,
    0x0000'0102'0408'1020,
    0x0001'0204'0810'2040,
    0x0102'0408'1020'4080,
    0x0204'0810'2040'8000,
    0x0408'1020'4080'0000,
    0x0810'2040'8000'0000,
    0x1020'4080'0000'0000,
    0x2040'8000'0000'0000,
    0x4080'0000'0000'0000,
    0x8000'0000'0000'0000,
};

static const flags_t secondary_diagonal_mask = 0x8040201008040201;
const flags_t second_diagonal_parallels[] = {
    0x0000'0000'0000'0080,
    0x0000'0000'0000'8040,
    0x0000'0000'0080'4020,
    0x0000'0000'8040'2010,
    0x0000'0080'4020'1008,
    0x0000'8040'2010'0804,
    0x0080'4020'1008'0402,
    0x4020'1008'0402'0100,
    0x2010'0804'0201'0000,
    0x1008'0402'0100'0000,
    0x0804'0201'0000'0000,
    0x0402'0100'0000'0000,
    0x0201'0000'0000'0000,
    0x0100'0000'0000'0000,
};

inline bool is_totally_under_threat(map_t map, int current_column)
{
    const auto column_mask = column_masks[current_column]; 
    const bool ret = (map & column_mask) == column_mask;
    if (verbose && ret)
    {
        std::cout << "Column " << current_column << " is totally under threat, " 
            << std::hex 
            << map << " includes " << column_mask
            << std::endl;
    }
    return ret;
}

std::vector<int> not_threatened_rows(const map_t map, int current_column)
{
    std::vector<int> result;
    result.reserve(8 - current_column); // This is as should be, believe me. 
    for (int i = 0; i < 8; ++i)
    {
        if ( (map & row_masks[i] & column_masks[7 - current_column]) == 0ULL)
        {
            result.push_back(i);
        }
    }
    if (verbose)
    {
        std::cout << "Found " << result.size() << " non-threatened rows at column " 
            << current_column 
            << " and they are ";
            for (auto val : result)
            {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
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
map_t threaten(map_t map, int row, int column)
{
    map_t shifted_diagonal = main_diagonal_mask << (row * 8 + column);
    map_t shifted_second_diagonal = secondary_diagonal_mask >> ((8 - row) * 8 - column);

    // Threaten current row
    // Threaten main diagonal, starting at current row and current column, to the right only (other columns already populated)
    // Threaten second diagonal, starting at current row and current column, to the right only (other columns already populated)
    map |= (row_masks[row] | shifted_diagonal | shifted_second_diagonal);

    if (verbose)
    {
        std::cout << "Just threatened cell [" << row << ", " << column << "]." << std::endl;
        show_map(map);
    }
    return map;
}

void do_solve(map_t map, std::vector<int> &solution, int current_column)
{
    if (current_column == solution.size())
    {
        // Success!
        solutions.push_back(solution);
        return;
    }
    map_t new_map = threaten(map, solution[current_column], current_column);
    int next_column = 1 + current_column;

    if (is_totally_under_threat(new_map, next_column))
    {
        ++failures_count;
        return;    
    }
    for (int current_row: not_threatened_rows(new_map, next_column) )
    {
        solution[next_column] = current_row;
        // Call recursively
        if (verbose)
        {
            std::cout << "Testing [" << current_row << ", " << next_column << "]." << std::endl;
        }
        do_solve(new_map, solution, next_column);
    }
    // Leave things as they were.
    solution[next_column] = -1;

}

void do_show_results()
{
    using std::cout;
    using std::endl;
    
    cout << "We had " << failures_count << " failures, and " << solutions.size() << " solutions." << endl;

    if (!verbose)
        return;
        
    cout << "The solutions are: " << endl << endl;
    for (const auto &result: solutions)
    {
        std::vector<std::string> display = { "--------", "--------", "--------", "--------", "--------", "--------", "--------", "--------" };
        for (int row = 0; row < result.size(); ++row)
        {
            display[row][result[row]]='Q';
        }
        for (const auto& line: display)
            cout << line << endl;
        cout << endl;
    }
}

void qns::solve()
{
    // Solution that works for an 8x8 chess board only (not generalized to n by n).
    // On the other hand, chess boards have 64 squares.
    std::vector<int> solution(8ULL, -1);
    for (int current_row : {0, 1, 2, 3})
    {
        map_t map = 0ULL;
        solution[0] = current_row;
        do_solve(map, solution, 0);
    }
    do_show_results();
}

void qns::set_verbose(bool new_val)
{
    std::cout << "Setting verbose to " << new_val << std::endl;
    verbose = new_val;
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
    show_map((map_t)main_diagonal_mask);

    for (int i = 0; i < sizeof(main_diagonal_parallels) / sizeof(main_diagonal_parallels[0]); ++i)
    {
        cout << "=== " << std::dec << i << " parallel ===" << endl;
        show_map((map_t)(main_diagonal_parallels[i]));
    }

    cout << "=== Second diagonal ===" << endl;
    show_map((map_t)secondary_diagonal_mask);

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

}