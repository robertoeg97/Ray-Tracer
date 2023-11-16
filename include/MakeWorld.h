#ifndef MAKEWORLD_H
#define MAKEWORLD_H

#include "HittableList.h"

//THIS FUNCTION SHOULD NEVER BE USED
//ALL SCENES MUST HAVE A SCENE SPECIFIC WORLD EXPLICITELY DEFINED WITH A TEMPLATE SPECIALIZATION
template <typename SceneTag> 
inline HittableList make_world() {
    return HittableList{};
}

#endif