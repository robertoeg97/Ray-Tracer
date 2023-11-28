#ifndef SCENEINFO_H
#define SCENEINFO_H

#include <memory>
#include "Constants.h"
#include "Vector3D.h"
#include "HittableList.h"
#include "Sphere.h" 
#include "Quad.h"
#include "Material.h"
#include "Random.h"
#include "Texture.h"
#include "MakeWorld.h"
#include "CameraParameters.h"
#include "Box.h"
#include "RotateY.h"
#include "Translate.h"
#include "ConstantMedium.h"
#include "BVH.h"

//Scene Tag
struct RandomSphereScene {};

/**
 * @brief Defines the Camera parameters for the scene
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

    constexpr static Color background{0.70, 0.80, 1.00};
};

/**
 * @brief Returns the world information for the scene
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
 * @brief Defines the Camera parameters for the scene
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

    constexpr static Color background{0.70, 0.80, 1.00};
};

/**
 * @brief Returns the world information for the scene
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
 * @brief Defines the Camera parameters for the scene
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

    constexpr static Color background{0.70, 0.80, 1.00};
};

/**
 * @brief Returns the world information for the scene
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



//Scene Tag
struct TwoPerlinSpheresScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<TwoPerlinSpheresScene> {
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

    constexpr static Color background{0.70, 0.80, 1.00};
};

/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<TwoPerlinSpheresScene>() {
    HittableList world;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Vector3D{0, -1000, 0}, 1000, std::make_shared<Lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(Vector3D{0, 2, 0}, 2, std::make_shared<Lambertian>(perlin_texture)));

    return world;
}



//Scene Tag
struct QuadrilateralsScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<QuadrilateralsScene> {
    constexpr static float_type aspect_ratio = 1;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {0, 0, 9};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 0, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 80;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit

    constexpr static Color background{0.70, 0.80, 1.00};
};

/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<QuadrilateralsScene>() {
    HittableList world;

    // Materials
    auto left_red     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quadrilaterals
    world.add(std::make_shared<Quad>(Vector3D(-3,-2, 5), Vector3D(0, 0,-4), Vector3D(0, 4, 0), left_red));
    world.add(std::make_shared<Quad>(Vector3D(-2,-2, 0), Vector3D(4, 0, 0), Vector3D(0, 4, 0), back_green));
    world.add(std::make_shared<Quad>(Vector3D( 3,-2, 1), Vector3D(0, 0, 4), Vector3D(0, 4, 0), right_blue));
    world.add(std::make_shared<Quad>(Vector3D(-2, 3, 1), Vector3D(4, 0, 0), Vector3D(0, 0, 4), upper_orange));
    world.add(std::make_shared<Quad>(Vector3D(-2,-3, 5), Vector3D(4, 0, 0), Vector3D(0, 0,-4), lower_teal));

    return world;
}



//Scene Tag
struct SimpleLightScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<SimpleLightScene> {
    constexpr static float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {26, 3, 6};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 2, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 20;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit

    constexpr static Color background{0, 0, 0};
};


/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<SimpleLightScene>() {
    HittableList world;

    auto noise = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Vector3D{0, -1000, 0}, 1000, std::make_shared<Lambertian>(noise)));
    world.add(std::make_shared<Sphere>(Vector3D{0, 2, 0}, 2, std::make_shared<Lambertian>(noise)));

    auto white_light = std::make_shared<DiffuseLights>(Color{4, 4, 4});   //color magnitude > 1 means color can more easily light up surroundings
    world.add(std::make_shared<Quad>(Vector3D{3, 1, -2}, Vector3D{2, 0, 0}, Vector3D{0, 2, 0}, white_light));
    world.add(std::make_shared<Sphere>(Vector3D{0,7,0}, 2, white_light));

    return world;
}



//Scene Tag
struct CornellBoxScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<CornellBoxScene> {
    constexpr static float_type aspect_ratio = 1;                                //desired image width to height ratio
    constexpr static int image_width = 600;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {278, 278, -800};                                 //the center of the camera
    constexpr static Vector3D camera_target {278, 278, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 40;                                              //degrees
    constexpr static int samples_per_pixel = 200;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit

    constexpr static Color background{0, 0, 0};
};


/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<CornellBoxScene>() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    auto light = std::make_shared<DiffuseLights>(Color{15, 15, 15});    //brightness = 15

    //Cornell box
    world.add(std::make_shared<Quad>(Vector3D{555, 0, 0}, Vector3D{0, 555, 0}, Vector3D{0, 0, 555}, green));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 0}, Vector3D{0, 555, 0}, Vector3D{0, 0, 555}, red));
    world.add(std::make_shared<Quad>(Vector3D{343, 554, 332}, Vector3D{-130, 0, 0}, Vector3D{0, 0, -105}, light));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 0}, Vector3D{555, 0, 0}, Vector3D{0, 0, 555}, white));
    world.add(std::make_shared<Quad>(Vector3D{555, 555, 555}, Vector3D{-555, 0, 0}, Vector3D{0, 0, -555}, white));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 555}, Vector3D{555, 0, 0}, Vector3D{0, 555, 0}, white));

    //objects inside
    std::shared_ptr<Hittable> box1 = box(Vector3D{0, 0, 0}, Vector3D{165, 330, 165}, white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3D{265, 0, 295});
    world.add(box1);

    std::shared_ptr<Hittable> box2 = box(Vector3D{0, 0, 0}, Vector3D{165, 165, 165}, white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3D{130, 0, 65});
    world.add(box2);

    return world;
}



//Scene Tag
struct CornellSmokeScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<CornellSmokeScene> {
    constexpr static float_type aspect_ratio = 1;                                //desired image width to height ratio
    constexpr static int image_width = 600;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {278, 278, -800};                                 //the center of the camera
    constexpr static Vector3D camera_target {278, 278, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 40;                                              //degrees
    constexpr static int samples_per_pixel = 200;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit

    constexpr static Color background{0, 0, 0};
};


/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<CornellSmokeScene>() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color{.65, .05, .05});
    auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    auto green = std::make_shared<Lambertian>(Color{.12, .45, .15});
    auto light = std::make_shared<DiffuseLights>(Color{15, 15, 15});    //brightness = 15

    //Cornell box
    world.add(std::make_shared<Quad>(Vector3D{555, 0, 0}, Vector3D{0, 555, 0}, Vector3D{0, 0, 555}, green));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 0}, Vector3D{0, 555, 0}, Vector3D{0, 0, 555}, red));
    world.add(std::make_shared<Quad>(Vector3D{343, 554, 332}, Vector3D{-130, 0, 0}, Vector3D{0, 0, -105}, light));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 0}, Vector3D{555, 0, 0}, Vector3D{0, 0, 555}, white));
    world.add(std::make_shared<Quad>(Vector3D{555, 555, 555}, Vector3D{-555, 0, 0}, Vector3D{0, 0, -555}, white));
    world.add(std::make_shared<Quad>(Vector3D{0, 0, 555}, Vector3D{555, 0, 0}, Vector3D{0, 555, 0}, white));

    //objects inside
    std::shared_ptr<Hittable> box1 = box(Vector3D{0, 0, 0}, Vector3D{165, 330, 165}, white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3D{265, 0, 295});
    world.add(std::make_shared<ConstantMedium>(box1, .01, Color{0, 0, 0}));

    std::shared_ptr<Hittable> box2 = box(Vector3D{0, 0, 0}, Vector3D{165, 165, 165}, white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3D{130, 0, 65});
    world.add(std::make_shared<ConstantMedium>(box2, .01, Color{1, 1, 1}));

    return world;
}



//Scene Tag
struct ComplexCornellScene {};

/**
 * @brief Defines the Camera parameters for the scene
 * 
 */
