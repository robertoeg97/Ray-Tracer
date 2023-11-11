#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include "Constants.h"
#include "Hittable.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Interval.h"
#include "Material.h"

/**
 * @brief Represents a spherical object that can interact with light rays.
 * 
 */
class Sphere : public Hittable {
private:
    Vector3D m_center;  //at time 0
    float_type m_radius; 
    std::shared_ptr<Material> m_material_ptr;
    bool m_is_moving = false;
    Vector3D m_velocity {};


public:
    /**
     * @brief Construct a stationary Sphere object
     * 
     * @param center a Vector3D object representing the position of the sphere's center
     * @param radius a floating point value representing the radius of the sphere
     * @param material_ptr a pointer to the type of the sphere material
     */
    Sphere (const Vector3D& center, float_type radius, std::shared_ptr<Material> material_ptr) : 
        m_center{center}, m_radius{radius}, m_material_ptr{material_ptr} {}

    /**
     * @brief Construct a moving Sphere object
     * 
     * @param center_begin the position of the Sphere at time 0
     * @param center_end the position of the sphere at time 1
     * @param radius a floating point value representing the radius of the sphere 
     * @param material_ptr a pointer to the type of the sphere material
     */
    Sphere (const Vector3D& center_begin, const Vector3D& center_end, float_type radius, std::shared_ptr<Material> material_ptr) : 
        m_center{center_begin}, 
        m_radius{radius}, 
        m_material_ptr{material_ptr}, 
        m_is_moving {true}, 
        m_velocity {center_end - center_begin}
    {}

    /**
     * @brief Determines whether the ray intersects the surface of the sphere. 
     * In the case of two solutions, the lower value of t is returned.
     * 
     * @param ray The ray to check.
     * @param t_interval The minimum and maximum ray constants that results in an accepted solution.
     * @return HitResult that indicates whether ray intersects the sphere surface within ray_tmin and ray_tmax.
     * If successful hit, returns a valid HitRecord.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        Vector3D center = current_center(ray.time());
        //quadratic formula to find where the ray hits the sphere surface
        Vector3D center_to_origin = ray.origin() - center;
        //use formula: -b+-sqrt(b*b-4ac)/(2a) = -half_b+-sqrt(half_b*half_b-ac)/(a)
        float_type a = ray.direction().length_squared();
        float_type half_b = ray.direction().dot(center_to_origin);
        float_type c = center_to_origin.dot(center_to_origin) - m_radius*m_radius;
        float_type discriminant = half_b*half_b - a*c;
        if (discriminant < 0) {
            return HitRecord{};
        } 
        float_type sqrtd = std::sqrt(discriminant);
        
        //determine which ray intersection point falls within the desired bounds of t
        //prioritizes the lower value of t if both ray intersection points fall within tmin and tmax
        float_type root = (-half_b - sqrtd) / (a);  //subtracting discriminant gives lower root
        if (!t_interval.surrounds(root)) {
            root = (-half_b + sqrtd) / (a);     //adding discriminant gives higher root
            if (!t_interval.surrounds(root)) {
                return HitRecord{};
            }
        }

        //pass hit information through hit record
        HitRecord rec;
        rec.is_hit = true;
        rec.t = root;
        rec.point = ray.at(rec.t);
        Vector3D outward_unit_normal = (rec.point - m_center) / m_radius;
        rec.set_face_and_normal(ray, outward_unit_normal);
        rec.material_ptr = m_material_ptr;

        return rec;
    }

private:
    /**
     * @brief finds the center of the Sphere at a given point in time.
     * 
     * @param time the time 
     * @return Vector3D the position of the center of the Sphere at time
     */
    Vector3D current_center(float_type time) const {
        return (m_is_moving) ? m_center + m_velocity * time : m_center;
    }
};


#endif