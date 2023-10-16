#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"
#include "Sphere.h"

class Camera {
private:
    //camera info
    constexpr static float_type focal_length = 1.0;
    constexpr static Vector3D camera_center {0, 0, 0};
    //image info
    float_type aspect_ratio {};        //image width to height ratio
    int image_width, image_height {};  //in pixels
    //viewport info
    Vector3D pixel_delta_u {};         //length of single pixel along width, pointing right
    Vector3D pixel_delta_v {};         //length of single pixel along height, pointing down
    Vector3D pixel00_loc {};           //upper left pixel location
    
public:
    Camera(float_type aspect_ratio_, int image_width_) : aspect_ratio{aspect_ratio_}, image_width{image_width_} {
        image_height = std::max(static_cast<int>(image_width/aspect_ratio), 1);        
        //viewport dimensions
        constexpr float_type viewport_height = 2.0;
        float_type viewport_width = viewport_height * (static_cast<float_type>(image_width)/image_height);
        //initialize vectors going right along the horizontal and down along the vertical viewport edges
        Vector3D viewport_u {viewport_width, 0, 0};
        Vector3D viewport_v {0, -viewport_height, 0};
        //initilaize delta vectors (1 pixel long)
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        //determine the location of the upper left pixel
        Vector3D viewport_upper_left = camera_center - Vector3D{0, 0, focal_length} - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + .5*(pixel_delta_u + pixel_delta_v);
    }

    void render(Hittable& world) {
        std::cout << "P3\n" << image_width << ' ' << image_height << '\n' << Color::max_pixel_val << '\n';
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                Vector3D pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
                Ray3D pixel_ray {camera_center, pixel_center - camera_center};
                Color pixel_color = ray_color(pixel_ray, world);
                pixel_color.write_pixel(std::cout);
            }
        }
        std::clog << "\rDone.                     \n";
    }

private:
    Color ray_color(Ray3D& pixel_ray, Hittable& world) {
        auto [is_hit, hit_record] = world.hit(pixel_ray, Interval(0, infinity));
        if (is_hit) {
            return .5 * (Color(hit_record.unit_normal.x()+1, hit_record.unit_normal.y()+1, hit_record.unit_normal.z()+1));   
        }

        Vector3D unit_direction = pixel_ray.direction().unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }
};


#endif