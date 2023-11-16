#ifndef SCENEINFO_H
#define SCENEINFO_H

#include <memory>
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Material.h"
#include "Random.h"
#include "Texture.h"
#include "MakeWorld.h"
#include "CameraParameters.h"

//Scene Tag
struct RandomSphereScene {};

/**
 * @brief Defines the Camera parameters for RandomSphereScene
 * 
 */
template <>
struct CameraParameters<RandomSphereScene> {
    constexpr static float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {13, 2, 3};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 0, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = .6;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 10;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 20;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit
};

/**
 * @brief Returns the world information for RandomSphereScene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<RandomSphereScene>() {
    HittableList world;

    //floor
    //auto material_ground = std::make_shared<Lambertian>(Color{.5, .5, .5});
    auto checker = std::make_shared<CheckerTexture>(.32, Color{.2, .3, .1}, Color{.9, .9, .9});
    auto material_ground = std::make_shared<Lambertian>(checker);
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

    return world;
}


//Scene Tag
struct TwoSpheresScene {};

/**
 * @brief Defines the Camera parameters for TwoSpheresScene
 * 
 */
template <>
struct CameraParameters<TwoSpheresScene> {
    constexpr static float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {13, 2, 3};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 0, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 20;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit
};

/**
 * @brief Returns the world information for TwoSpheresScene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<TwoSpheresScene>() {
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(.8, Color{.2, .3, .1}, Color{.9, .9, .9});

    world.add(std::make_shared<Sphere>(Vector3D{0, -10, 0}, 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Vector3D{0, 10, 0}, 10, std::make_shared<Lambertian>(checker)));

    return world;
}


//Scene Tag
struct EarthScene {};

/**
 * @brief Defines the Camera parameters for EarthScene
 * 
 */
template <>
struct CameraParameters<EarthScene> {
    constexpr static float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {0, 0, 12};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 0, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 20;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit
};

/**
 * @brief Returns the world information for EarthScene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<EarthScene>() {
    HittableList world;

    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Vector3D{0, 0, 0}, 2, earth_surface);

    world.add(globe);

    return world;
}


#endif