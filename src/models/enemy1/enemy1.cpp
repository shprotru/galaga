#include <iostream>

#include "enemy1.h"

namespace ENEMY1 {
    enemy1::enemy1 (
        SDL_Renderer* renderer,
        SDL_RendererFlip flp,  // Нужно для рендеринга
        ASSETS::loader &ldr
    )
    {
        flip = flp;
        gRenderer = renderer;
        state = MODEL::Mstate::initialized;

        for( auto it = std::make_pair( animWalk.begin(), ASSETS::enemy1_frame1 ) ;
                it.first != animWalk.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }

        for( auto it = std::make_pair( animDying.begin(), ASSETS::enemy_death1 ) ;
                it.first != animDying.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }
    }

    enemy1::~enemy1()
    {
        for( auto it = animWalk.begin(); it != animWalk.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy1).name() << " walk sprites freed" << std::endl;

        for( auto it = animDying.begin(); it != animDying.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy1).name() << " dying sprites freed" << std::endl;
    }
}
