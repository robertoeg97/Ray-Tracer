#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray3D.h"
#include "Hittable.h"

class Material {
public:
    virtual ~Material() {}

    virtual HitRecord scatter(const Ray3D& ray_in) = 0;
};

#endif