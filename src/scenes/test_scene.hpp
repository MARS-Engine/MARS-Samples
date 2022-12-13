#ifndef __TEST__SCENE__
#define __TEST__SCENE__

#include "MVRE/scenes/scene.hpp"

class test_scene : public mvre_scenes::scene {
    using mvre_scenes::scene::scene;

    void load() override;
};

#endif