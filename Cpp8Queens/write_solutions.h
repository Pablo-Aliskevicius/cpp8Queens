#pragma once

union __m256i; // forward declaration.

void do_show_results(unsigned long long failures_count, unsigned long long success_count, const std::vector<std::vector<int>>& solutions, int board_size);
void do_show_map(__m256i* map, int board_size);