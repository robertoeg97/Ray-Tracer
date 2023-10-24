#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Interval.h"

class Material;

struct HitRecord {
    bool is_hit = false;
    Vector3D point {};
    Vector3D unit_normal {};
    float_type t {};
    bool front_face {};
    std::shared_ptr<Material> material_ptr {};

    HitRecord() = default;

    /**
     * @brief Sets the unit_normal of the object to always point towards the intersecting ray. 
     * Sets front_face to true if the ray intersected the object from the outside, and false from the inside.
     * 
     * @param ray The ray that is hitting the object.
     * @param outward_unit_normal The outward facing unit normal vector of the object at the point of collision.
     */
    void set_face_and_normal(const Ray3D& ray, const Vector3D& outward_unit_normal) {
        front_face = ray.direction().dot(outward_unit_normal) < 0;
        unit_normal = front_face ? outward_unit_normal : -outward_unit_normal;
    }
};


class Hittable {
public:
    virtual ~Hittable() = default;

    virtual HitRecord hit(const Ray3D& ray, const Interval& t_interval) const = 0;
};

#endif