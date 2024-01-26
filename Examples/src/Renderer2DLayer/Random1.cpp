#include "Random1.h"

std::mt19937 Random1::s_RandomEngine1;
std::uniform_int_distribution<std::mt19937::result_type> Random1::s_Distribution1;