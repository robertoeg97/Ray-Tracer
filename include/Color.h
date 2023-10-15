#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "Constants.h"
#include "Triple.h"
#include "Ray3D.h"
#include "Sphere.h"
#include "Interval.h"

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

inline Color color_pixel(const Ray3D& pixel_ray, const Hittable& hittable) {
        auto [is_hit, hit_record] = hittable.hit(pixel_ray, Interval(0, infinity));
        if (is_hit) {
            return .5 * (Color(hit_record.unit_normal.x()+1, hit_record.unit_normal.y()+1, hit_record.unit_normal.z()+1));   
        }

        Vector3D unit_direction = pixel_ray.direction().unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }

#endif