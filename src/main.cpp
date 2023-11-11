#include <algorithm>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include "ProcessArguments.h"
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Camera.h"
#include "Material.h"
#include "Random.h"
#include "TimeFunction.h"

int main_(int argc, char *argv[]) {
    //process inputs to get filename
    std::string filename = process_arguments(argc, argv);

    //world info
    HittableList world;

    //floor
    auto material_ground = std::make_shared<Lambertian>(Color{.5, .5, .5});
    world.add(std::make_shared<Sphere>(Vector3D{0, -1000, 0}, 1000, material_ground));

    //random sphere generation
    for (int a = -11; a <= 11; ++a) {
        for (int b = -11; b <= 11; ++b) { 
            auto choose_material = Random::random_float(0, 1);
            Vector3D center {a + (float_type).9 * Random::random_float(0, 1), .2, b + (float_type).9 * Random::random_float(0, 1)};
            if ((center - Vector3D{4, .2, 0}).length() > .9) {
                std::shared_ptr<Material> sphere_material;
                if (choose_material < .8) {
                    //matte
                    Color albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    Vector3D center_end = center + Vector3D{0, Random::random_float(0, .5), 0}; //spheres will move
                    world.add(std::make_shared<Sphere>(center, center_end, .2, sphere_material));
                }
                else if (choose_material < .95) {
                    //metal
                    Color albedo = Color::random(.5, 1);
                    float_type fuzz_factor = Random::random_float(0, .5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz_factor);
                    world.add(std::make_shared<Sphere>(center, .2, sphere_material));
                }
                else {
                    //glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, .2, sphere_material));
                }   
            }
        }
    }

    //extra big spheres
    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vector3D(0, 1, 0), 1, material1));

    auto material2 = std::make_shared<Lambertian>(Color(.4, .2, .1));
    world.add(std::make_shared<Sphere>(Vector3D(-4, 1, 0), 1, material2));

    auto material3 = std::make_shared<Metal>(Color(.7, .6, .5), 0);
    world.add(std::make_shared<Sphere>(Vector3D(4, 1, 0), 1, material3));

    //camera construction
    Camera camera {};

    //render
    camera.render(world, filename);

    return 0;
}

int main(int argc, char *argv[]) {
    time_function(main_, argc, argv);

    return 0;
}