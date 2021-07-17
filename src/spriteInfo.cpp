#include <iostream>

#include "assets.h"

namespace ASSETS {

    spriteInfo::spriteInfo( SDL_Texture *t, uint16_t w, uint16_t h ) {
        texture = t;
        width = w;
        heigth = h;
    }

    spriteInfo::~spriteInfo() {
        SDL_DestroyTexture( texture );
        std::cout << typeid(spriteInfo).name() << " textures freed" << std::endl;
    }
}
