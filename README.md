# Ray-Tracer

A high-performance ray tracer written in modern C++20, designed to render complex 3D scenes with realistic lighting, materials, and textures. This project demonstrates advanced systems-level programming, multithreading, and low-level memory management, with a strong focus on performance optimization and clean software design.

---

## Project Overview

This ray tracer is a personal exploration of computer graphics and systems programming. It supports a wide variety of surface types (matte, reflective, emissive, transparent, and image-mapped), and is engineered for speed and scalability. The codebase leverages modern C++ features, multithreading, and memory-mapped I/O to efficiently render high-quality images.

**Key technical highlights:**
- **C++20:** Extensive use of modern language features, including `constexpr`, template metaprogramming, and strong type safety.
- **Multithreading:** Spatial tiling and thread-safe memory-mapped I/O for efficient parallel rendering.
- **Performance:** Bounding Volume Hierarchy (BVH) for fast ray-object intersection, compile-time scene configuration, and aggressive compiler optimizations.
- **Graphics:** Realistic materials, procedural and image-based textures, depth of field, and volumetric effects.

---

## Features

- **Complex Scene Rendering:** Supports matte, reflective, emissive, transparent, and image-mapped materials for photorealistic scenes.
- **Performance Optimizations:** BVH reduces ray intersection tests by ~70%, dramatically improving render times.
- **Multithreaded Spatial Tiling:** Divide-and-conquer approach partitions the scene into tiles, distributing work across CPU cores (5x speedup on 6-core CPUs).
- **Memory-Mapped I/O:** Enables thread-safe, lock-free pixel writes for concurrent rendering.
- **Compile-Time Computations:** Uses `constexpr` and templated structs for scene configuration, minimizing runtime overhead.
- **Procedural & Image Textures:** Includes Perlin noise, checkered, striped, and image-mapped textures.
- **Advanced Camera:** Adjustable FOV, arbitrary movement/orientation, and depth of field.
- **Volumetric Effects:** Constant density volumes for smoke/fog.
- **Multiple Geometric Primitives:** Spheres, parallelograms, and boxes with support for translation and rotation.

---

## Design Decisions

- **Spatial Tiling vs. Sample-Based Multithreading:** Chose spatial tiling for deeper exploration of concurrency and memory management, despite increased complexity.
- **Memory-Mapped I/O:** Used `mmap` for lock-free, thread-safe image output, trading higher setup cost for runtime efficiency.
- **BVH Optimization:** Reduced intersection complexity from O(n) to O(log n) for large scenes.
- **Compile-Time Scene Configuration:** Leveraged template metaprogramming to eliminate runtime branching for static scenes.

---

## Challenges & Lessons Learned

- **Concurrency Trade-Offs:** Spatial tiling and memory-mapped I/O required careful profiling and debugging to balance setup cost and runtime speed.
- **Compile-Time Programming:** Designing `constexpr` structs and templates deepened my understanding of C++20 metaprogramming.
- **Performance Profiling:** Used Valgrind and `std::chrono` to identify and resolve bottlenecks, especially in memory access and threading.
- **Systems-Level Thinking:** Gained experience with low-level OS features (e.g., `mmap`) and their impact on performance and portability.

---

## Technical Skills Demonstrated

- Modern C++ (C++20), template metaprogramming, and compile-time programming
- Multithreading, concurrency, and lock-free programming
- Memory-mapped file I/O and systems programming (POSIX)
- Performance profiling and optimization
- Computer graphics: ray tracing, BVH, procedural textures, and physically-based rendering

---

## Dependencies

- C++20 compiler (GCC 10+, Clang 10+)
- CMake 3.1+
- POSIX-compliant system (Linux, macOS; required for `mmap`)

---

## Installation

This project uses CMake and is compatible with Unix-based systems.

---

## Cloning

```sh
git clone https://github.com/robertoeg97/Ray-Tracer.git
```

---

## Building and Running

**Debug build:**
```sh
cd Ray-Tracer
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
./ray-tracer <image-filename>
```

**Release build:**
```sh
cd Ray-Tracer
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./ray-tracer <image-filename>
```

---

## Usage

Run the executable with an output image filename (e.g., `output.ppm`). The ray tracer renders a predefined scene (configurable in code) and saves the result in PPM format.

---

## Example Renders

Below are some example images produced by the ray tracer, demonstrating its features and progression:

