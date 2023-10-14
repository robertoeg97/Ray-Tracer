#ifndef RAY3D_H
#define RAY3D_H

#include "Vector3D.h"
#include "Color.h"
#include "Hittable.h"

class Ray3D {
private:
    Vector3D m_origin {};
    Vector3D m_direction {};

public:
    Ray3D() = default;
    Ray3D(const Vector3D& origin, const Vector3D& direction) : m_origin{origin}, m_direction{direction} {}

    Vector3D origin() const  {return m_origin;}
    Vector3D direction() const { return m_direction;}

    Vector3D at(float_type t) const {
        return {m_origin + m_direction*t};
    }

    /*Color color() const {
        Sphere sphere {{0, 0, -1}, .5};
        HitRecord rec;
        if (sphere.hit(*this, 0, INT_MAX, rec)) {
            return .5 * (Color(rec.unit_normal.x()+1, rec.unit_normal.y()+1, rec.unit_normal.z()+1));   
        }

        Vector3D unit_direction = m_direction.unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }*/
};

#endif