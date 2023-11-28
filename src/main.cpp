#include <memory>
#include <string>
#include "ProcessArguments.h"
#include "HittableList.h"
#include "Camera.h"
#include "SceneInfo.h"
#include "TimeFunction.h"
#include "BVH.h"

//Scene Tag: defined in SceneInfo.h
using Scene = ComplexCornellScene;

/**
 * @brief renders the scene Scene and writes the .ppm file information to filename
 * 
 * @param filename the .ppm filename that the scene will be written to
 */
void render_scene(const std::string& filename) {
    //get world info
    HittableList world = make_world<Scene>();

    //make Bounding Volume Heirarchy
    world = HittableList{std::make_shared<BVH_node>(world)};

    //render
    Camera<Scene> camera {};
    camera.render(world, filename);
}

int main(int argc, char *argv[]) {
    //process inputs to get filename
    std::string filename = process_arguments(argc, argv);
    
    time_function(render_scene, filename);

    return 0;
}