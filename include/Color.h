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
        out << static_cast<int>(x() * (max_pixel_val+.999)) << ' '
            << static_cast<int>(y() * (max_pixel_val+.999)) << ' '
            << static_cast<int>(z() * (max_pixel_val+.999)) << '\n';
    }
};

class ColorSum : public Triple<Color> {
public:

    using Triple<Color>::Triple;    //inherit base class constructors

    Color scale(int number_of_samples) const {
        float_type scale = 1.0/number_of_samples;
        return scale * Color{x(), y(), z()};
    }

    ColorSum operator+=(Color color) {
        m_vec[0] += color.x();
        m_vec[1] += color.y();
        m_vec[2] += color.z();
        return *this;
    }
};

#endif