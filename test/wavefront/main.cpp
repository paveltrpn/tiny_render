
#include <iostream>
#include <stdexcept>

#include "spdlog/spdlog.h"

import geometry;

int main( int argc, char **argv ) {
    try {
        tire::WavefrontObj demon{ "/mnt/main_disk/code/tiny_render/assets/demon_baby.obj" };
        std::cout << demon;
    } catch ( const std::runtime_error &e ) {
        spdlog::critical( "catch exception: {}", e.what() );
        return 0;
    }

    return 0;
}
