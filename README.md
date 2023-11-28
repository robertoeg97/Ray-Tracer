# Ray-Tracer

This project implements a ray tracer following the Ray Tracing in One Weekend book series: https://raytracing.github.io/

## Instructions

This project relies on the mmap functionality to enable multithreading. As a result, it will not be compatible with non-Unix operating systems.

### Cloning

1. In  a terminal, navigate to the directory where you would like to clone the project.

2. `git clone https://github.com/robertoeg97/Ray-Tracer.git`

### Generate and run Debug Build Files

```
cd Ray-Tracer
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
./ray-tracer <image-filename>
```

### Generate and run Release Build Files

```
cd Ray-Tracer
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./ray-tracer <image-filename>
```


## Images

Background Gradient

![Blue-White Gradient](/rendered_images/pngs/blue-to-white.png)


Single Red Sphere

![Red Sphere](/rendered_images/pngs/red-sphere.png)


Coloring RGB based on components of surface normal 

![Normal Components Shaded Sphere](/rendered_images/pngs/normals-sphere.png)


Antialiasing Added

![Antialiased Normal Components Shaded Sphere](/rendered_images/pngs/antialiased_colored_sphere.png)


Added Diffuse (Matte) Materials

![Diffuse Sphere](rendered_images/pngs/grey_diffuse_sphere.png)


Added Depth Limit to number of reflections

![Depth Limited Render](rendered_images/pngs/depth_limited_diffuse.png)


Removed Shadow Acne

![Removed Shadow Acne](rendered_images/pngs/no_shadow_acne.png)


Lambertian scattering is now properly probabilitically weighted towards the normal

![Correct Lambertian Scatter Probability](rendered_images/pngs/correct_lambertian.png)


Added Metal Surfaces

![Metal Spheres](rendered_images/pngs/metal_spheres.png)


Added Fuzzy Metals

![Metal Fuzzing](rendered_images/pngs/fuzzed_metal_spheres.png)


Added basic Dielectric Materials

![Always Refracting Glass Implementation](rendered_images/pngs/refracting_glass.png)


Added Total Internal Reflection to Dielectric Model

![Glass Implementing Total Internal Reflection](rendered_images/pngs/TIR_glass.png)


Trick: Glass Sphere within a glass sphere gives a hollow sphere.

Added random Dielctric reflection via reflectance : Schlick approximation.

![Hollow Glass: Schlick Reflectance Approximation](rendered_images/pngs/hollow_glass_sphere.png)


Added adjustable FOV

![Adjustable FOV](rendered_images/pngs/fov_testing.png)


Added ability to move and orient camera arbitrarily

![Completely Adjustable Camera](rendered_images/pngs/moveable_camera.png)


Zoomed in

![Adjustable Camera Zoom](rendered_images/pngs/moveable_camera_zoom.png)


Added Depth of Field/Defocus Blur

![Depth of Field / Defocus Blur](rendered_images/pngs/defocus_blur.png)


Cool Render

![Bunch of Spheres](/rendered_images/pngs/multiple_sphere_render.png)

## A Note on Performance

At this point, I implemented multithreading.
A rough benchmark rendering the above image on a 6 core PC 
has the above render taking ~1831 seconds before adding multithreading, 
and ~473 seconds after adding multithreading, 
for a speedup of ~3.87.

## More Images

Added object movement

![Moving Spheres](rendered_images/pngs/moving_spheres.png)


At this point, I implemented a Bounding Volume Heirarchy to increase Performance.

Added Texture interface, with solid color and checkered texture implementations. 

![Checkered Ground](rendered_images/pngs/spheres_on_checkered_ground.png)


At this point, I implemented the ability to add multiple scenes.

Due to previous design decisions, this had to be compile-time information, so template meta-programming was used.

![Checkered Spheres](rendered_images/pngs/checkered_spheres.png)


Added ability to make textures mapped to images.

![Earth Render](rendered_images/pngs/earth_render.png)


Added Perlin Textures: Hash based random tiling

![Random Tiles](rendered_images/pngs/perlin_spheres.png)


Added Interpolation to smooth image

![Smoothed Random Tiles](rendered_images/pngs/perlin_smoothed.png)


Added Hermite Cubing to for even more smoothing

![Hermite Cubing](rendered_images/pngs/perlin_hermite_cubing.png)


Added ability to scale the frequency of the Perlin noise

![Scalable Perlin Noise](rendered_images/pngs/perlin_scaling.png)


Added shifting when generating noise, to reduce the blocky appearance

![Shifted Perlin Noise](rendered_images/pngs/perlin-shifted.png)


Added Turbulence: Weighted sum of multiple frequencies of the hash

![Perlin Turbulence](rendered_images/pngs/perlin_turbulence.png)


Added striped texture using the Perlin noise

![Undulating Perlin Ball](rendered_images/pngs/undulating_perlin_ball.png)


Added Ability to Render Second Geometric Primitive: Parallelograms

![Simple Quadrilaterals](rendered_images/pngs/quadrilaterals.png)


Added Light Emitting Material

![Single Light Source](rendered_images/pngs/simple_quad_light.png)


Two Light Sources

![Dual Light Sources](rendered_images/pngs/simple_dual_lights.png)


Cornell Box Scene

![Cornell Box](rendered_images/pngs/cornell_box.png)


Implemented generic Box out of Quad primitives

![Cornell Box with Boxes](rendered_images/pngs/cornell_box_with_boxes.png)


Implemented Classes for Arbitrary Rotation and Translation of Objects

![Boxes with Translation and Rotation](rendered_images/pngs/boxes_w_translation_rotation.png)


Added constant density volumes as Hittable objects

![Cornell Box With Smoky Volumes](rendered_images/pngs/cornell_box_smoky.png)


Cool Render

![Complex Cornell Render](rendered_images/pngs/complex_cornell_render.png)