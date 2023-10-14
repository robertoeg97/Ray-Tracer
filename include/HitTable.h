#ifndef HITTABLE_H
#define HITTABLE_H

#include "Vector3D.h"
#include "Ray3D.h"

struct HitRecord {
    Vector3D point {};
    Vector3D normal {};
    float_type t;
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray3D& ray, float_type ray_tmin, float_type ray_tmax, HitRecord& rec) const = 0;
};

#endif