// Cpp8Queens.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctype.h>

#include "queens.h"

/*
Command line arguments:
-v   verbose
-t   test
-s n short(n) - try only N different solutions, showing failures

*/

int main(int argc, const char** argv)
{
    bool verbose = false;
    bool test = false;

    for (int i = 1; i < argc; ++i)
    {
        const char* arg = argv[i];
        if (arg[0] != '-' && arg[0] != '/')
            continue; // for

        switch (tolower(arg[1]))
        {
            case 'v':
                verbose = true;
                break;
            case 't':
                test = true;
                break;
            case 's':
                int short_trials = atoi(argv[++i]);
                if (0 < short_trials)
                {
                    qns::set_short(short_trials);
                }
        } // switch
    }

    if (verbose)
    {
        qns::set_verbose(true);
    }
    if (test)
    {
        qns::test();
        return 0;
    }
    for (int desired_board_size = 4; desired_board_size < 9; ++desired_board_size)
    {
        qns::set_board_size(desired_board_size);
        qns::solve();
    }

    return 0;
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
