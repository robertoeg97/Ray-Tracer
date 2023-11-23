#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <memory>
#include "Vector3D.h"
#include "Ray3D.h"
#include "Interval.h"
#include "Hittable.h"

/**
 * @brief A class represeting a Hittable that is a translation of some other Hittable.
 * The hittable will act as though it has been offset by some displacement Vector3D.
 * 
 */
class Translate : public Hittable {
public:
    /**
     * @brief Construct a new Translate object
     * 
     * @param hittable_ The Hittable that is being translated.
     * @param offset_ The displacement Vector3D.
     */
    Translate(std::shared_ptr<Hittable> hittable_, const Vector3D& offset_) : 
        hittable {hittable_}, 
        offset {offset_},
        bbox {hittable->bounding_box() + offset}
    {}

    /**
     * @brief Details how a light ray hits the translated object, and the material that it collided with.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered a non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        //move the ray in the reverse direction of the offset
        Ray3D offset_ray {ray.origin() - offset, ray.direction(), ray.time()};

        //determine where an intersection occurs along the offset ray with the original hittable
        HitRecord hit_record = hittable->hit(offset_ray, t_interval);
        
        //move the intersection point forward by the offset
        //this is the intersection of the original ray if the hittable was translated by the offset
        hit_record.point += offset;

        return hit_record;
    }

    /**
     * @brief returns a bouuding box surrounding the Translate.
     * 
     * @return AABB the bounding box that entirely contains the Translate.
     */
    AABB bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<Hittable> hittable;
    Vector3D offset;
    AABB bbox;
};

#endif