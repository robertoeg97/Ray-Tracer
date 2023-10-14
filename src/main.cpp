#include <iostream>
#include <algorithm>
#include "Color.h"
#include "Vector3D.h"
#include "Ray3D.h"


int main() {
    //image info
    constexpr float_type aspect_ratio = 16.0/9.0;
    constexpr int image_width = 400;
    constexpr int image_height = std::max(static_cast<int>(image_width/aspect_ratio), 1);

    //camera info
    constexpr float_type focal_length = 1.0;
    constexpr Vector3D camera_center {0, 0, 0};

    //viewport dimensions
    constexpr float_type viewport_height = 2.0;
    constexpr float_type viewport_width = viewport_height * (static_cast<float_type>(image_width)/image_height);
    
    //initialize vectors going right along the horizontal and down along the vertical viewport edges
    constexpr Vector3D viewport_u {viewport_width, 0, 0};
    constexpr Vector3D viewport_v {0, -viewport_height, 0};

    //initilaize delta vectors (1 pixel long)
    constexpr Vector3D pixel_delta_u = viewport_u / image_width;
    constexpr Vector3D pixel_delta_v = viewport_v / image_height;

    //determine the location of the upper left pixel
    constexpr Vector3D viewport_upper_left = camera_center - Vector3D{0, 0, focal_length} - viewport_u/2 - viewport_v/2;
    constexpr Vector3D pixel0_loc = viewport_upper_left + .5*(pixel_delta_u + pixel_delta_v);

    //rendering
    std::cout << "P3\n" << image_width << ' ' << image_height << '\n' << Color::max_pixel_val << '\n';
    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Vector3D pixel_center = pixel0_loc + i*pixel_delta_u + j*pixel_delta_v;
            Ray3D pixel_ray {camera_center, pixel_center - camera_center};
            Sphere sphere {{0, 0, -1}, .5};
            Color pixel_color = color_pixel(pixel_ray, sphere);
            pixel_color.write_pixel(std::cout);
        }
    }
    std::clog << "\rDone.                     \n";

    return 0;
}