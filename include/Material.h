#ifndef MATERIAL_H
#define MATERIAL_H

#include "Constants.h"
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

class Metal : public Material {
private:
    Color albedo;
    float_type fuzz;

public:
    Metal(const Color& albedo_, float_type fuzz_) : albedo{albedo_}, fuzz{fuzz_ <= 1 ? fuzz_ : 1} {}

    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;
        Vector3D reflected_direction = ray_in.direction().unit_vector().reflect(hit_record.unit_normal);
        Ray3D reflected_ray {hit_record.point, reflected_direction + fuzz*Vector3D::random_sphere_unit_vector()};
        return ScatterRecord{success, reflected_ray, albedo};
    }
};


class Dielectric : public Material {
private:
    float_type refractive_index;

    static float_type reflectance(float_type cosine, float_type ref_idx) {
        //use Sclick's reflectance approximation
        float_type r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow(1-cosine, 5);
    }
    
public:
    Dielectric(float_type refractive_index_) : refractive_index {refractive_index_} {}

    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;
        constexpr Color attenuation {1, 1, 1};
        constexpr float_type air_ri = 1.0;
        float_type refraction_ratio = (hit_record.front_face) ? air_ri / refractive_index : refractive_index / air_ri;
        Vector3D unit_direction = ray_in.direction().unit_vector();
        float_type cos_theta = fmin(-unit_direction.dot(hit_record.unit_normal), 1.0);
        Vector3D refracted_direction = unit_direction.refract(hit_record.unit_normal, refraction_ratio, reflectance(cos_theta, refraction_ratio));
        Ray3D refracted_ray {hit_record.point, refracted_direction};
        return ScatterRecord{success, refracted_ray, attenuation};  
    }
};


#endif