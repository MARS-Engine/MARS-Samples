#ifndef TEST_SCENE_
#define TEST_SCENE_

#include "MARS/scenes/scene.hpp"

class test_scene : public mars_scenes::scene {
    using mars_scenes::scene::scene;

    void load() override;
};

#endif