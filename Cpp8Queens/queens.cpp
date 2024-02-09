#if _MSVC_LANG >= 201703L
#define AT_LEAST_2017
#endif

#ifdef AT_LEAST_2017
#include <array>
#endif
#include <algorithm>
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

    namespace row_msks
    {
        // Declare and call a constexpr lambda. The array will be filled at compile time. Requires C++17 at least.
        template<typename T, size_t size>
        constexpr const std::array<T, size> get_row_masks = [] {
            std::array<T, size> A = {};
            for (unsigned i = 0; i < size; i++) {
                // A[0] = 0xff;
                // A[1] = 0xff00;
                // ...
                // A[6] = 0xff'0000'0000'0000
                // A[7] = 0xff00'0000'0000'0000
                A[i] = static_cast<T>(0xff) << (8 * i);
            }
            return A;
        }();

        auto _row_masks = get_row_masks<flags_t, maximum_allowed_board_size>;
    }

    static const flags_t* const row_masks = row_msks::_row_masks.data();

    namespace col_masks
    { 
        // Replicate 1 byte, 8 times, at compile time.
        template<typename T, size_t size>
        constexpr T replicate(unsigned char c) {
            T t(c);
            for (unsigned i = 1; i < size; ++i)
                t |= (T(c) << (i * 8));
            return t;
        }

        template<typename T, size_t size>
        constexpr const std::array<T, size> get_column_masks = [] {
            std::array<T, size> A = {};
            for (unsigned i = 0; i < size; i++) {
                // A[0] = replicate<8>(0x80)
                // A[1] = replicate<8>(0x40)
                // ...
                // A[6] = replicate<8>(0x02)
                // A[7] = replicate<8>(0x01)
                A[i] = replicate<T, size>(1 << ((size & 0xf) - 1 - i));
            }
            return A;
        }();

        auto _col_masks = get_column_masks<flags_t, maximum_allowed_board_size>;
    }

    static const flags_t* const column_masks = col_masks::_col_masks.data();

    const int sentinel = -1;

    void show_map(map_t map)
    {
        /// <summary>
        /// Not just for debugging, also called by qns::test().
        /// Show a map as text on the console. 
        /// </summary>
        /// <param name="map"></param>
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

    namespace threats 
    {
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

        template <typename T>
        constexpr T get_diagonal_size(T size)
        {
            return size * 2 - 1;
        }

        template <typename T, size_t size>
        constexpr const std::array<T, get_diagonal_size(size)> get_main_diagonal = [] 
        {
            std::array<T, get_diagonal_size(size)> A = { 0 };
            T bit{ 1 };
            for (unsigned row = 0; row < size; ++row)
            {
                for (unsigned col = 0; col < size; ++col)
                {
                    unsigned item = row - col + (unsigned) size - 1;
                    A[item] |= (bit << ((7 - col) * 8 + row));
                }
            }
            return A;
        }();

        template <typename T, size_t size>
        constexpr const std::array<T, get_diagonal_size(size)> get_second_diagonal = [] 
        {
            std::array<T, get_diagonal_size(size)> A = { 0 };
            T bit{ 1 };
            for (unsigned row = 0; row < size; ++row)
            {
                for (unsigned col = 0; col < size; ++col)
                {
                    unsigned item = unsigned(get_diagonal_size(size)) - 1 - row - col;
                    A[item] |= (bit << ((7 - col) * 8 + row));
                }
            }
            return A;
        }();

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



        // Compile-time calculation since C++ 17. Rules are TIGHT.
        template<typename VALUETYPE, unsigned int BOARD_SIZE>
        constexpr std::array<VALUETYPE, BOARD_SIZE* BOARD_SIZE> get_threats = [] { // OR: constexpr auto table
            std::array<VALUETYPE, BOARD_SIZE* BOARD_SIZE> A = {};
            const auto row__masks = row_msks::get_row_masks<VALUETYPE, BOARD_SIZE>;
            const auto main_diagonal_parallels = get_main_diagonal<VALUETYPE, BOARD_SIZE>;
            const auto second_diagonal_parallels = get_second_diagonal<VALUETYPE, BOARD_SIZE>;

            for (unsigned row = 0; row < BOARD_SIZE; row++) {
                for (unsigned column = 0; column < BOARD_SIZE; column++) {
                    A[row * BOARD_SIZE + column] = row__masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column];
                }
            }
            return A;
        }();

        auto _threats = get_threats<map_t, maximum_allowed_board_size>;
        // const map_t* threats = _threats.data();

        // Everything by value, on purpose.
        inline map_t threaten(map_t map, int row, int column)
        {
            // Before template metaprogramming we did this:
            //  map |= (row_masks[row] | main_diagonal_parallels[row + 7 - column] | second_diagonal_parallels[row + column]);
            const map_t* threats = _threats.data();
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
    } // namespace threats
    
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
        const map_t new_map = threats::threaten(map, solution[current_column], current_column);
        const int next_column = 1 + current_column;

        if (threats::is_totally_under_threat(new_map, next_column))
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
        for (auto current_row : threats::not_threatened_rows(new_map, next_column))
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
    } // void do_solve
} // namespace qns 

