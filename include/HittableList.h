#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
#include "Constants.h"
#include "Hittable.h"
#include "Ray3D.h"
#include "Interval.h"
#include "AABB.h"

/**
 * @brief A class containing multiple possible Hittable objects.
 * 
 */
class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> hittables {};

    /**
     * @brief Construct an empty Hittable List object
     * 
     */
    HittableList() = default;
    
    /**
     * @brief Construct a new HittableList object with one Hittable
     * 
     * @param hittable a shared_ptr to the single hittable object
     */
    HittableList(std::shared_ptr<Hittable> hittable) {
        add(hittable);
    }

    /**
     * @brief Empties out the HittablieList
     * 
     */
    void clear() {hittables.clear();}

    /**
     * @brief Add one object to the HittableList
     * 
     * @param hittable A shared_ptr to the Hittable object.
     */
    void add(std::shared_ptr<Hittable> hittable) {
        hittables.push_back(hittable);
        bbox = AABB{bbox, hittable->bounding_box()};
    }

    /**
     * @brief Details how a light ray interacts with all objects within the list.
     * If light would collide with multiple objects, it collides with the one closest to the light's origin.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered na non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        HitRecord hit_record {};
        float_type t_closest = t_interval.max;
        for (const auto& hittable_ptr : hittables) {
            HitRecord cur_record = hittable_ptr->hit(ray, Interval{t_interval.min, t_closest});
            if (cur_record.is_hit) {
                t_closest = cur_record.t;
                hit_record = cur_record;
            }
        }

        return hit_record;
    }

    /**
     * @brief returns the Axis Aligned Bounding Box that contains all hittables in the list.
     * 
     * @return AABB the bounding box
     */
    AABB bounding_box() const override {
        return bbox;
    }

private:
    AABB bbox;
};


#endif