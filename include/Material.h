#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray3D.h"
#include "Hittable.h"

struct HitRecord;

class Material {
public:
    virtual ~Material() {}

    virtual HitRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const = 0;
};



#endif