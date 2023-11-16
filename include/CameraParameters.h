#ifndef CAMERAPARAMETERS_H
#define CAMERAPARAMETERS_H

#include "Constants.h"
#include "Vector3D.h"

/**
 * @brief Calculate the image height in pixels
 * 
 * @param image_width the size of the image in pixels
 * @param aspect_ratio the target aspect ratio of the image. 
 * The exact aspect ratio will be determined by image_height/image_width
 * @return constexpr int: the height of the image in pixels
 */
constexpr inline int get_image_height (int image_width, float_type aspect_ratio) {
    return std::max(static_cast<int>(image_width/aspect_ratio), 1);   
} 

//THESE PARAMETER VALUES SHOULD NEVER BE USED
//ALL SCENES MUST HAVE SCENE SPECIFIC PARAMETERS EXPLICITELY DEFINED WITH A TEMPLATE SPECIALIZATION
template <typename Scene>
struct CameraParameters {
    constexpr static float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr static int image_width = 400;                                             //in pixels
    constexpr static int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr static Vector3D camera_center {13, 2, 3};                                 //the center of the camera
    constexpr static Vector3D camera_target {0, 0, 0};                                  //where the camera is pointing   
    constexpr static Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the camera lens is pointing
    constexpr static Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr static float_type defocus_angle = .6;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    //FOCUS DISTANCE MUST NOT BE 0
    constexpr static float_type focus_distance = 10;                                    //units in the direction of the lens that images will be in focus
    constexpr static float_type vfov = 20;                                              //degrees
    constexpr static int samples_per_pixel = 100;                                       //provides antialiasing
    constexpr static int max_depth = 50;                                                //recursion limit
};


#endif