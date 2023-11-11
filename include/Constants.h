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
namespace ColorConstants {
    inline constexpr int num_components = 3;
    inline constexpr int max_pixel_val = 255;   //the max pixel value of a color, representing full saturation
    inline constexpr int max_pixel_val_digits = 3;
}

//utility
inline constexpr float_type degrees_to_radians(float_type degrees) {
    return degrees * pi / 180;
}

#endif