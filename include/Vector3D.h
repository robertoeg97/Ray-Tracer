#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "Triple.h"

class Vector3D : public Triple<Vector3D> {
public:
    using Triple<Vector3D>::Triple;  //inherit base class constructors
};

#endif