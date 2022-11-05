#include <iostream>
#include <numeric>  // for std::iota
#include <vector>
#include <set>
#include <string>

#include <immintrin.h>  // Using intel intrinsics to learn about it. Precondition: you need AVX2 at least (which you probably have).

#include "write_solutions.h"

using board_t = std::vector<std::string>;
using solutions_t = std::vector<std::vector<int>>;

void write_solutions_to_html_table(const std::vector<board_t>& boards, int board_size)
{
    using std::vector;
    using std::string;
    using std::cout;
    using std::endl;

    const string unicode_queen{ "&#9819;" };
    const vector<string> td_openers{ {"<td class='yellow cell'>"}, {"<td class='black cell'>" } };
    const string inner_separator{ "<td class='wide cell' />" };
    const string str_empty{ "" };

    const int htm_width = 5;
    ldiv_t groups = ldiv(static_cast<long>(boards.size()), htm_width);
    cout << endl << "<table class='vertical.center'><tbody>" << endl;

    for (int base = 0; base < boards.size(); base += htm_width)
    {
        // Display groups of line_width
        std::vector<int> indices(htm_width);
        std::iota(indices.begin(), indices.end(), base);
        while (indices.back() >= boards.size())
        {
            indices.pop_back();
        }
        for (int row = 0; row < board_size; ++row)
        {
            cout << "<tr>" << endl;
            for (int i_board : indices)
            {
                for (int col = 0; col < board_size; ++col)
                {
                    char c = boards[i_board][row][col];
                    int i_cell = (col + row) % 2;
                    cout << td_openers[i_cell] << ((c == 'Q') ? unicode_queen : str_empty) << "</td>";
                }
                cout << inner_separator;
            }
            cout << "</tr>" << endl;
        }
        cout << "<tr><td class='cell' /></tr>" << endl; // Separation between rows of boards: height of cell.
    }
    cout << "</tbody></table>" << endl << endl;
}

solutions_t fill_solutions(const std::vector<std::vector<int>>& solutions, size_t success_count, int board_size)
{
    solutions_t full_solutions;
    size_t available_solutions = std::min(solutions.size(), success_count);
    full_solutions.reserve(available_solutions * 2);
    std::copy(solutions.cbegin(),
        solutions.cbegin() + available_solutions,
        std::back_inserter(full_solutions));
    solutions_t::value_type subtract_parent(board_size, board_size - 1); // e.g., 8 sevens
    const int middle_row = (board_size / 2);

    auto sub_256 = [&](int start_offset, int* solution_data, int* symmetric_solution_data) {
        // Subtract 8 32-bit integers in 1 fell swoop, using the AVX2 instruction set. In every 
        // Intel machine for a lot of years now  (since the Haswell processor, back in 2013). 
        // __m256i _mm256_sub_epi64(__m256i a, __m256i b)
        __m256i* pA = reinterpret_cast<__m256i*>(subtract_parent.data() + start_offset);
        __m256i* pB = reinterpret_cast<__m256i*>(solution_data);
        __m256i* pC = reinterpret_cast<__m256i*>(symmetric_solution_data);
        *pC = _mm256_sub_epi64(*pA, *pB);
    };
    auto sub_128 = [&](int start_offset, int* solution_data, int* symmetric_solution_data) {
        // Subtract 4 integers in 1 fell swoop, using the good old SSE2 instruction set.
        // __m128i _mm_sub_epi32(__m128i a, __m128i b)
        __m128i* pA = reinterpret_cast<__m128i*>(subtract_parent.data());
        __m128i* pB = reinterpret_cast<__m128i*>(solution_data);
        __m128i* pC = reinterpret_cast<__m128i*>(symmetric_solution_data);
        *pC = _mm_sub_epi32(*pA, *pB);
    };

    for (auto it = solutions.crend() - available_solutions; it != solutions.crend(); ++it)
    {
        const auto& solution = *it;
        if (solution[0] == middle_row)
        {
            // The symmetric solution for the middle row in boards with an odd number of rows is already there.
            continue; // for
        }
        solutions_t::value_type symmetric_solution(board_size, -1);

        switch (board_size)
        {
        case 16:
        case 15:
        case 14:
        case 13:
            sub_256(0, const_cast<int*>(solution.data()), symmetric_solution.data());
            sub_256(board_size - 8, const_cast<int*>(solution.data()), symmetric_solution.data());
            break;

        case 12:
        case 11:
        case 10:
        case 9:
            sub_256(0, const_cast<int*>(solution.data()), symmetric_solution.data());
            sub_128(board_size - 4, const_cast<int*>(solution.data()), symmetric_solution.data());
            break;

        case 8:
            sub_256(0, const_cast<int*>(solution.data()), symmetric_solution.data());
            break;

        case 7:
            symmetric_solution[6] = subtract_parent[6] - solution[6];
            // fall through on purpose
        case 6:
            symmetric_solution[5] = subtract_parent[5] - solution[5];
        case 5:
            symmetric_solution[4] = subtract_parent[4] - solution[4];
        case 4: 
            sub_128(0, const_cast<int*>(solution.data()), symmetric_solution.data());
            break;

        default:
            std::cerr << "Invalid board size: should be in the range [4, 8] but it is " << board_size << std::endl;
            throw std::invalid_argument("At the moment, the board size should be between 4 and 8.");
        } // switch

        full_solutions.push_back(symmetric_solution);
    }

    return full_solutions;
}

