#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Interval.h"

class Material;

/**
 * @brief Data about how a light ray collided with a surface, and what the surface was.
 * 
 */
struct HitRecord {  
    bool is_hit = false;                            //true if a collision occured
    Vector3D point {};                              //the point of collision
    Vector3D unit_normal {};                        //the unit normal of the surface at the point of collision
    float_type t {};                                //ray.origin + ray.direction*t is the point of collision
    bool front_face {};                             //true if the collision occured on the same side as the normal
    std::shared_ptr<Material> material_ptr {};      //a shared_ptr to the material that is collided with

    HitRecord() = default;

    /**
     * @brief Sets the unit_normal of the object to always point towards the intersecting ray. 
     * Sets front_face to true if the ray intersected the object from the outside, and false from the inside.
     * 
     * @param ray The ray that is hitting the object.
     * @param outward_unit_normal The outward facing unit normal vector of the object at the point of collision.
     */
    void set_face_and_normal(const Ray3D& ray, const Vector3D& outward_unit_normal) {
        front_face = ray.direction().dot(outward_unit_normal) < 0;
        unit_normal = front_face ? outward_unit_normal : -outward_unit_normal;
    }
};


/**
 * @brief An abstract class detailing some object that ligth can collide with.
 * 
 */
class Hittable {
public:
    virtual ~Hittable() = default;

    /**
     * @brief Details how a light ray hit an object, and the material that it collided with.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered na non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    virtual HitRecord hit(const Ray3D& ray, const Interval& t_interval) const = 0;
};

#endif