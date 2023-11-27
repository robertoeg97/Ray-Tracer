#ifndef CONSTANTMEDIUM_H
#define CONSTANTMEDIUM_H

#include <memory>
#include <cmath>
#include "Constants.h"
#include "Hittable.h"
#include "Material.h"
#include "Texture.h"
#include "Interval.h"

/**
 * @brief Represents a convex volume that has constant density.
 * 
 */
class ConstantMedium : public Hittable {
public:
    /**
     * @brief Construct a new Constant Medium object
     * 
     * @param boundary_ The Hittable object representing the bounds of the volume. Everything inside has constant density.
     * @param density_ the density throughout the volume.
     * @param albedo_ The Texture shared_ptr that details the light attentuation.
     */
    ConstantMedium(std::shared_ptr<Hittable> boundary_, float_type density_, std::shared_ptr<Texture> albedo_) :
        boundary {boundary_},
        neg_inv_density {-1/density_},
        phase_function {std::make_shared<Isotropic>(albedo_)}
    {}

    /**
     * @brief Construct a new Constant Medium object
     * 
     * @param boundary_ The Hittable object representing the bounds of the volume. Everything inside has constant density.
     * @param density_ the density throughout the volume.
     * @param albedo_ The color that the light will be attenuated by.
     */
    ConstantMedium(std::shared_ptr<Hittable> boundary_, float_type density_, Color color_) :
        boundary {boundary_},
        neg_inv_density {-1/density_},
        phase_function {std::make_shared<Isotropic>(color_)}
    {}

    /**
     * @brief Details how a light ray hit an object, and the material that it collided with.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered a non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        //find first point that the ray collides with the boundary of the medium
        HitRecord entry_hit = boundary->hit(ray, Interval::universe);
        //early return if no hit at all
        if (!entry_hit.is_hit) {
            return entry_hit;
        }
        //find second point that the ray collides with the boundary of the medium
        constexpr float_type delta = .0001;
        HitRecord exit_hit = boundary->hit(ray, Interval{entry_hit.t+delta, infinity});
        //early return if there is only one hit
        if (!exit_hit.is_hit) {
            return exit_hit;
        }

        //clamp the entry and exit times to be inside of the t_interval
        if (entry_hit.t < t_interval.min)   entry_hit.t = t_interval.min;
        if (exit_hit.t > t_interval.max)    exit_hit.t = t_interval.max;
        //if there is no overlap between the entry-exit time interval and t_interval, there is no successful hit
        if (exit_hit.t < entry_hit.t) {
            return HitRecord{false};
        } 

        //the ray must start at the origin
        if (entry_hit.t < 0) {
            entry_hit.t = 0;
        }

        auto ray_length = ray.direction().length();
        auto distance_inside_boundary = (exit_hit.t - entry_hit.t) * ray_length;
        auto hit_distance = neg_inv_density * log(Random::random_float(0, 1));  //solution to diff eq lets us randomly generate travel distance like this
        //check if ray goes through medium without hit
        if (hit_distance > distance_inside_boundary) {
            return HitRecord{false};
        }

        //hit was successful
        constexpr bool success = true;
        HitRecord hit_record {success};
        hit_record.t = entry_hit.t + hit_distance / ray_length;
        hit_record.point = ray.at(hit_record.t);
        hit_record.unit_normal = Vector3D{1, 0, 0}; //arbitrary
        hit_record.front_face = true;               //arbitrary
        hit_record.material_ptr = phase_function;
        //note: we do not generate texture u, v coordinates
        return hit_record;
    }

    /**
     * @brief returns a bouuding box surrouding the Constant Medium volume.
     * 
     * @return AABB the bounding box that entirely contains the volume.
     */
    AABB bounding_box() const override {
        return boundary->bounding_box();
    }

private:
    std::shared_ptr<Hittable> boundary;
    float_type neg_inv_density;
    std::shared_ptr<Material> phase_function;
};

#endif