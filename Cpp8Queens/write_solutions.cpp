#include <iostream>
#include <numeric>  // for std::iota
#include <vector>
#include <string>


#include "write_solutions.h"

using board_t = std::vector<std::string>;


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
        cout << "<tr><td class='cell' /></tr>" << endl; // Separation between rows of boards.
    }
    cout << endl << "</tbody></table>" << endl;
}


void do_show_results(int failures_count, int success_count, const std::vector<std::vector<int>>& solutions, int board_size)
{
    using std::cout;
    using std::endl;

    cout << "We had " << failures_count << " failures, and " << success_count 
        << " solutions in a board of size " << board_size << " by " << board_size << "."
        << endl;

    //if (!verbose)
    //    return;

    cout << "The solutions are: " << endl << endl;
    // Buld the displays
    std::vector<board_t> boards;
    boards.reserve(success_count);
    for (const auto& result : solutions)
    {
        if (result[0] == -1)
        {
            break;
        }

        board_t display = { "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-", "-+-+-+-+", "+-+-+-+-" };
        display.resize(board_size); // Chop off the needless ones.
        for (int row = 0; row < board_size; ++row)
        {
            display[row].resize(board_size); // Cut redundant part. 
            display[row][result[row]] = 'Q';
        }
        boards.push_back(display);
        if (boards.size() == success_count)
        {
            break;
        }
    }

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
                cout << (boards[i_board][row]) << separator;
            }
            cout << endl;
        }
        cout << endl;
    }
// #define SHOW_CHESS_HTML_BOARDS
#ifdef SHOW_CHESS_HTML_BOARDS
    write_solutions_to_html_table(boards, board_size);
#endif
}

