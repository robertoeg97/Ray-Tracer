#ifndef RAY3D_H
#define RAY3D_H

#include "Vector3D.h"
#include "Color.h"

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
};

#endif