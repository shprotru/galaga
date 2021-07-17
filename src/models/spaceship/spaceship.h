#ifndef PLAYER_H
#define PLAYER_H

#include "../model.h"

namespace SPACESHIP {
    class spaceship : MODEL::PModel, MODEL::IModel
    {
        static const uint16_t anim_death_amount_frames = 4;
        ASSETS::spriteInfo *sprite;
        std::array<ASSETS::spriteInfo *, anim_death_amount_frames> animDeath;
    public:
        spaceship(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~spaceship();

        void move();
        void render();
        void setInitPosition( uint16_t screen_w, uint16_t screen_h );

        void onMouseMove( double mouseOx, double mouseOy );
        void onMoveLeft();
        void onMoveRight();
        void onFire();
    };
}

#endif // PLAYER_H
