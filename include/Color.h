#ifndef COLOR_H
#define COLOR_H

#include <string>
#include <algorithm>
#include "ImageData.h"
#include "Triple.h"
#include "Random.h"
#include "Constants.h"

/**
 * @brief color is represented by a 3D vector of floating-point types from 0.0 to 1.0
 * 
 */
class Color : public Triple<Color> {
public:
    using Triple<Color>::Triple;    //inherit base class constructors

    /**
     * @brief Generates a random color uniformly among all possible values of R, G, and B
     * 
     * @return Color: The resulting random color.
     */
    static Color random() {
        return Color{Random::random_float(0, 1), Random::random_float(0, 1), Random::random_float(0, 1)};
    }

    /**
     * @brief Generates a uniform random color from low to high, 
     * which represents the saturation the colors' components.
     * Color has no bias towards any of R, G, or B.
     * 
     * @param low The minimum possible value is 0.
     * @param high The maximum possible value is 1.
     * @return Color The random color.
     */
    static Color random(float_type low, float_type high) {
        float_type r = Random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        float_type g = Random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        float_type b = Random::random_float(std::fmax(0, std::fmin(1, low)), std::fmax(0, std::fmin(1, high)));
        return Color{r, g, b};
    }

    /**
     * @brief Prints the Color to out, with component value of 1 scaled to the max_pixel_val.
     * 
     * @tparam WIDTH 
     * @tparam HEIGHT 
     * @param row 
     * @param col 
     * @param image_data 
     */
    template <unsigned long WIDTH, unsigned long HEIGHT>
    void write_pixel(int row, int col, ImageData<WIDTH, HEIGHT>& image_data) const { 
        Color gamma = get_gamma();
        std::string r = std::to_string(static_cast<int>(gamma.x() * (ColorConstants::max_pixel_val+.999)));
        std::string g = std::to_string(static_cast<int>(gamma.y() * (ColorConstants::max_pixel_val+.999)));
        std::string b = std::to_string(static_cast<int>(gamma.z() * (ColorConstants::max_pixel_val+.999)));
        std::string pixel_data = r + ' ' + g + ' ' + b + '\n';
        image_data.write_pixel(row, col, pixel_data);
    }

private:
    /**
     * @brief Converts a linear Color space object to gamma Color space.
     * 
     * @return Color in gamma space.
     */
    Color get_gamma() const {
        return Color{std::sqrt(m_vec[0]), std::sqrt(m_vec[1]), std::sqrt(m_vec[2])};
    }
};

/**
 * @brief An intermediary object that is used to store the sum of multiple colors when sampling, 
 * before being scaled down to return a valid Color object.
 * 
 */
class ColorSum : public Triple<Color> {
public:

    using Triple<Color>::Triple;    //inherit base class constructors

    /**
     * @brief If the ColorSum object is a sum of num_of_samples samples,
     * returns a valid Color object that averages them.
     * 
     * @param number_of_samples the number of samples summed in the current object.
     * @return Color: the average Color of the samples.
     */
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