#ifndef TEST_SCENE_
#define TEST_SCENE_

#include "MVRE/scenes/scene.hpp"

class test_scene : public mvre_scenes::scene {
    using mvre_scenes::scene::scene;

    void load() override;
};

#endif