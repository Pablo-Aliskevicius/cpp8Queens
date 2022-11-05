#pragma once
// sixteen_queens_common.h
// Common stuff for both solutions for 16x16 (standard and AVX2)
#include <vector>

union __m256i; // forward declaration.

namespace qns16cmn
{
    using flags_t = ::__m256i;
    using map_t = ::__m256i;
    alignas(64) flags_t row_masks[];
    alignas(64) flags_t column_masks[];
    alignas(64) flags_t main_diagonal_parallels[];
    alignas(64) flags_t second_diagonal_parallels[];

    extern std::vector<std::vector<int>> safe_indices;
    extern const int sentinel;
    // We shall only show the first 50, duplicated by symmetry.
    extern const int max_solutions_to_show;
    extern std::vector<std::vector<int>> solutions;

    const std::vector<int>& not_threatened_rows(const map_t& map, int board_size, int current_column);
    // Multi threading support: no globals.
    const std::vector<int>& not_threatened_rows_mt(const map_t& map, int board_size, int current_column, std::vector<int>& result);
#ifdef _DEBUG
    namespace dbg
    {
        void show_map(const map_t& map, const std::vector<int>& solution, int board_size);
    }
#endif // _DEBUG
    static constexpr size_t maximum_allowed_board_size = 16;

    void test();
}