template <>
struct CameraParameters<ComplexCornellScene> {
    constexpr static float_type aspect_ratio = 1;                                //desired image width to height ratio
    constexpr static int image_width = 800;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {478, 278, -600};                                 //the center of the camera
    constexpr static Vector3D camera_target {278, 278, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = 0;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr static float_type focus_distance = 1;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 40;                                              //degrees
    constexpr static int samples_per_pixel = 10000;                                       //provides antialiasing
    constexpr static int max_depth = 40;                                                //recursion limit

    constexpr static Color background{0, 0, 0};
};


/**
 * @brief Returns the world information for the scene
 * 
 * @return HittableList the world information
 */
template <>
inline HittableList make_world<ComplexCornellScene>() {
    HittableList world;

    //boxes that make different ground elevations
    HittableList floor_boxes;
    auto ground = std::make_shared<Lambertian>(Color{0.48, 0.83, 0.53});
    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            float_type w = 100.0;
            float_type x0 = -1000.0 + i*w;
            float_type z0 = -1000.0 + j*w;
            float_type y0 = 0.0;
            float_type x1 = x0 + w;
            float_type y1 = Random::random_float(1,101);
            float_type z1 = z0 + w;
            floor_boxes.add(box(Vector3D{x0,y0,z0}, Vector3D{x1,y1,z1}, ground));
        }
    }
    world.add(std::make_shared<BVH_node>(floor_boxes));

    //light
    auto light = std::make_shared<DiffuseLights>(Color{7, 7, 7});
    world.add(std::make_shared<Quad>(Vector3D{123,554,147}, Vector3D{300,0,0}, Vector3D{0,0,265}, light));

    //moving sphere
    auto center1 = Vector3D{400, 400, 200};
    auto center2 = center1 + Vector3D{30,0,0};
    auto sphere_material = std::make_shared<Lambertian>(Color{0.7, 0.3, 0.1});
    world.add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

    //more spheres
    world.add(std::make_shared<Sphere>(Vector3D{260, 150, 45}, 50, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(Vector3D{0, 150, 145}, 50, std::make_shared<Metal>(Color{0.8, 0.8, 0.9}, 1.0)));

    //subsurface medium
    auto boundary = std::make_shared<Sphere>(Vector3D{360,150,145}, 70, std::make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color{0.2, 0.4, 0.9}));

    //thin mist over everything
    boundary = std::make_shared<Sphere>(Vector3D{0,0,0}, 5000, std::make_shared<Dielectric>(1.5));
    world.add(std::make_shared<ConstantMedium>(boundary, .0001, Color{1,1,1}));

    //earth map
    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    world.add(std::make_shared<Sphere>(Vector3D{400,200,400}, 100, emat));

    //perlin noise sphere
    auto pertext = std::make_shared<NoiseTexture>(0.1);
    world.add(std::make_shared<Sphere>(Vector3D{220,280,300}, 80, std::make_shared<Lambertian>(pertext)));

    //cube made of spheres
    HittableList sphere_cube;
    auto white = std::make_shared<Lambertian>(Color{.73, .73, .73});
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        sphere_cube.add(make_shared<Sphere>(Vector3D::random(0,165), 10, white));
    }
    world.add(std::make_shared<Translate>(
        std::make_shared<RotateY>(
            std::make_shared<BVH_node>(sphere_cube), 15),
            Vector3D{-100,270,395}
        )
    );

    return world;
}


#endif