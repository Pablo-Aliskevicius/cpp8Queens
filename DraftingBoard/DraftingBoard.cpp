// DraftingBoard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <thread>

using ull = unsigned long long;

void test_shifts()
{
    unsigned long long one = 1ULL;
    for (int i = 0; i < 72; i += 8)
    {
        unsigned long long tested = 0ULL;
        tested |= (one << i);
        std::cout << one << " << " << std::dec <<  i << " is " << std::hex << "0x" << tested << std::endl;
    }
}  // 1 << 64 is 0x1


template <typename T>
constexpr T get_diag_size(T size) 
{
    return size * 2 - 1;
}

template <typename T, size_t size>
constexpr T get_flags(int i)
{
    T flags = ((i < size) ? (T(1) << (i * 8)) : T(0)); // |
    // ((size < 1) ? T(0) : (i < size + 1) ? (T(1) << (1 + (i - 1) * 8)) : T(0));
    for (unsigned int j = 1; j < sizeof(T); ++j)
    {
        const unsigned shift = (j + (i - j) * 8) & (sizeof(T) * 8 - 1);
        const T bit = T(1) << shift;
        flags |= shift;
    }
    return flags;
}

template <typename T, size_t size>
constexpr const std::array<T, get_diag_size(size)> get_main_diagonal = [] {
    std::array<T, get_diag_size(size)> A = { 0 };
    T bit{ 1 };
    for (unsigned row = 0; row < size; ++row)
    {
        for (unsigned col = 0; col < size; ++col)
        {
            unsigned item = row - col + size - 1;
            A[item] |= (bit << ((7 - col) * 8 + row));
        }
    }
    return A;
}();

auto main_diagonal_parallels = get_main_diagonal<unsigned long long, 8>;

template <typename T, size_t size>
constexpr const std::array<T, get_diag_size(size)> get_second_diagonal = [] {
    std::array<T, get_diag_size(size)> A = { 0 };
    T bit{ 1 };
    for (unsigned row = 0; row < size; ++row)
    {
        for (unsigned col = 0; col < size; ++col)
        {
            unsigned item = unsigned(get_diag_size(size) - 1 - row - col);
            A[item] |= (bit << ((7 - col) * 8 + row));
        }
    }
    return A;
}();
auto second_diagonal_parallels = get_second_diagonal<unsigned long long, 8>;



void test_diags()
{
    using namespace std;
    for (int i = 0; i < 15; ++i)
    {
        unsigned long long ull = main_diagonal_parallels.data()[i];
        cout << hex << "0x" << ull << endl;
    }
    cout << endl;
    for (int i = 0; i < 15; ++i)
    {
        unsigned long long ull = second_diagonal_parallels.data()[i];
        cout << hex << "0x" << ull << endl;
    }

}

int main()
{
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";
    test_shifts();
    test_diags();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
