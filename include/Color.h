#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <algorithm>
#include "Triple.h"
#include "Random.h"

/**
 * @brief color is represented by a 3D vector of floating-point types from 0.0 to 1.0
 * 
 */
class Color : public Triple<Color> {
public:
    static constexpr int max_pixel_val = 255;   

    using Triple<Color>::Triple;    //inherit base class constructors

    static Color random() {
        return Color{random::random_float(0, 1), random::random_float(0, 1), random::random_float(0, 1)};
    }

    static Color random(float_type low, float_type high) {
        float_type r = random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        float_type g = random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        float_type b = random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        return Color{r, g, b};
    }

    void write_pixel(std::ostream& out) const {
        Color gamma = get_gamma();
        out << static_cast<int>(gamma.x() * (max_pixel_val+.999)) << ' '
            << static_cast<int>(gamma.y() * (max_pixel_val+.999)) << ' '
            << static_cast<int>(gamma.z() * (max_pixel_val+.999)) << '\n';
    }

private:
    Color get_gamma() const {
        return Color{std::sqrt(m_vec[0]), std::sqrt(m_vec[1]), std::sqrt(m_vec[2])};
    }
};

class ColorSum : public Triple<Color> {
public:

    using Triple<Color>::Triple;    //inherit base class constructors

    Color scale(int number_of_samples) const {
        float_type scale = 1.0/number_of_samples;
        return scale * Color{x(), y(), z()};
    }

    ColorSum operator+=(const Color& color) {
        m_vec[0] += color.x();
        m_vec[1] += color.y();
        m_vec[2] += color.z();
        return *this;
    }
};

#endif