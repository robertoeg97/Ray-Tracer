#include <algorithm>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include "processArguments.h"
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Camera.h"
#include "Material.h"
#include "Random.h"


int main(int argc, char *argv[]) {
    //process inputs and open output file
    std::fstream output_file = process_arguments(argc, argv);

    //world info
    HittableList world;

    //floor
    auto material_ground = std::make_shared<Lambertian>(Color{.5, .5, .5});
    world.add(std::make_shared<Sphere>(Vector3D{0, -1000, 0}, 1000, material_ground));

    //random sphere generation
    /*for (int a = -11; a <= 11; ++a) {
        for (int b = -11; b <= 11; ++b) { 
            auto choose_material = random::random_float(0, 1);
            Vector3D center {a + (float_type).9 * random::random_float(0, 1), .2, b + (float_type).9 * random::random_float(0, 1)};
            if ((center - Vector3D{4, .2, 0}).length() > .9) {
                std::shared_ptr<Material> sphere_material;
                if (choose_material < .8) {
                    //matte
                    Color albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                }
                else if (choose_material < .95) {
                    //metal
                    Color albedo = Color::random(.5, 1);
                    float_type fuzz_factor = random::random_float(0, .5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz_factor);
                }
                else {
                    //glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                }
                world.add(std::make_shared<Sphere>(center, .2, sphere_material));
            }
        }
    }*/

    //extra big spheres
    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vector3D(0, 1, 0), 1, material1));

    auto material2 = std::make_shared<Lambertian>(Color(.4, .2, .1));
    world.add(std::make_shared<Sphere>(Vector3D(-4, 1, 0), 1, material2));

    auto material3 = std::make_shared<Metal>(Color(.7, .6, .5), 0);
    world.add(std::make_shared<Sphere>(Vector3D(4, 1, 0), 1, material3));

    //camera info
    constexpr float_type aspect_ratio = 16.0/9.0;
    constexpr int image_width = 1200;
    constexpr Vector3D camera_center {13, 2, 3};
    constexpr Vector3D camera_target {0, 0, 0};
    constexpr Vector3D camera_lens_direction = camera_target - camera_center;
    constexpr Vector3D camera_up_direction {0, 1, 0};
    constexpr float_type defocus_angle = .6;
    constexpr float_type focus_distance = 10;
    constexpr float_type vfov = 20;
    constexpr int samples_per_pixel = 10;
    constexpr int max_depth = 50;
    const Camera camera (aspect_ratio, image_width, 
                        camera_center, camera_lens_direction, camera_up_direction, 
                        defocus_angle, focus_distance,
                        vfov, samples_per_pixel, max_depth);

    //render
    camera.render(world, output_file);

    //close file
    close_file(output_file);

    return 0;
}