#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include "Constants.h"
#include "Triple.h"
#include "Random.h"

class Vector3D : public Triple<Vector3D> {
public:
    using Triple<Vector3D>::Triple;  //inherit base class constructors

private:
    static Vector3D random(float_type min, float_type max) { 
        return Vector3D{random::random_float(min, max), random::random_float(min, max), random::random_float(min, max)};
    }

    static Vector3D random_in_unit_sphere() {
        while (true) {
            Vector3D vec = random(-1, 1);
            if (vec.length_squared() < 1) {
                return vec;
            }
        }
    }

public:
    static Vector3D random_sphere_unit_vector() {
        return random_in_unit_sphere().unit_vector();
    }

    static Vector3D random_unit_on_hemisphere(const Vector3D& normal) {
        Vector3D vec_on_unit_sphere = random_sphere_unit_vector();
        if (vec_on_unit_sphere.dot(normal) > 0) {   //same hemisphere as normal
            return vec_on_unit_sphere;
        }
        else {
            return -vec_on_unit_sphere;
        }
    }

    Vector3D reflect(const Vector3D& unit_normal) const {
        Vector3D projection_onto_normal = unit_normal * this->dot(unit_normal);
        return *this - 2*projection_onto_normal;
    }

    bool near_zero() const {
        //returns true if the vector is close to zero in all dimensions
        constexpr float_type epsilon = 1e-8;
        return (fabs(x()) < epsilon) && (fabs(y()) < epsilon) && (fabs(z()) < epsilon);
    }
};

#endif