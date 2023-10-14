#ifndef RAY3D_H
#define RAY3D_H

#include "Vector3D.h"
#include "Color.h"

class Ray3D {
private:
    Vector3D m_origin {};
    Vector3D m_direction {};

    struct Sphere {
        Vector3D center;
        float_type radius;
    };

    float_type hit_sphere(const Sphere& sphere) const {
        Vector3D center_to_origin = origin() - sphere.center;
        float_type a = direction().dot(direction());
        float_type b = 2.0 * direction().dot(center_to_origin);
        float_type c = center_to_origin.dot(center_to_origin) - sphere.radius*sphere.radius;
        float_type discriminant = b*b - 4*a*c;
        if (discriminant < 0) {
            return -1.0;
        } 
        //subtracting discriminant gives us the constant to find the contact point closest to us
        return (-b - std::sqrt(discriminant)) / (2.0*a);    
    }

public:
    Ray3D() = default;
    Ray3D(const Vector3D& origin, const Vector3D& direction) : m_origin{origin}, m_direction{direction} {}

    Vector3D origin() const  {return m_origin;}
    Vector3D direction() const { return m_direction;}

    Vector3D at(float_type c) const {
        return {m_origin + m_direction*c};
    }

    Color color() const {
        Sphere sphere {{0, 0, -1}, .5};
        float_type c = hit_sphere(sphere);
        if (c > 0) {
            Vector3D normal = at(c) - sphere.center;
            Vector3D unit_normal = normal.unit_vector();
            //0 unit normal component gives a value of 0 for the corresponding RGB, and 1 unit normal gives max val
            return .5 * (Color(unit_normal.x()+1, unit_normal.y()+1, unit_normal.z()+1));   
        }

        Vector3D unit_direction = m_direction.unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }
};

#endif