| Feature | Example |
|---------|---------|
| Background Gradient | ![Blue-White Gradient](/rendered_images/pngs/blue-to-white.png) |
| Single Red Sphere | ![Red Sphere](/rendered_images/pngs/red-sphere.png) |
| Surface Normal Shading | ![Normal Components Shaded Sphere](/rendered_images/pngs/normals-sphere.png) |
| Antialiasing | ![Antialiased Normal Components Shaded Sphere](/rendered_images/pngs/antialiased_colored_sphere.png) |
| Diffuse Materials | ![Diffuse Sphere](rendered_images/pngs/grey_diffuse_sphere.png) |
| Depth-Limited Reflections | ![Depth Limited Render](rendered_images/pngs/depth_limited_diffuse.png) |
| Shadow Acne Fix | ![Removed Shadow Acne](rendered_images/pngs/no_shadow_acne.png) |
| Correct Lambertian Scattering | ![Correct Lambertian Scatter Probability](rendered_images/pngs/correct_lambertian.png) |
| Metal Surfaces | ![Metal Spheres](rendered_images/pngs/metal_spheres.png) |
| Fuzzy Metals | ![Metal Fuzzing](rendered_images/pngs/fuzzed_metal_spheres.png) |
| Dielectric Materials | ![Always Refracting Glass Implementation](rendered_images/pngs/refracting_glass.png) |
| Total Internal Reflection | ![Glass Implementing Total Internal Reflection](rendered_images/pngs/TIR_glass.png) |
| Hollow Glass (Schlick Approximation) | ![Hollow Glass: Schlick Reflectance Approximation](rendered_images/pngs/hollow_glass_sphere.png) |
| Adjustable FOV | ![Adjustable FOV](rendered_images/pngs/fov_testing.png) |
| Moveable Camera | ![Completely Adjustable Camera](rendered_images/pngs/moveable_camera.png) |
| Camera Zoom | ![Adjustable Camera Zoom](rendered_images/pngs/moveable_camera_zoom.png) |
| Depth of Field | ![Depth of Field / Defocus Blur](rendered_images/pngs/defocus_blur.png) |
| Multiple Spheres | ![Bunch of Spheres](/rendered_images/pngs/multiple_sphere_render.png) |
| Moving Objects | ![Moving Spheres](rendered_images/pngs/moving_spheres.png) |
| Checkered Textures | ![Checkered Ground](rendered_images/pngs/spheres_on_checkered_ground.png) |
| Multiple Scenes | ![Checkered Spheres](rendered_images/pngs/checkered_spheres.png) |
| Image-Mapped Textures | ![Earth Render](rendered_images/pngs/earth_render.png) |
| Perlin Noise Textures | ![Random Tiles](rendered_images/pngs/perlin_spheres.png) |
| Smoothed Perlin Noise | ![Smoothed Random Tiles](rendered_images/pngs/perlin_smoothed.png) |
| Hermite Cubing | ![Hermite Cubing](rendered_images/pngs/perlin_hermite_cubing.png) |
| Scalable Perlin Noise | ![Scalable Perlin Noise](rendered_images/pngs/perlin_scaling.png) |
| Shifted Perlin Noise | ![Shifted Perlin Noise](rendered_images/pngs/perlin-shifted.png) |
| Perlin Turbulence | ![Perlin Turbulence](rendered_images/pngs/perlin_turbulence.png) |
| Striped Texture | ![Undulating Perlin Ball](rendered_images/pngs/undulating_perlin_ball.png) |
| Parallelograms | ![Simple Quadrilaterals](rendered_images/pngs/quadrilaterals.png) |
| Emissive Materials | ![Single Light Source](rendered_images/pngs/simple_quad_light.png) |
| Multiple Light Sources | ![Dual Light Sources](rendered_images/pngs/simple_dual_lights.png) |
| Cornell Box | ![Cornell Box](rendered_images/pngs/cornell_box.png) |
| Boxes | ![Cornell Box with Boxes](rendered_images/pngs/cornell_box_with_boxes.png) |
| Translation & Rotation | ![Boxes with Translation and Rotation](rendered_images/pngs/boxes_w_translation_rotation.png) |
| Volumetric Effects | ![Cornell Box With Smoky Volumes](rendered_images/pngs/cornell_box_smoky.png) |
| Complex Scene | ![Complex Cornell Render](rendered_images/pngs/complex_cornell_render.png) |

---
