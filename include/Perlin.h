#ifndef PERLIN_H
#define PERLIN_H

#include <algorithm>
#include <cmath>
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
            random_vecs[i] = Vector3D::random(-1, 1).unit_vector();
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
     * @brief Generates Perlin noise sample from [-1, 1] using a Vector3D point in space as a hash.
     * 
     * @param point A Vector3D point in space
     * @return float_type the noise value from [-1, 1]
     */
    float_type noise(const Vector3D& point) const {
        auto u = point.x() - floor(point.x());
        auto v = point.y() - floor(point.y());
        auto w = point.z() - floor(point.z());

        auto i = static_cast<int>(floor(point.x()));
        auto j = static_cast<int>(floor(point.y()));
        auto k = static_cast<int>(floor(point.z()));

        Vector3D sample_vecs[2][2][2];

        //get samples of a 2x2x2 area around the point to interpolate
        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    sample_vecs[di][dj][dk] = random_vecs[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                    ];

        return trilinear_interpolation(sample_vecs, u, v, w);
    }

    /**
     * @brief A modification of Perlin noise designed to introduce a turbulent effect to the generated noise pattern.
     *  It enhances the visual complexity and realism of procedural textures and animations.
     * 
     * @param point A Vector3D point in space
     * @param depth The number of frequencies of the point to include in the weighted sum.
     * @return float_type the noise value from [0, 1]
     */
    float_type turbulence(Vector3D point, int depth = 7) const {
        //weighted sum of the frequencies of point
        float_type accum = 0;
        float_type weight = 1;
        for (int i = 0; i < depth; ++i) {
            accum += weight*noise(point);
            weight *= .5;
            point *= 2;    
        }
        
        //map value to [0, 1]
        return fabs(accum);
    }

private:   
    /**
     * @brief Linearly interpolates a 3D matrix of noise samples.
     * 
     * @param sample_vecs the noise samples
     * @param u the fractional component of the x position in 3D space that served as the noise hash
     * @param v the fractional component of the y position in 3D space that served as the noise hash
     * @param w the fractional component of the z position in 3D space that served as the noise hash
     * @return float_type the interpolated noise value
     */
    static float_type trilinear_interpolation ( Vector3D sample_vecs[2][2][2], 
                                                float_type u, float_type v, float_type w) {
        //Hermite cubic to remove Mach bands
        float_type uu = u*u*(3-2*u);
        float_type vv = v*v*(3-2*v);
        float_type ww = w*w*(3-2*w);

        //interpolation
        auto accum = 0.0;
        for (int i=0; i < 2; i++) {
            for (int j=0; j < 2; j++) {
                for (int k=0; k < 2; k++) {
                    Vector3D weight_vector {u-i, v-j, w-k};
                    accum +=  (i*uu + (1-i)*(1-uu))
                            * (j*vv + (1-j)*(1-vv))
                            * (k*ww + (1-k)*(1-ww))
                            * sample_vecs[i][j][k].dot(weight_vector);  //moves min and maxx off of lattice
                }
            }        
        }
            
        return accum;
    }

    constexpr static unsigned int point_count = 256;
    std::array<Vector3D, point_count> random_vecs;  //use random vectors instead of floats to move min and max off of the lattice points
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;
};

#endif