#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include "Constants.h"
#include "Triple.h"
#include "Random.h"

/**
 * @brief Represents a 3-dimensional vector.
 * Commonly used for a point in space or direction of travel.
 * 
 */
class Vector3D : public Triple<Vector3D> {
public:
    using Triple<Vector3D>::Triple;  //inherit base class constructors

private:
    /**
     * @brief generates a random vector where each component had a value inbetweeen min and max.
     * 
     * @param min the minimum component value
     * @param max the maximum component value
     * @return Vector3D: a random vector
     */
    static Vector3D random(float_type min, float_type max) { 
        return Vector3D{random::random_float(min, max), random::random_float(min, max), random::random_float(min, max)};
    }

    /**
     * @brief Generates a random vector contained within the unit sphere.
     * 
     * @return Vector3D: a random vector
     */
    static Vector3D random_in_unit_sphere() {
        //rejection method
        while (true) {
            Vector3D vec = random(-1, 1);
            if (vec.length_squared() < 1) {
                return vec;
            }
        }
    }

public:
    /**
     * @brief Generates a random vector in the unit disk located in the plane z = 0.
     * 
     * @return Vector3D: a random vector
     */
    static Vector3D random_in_unit_disk() {
        //rejection method
        while (true) {
            Vector3D vec {random::random_float(-1, 1), random::random_float(-1, 1), 0};
            if (vec.length_squared() < 1) {
                return vec;
            }
        } 
    }
    
    /**
     * @brief Generates a random vector located on the unit sphere (with length 1).
     * 
     * @return Vector3D: a random vector
     */
    static Vector3D random_sphere_unit_vector() {
        return random_in_unit_sphere().unit_vector();
    }

    /**
     * @brief Generates a random unit vector located on the same side of the surface as the normal vector
     * (contained within the hemipshere).
     * 
     * @param normal The normal vector.
     * @return Vector3D: a random vector
     */
    static Vector3D random_unit_on_hemisphere(const Vector3D& normal) {
        Vector3D vec_on_unit_sphere = random_sphere_unit_vector();
        if (vec_on_unit_sphere.dot(normal) > 0) {   //same hemisphere as normal
            return vec_on_unit_sphere;
        }
        else {  //opposite side of norma;
            return -vec_on_unit_sphere;
        }
    }

    /**
     * @brief Returns the reflected Vector3D direction of this off of a surface, given the normal.
     * 
     * @param unit_normal the normal vector of unit length.
     * @return Vector3D: the resultant reflected direction.
     */
    Vector3D reflect(const Vector3D& unit_normal) const {
        Vector3D projection_onto_normal = unit_normal * this->dot(unit_normal);
        return *this - 2*projection_onto_normal;
    }

    /**
     * @brief Given a ratio of index of refractivities, the unit normal vector of the surface, 
     * and the reflectance of the other medium, returns the resulting direction of travel of the photon. 
     * Reflects if the collision results in total internal reflection OR 
     * the reflectance randomly generates a reflection.
     * Otherwise, refracts the light.
     * 
     * @param unit_normal the unit normal vector of the surface at the point of collision
     * @param eta_from_eta_to_ratio the ratio of the index of refractivities (the initial medium to the collising medium)
     * @param reflectance the reflectance of the colliding medium
     * @return Vector3D: the final direction of the photon.
     */
    Vector3D refract(const Vector3D& unit_normal, float_type eta_from_eta_to_ratio, float_type reflectance) const {
        //snell's law 
        float_type cos_theta = fmin(-(*this).dot(unit_normal), 1.0);
        float_type sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
        bool total_internal_reflection = eta_from_eta_to_ratio * sin_theta > 1.0;
        if (total_internal_reflection || reflectance > random::random_float(0, 1)) {
            //reflection
            return reflect(unit_normal);
        }
        else {
            //refraction
            Vector3D out_direction_perp = eta_from_eta_to_ratio * (*this + unit_normal * cos_theta);
            Vector3D out_direction_parallel = -std::sqrt(fabs(1-out_direction_perp.length_squared())) * unit_normal;
            return out_direction_perp + out_direction_parallel;
        }
    }

    /**
     * @brief Returns true if the vector is close to zero in all dimensions.
     * 
     * @return true if the vector is close to zero
     * @return false if not
     */
    bool near_zero() const {
        constexpr float_type epsilon = 1e-8;
        return (fabs(x()) < epsilon) && (fabs(y()) < epsilon) && (fabs(z()) < epsilon);
    }
};

#endif