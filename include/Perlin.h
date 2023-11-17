#ifndef PERLIN_H
#define PERLIN_H

#include <algorithm>
#include "Constants.h"
#include "Random.h"
#include "Vector3D.h"

/**
 * @brief Generates Perlin noise.
 * 
 */
class Perlin {
public:
    /**
     * @brief Construct a new Perlin object
     * 
     */
    Perlin() {
        //initialize random_floats with values [0,1]
        for (int i = 0; i < point_count; ++i) {
            random_floats[i] = Random::random_float(0, 1);
        }
        //intialize x, y, and z with random permutations of values [0, point_count]
        std::iota(perm_x.begin(), perm_x.end(), 0);
        std::shuffle(perm_x.begin(), perm_x.end(), Random::mt);
        std::iota(perm_y.begin(), perm_y.end(), 0);
        std::shuffle(perm_y.begin(), perm_y.end(), Random::mt);
        std::iota(perm_z.begin(), perm_z.end(), 0);
        std::shuffle(perm_z.begin(), perm_z.end(), Random::mt);
    }

    /**
     * @brief Uses a Vector3D point to output a noise sample.
     * 
     * @param point A point in space.
     * @return float_type a random float value
     */
    float_type noise(const Vector3D point) const {
        constexpr int scale = 4;            //higher value results in larger "blocks" of noise
        constexpr unsigned int mask = 255;  //bottom 8 LSB
        auto i = static_cast<unsigned int>(scale*point.x()) & mask;
        auto j = static_cast<unsigned int>(scale*point.y()) & mask;
        auto k = static_cast<unsigned int>(scale*point.z()) & mask;

        //use point coordinates to hash into the random floats
        return random_floats[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

private:
    constexpr static int point_count = 256;
    std::array<float_type, point_count> random_floats;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;
};

#endif