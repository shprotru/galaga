#ifndef ENEMY1_H
#define ENEMY1_H

#include "../model.h"

namespace ENEMY1 {
    class enemy1 : MODEL::Basic, MODEL::PModel, MODEL::IModel
    {
        uint16_t currAnimFrame; // текущий фрейм анимации
        long tRemForStep; // остаток времени от дельты при движении
        static const uint16_t anim_amount_frames = 2;
        static const uint16_t anim_death_amount_frames = 5;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animWalk;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animDying;
        int id;
    public:
        enemy1(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~enemy1();

        void move(long timeDelta);
        void render();
        void setInitPosition( uint16_t screenW, uint16_t screenH );
        void setPosition( uint16_t posX, uint16_t posY );
        void setID( int _id ) {
            id = _id;
        }
    };
}

#endif // ENEMY1_H
