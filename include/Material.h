#ifndef MATERIAL_H
#define MATERIAL_H

#include "Constants.h"
#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"

/**
 * @brief Data detailing what happens to light after a collision (a result of either refraction or reflection).
 * 
 */
struct ScatterRecord {
    bool success = false;   //true if a collision occured
    Ray3D ray_out {};       //the resulting light ray after collision
    Color attenuation {};   //the multiplicative factors of (r,g,b) that the light ray undergoes as a result of collision
};

/**
 * @brief The abstract class describing an interface to describe 
 * the light scattering behavior of different object materials.
 * 
 */
class Material {
public:
    virtual ~Material() {}

    /**
     * @brief Returns information about how light scatters after colliding with an object.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    virtual ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const = 0;
};

/**
 * @brief A diffuse (matte) material: randomly scatters in a hemisphere on the side of collision (the surface normal).
 * 
 */
class Lambertian : public Material { 
private:
    Color albedo;

public:
    /**
     * @brief Construct a new Lambertian object
     * 
     * @param albedo_ the multiplicative factor that light colliding with this material will undergo
     */
    Lambertian(const Color& albedo_) : albedo{albedo_} {}
    
    /**
     * @brief Returns information about how light scatters as a result of diffuse object collision.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        (void) ray_in;  //unused parameter
        constexpr bool success = true;  //always successful
        ////lambertian scattering equation: scattering proportional to cos(x), 
        //where x is the angle between normal and scatter direction
        Vector3D scatter_direction = Vector3D::random_sphere_unit_vector() + hit_record.unit_normal;    
        //catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = hit_record.unit_normal;
        }
        Ray3D scatter_ray {hit_record.point, scatter_direction};
        return ScatterRecord{success, scatter_ray, albedo};
    }
};

/**
 * @brief A reflective material that light can collide with.
 * 
 */
class Metal : public Material {
private:
    Color albedo;
    float_type fuzz;

public:
    /**
     * @brief Construct a new Metal object
     * 
     * @param albedo_ the multiplicative factor that light colliding with this material will ungergo
     * @param fuzz_ The "degree of randomness" that the reflected light direction =will have.
     * Resulting light will have its direction offset by a sphere of radius fuzz.
     */
    Metal(const Color& albedo_, float_type fuzz_) : albedo{albedo_}, fuzz{fuzz_ <= 1 ? fuzz_ : 1} {}

    /**
     * @brief Returns information about how light scatters as a result of collision with metal.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;  //always reflects
        Vector3D reflected_direction = ray_in.direction().unit_vector().reflect(hit_record.unit_normal);
        Ray3D reflected_ray {hit_record.point, reflected_direction + fuzz*Vector3D::random_sphere_unit_vector()};
        return ScatterRecord{success, reflected_ray, albedo};
    }
};

/**
 * @brief Clear material such as water, glass, or diamonds that light can collide with. 
 * 
 */
class Dielectric : public Material {
private:
    float_type refractive_index;

    /**
     * @brief Approximates the reflectance of the material.
     * 
     * @param cosine The cosine of the angle between the incoming light ray and the surface normal.
     * @param ref_idx The ratio of the current material's index of refraction 
     * to the colliding material's index of refraction
     * @return float_type: reflectance
     */
    static float_type reflectance(float_type cosine, float_type ref_idx) {
        //use Sclick's reflectance approximation
        float_type r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow(1-cosine, 5);
    }
    
public:
    /**
     * @brief Construct a new Dielectric object
     * 
     * @param refractive_index_ The index of refraction of the material.
     */
    Dielectric(float_type refractive_index_) : refractive_index {refractive_index_} {}

    /**
     * @brief Returns information about how light scatters as a result of collision with the dielectric.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;  //always reflects
        constexpr Color attenuation {1, 1, 1};
        constexpr float_type air_ri = 1.0;
        float_type refraction_ratio = (hit_record.front_face) ? air_ri / refractive_index : refractive_index / air_ri;
        Vector3D unit_direction = ray_in.direction().unit_vector();
        //theta = angle between normal and incident light ray
        float_type cos_theta = fmin(-unit_direction.dot(hit_record.unit_normal), 1.0);  
        Vector3D refracted_direction = unit_direction.refract(hit_record.unit_normal, refraction_ratio, reflectance(cos_theta, refraction_ratio));
        Ray3D refracted_ray {hit_record.point, refracted_direction};
        return ScatterRecord{success, refracted_ray, attenuation};  
    }
};


#endif