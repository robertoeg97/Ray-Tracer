# Ray-Tracer

This project implements a ray tracer following the Ray Tracing in One Weekend book series: https://raytracing.github.io/

## Instructions

This project relies on the mmap functionality to enable multithreading. As a result, it will not be compatible with non-Unix operating systems.

The image that is generated is specified by the scene drawn in main.cpp

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


## Sample Image

![Bunch of Spheres](/images/multiple_sphere_render.png)

## A Note on Performance

This project implements multithreading. 
A rough benchmark on a 6 core PC has the above render taking ~1831 seconds before adding multithreading, 
and ~473 seconds after adding multithreading, 
for a speedup of ~3.87.