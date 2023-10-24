#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"

struct ScatterRecord {
    bool success = false;
    Ray3D ray_out {};
    Color attenuation {};
};

class Material {
public:
    virtual ~Material() {}

    virtual ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const = 0;
};

class Lambertian : public Material { 
private:
    Color albedo;

public:
    Lambertian(const Color& albedo_) : albedo{albedo_} {}
    
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        (void) ray_in;  //unused parameter
        constexpr bool success = true;
        Vector3D scatter_direction = Vector3D::random_sphere_unit_vector() + hit_record.unit_normal;
        //catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = hit_record.unit_normal;
        }
        Ray3D scatter_ray {hit_record.point, scatter_direction};
        return ScatterRecord{success, scatter_ray, albedo};
    }
};


#endif