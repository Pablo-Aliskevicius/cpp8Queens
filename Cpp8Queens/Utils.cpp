#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "utils.h"
#include "write_solutions.h"

namespace utils
{
    void ComputeAndDisplayMedianSpeed(
        double& median_time,
        std::vector<hi_res_timer::microsecs_t>& times_vec,
        const hi_res_timer::microsecs_t min_time,
        const hi_res_timer::microsecs_t max_time)
    {
        std::time_t now = time(nullptr);

        std::sort(times_vec.begin(), times_vec.end());

        const auto loops = times_vec.size();
        const auto middle = loops / 2;

        median_time =
            loops & 0x1 ? times_vec[middle] :
            ((times_vec[middle] + times_vec[middle - 1]) / 2.0);  // e.g. 4 -> 2, 1; 2 -> 1, 0

        if (median_time < 1'000)
        {
            // Microseconds
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << min_time << " microseconds, the slowest took " << max_time
                << ", and a median of " << loops << " runs was " << median_time << "." << std::endl;
        }
        else if (median_time < 1'000'000)
        {
            // Milliseconds
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << double(min_time) / 1e3 << " milliseconds, the slowest took " << double(max_time) / 1e3
                << ", and a median of " << loops << " runs was " << median_time / 1e3 << "." << std::endl;
        }
        else
        {
            // Seconds. 16x16 takes above 17 seconds. 
            std::cout
                << std::asctime(std::localtime(&now))
                << " The fastest run took " << double(min_time) / 1e6 << " seconds, the slowest took " << double(max_time) / 1e6
                << ", and a median of " << loops << " runs was " << median_time / 1e6 << "." << std::endl;
        }
    }
}