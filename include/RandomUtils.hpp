#ifndef RANDOM_UTILS_HPP_
#define RANDOM_UTILS_HPP_

#include <cstdlib>

namespace BinaryRing {

inline int random(int lowerBound, int upperBound) noexcept
{
    return (rand() % (upperBound - lowerBound)) + lowerBound;
}

inline float frandom(float lowerBound, float upperbound) noexcept
{
    return lowerBound + static_cast<float>(rand())
    / (static_cast<float>(RAND_MAX / (upperbound - lowerBound)));
}

} /* namespace BinaryRing */

#endif /* RANDOM_UTILS_HPP_ */