#include <iostream>
#include <numeric>  // for std::iota
#include <vector>
#include <string>

#include <immintrin.h>  // Using intel intrinsics to learn about it. Precondition: you need AVX2 at least (which you probably have).

#include "write_solutions.h"
#include <algorithm>

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

solutions_t fill_solutions(const std::vector<std::vector<int>>& solutions, int success_count, int board_size)
{
    solutions_t full_solutions;
    full_solutions.reserve(success_count * 2);
    std::copy(solutions.cbegin(),
        solutions.cbegin() + success_count,
        std::back_inserter(full_solutions));
    solutions_t::value_type subtract_parent(board_size, board_size - 1); // e.g., 8 sevens
    const int middle_row = (board_size / 2);
    for (auto it = solutions.crend() - success_count; it != solutions.crend(); ++it)
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
        case 8:
        {
            // Subtract 16 integers in 1 fell swoop, using the AVX2 instruction set. In every Intel machine for a lot of years now 
            // (since the Haswell processor, back in 2013). 
            // __m256i _mm256_sub_epi64(__m256i a, __m256i b)
            __m256i* pA = reinterpret_cast<__m256i*>(subtract_parent.data());
            __m256i* pB = reinterpret_cast<__m256i*>(const_cast<int*>(solution.data()));
            __m256i* pC = reinterpret_cast<__m256i*>(symmetric_solution.data());
            *pC = _mm256_sub_epi64(*pA, *pB);
        }
            break;
        case 7:
            symmetric_solution[6] = subtract_parent[6] - solution[6];
            // fall through on purpose
        case 6:
            symmetric_solution[5] = subtract_parent[5] - solution[5];
        case 5:
            symmetric_solution[4] = subtract_parent[4] - solution[4];
        case 4: // Use 128-bit SSE intrinsics
        {
            // Subtract 4 integers in 1 fell swoop, using the good old SSE2 instruction set.
            // __m128i _mm_sub_epi32(__m128i a, __m128i b)
            __m128i* pA = reinterpret_cast<__m128i*>(subtract_parent.data());
            __m128i* pB = reinterpret_cast<__m128i*>(const_cast<int*>(solution.data()));
            __m128i* pC = reinterpret_cast<__m128i*>(symmetric_solution.data());
            *pC = _mm_sub_epi32(*pA, *pB);
        }
            break;
        default:
            // TODO: Throw some kind of exception.
            std::cerr << "Invalid board size: should be in the range [4, 8] but it is " << board_size << std::endl;
            throw std::invalid_argument("At the moment, the board size should be between 4 and 8.");
        }

        full_solutions.push_back(symmetric_solution);
    }

    return full_solutions;
}

std::vector<board_t> fill_boards(const solutions_t& full_solutions, int board_size)
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


void do_show_results(int failures_count, int success_count, const std::vector<std::vector<int>>& solutions, int board_size)
{
    using std::cout;
    using std::endl;

    solutions_t full_solutions = fill_solutions(solutions, success_count, board_size);

    cout << "We had " << failures_count << " failures, and " << success_count
        << " solutions (filled by symmetry to " << full_solutions.size()
        << ") in a board of size " << board_size << " by " << board_size << "."
        << endl;

    cout << "The solutions are: " << endl << endl;
    // Buld the displays
    std::vector<board_t> boards = fill_boards(full_solutions, board_size);

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
    cout << "<p>We had " << failures_count << " failures, and " << success_count
        << " solutions in a board of size " << board_size << " by " << board_size << ".</p>"
        << endl;
    write_solutions_to_html_table(boards, board_size);
#endif
}

