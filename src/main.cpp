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

    constexpr float_type aspect_ratio = 16.0/9.0;
    constexpr int image_width = 400;
    constexpr int samples_per_pixel = 100;
    constexpr int max_depth = 50;
    Camera camera (aspect_ratio, image_width, samples_per_pixel, max_depth);

    camera.render(world);

    return 0;
}