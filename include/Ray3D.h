#ifndef RAY3D_H
#define RAY3D_H

#include "Constants.h"
#include "Vector3D.h"
#include "Color.h"

/**
 * @brief A class representing a ray of light, with both an origin and direction.
 * 
 */
class Ray3D {
private:
    Vector3D m_origin {};
    Vector3D m_direction {};

public:
    Ray3D() = default;

    /**
     * @brief Construct a new Ray 3D object
     * 
     * @param origin a Vector3D object representing the initial point in space of the light ray.
     * @param direction a Vector3D object representing the direction that the ray travels out of the origin.
     */
    Ray3D(const Vector3D& origin, const Vector3D& direction) : m_origin{origin}, m_direction{direction} {}

    Vector3D origin() const  {return m_origin;}
    Vector3D direction() const { return m_direction;}

    /**
     * @brief If the ray travels 1 direction vector every 1 time unit, 
     * give the resulting position of light after travelling for t time units. 
     * 
     * @param t the number of time units the light travels
     * @return Vector3D the resulting position of the light 
     */
    Vector3D at(float_type t) const {
        return {m_origin + m_direction*t};
    }
};

#endif