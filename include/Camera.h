#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"
#include "Sphere.h"
#include "Random.h"

class Camera {
private:
    //recursion limit
    int max_depth = 10;
    //sampling info
    int samples_per_pixel = 10;     //provides antialiasing
    //camera info
    constexpr static float_type focal_length = 1.0;
    constexpr static Vector3D camera_center {0, 0, 0};
    //image info
    float_type aspect_ratio = 16.0/9.0;     //image width to height ratio
    int image_width = 640;                  //in pixels
    int image_height {};                    //in pixels
    //viewport info
    Vector3D pixel_delta_u {};         //length of single pixel along width, pointing right
    Vector3D pixel_delta_v {};         //length of single pixel along height, pointing down
    Vector3D pixel00_loc {};           //upper left pixel location
    
public:
    constexpr Camera(float_type aspect_ratio_, int image_width_, int samples_per_pixel_, int max_depth_) : 
        max_depth{max_depth_},
        samples_per_pixel{samples_per_pixel_},
        aspect_ratio{aspect_ratio_}, 
        image_width{image_width_}
    {
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

    void render(const Hittable& world) {
        std::cout << "P3\n" << image_width << ' ' << image_height << '\n' << Color::max_pixel_val << '\n';
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                ColorSum sum_color_samples {0, 0, 0};    
                for (int s = 0; s < samples_per_pixel; ++s) {
                    Color color_sample = ray_color(get_ray_sample(i, j), world);
                    sum_color_samples += color_sample;
                }
                Color pixel_color = sum_color_samples.scale(samples_per_pixel);
                pixel_color.write_pixel(std::cout);
            }
        }
        std::clog << "\rDone.                     \n";
    }

private:
    Vector3D get_random_point_in_pixel(const Vector3D& pixel_center) {
        Vector3D du = random::random_float(-.5, .5) * pixel_delta_u;
        Vector3D dv = random::random_float(-.5, .5) * pixel_delta_v;
        return pixel_center + du + dv;
    }

    Ray3D get_ray_sample(int i, int j) {
        Vector3D pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
        Vector3D random_point_in_pixel = get_random_point_in_pixel(pixel_center);
        return Ray3D{camera_center, random_point_in_pixel};
    }

    Color ray_color(const Ray3D& pixel_ray, const Hittable& world, int depth = 0) {
        //if we've exceeded the ray reflection limit, no more light is gathered
        if (depth >= max_depth) {
            return Color{0, 0, 0};
        }

        constexpr float_type min_travel_distance = 0.001;  //avoid shadow acne
        HitRecord hit_record = world.hit(pixel_ray, Interval(min_travel_distance, infinity));
        if (hit_record.is_hit) { 
            ScatterRecord scatter_record = hit_record.material_ptr->scatter(pixel_ray, hit_record);  
            if (scatter_record.success) {
                return scatter_record.attenuation * ray_color(scatter_record.ray_out, world, depth+1);
            }                                   
            return Color(0, 0, 0); //failed scatter
        }

        Vector3D unit_direction = pixel_ray.direction().unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }
};


#endif