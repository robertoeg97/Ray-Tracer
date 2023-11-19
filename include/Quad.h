#ifndef QUAD_H
#define QUAD_H

#include <memory>
#include <cmath>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Material.h"
#include "Hittable.h"
#include "AABB.h"

/**
 * @brief Represents a quadrilateral (specifically parallelogram) object that can interact with light rays.
 * 
 */
class Quad : public Hittable {
public:
    /**
     * @brief Construct a new Quad object
     * 
     * @param Q_ A vertex of the Quad
     * @param u_ one of the sides of the Quad, that points from Q to an adjecent vertex
     * @param v_ the other side of the Quad, that points from Q to the other adjecent vertex
     * @param material_ptr_ the std::shared_ptr to the material that the Quad is made out of
     * @param min_thickness the mimimum thickness of the Quad's bounding box is all dimensions
     */
    Quad(const Vector3D& Q_, const Vector3D u_, const Vector3D v_, 
        std::shared_ptr<Material> material_ptr_, float_type min_thickness = .001) :
        Q{Q_}, 
        u{u_}, 
        v{v_}, 
        unit_normal{u_.cross(v_).unit_vector()},   
        D{unit_normal.dot(Q_)},             //Ax+By+Cz: (A,B,C) is unit normal, (x,y,z) is some point on quad (Q)
        w{u_.cross(v_) / (u_.cross(v_).dot(u_.cross(v_)))}, //p = P - Q = au + bv: a = w dot (p x v)    b = w dot (u x p)
        material_ptr{material_ptr_}, 
        bbox{AABB{Q_, Q_+u_+v_}.pad(min_thickness)}      //Q+u+v = opposing corner of Q
    {}

    /**
     * @brief Details how a light ray hit a Quadrilaterial, and the material that it collided with.
     * 
     * @param ray The incoming light ray.
     * @param t_interval The interval of time units that will result in a valid collision.
     * Everything on or outside of this range is considered a non-successful collision.
     * @return HitRecord The resulting collision data.
     */
    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        auto denominator = unit_normal.dot(ray.direction());

        //no hit if the ray is parallel to the plane
        constexpr float_type epsilon = 1e-8;
        if (fabs(denominator) < epsilon) {
            return HitRecord{false};
        }

        //no hit if the hit point parameter t is outside of t_interval
        auto t = (D - unit_normal.dot(ray.origin())) / denominator;
        if (!t_interval.contains(t)) {
            return HitRecord{false};
        }

        //determine if the hit point lies inside of the quadrilateral using the quadrilateral's planar coordinates
        auto intersection = ray.at(t);
        auto planar_hit_vector = intersection - Q;
        auto alpha = w.dot(planar_hit_vector.cross(v));
        auto beta = w.dot(u.cross(planar_hit_vector));
        if (!is_interior(alpha, beta)) {
            return false;
        }

        //successful hit: fill in hit vector information
        HitRecord hit_record {true};
        hit_record.point = intersection;
        hit_record.t  = t;
        hit_record.u = alpha;   //planar coordinate
        hit_record.v = beta;    //planar coordinate
        hit_record.set_face_and_normal(ray, unit_normal);
        hit_record.material_ptr = material_ptr;
        return hit_record;
    }

    /**
     * @brief returns a bouuding bconstox surrounding the Quad.
     * 
     * @return AABB a bounding box that entirely contains the Quad, that has some minimum thickness in all dimensions. 
     * Because the quadrilateral is not guaranteed to lie on a camera axis, 
     * it is not guaranteed to have a "thin" dimension (where thin is the minimum thickness).
     */
    AABB bounding_box() const override {
        return bbox;
    }

private:
    Vector3D Q;             //corner of the quadrilateral
    Vector3D u, v;          //two sides of the quadrilateral
    Vector3D unit_normal;
    float_type D;           //plane equation: Ax+By+Cz = D
    Vector3D w;             //useful for computing a, b in P = Q + au + bv 
    std::shared_ptr<Material> material_ptr;
    AABB bbox;

    /**
     * @brief Given the hit point in plane coordinates, return whether it is 
     * inside of the geometric primitive (parallelogram).
     * 
     * @param alpha the u coordinate
     * @param beta the v coordinate
     * @return true the hit point is inside the parallelogram
     * @return false otherwise
     */
    static bool is_interior(float_type alpha, float_type beta) {
        return alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1;
    }
};

#endif