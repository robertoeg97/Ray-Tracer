#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "Triple.h"

/**
 * @brief color is represented by a 3D vector of floating-point types from 0.0 to 1.0
 * 
 */
class Color : public Triple<Color> {
public:
    static constexpr int max_pixel_val = 255;   

    using Triple<Color>::Triple;    //inherit base class constructors

    void write_pixel(std::ostream& out) const {
        out << static_cast<int>(x() * (max_pixel_val+.9999)) << ' '
            << static_cast<int>(y() * (max_pixel_val+.9999)) << ' '
            << static_cast<int>(z() * (max_pixel_val+.9999)) << '\n';
    }
};

#endif