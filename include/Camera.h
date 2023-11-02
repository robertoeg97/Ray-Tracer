#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>
#include <cmath>
#include "Constants.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Hittable.h"
#include "Color.h"
#include "Sphere.h"
#include "Material.h"
#include "Random.h"

/**
 * @brief A class representing a camera that can capture light from the world.
 * 
 */
class Camera {
private:
    //recursion limit
    int max_depth = 10;
    //sampling info
    int samples_per_pixel = 10;     //provides antialiasing
    //camera info
    Vector3D camera_center {0, 0, 0};               //the center of the camera
    Vector3D camera_lens_direction {0, 0, -1};      //the direction that the caemra elns is pointing
    Vector3D camera_up_direction {0, 1, 0};         //the direction that is "up" from the camer'as perspective
    float_type defocus_angle = 0;                   //the degree of the cone tip with apex at the focus center 
                                                    //and base at the lens
    float_type focus_distance = 10;                 //units in the direction of the lens that images will be in focus
    float_type vfov = 90.0;                         //degrees
    //image info
    float_type aspect_ratio = 16.0/9.0;     //image width to height ratio
    int image_width = 640;                  //in pixels
    int image_height {};                    //in pixels
    //viewport info
    Vector3D pixel_delta_u {};         //length of single pixel along width, pointing right
    Vector3D pixel_delta_v {};         //length of single pixel along height, pointing down
    Vector3D pixel00_loc {};           //upper left pixel location
    //camera frame basis vectors
    Vector3D w{}, u{}, v{};
    //defocus disk vectors
    Vector3D defocus_disk_u {}; //defocus disk horizontal radius
    Vector3D defocus_disk_v {}; //defocus disk vertical radius
    
public:
    /**
     * @brief Construct a new Camera object
     * 
     * @param aspect_ratio_ The target aspect ratio of the images the camera produces.
     * @param image_width_ The image width of the camera's images (in pixels).
     * @param camera_center_ The location of the center of the camera lens.
     * @param camera_lens_direction_ The direction that the camera lens is pointing.
     * @param camera_up_direction_ The direction that represents "up" from the camera's perspective.
     * @param defocus_angle_ The angle of the cone shaped by the lens and the focus point of the camera.
     * @param focus_distance_ The ditance of the point that will be in perfect focus.
     * @param vfov_ The vertical field of view of the image (in degrees).
     * @param samples_per_pixel_ The number of samples used to generate each pixel.
     * @param max_depth_ The max number of reflections/refractions that the camera will trace for any light ray.
     */
    Camera( float_type aspect_ratio_, int image_width_, 
            Vector3D camera_center_, Vector3D camera_lens_direction_, Vector3D camera_up_direction_,
            float_type defocus_angle_, float_type focus_distance_,
            float_type vfov_, int samples_per_pixel_, int max_depth_) : 
        max_depth{max_depth_},
        samples_per_pixel{samples_per_pixel_},
        camera_center{camera_center_},
        camera_lens_direction{camera_lens_direction_},
        camera_up_direction{camera_up_direction_},
        defocus_angle{defocus_angle_},
        focus_distance{focus_distance_},
        vfov{vfov_},
        aspect_ratio{aspect_ratio_},   
        image_width{image_width_}
    {
        image_height = std::max(static_cast<int>(image_width/aspect_ratio), 1);        
        //viewport dimensions
        float_type theta = degrees_to_radians(vfov);
        float_type h = focus_distance * std::tan(theta/2);
        float_type viewport_height = 2 * h;
        float_type viewport_width = viewport_height * (static_cast<float_type>(image_width)/image_height);
        //calculate the w,u,v basis vectors for the camera coordinate frame
        w = -camera_lens_direction.unit_vector();       //negative direction of camera lens
        u = camera_up_direction.cross(w).unit_vector(); //vector pointing right of camera
        v = w.cross(u);                                 //vector pointing up
        //calculate vectors going right along the horizontal and down along the vertical viewport edges
        Vector3D viewport_u = viewport_width * u;
        Vector3D viewport_v = viewport_height * -v;
        //initilaize delta vectors (1 pixel long)
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        //determine the location of the upper left pixel
        Vector3D viewport_upper_left = camera_center - (focus_distance*w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + .5*(pixel_delta_u + pixel_delta_v);
        //calculate the camera defocus distance basis vectors
        float_type defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    /**
     * @brief Writes an image of world that the camera captures in PPM format to file_out.
     * 
     * @param world the world that the camera is observing
     */
    void render(const Hittable& world, std::fstream& file_out) const {
        file_out << "P3\n" << image_width << ' ' << image_height << '\n' << Color::max_pixel_val << '\n';
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                ColorSum sum_color_samples {0, 0, 0};    
                for (int s = 0; s < samples_per_pixel; ++s) {
                    Color color_sample = ray_color(get_ray_sample(i, j), world);
                    sum_color_samples += color_sample;
                }
                Color pixel_color = sum_color_samples.scale(samples_per_pixel);
                pixel_color.write_pixel(file_out);
            }
        }
        std::clog << "\rDone.                     \n";
    }

private:
    /**
     * @brief Get a random point in the bounds of a pixel.
     * 
     * @param pixel_center The location of the center of the desired pixel sample.
     * @return Vector3D a random point in the pixel
     */
    Vector3D get_random_point_in_pixel(const Vector3D& pixel_center) const {
        Vector3D du = random::random_float(-.5, .5) * pixel_delta_u;
        Vector3D dv = random::random_float(-.5, .5) * pixel_delta_v;
        return pixel_center + du + dv;
    }

    /**
     * @brief Returns a random point in the defocus disk (lens)
     * 
     * @return Vector3D a ranodm point on the lens
     */
    Vector3D defocus_disk_sample() const {
        //returns random point in the defocus disk
        Vector3D random_unit_disk_point = Vector3D::random_in_unit_disk();
        return  camera_center + 
                (defocus_disk_u * random_unit_disk_point.x()) + 
                (defocus_disk_v * random_unit_disk_point.y());
    }

    /**
     * @brief Get a random ray that from the lens to the pixel.
     * 
     * @param i the horizontal index of the pixel
     * @param j the vertical index of the pixel
     * @return Ray3D the ranodm ray
     */
    Ray3D get_ray_sample(int i, int j) const {
        Vector3D pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
        Vector3D random_point_in_pixel = get_random_point_in_pixel(pixel_center);           //antialiasing
        Vector3D ray_origin = (defocus_angle > 0) ? defocus_disk_sample() : camera_center;  //defocus
        return Ray3D{ray_origin, random_point_in_pixel - ray_origin};
    }

    /**
     * @brief Traces a ray of light from the camera through the world to get the resulting color.
     * 
     * @param pixel_ray The light ray generated by the camera.
     * @param world The world that the camer is observing
     * @param depth The limit to the number of object interactions that a single ray can undergo.
     * @return Color: the resulting color captured by the light ray.
     */
    Color ray_color(const Ray3D& pixel_ray, const Hittable& world, int depth = 0) const {
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

        //background coloring logic
        Vector3D unit_direction = pixel_ray.direction().unit_vector();
        float_type a = .5 * (unit_direction.y() + 1.0);
        return (1.0-a)*Color(1, 1, 1) + a*Color(.5, .7, 1);
    }
};


#endif