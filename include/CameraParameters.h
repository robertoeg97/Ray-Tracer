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

namespace CameraParams {
    constexpr inline float_type aspect_ratio = 16.0/9.0;                                //desired image width to height ratio
    constexpr inline int image_width = 1200;                                            //in pixels
    constexpr inline int image_height = get_image_height(image_width, aspect_ratio);    //in pixels
    constexpr inline Vector3D camera_center {13, 2, 3};                                 //the center of the camera
    constexpr inline Vector3D camera_target {0, 0, 0};
    constexpr inline Vector3D camera_lens_direction = camera_target - camera_center;    //the direction that the caemra lens is pointing
    constexpr inline Vector3D camera_up_direction {0, 1, 0};                            //the direction that is "up" from the camera's perspective
    constexpr inline float_type defocus_angle = .6;                                     //the degree of the cone tip with apex at the focus center 
                                                                                        //and base at the lens
    constexpr inline float_type focus_distance = 10;                                    //units in the direction of the lens that images will be in focus
    constexpr inline float_type vfov = 20;                                              //degrees
    constexpr inline int samples_per_pixel = 500;                                       //provides antialiasing
    constexpr inline int max_depth = 50;                                                //recursion limit
}


#endif