#include <iostream>
#include <algorithm>
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Camera.h"
#include "Material.h"


int main() {
    //world info
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Color{.8, .8, 0});
    auto material_center = std::make_shared<Lambertian>(Color{.1, .2, .5});
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(Color{.8, .6, .2}, 0);

    world.add(std::make_shared<Sphere>(Vector3D{0, -100.5, -1}, 100, material_ground));
    world.add(std::make_shared<Sphere>(Vector3D{0, 0, -1}, .5, material_center));
    world.add(std::make_shared<Sphere>(Vector3D{-1, 0, -1}, .5, material_left));
    world.add(std::make_shared<Sphere>(Vector3D{1, 0, -1}, .5, material_right));

    constexpr float_type aspect_ratio = 16.0/9.0;
    constexpr int image_width = 400;
    constexpr int samples_per_pixel = 100;
    constexpr int max_depth = 50;
    Camera camera (aspect_ratio, image_width, samples_per_pixel, max_depth);

    camera.render(world);

    return 0;
}