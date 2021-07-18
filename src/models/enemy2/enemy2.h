#ifndef ENEMY2_H
#define ENEMY2_H

#include "../model.h"

namespace ENEMY2 {
    class enemy2 : MODEL::Basic, MODEL::PModel, MODEL::IModel
    {
        uint16_t currAnimFrame; // текущий фрейм анимации
        long tRemForStep; // остаток времени от дельты при движении
        static const uint16_t anim_amount_frames = 2;
        static const uint16_t anim_death_amount_frames = 5;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animWalk;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animDying;
    public:
        enemy2(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~enemy2();

        void move(long timeDelta);
        void render();
        void setInitPosition( uint16_t screenW, uint16_t screenH );
        void setPosition( uint16_t posX, uint16_t posY );
    };
}

#endif // ENEMY2_H
