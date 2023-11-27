#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "Constants.h"
#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"
#include "Texture.h"

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

    /**
     * @brief Returns the Color emitted by the material, specified by a light-emitting material's Texture.
     * By default, Materials will emit nothing (black).
     * This behavior will be overridden by light-emitting materials.
     * 
     * @param u the horizontal coordinate of the texture, where 0 is the leftmost point and 1 is the rightmost point.
     * @param v the vertical coordinate of the texture, where 0 is the bottommost point and 1 is the topmost point.
     * @param position The position in spance that the ray intersects with the Material.
     * @return Color The emitted color
     */
    virtual Color emitted(float_type u, float_type v, const Vector3D& position) const {
        return Color{0, 0, 0};
    }
};

/**
 * @brief A diffuse (matte) material: randomly scatters in a hemisphere on the side of collision (the surface normal).
 * 
 */
class Lambertian : public Material { 
private:
    std::shared_ptr<Texture> albedo;

public:
    /**
     * @brief Construct a new Lambertian object that scales the color of all light it refracts by some constant
     * 
     * @param scalar the RGB multiplicative factors that light colliding with this material will undergo
     */
    Lambertian(const Color& scalar) : albedo{std::make_shared<SolidColorTexture>(scalar)} {}
    
    /**
     * @brief Construct a new Lambertian object whose surface maps to some texture 
     * 
     * @param texture_ptr the std::shared_ptr to some Texture. 
     */
    Lambertian(std::shared_ptr<Texture> texture_ptr) : albedo{texture_ptr} {}

    /**
     * @brief Returns information about how light scatters as a result of diffuse object collision.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;  //always successful
        ////lambertian scattering equation: scattering proportional to cos(x), 
        //where x is the angle between normal and scatter direction
        Vector3D scatter_direction = Vector3D::random_sphere_unit_vector() + hit_record.unit_normal;    
        //catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = hit_record.unit_normal;
        }
        Ray3D scatter_ray {hit_record.point, scatter_direction, ray_in.time()};
        return ScatterRecord{success, scatter_ray, albedo->value(hit_record.u, hit_record.v, hit_record.point)};
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
     * @param fuzz_ The "degree of randomness" that the reflected light direction will have.
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
        Vector3D fuzzed_direction = reflected_direction + fuzz*Vector3D::random_sphere_unit_vector();
        Ray3D reflected_ray {hit_record.point, fuzzed_direction, ray_in.time()};
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
        Ray3D refracted_ray {hit_record.point, refracted_direction, ray_in.time()};
        return ScatterRecord{success, refracted_ray, attenuation};  
    }
};

/**
 * @brief A light emitting material, whose light pattern is determined by a shared_ptr<Texture>.
 * 
 */
class DiffuseLights : public Material {
public:
    /**
     * @brief Construct a new Diffuse Lights object
     * 
     * @param emit_ the Texture whose color the light will emit.
     */
    DiffuseLights(std::shared_ptr<Texture> emit_) : emit{emit_} {}

    /**
     * @brief Construct a new Diffuse Lights object
     * 
     * @param color A color that the light will uniformly emit.
     */
    DiffuseLights(Color color) : emit{std::make_shared<SolidColorTexture>(color)} {}

    /**
     * @brief Returns information about how light scatters with the Material.
     * For diffuse lights, there is never scattering.
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        return ScatterRecord{}; //failed scatter
    }

    /**
     * @brief Returns the Color emitted by the Diffuse Lights, specified by the emit Texture.
     * 
     * @param u the horizontal coordinate of the texture, where 0 is the leftmost point and 1 is the rightmost point.
     * @param v the vertical coordinate of the texture, where 0 is the bottommost point and 1 is the topmost point.
     * @param position The position in spance that the ray intersects with the Material.
     * @return Color The emitted color
     */
    Color emitted(float_type u, float_type v, const Vector3D& position) const override {
        return emit->value(u, v, position);
    }

private:    
    std::shared_ptr<Texture> emit;
};


/**
 * @brief An isotropic material scatters ligt uniformly in all directions.
 * 
 */
class Isotropic : public Material {
public:
    /**
     * @brief Construct a new Isotropic object
     * 
     * @param color the Color that will be used to uniformly attenuate any light ray that is scattered by the material.
     */
    Isotropic(Color color) : albedo {std::make_shared<SolidColorTexture>(color)} {}

    /**
     * @brief Construct a new Isotropic object
     * 
     * @param albedo_ the Texture that will be used to attenuate any light ray that is scattered by the material.
     */
    Isotropic(std::shared_ptr<Texture> albedo_) : albedo {albedo_} {}

        /**
     * @brief Returns information about how light scatters in the Isotropic material (randomly).
     * 
     * @param ray_in The incoming light ray.
     * @param hit_record The information about where a light ray collided with a surface, and what the surface was.
     * @return ScatterRecord detailing how the light behaves after the collision 
     */
    ScatterRecord scatter(const Ray3D& ray_in, const HitRecord& hit_record) const override {
        constexpr bool success = true;  //always successful
        Vector3D random_direction = Vector3D::random_sphere_unit_vector();
        Ray3D refracted_ray {hit_record.point, random_direction, ray_in.time()};
        Color attenuation = albedo->value(hit_record.u, hit_record.v, hit_record.point);
        return ScatterRecord{success, refracted_ray, attenuation};  
    }

private:
    std::shared_ptr<Texture> albedo;
};


#endif