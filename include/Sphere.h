#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vector3D.h"

class Sphere : public Hittable {
private:
    Vector3D m_center;
    float_type m_radius; 

public:
    Sphere (const Vector3D& center, float_type radius) : m_center{center}, m_radius{radius} {}

    /**
     * @brief determines whether the ray intersects a surface of the sphere, 
     * 
     * @param ray 
     * @param ray_tmin 
     * @param ray_tmax 
     * @param rec 
     * @return true 
     * @return false 
     */
    bool hit(const Ray3D& ray, float_type ray_tmin, float_type ray_tmax, HitRecord& rec) const override {
        //quadratic formula to find where the ray hits the sphere surface
        Vector3D center_to_origin = ray.origin() - m_center;
        //use formula: -b+-sqrt(b*b-4ac)/(2a) = -half_b+-sqrt(half_b*half_b-ac)/(a)
        float_type a = ray.direction().length_squared();
        float_type half_b = ray.direction().dot(center_to_origin);
        float_type c = center_to_origin.dot(center_to_origin) - m_radius*m_radius;
        float_type discriminant = half_b*half_b - a*c;
        if (discriminant < 0) {
            return false;
        } 
        float_type sqrtd = std::sqrt(discriminant);
        
        //determine which ray intersection point falls within the desired bounds of t
        //prioritizes the lower value of t if both ray intersection points fall within tmin and tmax
        float_type root = -(half_b - sqrtd) / (a);  //subtracting discriminant gives lower root
        if (root < ray_tmin || root > ray_tmax) {
            root = -(half_b + sqrtd) / (a);     //adding discriminant gives higher root
            if (root < ray_tmin || root > ray_tmax) {
                return false;
            }
        }

        //pass hit information through hit record
        rec.t = root;
        rec.point = ray.at(rec.t);
        rec.normal = (rec.point - m_center) / m_radius;

        return true;
    }
};


#endif