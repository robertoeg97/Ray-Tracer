#ifndef HITTABLE_H
#define HITTABLE_H

#include "Vector3D.h"

class Ray3D;

struct HitRecord {
    Vector3D point;
    Vector3D unit_normal;
    float_type t;
    bool front_face;

    void set_face_and_normal(const Ray3D& ray, const Vector3D& outward_unit_normal) {
        front_face = ray.direction().dot(outward_unit_normal) < 0;
        unit_normal = front_face ? outward_unit_normal : -outward_unit_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray3D& ray, float_type ray_tmin, float_type ray_tmax, HitRecord& rec) const = 0;
};

#endif