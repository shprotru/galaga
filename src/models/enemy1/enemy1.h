#ifndef ENEMY1_H
#define ENEMY1_H

#include "../model.h"

namespace ENEMY1 {
    class enemy1 : MODEL::PModel, MODEL::IModel
    {
        static const uint16_t anim_amount_frames = 2;
        static const uint16_t anim_death_amount_frames = 5;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animWalk;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animDying;
    public:
        enemy1(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~enemy1();
    };
}

#endif // ENEMY1_H