#define FOR_PROFILING
#ifdef FOR_PROFILING
double qns::solve()
{
    // Solution that works for an 8x8 chess board only (not generalized to n by n).
    // On the other hand, chess boards have 64 squares.
    // solutions.reserve(46); // Cheating? Nope, just using prior knowledge.
    std::vector<int> solution(maximum_allowed_board_size, -1);
#ifdef _DEBUG
    const int loops = 1;
#else
    const int loops = 1000;
#endif // DEBUG

    const int starting_rows_to_test = (board_size / 2) + (board_size % 2);
    hi_res_timer::microsecs_t max_time = 0ULL;
    hi_res_timer::microsecs_t min_time = std::numeric_limits<hi_res_timer::microsecs_t>::max();
    // hi_res_timer::microsecs_t tot_time = 0ULL;
    std::vector<hi_res_timer::microsecs_t> times_vec;
    times_vec.reserve(loops);

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
        // tot_time += microseconds;
        times_vec.push_back(microseconds);
    }

    std::sort(times_vec.begin(), times_vec.end());
    const double median_time =
        loops == 1 ? times_vec[0] :
        loops & 0x1 ? times_vec[loops / 2 + 1] :
        ((times_vec[loops / 2] + times_vec[loops / 2 + 1]) / 2.0);


    std::cout << "The fastest run took " << min_time << " microseconds, the slowest took " << max_time
        << ", and a median of " << loops << " runs was " << median_time << std::endl;
    do_show_results(failures_count, success_count, solutions, board_size);
    if (success_count < solutions.size())
    {
        solutions[success_count][0] = sentinel;
    }
    std::cout.flush();
    return median_time;
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

    const auto main_diagonal_parallels = threats::get_main_diagonal<map_t, maximum_allowed_board_size>;
    for (int i = 0; i < main_diagonal_parallels.size(); ++i)
    {
        cout << "=== " << std::dec << i << " parallel ===" << endl;
        show_map((map_t)(main_diagonal_parallels[i]));
    }

    cout << "=== Second diagonal ===" << endl;
    //show_map((map_t)secondary_diagonal_mask);


    const auto second_diagonal_parallels = threats::get_second_diagonal<map_t, maximum_allowed_board_size>;
    for (int i = 0; i < second_diagonal_parallels.size(); ++i)
    {
        cout << "=== " << std::dec << i << " second parallel ===" << endl;
        show_map((map_t)(second_diagonal_parallels[i]));
    }

    cout << "========" << endl;
    map_t map = 0ULL;
    map = threats::threaten(map, 1, 0);
    show_map(map);

    cout << "========" << endl;
    map = threats::threaten(map, 4, 1);
    show_map(map);

    cout << "========" << endl;
    map = threats::threaten(map, 7, 2);
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