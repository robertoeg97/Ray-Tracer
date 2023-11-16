#ifndef AABB_H
#define AABB_H

#include <cmath>
#include <cassert>
#include "Vector3D.h"
#include "Ray3D.h"
#include "Interval.h"

/**
 * @brief An Axis Aligned Bounding Box class.
 * Used to wrap around a cluster of object and more efficiently compute collisions with large groups.
 * 
 */
class AABB {
private:
    constexpr static int num_dimensions = 3;

public:
    Interval x, y, z;

    /**
     * @brief Construct an empty AABB object
     * 
     */
    AABB() = default;   //default constructed intervals are empty

    /**
     * @brief Construct a new Axis Aligned Bounding Box object given the planes of the box.
     * 
     * @param x_bounds the x axis aligned planes of the bounding box
     * @param y_bounds the y axis aligned planes of the bounding box
     * @param z_bounds the z axis aligned planes of the bounding box
     */
    AABB(const Interval& x_bounds, const Interval& y_bounds, const Interval& z_bounds) : 
        x{x_bounds}, y{y_bounds}, z{z_bounds} 
    {}

    /**
     * @brief Construct a new Axis Aligned Bounding Box object given two opposing corners.
     * 
     * @param point1 The first opposing corner of the box. 
     * @param point2 The second opposing corner of the box.  
     */
    AABB(const Vector3D& point1, const Vector3D& point2) :
        x (Interval(fmin(point1.x(), point2.x()), fmax(point1.x(), point2.x()))),
        y (Interval(fmin(point1.y(), point2.y()), fmax(point1.y(), point2.y()))),
        z (Interval(fmin(point1.z(), point2.z()), fmax(point1.z(), point2.z())))
    {}

    /**
     * @brief Construct a new Axis Aligned Bounding Box object that entirely contains two other AABBes.
     * 
     * @param box1 the first AABB
     * @param box2 the second AABB
     */
    AABB(const AABB box1, const AABB box2) :
        x {box1.x, box2.x},
        y {box1.y, box2.y},
        z {box1.z, box2.z}
    {}

    /**
     * @brief Takes an index of the desired axis and returns a const reference to it.
     * 
     * @param n an index from 0-2
     * @return const Interval& x, y, or z axis if n is 0, 1, or 2 respectively
     */
    const Interval& axis (int n) const {
        assert(n >= 0 && n <= 2 && "invalid input: axis(int n) only accepts values 0, 1, or 2.");
        if (n == 0) return x;
        if (n == 1) return y;
        return z;
    }

    /**
     * @brief Returns whether a ray travels through the bounding box during some time period.
     * 
     * @param ray A ray 
     * @param t_interval An interval of times during which we check for the ray's presence within the bounding box
     * @return true if the ray is within the bounding box at some time during t_interval
     * @return false otherwise
     */
    bool hit (const Ray3D& ray, Interval t_interval) const {
        for (size_t i = 0; i < num_dimensions; ++i) {
            auto invD = 1 / ray.direction()[i];
            auto origin_component = ray.origin()[i];
            
            auto t0 = (axis(i).min - origin_component) * invD;
            auto t1 = (axis(i).max - origin_component) * invD;

            if (invD < 0) {
                std::swap(t0, t1);
            }

            if (t0 > t_interval.min) t_interval.min = t0;
            if (t1 < t_interval.max) t_interval.max = t1;

            if (t_interval.max <= t_interval.min)
                return false;
        }

        return true;
    }
};

#endif