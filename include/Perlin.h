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
     * @brief Generates Perlin noise sample using a Vector3D point in space as a hash.
     * 
     * @param point A Vector3D point in space
     * @return float_type the noise value
     */
    float_type noise(const Vector3D& point) const {
        auto u = point.x() - floor(point.x());
        auto v = point.y() - floor(point.y());
        auto w = point.z() - floor(point.z());
        //Hermite cubic to remove Mach bands
        u = u*u*(3-2*u);
        v = v*v*(3-2*v);
        w = w*w*(3-2*w);

        auto i = static_cast<int>(floor(point.x()));
        auto j = static_cast<int>(floor(point.y()));
        auto k = static_cast<int>(floor(point.z()));

        float_type sample_vals[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    sample_vals[di][dj][dk] = random_floats[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                    ];

        return trilinear_interpolation(sample_vals, u, v, w);
    }

private:
    static float_type trilinear_interpolation ( float_type sample_vals[2][2][2], 
                                                float_type u, float_type v, float_type w) {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                            (j*v + (1-j)*(1-v))*
                            (k*w + (1-k)*(1-w))*sample_vals[i][j][k];

        return accum;
    }

    constexpr static unsigned int point_count = 256;
    std::array<float_type, point_count> random_floats;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;
};

#endif