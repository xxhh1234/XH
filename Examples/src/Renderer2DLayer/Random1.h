#pragma once

#include <random>

class Random1
{
public:
    static void Init()
    {
        s_RandomEngine1.seed(std::random_device()());
    }

    static float Float()
    {
        return static_cast<float>(s_Distribution1(s_RandomEngine1)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    }
private:
    static std::mt19937 s_RandomEngine1;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution1;
};