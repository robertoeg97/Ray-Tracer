#ifndef ROTATEY_H
#define ROTATEY_H

#include <memory>
#include <cmath>
#include "Constants.h"
#include "Interval.h"
#include "Hittable.h"
#include "AABB.h"

/**
 * A class represeting a Hittable that is a rotation about the y-axis of some other Hittable.
 * 
 */
class RotateY : public Hittable {
public:
    /**
     * @brief Construct a new Rotate Y object
     * 
     * @param hittable_ the Hittable that will be rotated
     * @param rotation_angle_ The angle of rotation, in degrees. The orientation will be s.t. a positive rotation 
     * will look counter-clockwise when viewed from the positive y-direction.
     */
    RotateY(std::shared_ptr<Hittable> hittable_, float_type rotation_angle_) : 
        hittable{hittable_},
        sin_theta{sin(degrees_to_radians(rotation_angle_))},
        cos_theta{cos(degrees_to_radians(rotation_angle_))},
        bbox{hittable->bounding_box()}  //temporary value
    {   
        //calculate AABB that entire contains the original AABB after rotation
        Vector3D min{infinity, infinity, infinity};
        Vector3D max{-infinity, -infinity, -infinity};

        //check the rotation of all 8 vertices of the original AABB to find the two new extremes
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    auto x = i*bbox.x.max + (1-i)*bbox.x.min;
                    auto y = j*bbox.y.max + (1-j)*bbox.y.min;
                    auto z = k*bbox.z.max + (1-k)*bbox.z.min;

                    Vector3D temp = object_to_world_space(Vector3D{x, y, z});

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], temp[c]);
                        max[c] = fmax(max[c], temp[c]);
                    }
                }
            }
        }

        bbox = AABB{min, max};
    }

    /**
     * @brief Details how a light ray hit the y-rotated object, and the material that it collided with.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered a non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        // ray is in world space
        //create a new ray in object space
        Vector3D new_origin = world_to_object_space(ray.origin());
        Vector3D new_direction = world_to_object_space(ray.direction());
        Ray3D new_ray {new_origin, new_direction, ray.time()};

        //try to find intersection in object space
        HitRecord hit_record = hittable->hit(new_ray, t_interval);
        //early return if no hit
        if (!hit_record.is_hit) {
            return hit_record;
        }

        //change the intersection point and normal from object space back to world space
        hit_record.point = object_to_world_space(hit_record.point);
        hit_record.unit_normal = object_to_world_space(hit_record.unit_normal);

        return hit_record;
    }

    /**
     * @brief returns a bouuding box surrounding the y-rotated Hittable.
     * 
     * @return AABB the bounding box that entirely contains the Hittable.
     */
    AABB bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<Hittable> hittable;
    float_type sin_theta;
    float_type cos_theta;
    AABB bbox;

    /**
     * @brief Converts a Vector3D representing a point in world-space,
     * to a Vector3D that is equivalent in the rotated (object) space.
     * 
     * @param vec the world-space point
     * @return Vector3D the object-space point
     */
    Vector3D world_to_object_space(const Vector3D& vec) const {
        return Vector3D{cos_theta*vec[0] - sin_theta*vec[2], vec[1], sin_theta*vec[0] + cos_theta*vec[2]};
    }

        /**
     * @brief Converts a Vector3D representing a point in rotated (object) space,
     * to a Vector3D that is equivalent in the world space.
     * 
     * @param vec the object-space point
     * @return Vector3D the world-space point
     */
    Vector3D object_to_world_space(const Vector3D& vec) const {
        return Vector3D{cos_theta*vec[0] + sin_theta*vec[2], vec[1], -sin_theta*vec[0] + cos_theta*vec[2]};
    }
};

#endif