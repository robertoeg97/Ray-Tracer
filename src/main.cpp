#include <iostream>
#include <algorithm>
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Camera.h"


int main() {
    //world info
    HittableList world;
    world.add(std::make_shared<Sphere>(Vector3D{0, 0, -1}, .5));
    world.add(std::make_shared<Sphere>(Vector3D{0, -100.5, -1}, 100));

    float_type aspect_ratio = 16.0/9.0;
    int image_width = 400;
    Camera camera (aspect_ratio, image_width);

    camera.render(world);
    
    return 0;
}