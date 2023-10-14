#ifndef RAY3D_H
#define RAY3D_H

#include "Vector3D.h"
#include "Color.h"

class Ray3D {
private:
    Vector3D m_origin {};
    Vector3D m_direction {};

    bool hit_sphere(const Vector3D& center, const float_type radius) const {
        Vector3D origin_to_center = origin() - center;
        float_type a = direction().dot(direction());
        float_type b = 2.0 * direction().dot(origin_to_center);
        float_type c = origin_to_center.dot(origin_to_center) - radius*radius;
        float_type discriminant = b*b - 4*a*c;
        return discriminant >= 0;   
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
        /*if (hit_sphere(Vector3D(0, 0, -1), .5)) {
            return Color(1, 0, 0);
        }*/

        Vector3D unit_direction = m_direction.unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }
};

#endif