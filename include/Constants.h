#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>
#include <memory>

//usings
using float_type = float;

//constants
inline constexpr float_type pi = 3.1415926535897932385;
inline constexpr float_type infinity = std::numeric_limits<float_type>::infinity();

//utility
inline float_type degrees_to_radians(float_type degrees) {
    return degrees * pi / 180;
}

#endif