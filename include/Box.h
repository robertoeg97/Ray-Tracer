#ifndef BOX_H
#define BOX_H

#include <cmath>
#include <memory>
#include "Vector3D.h"
#include "HittableList.h"
#include "Quad.h"

/**
 * @brief Returns the 3D box (six sides) that contains the two opposite vertices a & b.

 * 
 * @param corner1 one vertex of the box
 * @param corner2 the opposite vertex of the box
 * @param material_ptr the material of the surface of the box
 * @return std::shared_ptr<HittableList> A HittableList containing the 6 surfaces of the box
 */
inline std::shared_ptr<HittableList> box(const Vector3D& corner1, const Vector3D& corner2, std::shared_ptr<Material> material_ptr) {
    auto sides = std::make_shared<HittableList>();

    //for convenience, find two opposing vertices s.t. one has strictly lower coordinates in all dimensions
    auto low = Vector3D{fmin(corner1.x(), corner2.x()), fmin(corner1.y(), corner2.y()), fmin(corner1.z(), corner2.z())};
    auto high = Vector3D{fmax(corner1.x(), corner2.x()), fmax(corner1.y(), corner2.y()), fmax(corner1.z(), corner2.z())};

    auto dx = Vector3D{high.x() - low.x(), 0, 0};
    auto dy = Vector3D{0, high.y() - low.y(), 0};
    auto dz = Vector3D{0, 0, high.z() - low.z()};

    sides->add(std::make_shared<Quad>(Vector3D{low.x(), low.y(), high.z()},  dx,  dy, material_ptr));   //front
    sides->add(std::make_shared<Quad>(Vector3D{high.x(), low.y(), high.z()}, -dz,  dy, material_ptr));  //right
    sides->add(std::make_shared<Quad>(Vector3D{high.x(), low.y(), low.z()}, -dx,  dy, material_ptr));   //back
    sides->add(std::make_shared<Quad>(Vector3D{low.x(), low.y(), low.z()},  dz,  dy, material_ptr));    //left
    sides->add(std::make_shared<Quad>(Vector3D{low.x(), high.y(), high.z()},  dx, -dz, material_ptr));  //top
    sides->add(std::make_shared<Quad>(Vector3D{low.x(), low.y(), low.z()},  dx,  dz, material_ptr));    //bottom

    return sides;
}

#endif