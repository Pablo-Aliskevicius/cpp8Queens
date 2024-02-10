#pragma once

#include <vector>

#include "high_res_clock.h"

namespace utils
{
    void ComputeAndDisplayMedianSpeed(
        double& median_time,
        std::vector<hi_res_timer::microsecs_t>& times_vec,
        const hi_res_timer::microsecs_t min_time,
        const hi_res_timer::microsecs_t max_time);
} // namespace utils