std::vector<board_t> fill_boards_8(const solutions_t& full_solutions, int board_size)
{
    // Buld the displays
    std::vector<board_t> boards;
    boards.reserve(full_solutions.size());
    for (const auto& result : full_solutions)
    {
        if (result[0] == -1)
        {
            return boards;
        }
        // Board size must be 8 at most. 
        board_t display = { "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-" };
        display.resize(board_size); // Chop off the needless ones.
        for (int row = 0; row < board_size; ++row)
        {
            display[row].resize(board_size); // Cut redundant part. 
            if (result[row] == -1)
            {
                return boards;
            }
            display[row][result[row]] = 'Q';
        }
        boards.push_back(display);
    }
    return boards;
}


std::vector<board_t> fill_boards_16(const solutions_t& full_solutions, int board_size)
{
    // Buld the displays
    std::vector<board_t> boards;
    boards.reserve(full_solutions.size());
    for (const auto& result : full_solutions)
    {
        if (result[0] == -1)
        {
            return boards;
        }
        // Board size must be 16 at most. 
        board_t display = { 
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
            "-+-+-+-+-+-+-+-+",
            "+-+-+-+-+-+-+-+-",
        };
        display.resize(board_size); // Chop off the needless ones.
        for (int row = 0; row < board_size; ++row)
        {
            display[row].resize(board_size); // Cut redundant part. 
            if (result[row] == -1)
            {
                return boards;
            }
            display[row][result[row]] = 'Q';
        }
        boards.push_back(display);
    }
    return boards;
}

void do_show_results(unsigned long long failures_count, unsigned long long success_count, const std::vector<std::vector<int>>& solutions, int board_size)
{
    using std::cout;
    using std::endl;

    solutions_t full_solutions = fill_solutions(solutions, success_count, board_size);

    cout << "We had " << failures_count << " failures, and " << success_count
        << " solutions (" 
        << ((full_solutions.size() > success_count) ? "filled by symmetry to ": "showing only ")
        << full_solutions.size()
        << ") in half a board of size " << board_size << " by " << board_size << "."
        << endl;

    cout << "The ";
    if (success_count > solutions.size())
    {
        cout << "chosen " << full_solutions.size(); // For 15x15, there are millions; showing a tiny fraction.
    }
    cout << " solutions are: " << endl << endl;

#ifdef _DEBUG
    for (const auto& solution : solutions)
    {
        std::set<int> uniques;
        for (auto row : solution)
        {
            if (row == -1)
            {
                break;
            }
            cout << std::hex << row << ",";
            uniques.insert(row);
        }
        if (uniques.size() < board_size && !uniques.empty())
        {
            cout << " ******************";
        }
        cout << std::dec << endl << endl;
    }

#endif // def _DEBUG

    // Buld the displays
    std::vector<board_t> boards =
        board_size < 9 ?
        fill_boards_8(full_solutions, board_size) :
        fill_boards_16(full_solutions, board_size);

    const int line_width = 9;
    const std::string separator{ "    " };
    for (int base = 0; base < boards.size(); base += line_width)
    {
        // Display groups of line_width
        std::vector<int> indices(line_width);
        std::iota(indices.begin(), indices.end(), base);
        while (indices.back() >= boards.size())
        {
            indices.pop_back();
        }
        for (int row = 0; row < board_size; ++row)
        {
            for (int i_board : indices)
            {
                cout << boards[i_board][row] << separator;
            }
            cout << endl;
        }
        cout << endl;
    }
// #define SHOW_CHESS_HTML_BOARDS
#ifdef SHOW_CHESS_HTML_BOARDS
    // Tested only for 8x8
    if (board_size == 8)
    {
        write_solutions_to_html_table(boards, board_size);
    }
#endif
}

void do_show_map(__m256i* pMap, int board_size)
{
    std::cout << __FUNCTION__ << std::endl;
    for (int row = 0; row < board_size; ++row)
    {
        auto row_flags = pMap->m256i_u16[row];
        //                  01234567890123456
        // Yes, initializing again and again inside the loop.
        char human_row[] = "+-+-+-+-+-+-+-+-+";
        char* pOutput = human_row + row % 2;
        for (int col = 0; col < board_size; ++col)
        {
            unsigned short bit = 1 << (15 - col);
            if (row_flags & bit)
            {
                pOutput[col] = '*';
            }
        }
        // It will look rugged at the right side. I don't care.
        std::cout << pOutput << " 0x" << std::hex << row_flags << std::endl;
    }
    std::cout << std::endl;
}