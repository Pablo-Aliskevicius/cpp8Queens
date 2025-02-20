// #include <iostream>
// include <vector>
// include <chrono>

import <iostream>;
import <vector>;
import <chrono>;

constexpr int N = 8;
using Board = uint64_t;

constexpr Board col_mask = (1ULL << N) - 1;
constexpr Board diag1_mask = (1ULL << (2 * N - 1)) - 1;
constexpr Board diag2_mask = (1ULL << (2 * N - 1)) - 1;

/*
constexpr Board col_threats[N] = {
    0x0101010101010101ULL,
    0x0202020202020202ULL,
    0x0404040404040404ULL,
    0x0808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL
};

constexpr Board diag1_threats[2 * N - 1] = {
    0x0000000000000080ULL,
    0x0000000000008040ULL,
    0x0000000000804020ULL,
    0x0000000080402010ULL,
    0x0000008040201008ULL,
    0x0000804020100804ULL,
    0x0080402010080402ULL,
    0x8040201008040201ULL,
    0x4020100804020100ULL,
    0x2010080402010000ULL,
    0x1008040201000000ULL,
    0x0804020100000000ULL,
    0x0402010000000000ULL,
    0x0201000000000000ULL,
    0x0100000000000000ULL
};

constexpr Board diag2_threats[2 * N - 1] = {
    0x0000000000000001ULL,
    0x0000000000000102ULL,
    0x0000000000010204ULL,
    0x0000000001020408ULL,
    0x0000000102040810ULL,
    0x0000010204081020ULL,
    0x0001020408102040ULL,
    0x0102040810204080ULL,
    0x0204081020408000ULL,
    0x0408102040800000ULL,
    0x0810204080000000ULL,
    0x1020408000000000ULL,
    0x2040800000000000ULL,
    0x4080000000000000ULL,
    0x8000000000000000ULL
};
//*/

void solve(Board col, Board diag1, Board diag2, int row, std::vector<Board>& solutions, Board board) {
    if (row == N) [[unlikely]] {
        solutions.push_back(board);
        return;
    }

    Board safe = ~(col | diag1 | diag2) & col_mask;

    while (safe) {
        Board p = safe & (-safe);
        safe -= p;
        solve(col | p, (diag1 | p) << 1, (diag2 | p) >> 1, row + 1, solutions, board | (p << (row * N)));
    }
}

void print_solution(Board board) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << ((board & (1ULL << (i * N + j))) ? "1 " : "0 ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::vector<Board> solutions;
    auto start = std::chrono::high_resolution_clock::now();
    solve(0, 0, 0, 0, solutions, 0);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken: " << elapsed.count() << " seconds\n";
    std::cout << "Number of solutions found: " << solutions.size() << "\n";

    for (int i = 0; i < std::min(3, static_cast<int>(solutions.size())); ++i) {
        std::cout << "Solution " << i + 1 << ":\n";
        print_solution(solutions[i]);
    }

    return 0;
}

/*
Time taken: 2.68e-05 seconds
Number of solutions found: 92
Solution 1:
1 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 1
0 0 0 0 0 1 0 0
0 0 1 0 0 0 0 0
0 0 0 0 0 0 1 0
0 1 0 0 0 0 0 0
0 0 0 1 0 0 0 0

Solution 2:
1 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 1
0 0 1 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 1 0 0 0 0
0 1 0 0 0 0 0 0
0 0 0 0 1 0 0 0

Solution 3:
1 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 1 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 1
0 1 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 1 0 0 0 0 0


E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\x64\Release\CopilotCpp8Qeens.exe (process 10280) exited with code 0.
Press any key to close this window . . .
*/